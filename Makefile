NAME = ito.a

# directories
SRC_D = src
OBJ_D = obj
INC_D = inc

# C source and header files
SRC =	$(SRC_D)/api.c														\
		$(SRC_D)/package_compilation.c										\
		$(SRC_D)/error_handling.c

INC =	$(INC_D)/ito.h

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

$(OBJ): $(OBJ_D)/%.o: $(SRC_D)/%.c
	@$(ECHO) "Compiling $<..."
	@$(CC) -I$(INC_D) -I$(SRC_D)/libft/inc $(CC_FLAGS) -c $< -o $@ 2>$(CC_LOG) 	\
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

fclean: clean
	@$(RM) $(NAME)

re: fclean all

.PHONY = all clean fclean re
