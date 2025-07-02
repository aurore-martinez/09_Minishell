# 🗺️ Cartographie du Code - Minishell

Cette documentation présente l'architecture détaillée et l'organisation du code du projet Minishell.

## 📁 Structure générale du projet

```
09_Minishell/
├── main.c                    # Point d'entrée principal
├── Makefile                  # Configuration de compilation
├── minishell                 # Exécutable généré
├── include/
│   └── minishell.h          # Header principal avec toutes les définitions
├── src/                     # Code source organisé par modules
├── lib/                     # Bibliothèques externes (libft, ft_printf, gnl)
├── objs/                    # Fichiers objets de compilation
└── tests/                   # Scripts et outils de test
```

## 🏗️ Architecture modulaire

### 🎯 **main.c** - Point d'entrée
```c
main()
├── check_envp()             # Vérification environnement
├── init_shell()             # Initialisation structures globales
├── env_from_envp()          # Conversion environnement système
└── ft_prompt()              # Boucle principale du shell
```

### 📋 **include/minishell.h** - Définitions centrales

#### Structures de données principales :
```c
typedef struct s_shell       # Structure principale du shell
├── t_gc gc                  # Garbage collector
├── t_env *env               # Variables d'environnement
├── char **paths             # Chemins PATH
├── t_builtin builtins[8]    # Commandes intégrées
├── int exit_status          # Code de retour
└── t_std_backup std_backup  # Sauvegarde stdin/stdout/stderr

typedef struct s_cmd         # Représentation d'une commande
├── char *cmd                # Nom de la commande
├── char **args              # Arguments
├── int fd_in/fd_out         # Descripteurs de fichiers
├── int pid                  # ID du processus
├── int pipe[2]              # Pipe pour communication
├── t_bool is_builtin        # Flag builtin
├── t_redir *redir           # Liste des redirections
└── struct s_cmd *next/prev  # Liste chaînée

typedef struct s_token       # Token d'analyse lexicale
├── int token_type           # Type (WORD, PIPE, IN, OUT, etc.)
├── char *token_value        # Valeur du token
├── char *token_raw          # Valeur brute avec quotes
└── struct s_token *next/prev # Liste chaînée

typedef struct s_env         # Variable d'environnement
├── char *key                # Nom de la variable
├── t_bool equal             # Présence du signe =
├── char *value              # Valeur
└── struct s_env *next       # Liste chaînée
```

## 📦 Modules détaillés

### 🔧 **src/utils/** - Utilitaires et Garbage Collector

#### **Garbage Collector**
```
utils_gc.c
├── gc_mem()                 # Fonction principale de gestion mémoire
│   ├── GC_ALLOC            # Allocation avec tracking
│   ├── GC_FREE_ONE         # Libération d'un pointeur
│   └── GC_FREE_ALL         # Libération par type
├── gc_strdup()             # strdup avec tracking
├── gc_strndup()            # strndup avec tracking
├── gc_split()              # ft_split avec tracking
└── gc_strjoin()            # strjoin avec tracking

Types de GC :
├── GC_ENV                  # Variables d'environnement
├── GC_TKN                  # Tokens de parsing
├── GC_CMD                  # Structures de commandes
└── GC_TMP                  # Fichiers temporaires
```

#### **Utilitaires généraux**
```
utils.c
├── error_exit()            # Sortie d'erreur avec nettoyage
├── get_shell()             # Singleton pour structure shell
├── init_shell()            # Initialisation complète
└── free_gc_exit()          # Nettoyage avant sortie
```

### 🎨 **src/prompt/** - Interface utilisateur

```
prompt.c
└── ft_prompt()             # Boucle principale interactive
    ├── readline()          # Lecture ligne de commande
    ├── handle_ctrl_c()     # Gestion Ctrl+C
    ├── handle_ctrl_d()     # Gestion Ctrl+D
    ├── process_prompt_line() # Traitement d'une ligne
    │   ├── add_history()   # Ajout à l'historique
    │   ├── parsing()       # Analyse syntaxique
    │   └── exec_dispatcher() # Exécution
    └── signal management   # Basculement mode signaux
```

### 🔍 **src/parsing/** - Analyseur syntaxique et lexical

#### **Pipeline de parsing**
```
parsing.c
└── parsing()               # Fonction principale
    ├── first_syntax_check() # Vérification syntaxe basique
    ├── tokenization()      # Découpage en tokens
    ├── check_token()       # Validation tokens
    ├── handle_expansion()  # Expansion variables/quotes
    └── parse_tokens()      # Construction structures cmd
```

#### **Tokenisation**
```
token_*.c
├── token_operator.c        # Opérateurs (|, <, >, <<, >>)
├── token_utils.c           # Utilitaires tokenisation
└── token_lst_utils.c       # Gestion listes de tokens
```

#### **Expansion des variables**
```
expansion.c & expnd_*.c
├── handle_expansion()      # Point d'entrée expansion
├── quotes_first_segmentation() # Segmentation par quotes
├── scnd_segmentation()     # Segmentation variables
├── dquotes_scnd_segmentation() # Traitement double quotes
└── join_xpnd()            # Reconstruction finale
```

#### **Construction des commandes**
```
parsing_cmd.c
├── parse_tokens()          # Conversion tokens -> commandes
├── init_new_cmd()          # Initialisation nouvelle commande
├── handle_pipe_token()     # Gestion pipes
└── handle_redir_token()    # Gestion redirections
```

### ⚡ **src/exec/** - Moteur d'exécution

#### **Dispatcher principal**
```
exec_dispatcher.c
└── exec_dispatcher()       # Aiguillage exécution
    ├── handle_all_heredocs() # Traitement here-documents
    ├── exec_single_cmd()   # Commande simple
    └── exec_pipeline()     # Pipeline de commandes
```

#### **Commandes simples**
```
exec_single_cmd.c
└── exec_single_cmd()       # Exécution commande unique
    ├── is_valid_command()  # Validation commande
    ├── apply_redirections() # Application redirections
    ├── handle_builtin()    # Commandes intégrées
    └── execve()           # Commandes externes
```

#### **Pipelines**
```
pipes.c
└── exec_pipeline()         # Exécution pipeline
    ├── pipe()              # Création tubes
    ├── fork()              # Création processus enfants
    ├── dup2()              # Redirection descripteurs
    └── wait_for_children() # Attente fin processus
```

#### **Redirections**
```
redirections.c
├── apply_redirections()    # Application toutes redirections
├── handle_input_redir()    # Redirections entrée (< <<)
├── handle_output_redir()   # Redirections sortie (> >>)
└── restore_std()          # Restauration stdin/stdout/stderr
```

#### **Here-documents**
```
heredocs.c
├── handle_all_heredocs()   # Traitement tous heredocs
├── handle_heredoc()        # Traitement un heredoc
├── heredoc_childhood()     # Processus enfant heredoc
└── create_temp_file()      # Création fichier temporaire
```

#### **Gestion des chemins**
```
paths.c
├── find_command_path()     # Recherche commande dans PATH
├── is_directory()          # Vérification répertoire
└── access()               # Vérification droits d'accès
```

### 🔧 **src/builtins/** - Commandes intégrées

#### **Gestionnaire principal**
```
handle_builtins.c
├── handle_builtin()        # Dispatcher builtins
├── is_builtin()           # Détection builtin
└── builtins[8]            # Table de correspondance
```

#### **Commandes implémentées**
```
builtin_echo.c    → ft_echo()     # echo [-n] [args...]
builtin_pwd.c     → ft_pwd()      # pwd
builtin_cd.c      → ft_cd()       # cd [path]
builtin_env.c     → ft_env()      # env
builtin_export.c  → ft_export()   # export [var[=value]...]
builtin_unset.c   → ft_unset()    # unset [var...]
builtin_exit.c    → ft_exit()     # exit [status]
```

### 🌍 **src/env/** - Gestion environnement

```
env.c
├── env_from_envp()         # Conversion char** -> t_env*
├── env_to_env_tab_for_execve() # Conversion t_env* -> char**
├── get_env_value()         # Récupération valeur variable
├── update_env_value()      # Modification variable
├── env_new()              # Création nouvelle variable
└── env_add_back()         # Ajout à la liste
```

### 📡 **src/signal/** - Gestion des signaux

```
signal.c
├── init_signals()          # Initialisation gestionnaires
├── signal_handler()        # Gestionnaire mode interactif
├── signal_handler_exec()   # Gestionnaire mode exécution
├── signal_handler_heredoc() # Gestionnaire heredoc
├── set_signals_interactive() # Mode interactif
└── set_signals_exec()      # Mode exécution
```

### 🐛 **src/debug/** - Outils de débogage

```
debug.c
├── debug_env()             # Affichage environnement
├── debug_cmd()             # Affichage commande
├── debug_cmd_list()        # Affichage liste commandes
└── debug_pipe()            # Affichage état pipes
```

## 🔄 Flux d'exécution principal

```
1. main()
   └── ft_prompt()

2. ft_prompt() [BOUCLE INFINIE]
   ├── readline() → ligne de commande
   ├── process_prompt_line()
   │   ├── parsing()
   │   │   ├── first_syntax_check()
   │   │   ├── tokenization()
   │   │   ├── handle_expansion()
   │   │   └── parse_tokens() → t_cmd*
   │   └── exec_dispatcher()
   │       ├── handle_all_heredocs()
   │       ├── exec_single_cmd() OU exec_pipeline()
   │       └── wait_for_children()
   └── gc_mem(GC_FREE_ALL) → nettoyage
```

## 🧠 Algorithmes clés

### **Expansion des variables**
1. **Segmentation par quotes** : Découpage selon ' et "
2. **Segmentation par variables** : Identification $VAR dans chaque segment
3. **Résolution** : Remplacement par valeurs d'environnement
4. **Reconstruction** : Jointure de tous les segments

### **Gestion des pipes**
1. **Création pipe()** pour chaque commande sauf la dernière
2. **Fork()** pour chaque commande
3. **Redirection** : stdin/stdout vers pipes appropriés
4. **Fermeture** descripteurs inutiles
5. **Attente** de tous les processus enfants

### **Here-documents**
1. **Détection** de tous les `<<` avant exécution
2. **Fork()** pour chaque heredoc
3. **Lecture** ligne par ligne jusqu'au délimiteur
4. **Écriture** dans fichier temporaire `/tmp/minishell_heredoc_*`
5. **Remplacement** redirection par fichier temporaire

## 📊 Métriques du code

- **Fichiers sources** : ~40 fichiers .c
- **Lignes de code** : ~3000 lignes
- **Fonctions** : ~150 fonctions
- **Structures** : 15 structures principales
- **Complexité cyclomatique** : Maintenue sous 10 par fonction
- **Compliance** : 100% Norm 42

## 🔍 Points d'optimisation implémentés

1. **Garbage Collector** : Prévention automatique des leaks
2. **Structures chaînées** : Allocation dynamique optimisée
3. **Parsing en une passe** : Évite multiple parcours
4. **Réutilisation processus** : Builtins sans fork()
5. **Fichiers temporaires** : Gestion automatique heredocs

---

*Cette cartographie reflète l'architecture au 27/06/2025*
