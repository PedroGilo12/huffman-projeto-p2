# Nome do seu executável
TARGET = GZIP_V1

# Compilador
CC = gcc

# Ferramenta de recursos para Windows (windres)
WINDRES = windres

# Opções de compilação
CFLAGS = -Wall -Iinclude

# Diretórios
SRC_DIR = include
INCLUDE_DIR = include
BUILD_DIR = build

# Lista de arquivos fonte
SOURCES = $(wildcard $(SRC_DIR)/*.c)

# Lista de arquivos objeto gerados
OBJECTS = $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SOURCES))

# Arquivo de recurso para o ícone
ICON_RESOURCE = icon.res

# Regra de compilação do executável
$(TARGET): $(OBJECTS) $(ICON_RESOURCE)
	$(CC) main.c $(CFLAGS) $^ -o $@

# Regra de compilação dos arquivos objeto
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Regra para compilar o arquivo de recurso do ícone
$(ICON_RESOURCE): icon.rc
	$(WINDRES) $< -O coff -o $@

# Criação do diretório de build, se não existir
$(BUILD_DIR):
	mkdir $(BUILD_DIR)

# Regra para limpar os arquivos gerados
clean:
	rm -rf $(BUILD_DIR) $(TARGET) $(ICON_RESOURCE)

# Indica que 'clean' não é um arquivo de saída
.PHONY: clean
