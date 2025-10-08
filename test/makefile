CC = gcc
CFLAGS = -Wall -Wextra -g -std=c99 -O1

# Chemins
SRCDIR = ../src
SOURCES = $(wildcard $(SRCDIR)/*.c)
SOURCES := $(filter-out $(SRCDIR)/main.c, $(SOURCES))

# Fichiers test
TEST_SOURCES = $(wildcard test_*.c)
TEST_EXECUTABLES = $(TEST_SOURCES:.c=.exe)

all: run_tests

# Compiler directement tous les .c ensemble
%.exe: %.c $(SOURCES)
	$(CC) $(CFLAGS) $< $(SOURCES) -o $@

run_tests: $(TEST_EXECUTABLES)
	@echo "=== LANCEMENT DES TESTS ==="
	@for test in $^; do \
		echo "----------------------------------------"; \
		echo "Exécution de $$test..."; \
		echo "----------------------------------------"; \
		./$$test; \
		echo ""; \
	done
	@echo "=== TOUS LES TESTS SONT TERMINÉS ==="

clean:
	rm -f *.exe

.PHONY: all run_tests clean


# CC = gcc
# CFLAGS = -Wall -Wextra -g -std=c99

# # Chaque test est compilé séparément
# test_fonctions.exe: test_fonctions.c ../fonctions.c
# 	$(CC) $(CFLAGS) test_fonctions.c ../fonctions.c -o $@

# test_combat.exe: test_combat.c ../fonctions.c
# 	$(CC) $(CFLAGS) test_combat.c ../fonctions.c -o $@

# # all compile et exécute CHAQUE test SÉPARÉMENT
# all: 
# 	@echo "=== COMPILATION ET EXÉCUTION DES TESTS ==="
# 	@echo ""
# 	@echo "1. Test fonctions:"
# 	@make test_fonctions.exe
# 	@./test_fonctions.exe
# 	@echo ""
# 	@echo "2. Test combat:"
# 	@make test_combat.exe
# 	@./test_combat.exe
# 	@echo ""
# 	@echo "=== TOUS LES TESTS TERMINÉS ==="

# # Alternative : seulement compiler
# build: test_fonctions.exe test_combat.exe

# # Exécuter sans recompiler
# run: build
# 	@./test_fonctions.exe
# 	@echo ""
# 	@./test_combat.exe

# clean:
# 	rm -f *.exe

# .PHONY: all build run clean