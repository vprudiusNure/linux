CC=gcc

.PHONY: all clean
.DEFAULT_GOAL: all

BINS=signal exec args

all: $(BINS)

%: %.c
	$(CC) $< -o $@

clean:
	rm $(BINS)
