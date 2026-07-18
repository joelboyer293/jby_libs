CC      := gcc
CFLAGS  := -Wall -Wextra -Wpedantic -std=c99 -I. -O2
SRC_DIR := tests
BIN_DIR := bin

TEST_SRCS := $(wildcard $(SRC_DIR)/*.c)
TEST_BINS := $(patsubst $(SRC_DIR)/%.c, $(BIN_DIR)/%, $(TEST_SRCS))

all: $(BIN_DIR) $(TEST_BINS)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

$(BIN_DIR)/%: $(SRC_DIR)/%.c trees.h ring_buffer.h
	$(CC) $(CFLAGS) $< -o $@

run: all
	@for test in $(TEST_BINS); do \
		echo "Executing $$test..."; \
		./$$test || exit 1; \
	done

clean:
	rm -rf $(BIN_DIR)

.PHONY: all run clean
