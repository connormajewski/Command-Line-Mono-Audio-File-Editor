CC = gcc
INCLUDES = -Iinclude
LIBRARY = -Llib
LINKER = -lsndfile 

all: audio

audio: audio.c
	$(CC) -Wall -Werror test.c test.h audio.c audio.h -o audio $(LINKER)

run: audio
	./audio

test: audio
	./audio -t && clear && cat test.txt

clean: audio.exe
	rm audio.exe
