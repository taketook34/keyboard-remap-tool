TARGET = keyboard-remap-tool

CC = gcc
CFLAGS = -Wall -Wextra -O2 -Iinclude

SRC_DIR = src
INC_DIR = include
OBJ_DIR = obj

SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRCS))


PREFIX = /usr/local
BIN_DIR = $(PREFIX)/bin
INC_DIR_INSTALL = $(PREFIX)/include
CONF_FILE = /etc/krt.conf

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

clean:
	rm -rf $(OBJ_DIR) $(TARGET)

install: $(TARGET)
	mkdir -p $(BIN_DIR) $(INC_DIR_INSTALL)
	cp $(TARGET) $(BIN_DIR)
	touch $(CONF_FILE)

uninstall:
	rm -f $(BIN_DIR)/$(TARGET)
	rm $(CONF_FILE)

.PHONY: all clean
