CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -Iinclude
SRCDIR = src
INCDIR = include
OBJ = $(SRCDIR)/main.o $(SRCDIR)/joueur.o $(SRCDIR)/creatures.o $(SRCDIR)/combat.o $(SRCDIR)/inventaire.o $(SRCDIR)/sauvegarde.o $(SRCDIR)/carte.o
EXEC = oceandepths

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

$(SRCDIR)/main.o: $(SRCDIR)/main.c
	$(CC) $(CFLAGS) -c $< -o $@

$(SRCDIR)/joueur.o: $(SRCDIR)/joueur.c
	$(CC) $(CFLAGS) -c $< -o $@

$(SRCDIR)/creatures.o: $(SRCDIR)/creatures.c
	$(CC) $(CFLAGS) -c $< -o $@

$(SRCDIR)/combat.o: $(SRCDIR)/combat.c
	$(CC) $(CFLAGS) -c $< -o $@

$(SRCDIR)/inventaire.o: $(SRCDIR)/inventaire.c
	$(CC) $(CFLAGS) -c $< -o $@

$(SRCDIR)/sauvegarde.o: $(SRCDIR)/sauvegarde.c
	$(CC) $(CFLAGS) -c $< -o $@

$(SRCDIR)/carte.o: $(SRCDIR)/carte.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(SRCDIR)/*.o $(EXEC)
