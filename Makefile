V = false

ifeq ($(V),true)
	ECHO =
else
	ECHO = @
endif

# Nom du programme
NAME = minishell

# Compilation
CC = cc
CFLAGS = -Wall -Wextra -Werror -g3 #-fsanitize=address
OS = $(shell uname | tr '[:upper:]' '[:lower:]')

MAKE = make -sC
MKDIR = mkdir -p
RM = rm -rf

# Lib perso
LIB_DIR = lib
LIB = $(LIB_DIR)/lib.a
LINKER = $(LIB)

# Info système
$(info 🖥️ OS détecté : $(OS))

# Bibliothèques système
ifeq ($(OS), linux)
	LINKER += -lreadline -L/usr/lib
	INCLUDE_FLAG += -I/usr/include
else ifeq ($(OS), darwin)
	LINKER += -lreadline -L/opt/homebrew/opt/readline/lib
	INCLUDE_FLAG += -I/opt/homebrew/opt/readline/include
endif

# Includes
INCLUDE_DIR = include
LIB_SUBDIR = $(wildcard $(LIB_DIR)/*)

INCLUDE_FLAG = -I$(INCLUDE_DIR) \
			   $(foreach dir, $(LIB_SUBDIR), -I$(dir))

INCLUDE = $(wildcard $(INCLUDE_DIR)/*.h) \
		  $(foreach dir, $(LIB_SUBDIR), $(wildcard $(dir)/*.h))

# Dossiers sources
SRC_DIR = src/

# Fichiers sources (relatifs à SRC_DIR)
SRC_BUILTINS = \
	builtins/builtin_cd.c \
	builtins/builtin_echo.c \
	builtins/builtin_env.c \
	builtins/builtin_exit.c \
	builtins/builtin_export.c \
	builtins/builtin_export_utils.c \
	builtins/builtin_pwd.c \
	builtins/builtin_unset.c \
	builtins/handle_builtins.c

SRC_UTILS = \
	utils/error_management.c \
	utils/garbage_collector.c \
	utils/gc_utils.c \
	utils/init_struct.c \
	utils/functions_utils.c \
	utils/cleanup.c \
	utils/gc_utils2.c \

SRC_PROMPT = \
	prompt/prompt.c \

SRC_ENV = \
	env/print_env.c \
	env/env_build.c \
	env/env_tab_build.c \
	env/env_access.c \

SRC_EXEC = \
	exec/paths.c \
	exec/checks_full_cmd.c \
	exec/exec_dispatcher.c \
	exec/exec_single_cmd.c \
	exec/heredocs.c \
	exec/pipes.c \
	exec/redirections.c \

SRC_PARSING = \
	parsing/parsing.c \
	parsing/parsing_first_syntax_check.c \
	parsing/parsing_lst_utils.c \
	parsing/parsing_helper.c \
	parsing/token_utils.c \
	parsing/token_lst_utils.c \
	parsing/token_operator.c \
	parsing/token_check.c \
	parsing/parsing_cmd.c \
	parsing/parsing_cmd_utils.c \
	parsing/parsing_redir.c \
	parsing/expansion.c \
	parsing/expnd_lst_utils.c \
	parsing/expnd_lst_helper.c \
	parsing/expnd_utils.c \
	parsing/expnd_post_sgmt.c \
	parsing/expnd_first_sgmt.c \
	parsing/expnd_scnd_sgmt.c \
	parsing/expnd_scnd_sgmt_dquotes.c \



SRC_SIGNAL = \
	signal/signal.c \
	signal/signal_helper.c \

SRC_DEBUG = \
	debug/debug_env.c \
	debug/debug_cmd.c \

SRC_FILES = main.c $(SRC_BUILTINS) \
			$(SRC_UTILS) $(SRC_PROMPT) \
			$(SRC_ENV) \
			$(SRC_EXEC) \
			$(SRC_PARSING) \
			$(SRC_DEBUG) \
			$(SRC_SIGNAL) \


# Chemins complets des sources et objets
SRCS = $(addprefix $(SRC_DIR), $(SRC_FILES))
OBJS_DIR = objs/
OBJS = $(addprefix $(OBJS_DIR), $(SRC_FILES:.c=.o))

# Couleurs ANSI
GREEN = \033[32m
RED = \033[31m
RESET = \033[0m
YELLOW = \033[33m
CLEAR_LINE = \033[2K\r

# Logo cool
PRINT_LOGO = \
	if [ "$(SILENT)" = "false" ]; then \
	echo "   _____ __ __    ___  _      _      ___ ___   ____  ______    ___  _____"; \
	echo "  / ___/|  |  |  /  _]| |    | |    |   |   | /    ||      |  /  _]/ ___/"; \
	echo " (   \_ |  |  | /  [_ | |    | |    | _   _ ||  o  ||      | /  [_(   \_ "; \
	echo "  \__  ||  _  ||    _]| |___ | |___ |  \_/  ||     ||_|  |_||    _]\__  |"; \
	echo "  /  \ ||  |  ||   [_ |     ||     ||   |   ||  _  |  |  |  |   [_ /  \ |"; \
	echo "  \    ||  |  ||     ||     ||     ||   |   ||  |  |  |  |  |     |\    |"; \
	echo "   \___||__|__||_____||_____||_____||___|___||__|__|  |__|  |_____| \___|"; \
	echo "                                                                         "; \
	echo "                                           "; \
	echo "              SHELLMATES                  "; \
	echo ""; \
	echo "  ┌─────────────────────────────┐"; \
	echo "  │  Partnered to code & debug  │"; \
	echo "  │   through bugs and segfault │"; \
	echo "  │    for better or for bash   │"; \
	echo "  └─────────────────────────────┘"; \
	echo "                                           "; \
	echo "     tjacquel 🤝 aumartin"

# Compilation principale
all: $(LIB) $(NAME)
# $(PRINT_LOGO)

# Compilation lib
$(LIB):
	@echo "$(YELLOW)📦 Compilation de la lib...$(RESET)\r"
	@$(MAKE) $(LIB_DIR) > /dev/null 2>&1 \
	&& echo -e "$(CLEAR_LINE)✅ Compilation lib réussie (✔)" \
	|| { echo -e "$(CLEAR_LINE)❌ Erreur : Compilation de la lib échouée (✘)"; exit 1; }

# Règle pour objets
$(OBJS_DIR):
	@$(MKDIR) $(OBJS_DIR)

$(OBJS_DIR)%.o: $(SRC_DIR)%.c $(INCLUDE)
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) $(INCLUDE_FLAG) -c $< -o $@

$(OBJS_DIR)main.o: main.c $(INCLUDE) | $(OBJS_DIR)
	@$(CC) $(CFLAGS) $(INCLUDE_FLAG) -c $< -o $(OBJS_DIR)main.o

# Compilation du binaire
$(NAME): $(OBJS) $(LIB)
	@echo "🚀 Compilation de $(NAME)..."
	@$(CC) $(CFLAGS) $(OBJS) $(LINKER) -o $(NAME) \
	&& echo "✅ $(NAME) a été créé avec succès (✔)" \

# Nettoyage
clean:
	@echo "$(YELLOW)🧹 Nettoyage clean en cours...$(RESET)\r"
	@$(RM) $(OBJS_DIR)
	@echo -e "$(CLEAR_LINE)✅ Nettoyage clean réussi (✔)"

fclean: clean
	@echo "$(YELLOW)🧼 Nettoyage complet fclean en cours...$(RESET)\r"
	@$(RM) $(NAME)
	@$(MAKE) $(LIB_DIR) fclean
	@echo -e "$(CLEAR_LINE)✅ Nettoyage complet fclean réussi (✔)"

re: fclean all

.PHONY: all clean fclean re
