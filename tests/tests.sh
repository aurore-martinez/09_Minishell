#!/bin/bash

# === CONFIGURATION ===
MINISHELL=./minishell
BASH=$(which bash)
COMMANDS_FILE=commands_test.txt
MINISHELL_OUT=minishell_output.txt
BASH_OUT=bash_output.txt
DIFF_FILE=diff_output.txt

echo "🧪 Running shell comparison on command file: $COMMANDS_FILE"

rm -f $MINISHELL_OUT $BASH_OUT $DIFF_FILE

# Execution avec Minishell
echo "Executing commands with Minishell..." >> $MINISHELL_OUT
while IFS= read -r line || [[ -n "$line" ]]; do
  echo "$ $line" >> $MINISHELL_OUT
  echo "$line" | $MINISHELL >> $MINISHELL_OUT 2>&1
  echo "----------------" >> $MINISHELL_OUT
done < "$COMMANDS_FILE"

# Execution avec Bash
echo "Executing commands with Bash..." >> $BASH_OUT
while IFS= read -r line || [[ -n "$line" ]]; do
  echo "$ $line" >> $BASH_OUT
  echo "$line" | $BASH --posix >> $BASH_OUT 2>&1 
  echo "----------------" >> $BASH_OUT
done < "$COMMANDS_FILE"

# Comparaison
diff -u $BASH_OUT $MINISHELL_OUT > $DIFF_FILE

echo ""
if [ -s "$DIFF_FILE" ]; then
  echo "❌ Differences found. Check $DIFF_FILE"
else
  echo "✅ No differences. Output matches Bash."
fi
