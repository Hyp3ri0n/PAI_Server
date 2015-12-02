# Oct.2015 - L3 Miage
# Fernandes Aurélien & Martinier Alexis

#Definition du Compilateur
CC = gcc

#Definition des repertoires
SRCS_DIR = src
BIN_DIR = bin
OBJS_DIR = objets
HEAD_DIR = includes

#Definition des sources & objets neccesaire a la compilation
SRCS = $(wildcard $(SRCS_DIR)/*.c)
OBJS = $(SRCS:$(SRCS_DIR)/%.c=$(OBJS_DIR)/%.o)

#Definition des Flags pour la compilation
#-D'NB_NODE=4' -> definition de la constante NB_NODE à 4
#-I$(HEAD_DIR) -> inclusion des .h
#-std=c99      -> demande a compiler avec le standard c99 (permet entre autre la déclaration de variable dans des conditions (int i dans un for))
CFLAGS = -I$(HEAD_DIR) -std=c99 -D'NB_NODE=4' -D'NB_DIGIT=5' -w

#Definition du programme principal
EXEC = $(BIN_DIR)/dictionnaire

all: $(EXEC)

#Generation de l'executable
$(EXEC): $(OBJS)
	@echo "Compilation ..."
	@$(CC) $(OBJS) -o $(EXEC)
	@echo "done"

#Generarion des .o depuis les .c
$(OBJS_DIR)/%.o: $(SRCS_DIR)/%.c
	@$(CC) $(CFLAGS) -c $< -o $@

#Regle de nettoyage
clean:
	@echo "Delete executable & objets files"
	@rm $(OBJS)
	@rm $(EXEC)
	@echo "done"
