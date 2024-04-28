CFLAGS = -g -Wall -Wextra -pedantic -Wno-unused-parameter 
CC = gcc
SRC_DIR = src
INCLUDE_DIR = header
BUILD_DIR = build

PROGRAMS = chaine_main reconstitue_Reseau Chrono

all: $(PROGRAMS)

chaine_main: $(BUILD_DIR)/ChaineMain.o $(BUILD_DIR)/Chaine.o $(BUILD_DIR)/SVGwriter.o $(BUILD_DIR)/Reseau.o $(BUILD_DIR)/Hachage.o $(BUILD_DIR)/ArbreQuat.o -lm
	$(CC) -o $@ $(CFLAGS) $^

reconstitue_Reseau: $(BUILD_DIR)/Reconstitue_Reseau.o $(BUILD_DIR)/Chaine.o $(BUILD_DIR)/Struct_File.o $(BUILD_DIR)/SVGwriter.o $(BUILD_DIR)/Reseau.o $(BUILD_DIR)/Hachage.o $(BUILD_DIR)/ArbreQuat.o $(BUILD_DIR)/Graphe.o -lm
	$(CC) -o $@ $(CFLAGS) $^

Chrono: $(BUILD_DIR)/Chrono.o $(BUILD_DIR)/Chaine.o $(BUILD_DIR)/SVGwriter.o $(BUILD_DIR)/Reseau.o $(BUILD_DIR)/Hachage.o $(BUILD_DIR)/ArbreQuat.o $(BUILD_DIR)/Generateur.o -lm
	$(CC) -o $@ $(CFLAGS) $^

$(BUILD_DIR)/Chaine.o: $(SRC_DIR)/Chaine.c $(INCLUDE_DIR)/Chaine.h $(INCLUDE_DIR)/SVGwriter.h | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/ChaineMain.o: $(SRC_DIR)/ChaineMain.c $(INCLUDE_DIR)/Chaine.h $(INCLUDE_DIR)/SVGwriter.h | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/SVGwriter.o: $(SRC_DIR)/SVGwriter.c $(INCLUDE_DIR)/SVGwriter.h | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/Reseau.o: $(SRC_DIR)/Reseau.c $(INCLUDE_DIR)/Reseau.h | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/Reconstitue_Reseau.o: $(SRC_DIR)/Reconstitue_Reseau.c $(INCLUDE_DIR)/Reseau.h $(INCLUDE_DIR)/Chaine.h $(INCLUDE_DIR)/Hachage.h $(INCLUDE_DIR)/Graphe.h | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/Chrono.o: $(SRC_DIR)/Chrono.c $(INCLUDE_DIR)/Reseau.h $(INCLUDE_DIR)/Chaine.h $(INCLUDE_DIR)/Hachage.h $(INCLUDE_DIR)/Generateur.h | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/Hachage.o: $(SRC_DIR)/Hachage.c $(INCLUDE_DIR)/Hachage.h | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/ArbreQuat.o: $(SRC_DIR)/ArbreQuat.c $(INCLUDE_DIR)/ArbreQuat.h | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/Generateur.o: $(SRC_DIR)/Generateur.c $(INCLUDE_DIR)/Generateur.h | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/Struct_File.o: $(SRC_DIR)/Struct_File.c $(INCLUDE_DIR)/Struct_File.h | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/Graphe.o: $(SRC_DIR)/Graphe.c $(INCLUDE_DIR)/Graphe.h  | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

clean:
	rm -rf $(BUILD_DIR) $(PROGRAMS) *.html *.txt
