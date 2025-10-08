CC = gcc
CFLAGS = -Wall -Wextra -g -std=c99 -O1

TARGET = oceandepth
SRCDIR = src
OBJDIR = obj
TESTDIR = test
BACKUPDIR = backup

# Recherche récursive de tous les .c SAUF main.c
SOURCES = $(shell find $(SRCDIR) -name "*.c")
SOURCES := $(filter-out %/main.c, $(SOURCES))

# Extraire juste les noms de fichiers sans chemins
SOURCE_NAMES = $(notdir $(SOURCES))
OBJECTS = $(SOURCE_NAMES:%.c=$(OBJDIR)/%.o)

# Créer les dossiers
$(shell mkdir -p $(OBJDIR) $(BACKUPDIR))

$(TARGET): $(OBJDIR)/main.o $(OBJECTS)
	$(CC) $(CFLAGS) $^ -o $(TARGET)
	@echo " Compilation réussie: $(TARGET)"
	$(MAKE) backup

# Règle pour main.o
$(OBJDIR)/main.o: $(SRCDIR)/main.c
	$(CC) $(CFLAGS) -c $< -o $@

# Règle générique pour TOUS les fichiers objets à la racine de obj/
$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/%.o: $(SRCDIR)/**/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Règle pour créer un backup numéroté
BACKUP_COUNT = $(shell ls -1 $(BACKUPDIR)/$(TARGET)_backup_* 2>/dev/null | wc -l)
BACKUP_NAME = $(TARGET)_backup_$(shell printf "%02d" $(shell echo $$(($(BACKUP_COUNT)+1))))

backup:
	@cp $(TARGET) $(BACKUPDIR)/$(BACKUP_NAME)
	@echo " Backup créé: $(BACKUPDIR)/$(BACKUP_NAME)"

# Règle pour lister les backups
list-backups:
	@echo " Backups disponibles:"
	@ls -la $(BACKUPDIR)/$(TARGET)_backup_* 2>/dev/null || echo "Aucun backup trouvé"

# Règle pour restaurer un backup spécifique
restore:
	@if [ -n "$(VERSION)" ]; then \
		cp $(BACKUPDIR)/$(TARGET)_backup_$(VERSION) $(TARGET); \
		echo " Backup $(VERSION) restauré"; \
	else \
		echo "Usage: make restore VERSION=01"; \
		echo "Backups disponibles:"; \
		ls -1 $(BACKUPDIR)/$(TARGET)_backup_* 2>/dev/null | xargs -n1 basename || echo "Aucun backup"; \
	fi

# Règle pour clean les backups
clean-backups:
	rm -f $(BACKUPDIR)/$(TARGET)_backup_*
	@echo " Tous les backups supprimés"

test:
	@echo "=== COMPILATION ET EXÉCUTION DES TESTS ==="
	$(MAKE) -C $(TESTDIR) run_tests

clean:
	rm -f $(TARGET)
	rm -rf $(OBJDIR)
	@echo " Nettoyage terminé"
	
clean_test:
	$(MAKE) -C $(TESTDIR) clean

debug: CFLAGS += -DDEBUG
debug: $(TARGET)

.PHONY: clean debug test backup list-backups restore clean-backups