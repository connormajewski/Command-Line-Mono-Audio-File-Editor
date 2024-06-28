CC = gcc
INCLUDES = -Iinclude
LIBRARY = -Llib
LINKER = -lsndfile 

all: audio

audio: audio.c
	$(CC) -Wall -Werror audio.c -o audio $(LINKER)

run: audio
	./audio

clean: audio.exe
	rm audio.exe
