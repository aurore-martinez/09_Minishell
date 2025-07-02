# 🐚 Minishell

*As beautiful as a shell*

Un shell Unix simple et fonctionnel développé en C, reproduisant les fonctionnalités de base de bash.

## 📋 Table des matières

- [Description](#description)
- [Fonctionnalités implémentées](#fonctionnalités-implémentées)
- [Compilation et utilisation](#compilation-et-utilisation)
- [Architecture du projet](#architecture-du-projet)
- [Tests](#tests)
- [Auteurs](#auteurs)

## 🎯 Description

Minishell est une implémentation simplifiée d'un shell Unix, développée dans le cadre du curriculum de l'École 42. Ce projet permet de comprendre en profondeur le fonctionnement des processus, des descripteurs de fichiers, et de l'interface entre l'utilisateur et le système d'exploitation.

## ✨ Fonctionnalités implémentées

### 🔹 Fonctionnalités de base
- ✅ **Prompt interactif** avec couleurs personnalisées (`minishell:`)
- ✅ **Historique des commandes** avec support de readline
- ✅ **Recherche et exécution** des commandes via PATH ou chemins absolus/relatifs
- ✅ **Gestion des signaux** :
  - `Ctrl+C` : Nouveau prompt sur nouvelle ligne
  - `Ctrl+D` : Sortie du shell
  - `Ctrl+\` : Ignoré (comme dans bash)

### 🔹 Gestion des quotes
- ✅ **Simple quotes (`'`)** : Empêche l'interprétation des métacaractères
- ✅ **Double quotes (`"`)** : Permet l'expansion des variables (`$`)

### 🔹 Redirections
- ✅ **`<`** : Redirection d'entrée
- ✅ **`>`** : Redirection de sortie
- ✅ **`<<`** : Here-document avec délimiteur
- ✅ **`>>`** : Redirection de sortie en mode append

### 🔹 Pipes
- ✅ **`|`** : Pipeline entre commandes
- ✅ **Pipes multiples** : Support des chaînes de commandes complexes

### 🔹 Variables d'environnement
- ✅ **Expansion des variables** (`$VAR`)
- ✅ **`$?`** : Code de retour de la dernière commande
- ✅ **Gestion complète de l'environnement**

### 🔹 Commandes intégrées (builtins)
- ✅ **`echo`** avec option `-n`
- ✅ **`cd`** avec chemins relatifs et absolus
- ✅ **`pwd`** sans options
- ✅ **`export`** sans options
- ✅ **`unset`** sans options
- ✅ **`env`** sans options ni arguments
- ✅ **`exit`** sans options

### 🔹 Fonctionnalités avancées
- ✅ **Garbage Collector** personnalisé pour la gestion mémoire
- ✅ **Gestion d'erreurs** robuste avec messages informatifs
- ✅ **Validation syntaxique** des commandes
- ✅ **Support des here-documents** avec fichiers temporaires
- ✅ **Détection automatique** des commandes intégrées vs externes

## 🚀 Compilation et utilisation

### Prérequis
- `gcc` ou `clang`
- `make`
- `readline` library
- Système Unix/Linux

### Compilation
```bash
# Compilation standard
make

# Compilation en mode debug
make debug

# Nettoyage
make clean      # Supprime les fichiers objets
make fclean     # Supprime tout
make re         # Recompile tout
```

### Utilisation
```bash
# Lancement du shell
./minishell

# Exemples d'utilisation
minishell: echo "Hello World"
minishell: ls -la | grep .c
minishell: cat < input.txt > output.txt
minishell: echo "test" >> file.txt
minishell: cat << EOF
> Hello
> World
> EOF
```

## 🏗️ Architecture du projet

Le projet suit une architecture modulaire claire :

```
09_Minishell/
├── main.c                    # Point d'entrée principal
├── include/
│   └── minishell.h          # Définitions et prototypes
├── src/
│   ├── builtins/            # Commandes intégrées
│   ├── env/                 # Gestion des variables d'environnement
│   ├── exec/                # Moteur d'exécution
│   ├── parsing/             # Analyseur syntaxique et lexical
│   ├── prompt/              # Interface utilisateur
│   ├── signal/              # Gestion des signaux
│   └── utils/               # Utilitaires et garbage collector
├── lib/                     # Bibliothèques externes
└── tests/                   # Scripts de test
```

### 🔧 Modules principaux

#### **Parsing** (`src/parsing/`)
- **Tokenisation** : Découpage de la ligne en tokens
- **Analyse syntaxique** : Validation de la grammaire
- **Expansion** : Résolution des variables et quotes
- **Structure de données** : Construction de l'AST des commandes

#### **Exécution** (`src/exec/`)
- **Dispatcher** : Aiguillage entre commandes simples et pipelines
- **Pipes** : Gestion des tubes de communication
- **Redirections** : Manipulation des descripteurs de fichiers
- **Here-documents** : Traitement des entrées multi-lignes

#### **Builtins** (`src/builtins/`)
- Implémentation de toutes les commandes intégrées
- Interface uniforme avec le système d'exécution
- Gestion des codes de retour

#### **Environnement** (`src/env/`)
- Structure de données pour les variables
- Fonctions d'accès et de modification
- Conversion vers format execve

#### **Garbage Collector** (`src/utils/`)
- Système de gestion mémoire automatisée
- Classification par types (ENV, TKN, CMD, TMP)
- Prévention des fuites mémoire

## 🧪 Tests

Le projet inclut plusieurs outils de test :

```bash
# Tests basiques
bash tests/tests.sh

# Tests d'expansion
bash tests/expand_test.sh

# Tests avec minishell_tester externe
cd tests/minishell_tester && bash test.sh
```

### Cas de test couverts
- ✅ Commandes simples et complexes
- ✅ Pipes multiples
- ✅ Redirections combinées
- ✅ Gestion des erreurs
- ✅ Variables d'environnement
- ✅ Here-documents
- ✅ Gestion des signaux

## 🛠️ Fonctionnalités techniques

### Gestion mémoire
- **Garbage Collector** personnalisé avec classification par types
- **Libération automatique** à la fin de chaque commande
- **Prévention des leaks** même en cas d'interruption

### Robustesse
- **Validation syntaxique** complète avant exécution
- **Gestion d'erreurs** avec messages informatifs
- **Restauration des descripteurs** après redirections
- **Nettoyage automatique** des fichiers temporaires

### Performance
- **Parsing en une passe** avec structures optimisées
- **Réutilisation des processus** pour les builtins
- **Gestion efficace** des pipes et redirections

## 📊 Statistiques du projet

- **~3000 lignes** de code C
- **40+ fichiers** sources organisés en modules
- **100% Norm** compliant (norme 42)
- **0 memory leaks** (validé avec Valgrind)
- **Support complet** des spécifications bash essentielles

## 👥 Auteurs

- **aumartin** - Développement principal
- **tjacquel** - Développement et tests

---

*Développé dans le cadre du cursus de l'École 42*
