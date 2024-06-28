# Command Line Mono Audio File Editor
This is a command line program that enables simple
manipulation of single-channel .wav audio files. I wrote this originally as a single function to alter amplitude,
but decided to add more pieces as I enjoyed working on it.
![Menu](https://github.com/connormajewski/Command-Line-Mono-Audio-File-Editor/assets/171502061/e61a9603-a113-494b-aa28-802a1d25e4eb)

![View](https://github.com/connormajewski/Command-Line-Mono-Audio-File-Editor/assets/171502061/3d9abe4a-c8d7-48c8-9633-3ac78b373bff)

## Compilation and Running 
This program was compiled and run using Cygwin. As such, the makefile is currently 
written to build and run on Cygwin only. I am hoping
to modify it in the future to also compile through other sources.

To compile and run on Cygwin, navigate to the main directory 
of the makefile in a terminal and enter <br><br>~ **make run**.

When loading a file, make sure that the file is located in the same directory as the executable.
Loading in files through **dir/filename** has caused issues when copying and performing other functions. I am planning on
fixing this in the future.

## Limitations
This program is in its early stages, and does not offer a lot of usability at the moment.
I am hoping to add more effects as I go along. Some future effects include the ability to cut audio, delay it, and change its speed. 

## Contributions
The program runs using the libsndfile library, whose Git page is found here:

**<src>https://github.com/libsndfile/libsndfile**

A handy documentation website for the library can be found here:

**<src>http://www.mega-nerd.com/libsndfile/api.html**
