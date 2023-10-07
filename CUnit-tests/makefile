# Nome do seu executável
TARGET = tests-huffman

# Compilador
CC = gcc

# Opções de compilação
CFLAGS = -Wall -Iinclude -lcunit

# Diretórios
SRC_DIR = ../include
INCLUDE_DIR = ../include
BUILD_DIR = build

# Lista de arquivos fonte
SOURCES = $(wildcard $(SRC_DIR)/*.c)

# Lista de arquivos objeto gerados
OBJECTS = $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SOURCES))

# Regra de compilação do executável
$(TARGET): $(OBJECTS)
	$(CC) main.c $(CFLAGS) $^ -o $@

# Regra de compilação dos arquivos objeto
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Criação do diretório de build, se não existir
$(BUILD_DIR):
	mkdir $(BUILD_DIR)

# Regra para limpar os arquivos gerados
clean:
	rm -rf $(BUILD_DIR) $(TARGET)

# Indica que 'clean' não é um arquivo de saída
.PHONY: clean
