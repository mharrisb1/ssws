CC := cc
CFLAGS := -Wall -Wextra -std=c2x -Iinclude -Ilib
SRC_DIR := src
BUILD_DIR := build
SRCS := $(wildcard $(SRC_DIR)/*.c)
OBJS := $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SRCS))
TARGET := $(BUILD_DIR)/app

all: $(TARGET)

$(TARGET): $(OBJS)
	@mkdir -p $(BUILD_DIR)
	$(CC) $(OBJS) -o $(TARGET) $(CFLAGS)
	@echo "Linked executable: $@"

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@
	@echo "Compiled: $< -> $@"

clean:
	@rm -rf $(BUILD_DIR)
	@echo "Cleaned build directory."

.PHONY: all clean
