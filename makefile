CC = gcc
CFLAGS = -Wall -Wextra -g -std=c99 -O1

TARGET = oceandepth
SRCDIR = src
TESTDIR = test

SOURCES = $(wildcard $(SRCDIR)/*.c)
#SOURCES := $(filter-out $(SRCDIR)/main.c, $(SOURCES))

$(TARGET): $(SOURCES)
	$(CC) $(CFLAGS) $(SOURCES) -o $(TARGET)

# CORRECTION : Appeler la cible 'run' au lieu de 'all'
test:
	@echo "=== COMPILATION ET EXÉCUTION DES TESTS ==="
	$(MAKE) -C $(TESTDIR) run_tests

clean:
	rm -f $(TARGET)
	
clean_test:
	$(MAKE) -C $(TESTDIR) clean

debug: CFLAGS += -DDEBUG
debug: $(TARGET)

.PHONY: clean debug test
