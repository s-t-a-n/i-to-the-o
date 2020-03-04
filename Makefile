NAME = ito.a

# directories
SRC_D = src
OBJ_D = obj
INC_D = inc

# C source and header files
SRC =	$(SRC_D)/api.c														\
		$(SRC_D)/error_and_logging.c										\
		$(SRC_D)/packaging/package_compilation.c							\
		$(SRC_D)/packaging/package_decompilation.c							\
		$(SRC_D)/networking/client.c										\
		$(SRC_D)/networking/server.c										\
		$(SRC_D)/networking/queue.c											\
		$(SRC_D)/networking/pooling.c


INC =	$(INC_D)/ito.h														\
		$(INC_D)/ito_internal.h												\
		$(INC_D)/packaging.h												\
		$(INC_D)/networking.h

OBJ :=	$(SRC:$(SRC_D)/%.c=$(OBJ_D)/%.o)

# outputting
CC_LOG=./.cc.log
CC_ERROR=./.cc.error

NO_COLOR=\x1b[0m
OK_COLOR=\x1b[32;01m
ERROR_COLOR=\x1b[31;01m
WARN_COLOR=\x1b[33;01m

OK_STRING=$(OK_COLOR)[OK]$(NO_COLOR)
ERROR_STRING=$(ERROR_COLOR)[ERRORS]$(NO_COLOR)
WARN_STRING=$(WARN_COLOR)[WARNINGS]$(NO_COLOR)

ECHO=printf
CAT=cat

# compiler and linker
CC = clang
#CC = gcc

# compile flags
CC_FLAGS = -Werror -Wextra -Wall
CC_FLAGS_TESTS = -Werror -Wextra -Wall -g -fsanitize=address -DDEBUG

# debugging or optimization flags
ifeq ($(DEBUG),1)
    CC_FLAGS += -g -fsanitize=address -DDEBUG
else
    CC_FLAGS += -O3 -march=native
endif

# make commands
all: $(NAME)

$(NAME): $(OBJ_D) $(OBJ) $(INC)
	@$(ECHO) "Linking $(NAME)..."
	@ar -rcs $(NAME) $(OBJ) 2>$(CC_LOG) || touch $(CC_ERROR)
	@if test -e $(CC_ERROR); then											\
		$(ECHO) "$(ERROR_STRING)\n" && $(CAT) $(CC_LOG);					\
	elif test -s $(CC_LOG); then											\
		$(ECHO) "$(WARN_STRING)\n" && $(CAT) $(CC_LOG);						\
	else																	\
		$(ECHO) "$(OK_STRING)\n";											\
	fi
	@$(RM) -f $(CC_LOG) $(CC_ERROR)

$(OBJ_D):
	@mkdir -p $(OBJ_D)
	@mkdir -p $(OBJ_D)/packaging
	@mkdir -p $(OBJ_D)/networking

$(OBJ): $(OBJ_D)/%.o: $(SRC_D)/%.c
	@$(ECHO) "Compiling $<..."
	@$(CC) $(CC_FLAGS) -I$(INC_D) -I$(SRC_D)/libft/inc -c $< -o $@ 2>$(CC_LOG)	\
		|| touch $(CC_ERROR)
	@if test -e $(CC_ERROR); then											\
		$(ECHO) "$(ERROR_STRING)\n" && $(CAT) $(CC_LOG);					\
	elif test -s $(CC_LOG); then											\
		$(ECHO) "$(WARN_STRING)\n" && $(CAT) $(CC_LOG);						\
	else																	\
		$(ECHO) "$(OK_STRING)\n";											\
	fi
	@$(RM) -f $(CC_LOG) $(CC_ERROR)

clean:
	@$(RM) $(OBJ)
	@$(RM) -r $(OBJ_D)

api_test: TEST='main_api_t'
api_test: $(NAME)
	@$(ECHO) "Compiling $(TEST).c..." 2>$(CC_LOG) || touch $(CC_ERROR)
	@$(CC) $(CC_FLAGS_TESTS) -I$(INC_D) -o $(TEST) tests/$(TEST).c $(NAME)
	@if test -e $(CC_ERROR); then                                           \
        $(ECHO) "$(ERROR_STRING)\n" && $(CAT) $(CC_LOG);					\
    elif test -s $(CC_LOG); then                                            \
        $(ECHO) "$(WARN_STRING)\n" && $(CAT) $(CC_LOG);                     \
    else                                                                    \
        $(ECHO) "$(OK_STRING)\n";                                           \
    fi
	@$(ECHO) "Running $(TEST)...\n"
	@./$(TEST) && $(RM) -f $(TEST) && $(RM) -rf $(TEST).dSYM 2>$(CC_LOG) || touch $(CC_ERROR)
	@if test -e $(CC_ERROR); then                                           \
		$(ECHO) "Completed $(TEST): $(ERROR_STRING)\n" && $(CAT) $(CC_LOG);		\
    elif test -s $(CC_LOG); then											\
		$(ECHO) "Completed $(TEST): $(WARN_STRING)\n" && $(CAT) $(CC_LOG);		\
    else                                                                    \
		$(ECHO) "Completed $(TEST): $(OK_STRING)\n";								\
    fi
	@$(RM) -f $(CC_LOG) $(CC_ERROR)

server_test: TEST='main_server_t'
server_test: $(NAME)
	@$(ECHO) "Compiling $(TEST).c..." 2>$(CC_LOG) || touch $(CC_ERROR)
	@$(CC) $(CC_FLAGS_TESTS) -I$(INC_D) -o $(TEST) tests/$(TEST).c $(NAME)
	@if test -e $(CC_ERROR); then                                           \
        $(ECHO) "$(ERROR_STRING)\n" && $(CAT) $(CC_LOG);					\
    elif test -s $(CC_LOG); then                                            \
        $(ECHO) "$(WARN_STRING)\n" && $(CAT) $(CC_LOG);                     \
    else                                                                    \
        $(ECHO) "$(OK_STRING)\n";                                           \
    fi
	@$(ECHO) "Running $(TEST)...\n"
	@./$(TEST) && $(RM) -f $(TEST) && $(RM) -rf $(TEST).dSYM 2>$(CC_LOG) || touch $(CC_ERROR)
	@if test -e $(CC_ERROR); then                                           \
		$(ECHO) "Completed $(TEST): $(ERROR_STRING)\n" && $(CAT) $(CC_LOG);		\
    elif test -s $(CC_LOG); then											\
		$(ECHO) "Completed $(TEST): $(WARN_STRING)\n" && $(CAT) $(CC_LOG);		\
    else                                                                    \
		$(ECHO) "Completed $(TEST): $(OK_STRING)\n";								\
    fi
	@$(RM) -f $(CC_LOG) $(CC_ERROR)

client_test: TEST='main_client_t'
client_test: $(NAME)
	@$(ECHO) "Compiling $(TEST).c..." 2>$(CC_LOG) || touch $(CC_ERROR)
	@$(CC) $(CC_FLAGS_TESTS) -I$(INC_D) -o $(TEST) tests/$(TEST).c $(NAME)
	@if test -e $(CC_ERROR); then                                           \
        $(ECHO) "$(ERROR_STRING)\n" && $(CAT) $(CC_LOG);					\
    elif test -s $(CC_LOG); then                                            \
        $(ECHO) "$(WARN_STRING)\n" && $(CAT) $(CC_LOG);                     \
    else                                                                    \
        $(ECHO) "$(OK_STRING)\n";                                           \
    fi
	@$(ECHO) "Running $(TEST)...\n"
	@./$(TEST) && $(RM) -f $(TEST) && $(RM) -rf $(TEST).dSYM 2>$(CC_LOG) || touch $(CC_ERROR)
	@if test -e $(CC_ERROR); then                                           \
		$(ECHO) "Completed $(TEST): $(ERROR_STRING)\n" && $(CAT) $(CC_LOG);		\
    elif test -s $(CC_LOG); then											\
		$(ECHO) "Completed $(TEST): $(WARN_STRING)\n" && $(CAT) $(CC_LOG);		\
    else                                                                    \
		$(ECHO) "Completed $(TEST): $(OK_STRING)\n";								\
    fi
	@$(RM) -f $(CC_LOG) $(CC_ERROR)

fclean: clean
	@$(RM) $(NAME)

re: fclean all

.PHONY = all clean fclean re test
