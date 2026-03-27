/**
 * @file infixtopostfix.h
 * @brief Interface for infix-to-postfix expression conversion and evaluation.
 *
 * Implements Dijkstra's Shunting Yard algorithm to convert a human-readable
 * infix expression (e.g. "3 + 4 * 2") into Reverse Polish Notation / postfix
 * (e.g. "3 4 2 * +"), which can then be evaluated with a simple operand stack.
 *
 * Supported tokens:
 *   - Integer operands (multi-digit, e.g. 123).
 *   - Binary operators: +  -  *  /  ^ (exponentiation, right-associative).
 *   - Grouping parentheses: ( ).
 *   - Whitespace between tokens is ignored.
 *
 * Operator precedence (highest first):
 *   ^  >  * /  >  + -
 *
 * Output format:
 *   Tokens are written to the caller-supplied postfix buffer separated by a
 *   single space, e.g. "3 4 2 * 1 5 - 2 3 ^ ^ / +".
 *
 * Error handling:
 *   Both itp_convert() and itp_evaluate() return one of the ITP_* codes below.
 *   Pass the code to itp_strerror() to obtain a human-readable description.
 */

#ifndef INFIXTOPOSTFIX_H
#define INFIXTOPOSTFIX_H

/* Maximum length (including '\0') of an expression string. */
#define ITP_MAX_EXPR 512

/* ── Return / error codes ──────────────────────────────────────────────── */

#define ITP_OK                 0  /**< Operation completed successfully.      */
#define ITP_ERR_UNMATCHED_OPEN 1  /**< '(' has no matching ')'.               */
#define ITP_ERR_UNMATCHED_CLOSE 2 /**< ')' has no matching '('.               */
#define ITP_ERR_INVALID_TOKEN  3  /**< Unrecognised character in expression.  */
#define ITP_ERR_BUFFER_OVERFLOW 4 /**< Postfix output buffer is too small.    */
#define ITP_ERR_STACK_OVERFLOW 5  /**< Internal operator stack capacity hit.  */
#define ITP_ERR_DIV_BY_ZERO    6  /**< Division or modulo by zero detected.   */
#define ITP_ERR_EMPTY_EXPR     7  /**< Input expression is empty.             */
#define ITP_ERR_BAD_POSTFIX    8  /**< Postfix is malformed (too few/many operands). */

/* ── Public API ────────────────────────────────────────────────────────── */

/**
 * @brief Converts an infix expression string to its postfix equivalent.
 *
 * Uses the Shunting Yard algorithm.  Operands may be multi-digit integers.
 * The result is written into @p postfix as a null-terminated string with
 * tokens separated by single spaces.
 *
 * @param infix        Null-terminated infix expression (e.g. "(1+2)*3").
 * @param postfix      Caller-allocated buffer to receive the postfix string.
 * @param postfix_size Capacity of @p postfix in bytes (including space for '\0').
 * @return ITP_OK on success, or an ITP_ERR_* code describing the failure.
 */
int itp_convert(const char *infix, char *postfix, int postfix_size);

/**
 * @brief Evaluates a postfix expression and stores the integer result.
 *
 * Scans the postfix string left to right:
 *   - Numbers are pushed onto an operand stack.
 *   - Operators pop two operands, compute the result, and push it back.
 * The final value remaining on the stack is the expression result.
 *
 * @param postfix Null-terminated postfix expression (e.g. "1 2 + 3 *").
 * @param result  Pointer to an int that receives the computed value on success.
 *                Unchanged on failure.
 * @return ITP_OK on success, or an ITP_ERR_* code describing the failure.
 */
int itp_evaluate(const char *postfix, int *result);

/**
 * @brief Returns a human-readable description of an ITP_* error code.
 *
 * @param err An ITP_* error code.
 * @return Pointer to a static string describing the error (never NULL).
 */
const char *itp_strerror(int err);

#endif /* INFIXTOPOSTFIX_H */
