# Date: 29.1.26
# Author: Hadar Padael

GREEN  := \033[32m
BLUE   := \033[34m
YELLOW := \033[33m
RESET  := \033[0m

# -------------------------------------------- #
CC := gcc
CFLAGS_COMMON := -ansi -pedantic-errors -Wall -Wextra -fPIC 
DEBUG_CFLAGS = $(CFLAGS_COMMON) -g
RELEASE_CFLAGS = $(CFLAGS_COMMON) -O3 -DNDEBUG
LDLIBS = -L$(LIBS_DIR) -lds_debug
LDFLAGS = -Wl,-rpath,$(LIBS_DIR)

CFLAGS = $(DEBUG_CFLAGS)
INCLUDES = $(addprefix -I, $(INCLUDE_DIRS))

INCLUDE_DIRS = include $(wildcard ../c/*)
LIBS_DIR = libs
SRC_DIR := src
BIN_DIR = bin
TEST_DIR := test
OBJ_DIR = obj
C_DIR := ../c

vpath %.c $(SRC_DIR) $(TEST_DIR) $(wildcard $(C_DIR)/*)

# ----------------------------------------------------- #
DS_SRCS = $(wildcard $(SRC_DIR)/*.c) 
DS_TESTS = $(wildcard $(TEST_DIR)/*.c)
C_TESTS = $(wildcard ../c/*/test*.c) $(wildcard ../c/expressions/*)
C_SRCS = $(filter-out $(C_TESTS),$(wildcard ../c/*/*.c))

# ------------------------------------------------------- #
TEST_OBJS = $(patsubst $(TEST_DIR)/%.c,$(OBJ_DIR)/%.o,$(DS_TESTS))
SRC_OBJ = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(DS_SRCS)) 
C_OBJ = $(foreach f,$(C_SRCS),$(OBJ_DIR)/$(notdir $(f:.c=.o)))

OBJS = $(SRC_OBJ) $(C_OBJ)

# -------------------- #
LIB_TARGET_DEBUG := $(LIBS_DIR)/libds_debug.so
LIB_TARGET_REL   := $(LIBS_DIR)/libds_release.so

TEST_TARGET = $(patsubst $(OBJ_DIR)/%.o, $(BIN_DIR)/%, $(TEST_OBJS))

# ---------------------------------------------------------------- #
$(BIN_DIR)/%: $(OBJ_DIR)/%.o $(LIB_TARGET_DEBUG) | $(BIN_DIR)
	$(CC) $< $(LDLIBS) $(LDFLAGS) -o $@
	@echo "$(GREEN)LD $@$(RESET)"

$(OBJ_DIR)/%.o: %.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -MMD -MP -MT $@ -MF $(@:.o=.d) -c $< -o $@
	@echo "$(GREEN)CC $<$(RESET)"

$(LIB_TARGET_DEBUG): $(OBJS) | $(LIBS_DIR)
	$(CC) $(CFLAGS) -shared $^ -o $@
	@echo "$(GREEN)SO $@$(RESET)"
	
$(LIB_TARGET_REL): $(OBJS) | $(LIBS_DIR)
	$(CC) $(CFLAGS) -shared $^ -o $@
	@echo "$(GREEN)SO $@$(RESET)" 

# ------------------------------------------------------------ #	
$(OBJ_DIR):
	@mkdir -p $@
	@echo "$(YELLOW)mkdir $@$(RESET)"
	
$(BIN_DIR):
	@mkdir -p $@
	@echo "$(YELLOW)mkdir $@$(RESET)"
	
$(LIBS_DIR):
	@mkdir -p $@ 
	@echo "$(YELLOW)mkdir $@$(RESET)"
	
# --------------------------------------------------------- #
.PHONY : clean release debug test all

all: release debug

test: CFLAGS=$(DEBUG_CFLAGS)
test: $(TEST_TARGET)

release: CFLAGS=$(RELEASE_CFLAGS)
release: $(LIB_TARGET_REL)

debug: CFLAGS=$(DEBUG_CFLAGS)
debug: $(LIB_TARGET_DEBUG)

clean:
	@rm -rf $(BIN_DIR) $(OBJ_DIR) $(LIBS_DIR)
	@echo "$(BLUE)Cleaned$(RESET)"
	
DEPFILES := $(OBJS:.o=.d)
-include $(DEPFILES)
