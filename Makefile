# Имя выходного файла
TARGET = keyboard-remap-tool

# Компилятор и флаги
CC = gcc
CFLAGS = -Wall -Wextra -O2 -Iinclude

# Папки
SRC_DIR = src
INC_DIR = include
OBJ_DIR = obj

# Все исходники .c
SRCS = $(wildcard $(SRC_DIR)/*.c)
# Соответствующие объектные файлы
OBJS = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRCS))

# Правило по умолчанию
all: $(TARGET)

# Создаём бинарник из объектных файлов
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

# Компиляция каждого .c в .o
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Создаём папку obj, если её нет
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# Очистка
clean:
	rm -rf $(OBJ_DIR) $(TARGET)

.PHONY: all clean
