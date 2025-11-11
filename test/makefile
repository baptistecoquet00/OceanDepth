CC = gcc
CFLAGS = -Wall -Wextra -g -std=c99 -O1 -I../src

# Chemins
SRCDIR = ../src
OBJDIR = ../obj

# Fichiers test
TEST_SOURCES = $(wildcard test_*.c)
TEST_EXECUTABLES = $(TEST_SOURCES:.c=.exe)

all: run_tests

# Compiler directement depuis les sources (plus simple !)
%.exe: %.c
	$(CC) $(CFLAGS) $< $(shell find $(SRCDIR) -name "*.c" ! -name "main.c") -o $@

run_tests: $(TEST_EXECUTABLES)
	@echo "=== LANCEMENT DES TESTS ==="
	@for test in $(TEST_EXECUTABLES); do \
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
