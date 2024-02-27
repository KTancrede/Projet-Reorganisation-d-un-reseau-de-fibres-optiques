CFLAGS = -g -Wall -Wextra -pedantic -Wno-unused-parameter 
CC = gcc
SRC_DIR = src
INCLUDE_DIR = header
BUILD_DIR = build

PROGRAMS = chaine_main

all: $(PROGRAMS)

chaine_main: $(BUILD_DIR)/ChaineMain.o $(BUILD_DIR)/Chaine.o $(BUILD_DIR)/SVGwriter.o -lm
	$(CC) -o $@ $(CFLAGS) $^

$(BUILD_DIR)/Chaine.o: $(SRC_DIR)/Chaine.c $(INCLUDE_DIR)/Chaine.h $(INCLUDE_DIR)/SVGwriter.h| $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/ChaineMain.o: $(SRC_DIR)/ChaineMain.c $(INCLUDE_DIR)/Chaine.h $(INCLUDE_DIR)/SVGwriter.h | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/SVGwriter.o: $(SRC_DIR)/SVGwriter.c $(INCLUDE_DIR)/SVGwriter.h | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

clean:
	rm -rf $(BUILD_DIR) $(PROGRAMS)
