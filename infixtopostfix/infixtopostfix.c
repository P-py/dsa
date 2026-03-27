/**
 * @file infixtopostfix.c
 * @brief Implementation of infix-to-postfix conversion and postfix evaluation.
 *
 * ── Algorithm overview: Shunting Yard (Dijkstra, 1961) ───────────────────
 *
 * The algorithm maintains two structures:
 *   output  – a growing string (written directly into the caller's buffer).
 *   op_stack – a LIFO stack of pending operators and left parentheses.
 *
 * For each token in the infix expression (left to right):
 *
 *   NUMBER   → copy digits directly to output, append a space separator.
 *
 *   OPERATOR → before pushing, pop operators from op_stack to output while:
 *                 the stack top is an operator  AND
 *                 (its precedence is higher  OR
 *                  its precedence is equal AND the new op is left-associative)
 *                 AND the stack top is not '('
 *              Then push the new operator.
 *
 *   '('      → push onto op_stack unconditionally.
 *
 *   ')'      → pop op_stack to output until '(' is found; discard the '('.
 *              If the stack empties without finding '(' → mismatched paren.
 *
 * After all tokens, drain remaining operators from op_stack to output.
 * If a '(' is encountered during drain → unmatched open paren.
 *
 * ── Postfix evaluation ───────────────────────────────────────────────────
 *
 * Scan the postfix string left to right:
 *   NUMBER   → parse it and push onto an integer operand stack.
 *   OPERATOR → pop right-hand operand, pop left-hand operand,
 *              apply the operator, push the result.
 * After scanning, the single remaining stack value is the answer.
 *
 * ── Internal stacks ──────────────────────────────────────────────────────
 *
 * Two private stack types are defined here:
 *   CharStack – stores char values; used for operators during conversion.
 *   IntStack  – stores int values;  used for operands during evaluation.
 *
 * Neither type is exposed in the public header; they are implementation details.
 */

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "infixtopostfix.h"

/* ── Internal: CharStack ─────────────────────────────────────────────────
 *
 * A fixed-capacity stack of char values.  Used to hold pending operators
 * and parentheses during the Shunting Yard pass.
 *
 * Capacity is set to ITP_MAX_EXPR: in the worst case (expression of all
 * opening parentheses) every character is pushed without being popped.
 */
#define CHAR_STACK_CAP ITP_MAX_EXPR

typedef struct {
    char items[CHAR_STACK_CAP];
    int  top; /* index of topmost element; -1 when empty */
} CharStack;

static void cs_init(CharStack *s) {
    (*s).top = -1;
}

static int cs_push(CharStack *s, char c) {
    if ((*s).top == CHAR_STACK_CAP - 1) {
        return 0; /* overflow */
    }
    (*s).top++;
    (*s).items[(*s).top] = c;
    return 1;
}

static int cs_pop(CharStack *s, char *out) {
    if ((*s).top == -1) {
        return 0; /* underflow */
    }
    *out = (*s).items[(*s).top];
    (*s).top--;
    return 1;
}

/* Inspect the top without removing it. */
static int cs_peek(const CharStack *s, char *out) {
    if ((*s).top == -1) {
        return 0;
    }
    *out = (*s).items[(*s).top];
    return 1;
}

static int cs_isEmpty(const CharStack *s) {
    return (*s).top == -1;
}

/* ── Internal: IntStack ──────────────────────────────────────────────────
 *
 * A fixed-capacity stack of int values.  Used to hold operands during
 * postfix evaluation.
 */
#define INT_STACK_CAP 256

typedef struct {
    int items[INT_STACK_CAP];
    int top; /* index of topmost element; -1 when empty */
} IntStack;

static void is_init(IntStack *s) {
    (*s).top = -1;
}

static int is_push(IntStack *s, int v) {
    if ((*s).top == INT_STACK_CAP - 1) {
        return 0; /* overflow */
    }
    (*s).top++;
    (*s).items[(*s).top] = v;
    return 1;
}

static int is_pop(IntStack *s, int *out) {
    if ((*s).top == -1) {
        return 0; /* underflow */
    }
    *out = (*s).items[(*s).top];
    (*s).top--;
    return 1;
}

static int is_isEmpty(const IntStack *s) {
    return (*s).top == -1;
}

/* ── Operator helpers ────────────────────────────────────────────────────*/

/**
 * @brief Returns the precedence level of an operator character.
 *
 * Higher value → evaluated before lower-value operators.
 *   ^  → 3  (exponentiation)
 *   *  /  → 2  (multiplicative)
 *   +  -  → 1  (additive)
 *   anything else → 0 (not an operator)
 */
static int precedence(char op) {
    switch (op) {
        case '^':           return 3;
        case '*': case '/': return 2;
        case '+': case '-': return 1;
        default:            return 0;
    }
}

/**
 * @brief Returns 1 if the operator is left-associative, 0 if right-associative.
 *
 * In this implementation only '^' is right-associative.
 * Example: 2^3^2 is parsed as 2^(3^2) = 512, not (2^3)^2 = 64.
 */
static int is_left_assoc(char op) {
    return op != '^';
}

/**
 * @brief Returns 1 if the character is a supported binary operator.
 */
static int is_operator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '^';
}

/**
 * @brief Appends a single character to the output buffer.
 *
 * @param buf      The output buffer.
 * @param buf_size Total capacity of @p buf.
 * @param pos      Current write position (updated on success).
 * @param c        Character to append.
 * @return 1 on success, 0 if the buffer is full.
 */
static int buf_putc(char *buf, int buf_size, int *pos, char c) {
    /* reserve one byte for the '\0' terminator */
    if (*pos >= buf_size - 1) {
        return 0;
    }
    buf[(*pos)++] = c;
    return 1;
}

/* ── Public: itp_convert ─────────────────────────────────────────────────*/

int itp_convert(const char *infix, char *postfix, int postfix_size) {
    if (infix == NULL || infix[0] == '\0') {
        return ITP_ERR_EMPTY_EXPR;
    }

    CharStack ops;   /* pending operators / parentheses */
    cs_init(&ops);

    int out = 0;     /* write cursor into postfix[] */
    int i   = 0;     /* read cursor into infix[]    */

    while (infix[i] != '\0') {
        char c = infix[i];

        /* ── Skip whitespace ─────────────────────────────────────────── */
        if (c == ' ' || c == '\t') {
            i++;
            continue;
        }

        /* ── Number (multi-digit) ─────────────────────────────────────
         *
         * Copy every consecutive digit straight to the output buffer,
         * then append a single space as a token separator.
         */
        if (isdigit((unsigned char)c)) {
            while (isdigit((unsigned char)infix[i])) {
                if (!buf_putc(postfix, postfix_size, &out, infix[i])) {
                    return ITP_ERR_BUFFER_OVERFLOW;
                }
                i++;
            }
            /* token separator */
            if (!buf_putc(postfix, postfix_size, &out, ' ')) {
                return ITP_ERR_BUFFER_OVERFLOW;
            }
            continue; /* i already advanced inside the while above */
        }

        /* ── Operator ─────────────────────────────────────────────────
         *
         * Before pushing, pop operators that have higher (or equal
         * left-associative) precedence so they appear in output first.
         */
        if (is_operator(c)) {
            char top;
            while (!cs_isEmpty(&ops) &&
                   cs_peek(&ops, &top) &&
                   top != '(' &&
                   (precedence(top) > precedence(c) ||
                    (precedence(top) == precedence(c) && is_left_assoc(c)))) {

                cs_pop(&ops, &top);
                if (!buf_putc(postfix, postfix_size, &out, top) ||
                    !buf_putc(postfix, postfix_size, &out, ' ')) {
                    return ITP_ERR_BUFFER_OVERFLOW;
                }
            }
            if (!cs_push(&ops, c)) {
                return ITP_ERR_STACK_OVERFLOW;
            }
            i++;
            continue;
        }

        /* ── Left parenthesis ─────────────────────────────────────────*/
        if (c == '(') {
            if (!cs_push(&ops, c)) {
                return ITP_ERR_STACK_OVERFLOW;
            }
            i++;
            continue;
        }

        /* ── Right parenthesis ────────────────────────────────────────
         *
         * Pop operators to output until the matching '(' is found.
         * The '(' itself is discarded (not written to output).
         */
        if (c == ')') {
            char top;
            int  found_open = 0;

            while (!cs_isEmpty(&ops)) {
                cs_pop(&ops, &top);
                if (top == '(') {
                    found_open = 1;
                    break;
                }
                if (!buf_putc(postfix, postfix_size, &out, top) ||
                    !buf_putc(postfix, postfix_size, &out, ' ')) {
                    return ITP_ERR_BUFFER_OVERFLOW;
                }
            }

            if (!found_open) {
                return ITP_ERR_UNMATCHED_CLOSE;
            }
            i++;
            continue;
        }

        /* ── Unrecognised token ───────────────────────────────────────*/
        return ITP_ERR_INVALID_TOKEN;
    }

    /* ── Drain remaining operators ────────────────────────────────────────
     *
     * Any operator left on op_stack belongs at the end of the output.
     * Encountering '(' here means there was a '(' without a matching ')'.
     */
    char top;
    while (!cs_isEmpty(&ops)) {
        cs_pop(&ops, &top);
        if (top == '(') {
            return ITP_ERR_UNMATCHED_OPEN;
        }
        if (!buf_putc(postfix, postfix_size, &out, top) ||
            !buf_putc(postfix, postfix_size, &out, ' ')) {
            return ITP_ERR_BUFFER_OVERFLOW;
        }
    }

    /* Trim the trailing space added after the last token, then terminate. */
    if (out > 0 && postfix[out - 1] == ' ') {
        out--;
    }
    postfix[out] = '\0';

    return ITP_OK;
}

/* ── Public: itp_evaluate ────────────────────────────────────────────────*/

/**
 * @brief Raises @p base to the power @p exp using repeated multiplication.
 *
 * Only supports non-negative exponents.  Returns 1 for base^0.
 */
static int int_pow(int base, int exp) {
    int result = 1;
    for (int i = 0; i < exp; i++) {
        result *= base;
    }
    return result;
}

int itp_evaluate(const char *postfix, int *result) {
    if (postfix == NULL || postfix[0] == '\0') {
        return ITP_ERR_EMPTY_EXPR;
    }

    IntStack operands;
    is_init(&operands);

    int i = 0;

    while (postfix[i] != '\0') {
        char c = postfix[i];

        /* ── Skip spaces (token separators) ──────────────────────────*/
        if (c == ' ') {
            i++;
            continue;
        }

        /* ── Number ───────────────────────────────────────────────────
         *
         * Parse all consecutive digits into a single integer value,
         * then push it onto the operand stack.
         */
        if (isdigit((unsigned char)c)) {
            int value = 0;
            while (isdigit((unsigned char)postfix[i])) {
                value = value * 10 + (postfix[i] - '0');
                i++;
            }
            if (!is_push(&operands, value)) {
                return ITP_ERR_STACK_OVERFLOW;
            }
            continue;
        }

        /* ── Operator ─────────────────────────────────────────────────
         *
         * Pop the right-hand operand first (it was pushed last),
         * then the left-hand operand.  Apply the operation and push
         * the result back.
         *
         * Stack before:  ... | left | right |   ← top
         * Stack after:   ... | (left OP right) | ← top
         */
        if (is_operator(c)) {
            int rhs, lhs;

            if (!is_pop(&operands, &rhs) || !is_pop(&operands, &lhs)) {
                /* Not enough operands — expression is malformed. */
                return ITP_ERR_BAD_POSTFIX;
            }

            int value;
            switch (c) {
                case '+': value = lhs + rhs;               break;
                case '-': value = lhs - rhs;               break;
                case '*': value = lhs * rhs;               break;
                case '/':
                    if (rhs == 0) return ITP_ERR_DIV_BY_ZERO;
                    value = lhs / rhs;
                    break;
                case '^': value = int_pow(lhs, rhs);       break;
                default:  return ITP_ERR_INVALID_TOKEN;
            }

            if (!is_push(&operands, value)) {
                return ITP_ERR_STACK_OVERFLOW;
            }
            i++;
            continue;
        }

        return ITP_ERR_INVALID_TOKEN;
    }

    /* After a well-formed postfix expression exactly one value remains. */
    if (is_isEmpty(&operands)) {
        return ITP_ERR_EMPTY_EXPR;
    }

    int final;
    is_pop(&operands, &final);

    /* If anything is still on the stack the expression had too many operands. */
    if (!is_isEmpty(&operands)) {
        return ITP_ERR_BAD_POSTFIX;
    }

    *result = final;
    return ITP_OK;
}

/* ── Public: itp_strerror ────────────────────────────────────────────────*/

const char *itp_strerror(int err) {
    switch (err) {
        case ITP_OK:                  return "Sucesso";
        case ITP_ERR_UNMATCHED_OPEN:  return "Parentese de abertura '(' sem fechamento ')'";
        case ITP_ERR_UNMATCHED_CLOSE: return "Parentese de fechamento ')' sem abertura '('";
        case ITP_ERR_INVALID_TOKEN:   return "Caractere invalido na expressao";
        case ITP_ERR_BUFFER_OVERFLOW: return "Buffer de saida muito pequeno para a expressao";
        case ITP_ERR_STACK_OVERFLOW:  return "Estouro interno da pilha de operadores";
        case ITP_ERR_DIV_BY_ZERO:     return "Divisao por zero";
        case ITP_ERR_EMPTY_EXPR:      return "Expressao vazia";
        case ITP_ERR_BAD_POSTFIX:     return "Expressao posfixa malformada (operandos em excesso ou falta)";
        default:                      return "Erro desconhecido";
    }
}
