CC      := gcc
CFLAGS  := -std=c11 -Wall -Wextra -Werror 
LDLIBS  := -lcheck -lsubunit -lm

SRC_DIR    := $(abspath .)
BUILD_DIR  := $(abspath ./build)
TARGET     := $(BUILD_DIR)/tests

ALL_SRC      := $(shell find $(SRC_DIR) -name "*.c")
FORMAT_FILES := $(shell find $(SRC_DIR) -name "*.c" -o -name "*.h")

# Исключаем лишние точки входа
SRC := $(filter-out %/test_controls.c, $(ALL_SRC))

# Авто-интклюды
INC_DIRS := $(shell find $(SRC_DIR) -type d)
IFLAGS   := $(addprefix -I, $(INC_DIRS))

# Объектные файлы
OBJ := $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SRC))

all: $(TARGET) test

$(TARGET): $(OBJ)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $^ -o $@ $(LDLIBS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(IFLAGS) -c $< -o $@

# check:
# 	clang-format -n --dry-run --Werror $(FORMAT_FILES)

test: $(TARGET)
	$(TARGET)

clean:
	rm -rf $(BUILD_DIR)/*

rebuild: clean all

.PHONY: all clean rebuild check
