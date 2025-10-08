CC = gcc
CFLAGS = -Wall -Wextra -g -std=c99 -O1

TARGET = oceandepth
SRCDIR = src
OBJDIR = obj
TESTDIR = test

# Recherche récursive de tous les .c SAUF main.c
SOURCES = $(shell find $(SRCDIR) -name "*.c")
SOURCES := $(filter-out %/main.c, $(SOURCES))

# Extraire juste les noms de fichiers sans chemins
# src/combat.c -> combat.o
# src/sousdossier/perso.c -> perso.o
SOURCE_NAMES = $(notdir $(SOURCES))
OBJECTS = $(SOURCE_NAMES:%.c=$(OBJDIR)/%.o)

# Créer le dossier obj
$(shell mkdir -p $(OBJDIR))

$(TARGET): $(OBJDIR)/main.o $(OBJECTS)
	$(CC) $(CFLAGS) $^ -o $(TARGET)

# Règle pour main.o
$(OBJDIR)/main.o: $(SRCDIR)/main.c
	$(CC) $(CFLAGS) -c $< -o $@

# Règle générique pour TOUS les fichiers objets à la racine de obj/
$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/%.o: $(SRCDIR)/**/%.c
	$(CC) $(CFLAGS) -c $< -o $@

test:
	@echo "=== COMPILATION ET EXÉCUTION DES TESTS ==="
	$(MAKE) -C $(TESTDIR) run_tests

clean:
	rm -f $(TARGET)
	rm -rf $(OBJDIR)
	
clean_test:
	$(MAKE) -C $(TESTDIR) clean

debug: CFLAGS += -DDEBUG
debug: $(TARGET)

.PHONY: clean debug test