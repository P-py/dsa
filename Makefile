# Makefile for the DSA repo — builds every module application.
#
# Binaries are written to $(BUILD_DIR) so their names can match the module
# (e.g. `bst`, `staticstack`) without colliding with the source folder of
# the same name at the repo root.
#
# Adding a new module is a two-line change:
#   1) append its name to APPS
#   2) define <name>_SRCS with its source files
#
# Usage:
#   make            build every app
#   make <name>     build a single app (see `make list`)
#   make clean      remove the build directory
#   make list       list all app names
#   make help       show this summary

CC        := gcc
CFLAGS    := -Wall -Wextra -g
BUILD_DIR := build

APPS := \
    sensor1d \
    bag \
    staticstack \
    dynamicstack \
    infixtopostfix \
    bst \
    abb \
    avl \
    palindrome \
    problem2

# --- per-app sources ---------------------------------------------------------
sensor1d_SRCS       := tad/sensor1d.c tad/main.c
bag_SRCS            := bag/bag.c bag/aplicacaoBag.c
staticstack_SRCS    := staticstack/staticstack.c staticstack/main.c
dynamicstack_SRCS   := dynamicstack/dynamicstack.c dynamicstack/main.c
infixtopostfix_SRCS := infixtopostfix/infixtopostfix.c infixtopostfix/main.c
bst_SRCS            := bst/bst.c bst/main.c
abb_SRCS            := abb/abb.c abb/main.c
avl_SRCS            := avl/avl.c avl/main.c
palindrome_SRCS     := problems/palindrome.c
problem2_SRCS       := problems/problem2.c

# -----------------------------------------------------------------------------

.PHONY: all clean list help $(APPS)

all: $(APPS)

$(BUILD_DIR):
	@mkdir -p $(BUILD_DIR)

# Generate one build rule per app from its *_SRCS variable.
# Each <name> is a phony alias that depends on the real binary in $(BUILD_DIR).
# Any change to a listed .c triggers a full relink — fine at this scale.
define APP_template
$(1): $(BUILD_DIR)/$(1)
$(BUILD_DIR)/$(1): $$($(1)_SRCS) | $(BUILD_DIR)
	$(CC) $(CFLAGS) $$^ -o $$@
endef
$(foreach app,$(APPS),$(eval $(call APP_template,$(app))))

clean:
	rm -rf $(BUILD_DIR) problems/a.out

list:
	@printf '%s\n' $(APPS)

help:
	@echo 'Targets:'
	@echo '  make            build all apps'
	@echo '  make <name>     build one app (see "make list")'
	@echo '  make clean      remove build directory'
	@echo '  make list       list all app names'
