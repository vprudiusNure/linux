CC=gcc

.PHONY: all clean
.DEFAULT_GOAL: all

BINS=signal

all: $(BINS)

%: %.c
	$(CC) $< -o $@

clean:
	rm $(BINS)
