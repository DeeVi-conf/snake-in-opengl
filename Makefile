C = clang
CFLAGS = -Wall -Wextra -Wpedantic -g -Iinclude

LDFLAGS = -lglfw -ldl -lGL

SRC = src/main.c glad/glad.c
OUT = bin/out

all : $(OUT)

$(OUT): $(SRC)
	$(C) $(CFLAGS) $(SRC) -o $(OUT) $(LDFLAGS)

clean:
	rm -rf $(OUT)