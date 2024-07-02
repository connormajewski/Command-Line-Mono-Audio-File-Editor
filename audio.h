#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <sndfile.h>
#include <unistd.h>

// Number of frames per block.

#define NFRAMES (1024)

// Maximum size of user-input buffer. Used to dictate sizes of command, infilename, outfilename, confirmation, and dur char arrays.

#define TEXT_BUFFER (255)

// X,Y dimensions of full screen in terms of the number of ASCII characters that can fit vertically and horizontally.

#define SCREEN_X (237)
#define SCREEN_Y (64)

// Enumeration of command line arguments.

enum
{
    ARG_PROGNAME,
    ARG_TEST,
    ARG_NARGS
};

enum
{
    PASS,
    FAIL
};

// Seperator to frame menu items.

extern char * seperator;

// Char array holding user input. This is the main array to hold almost all input from the effects() function
// and its calling functions.

extern char command[TEXT_BUFFER];

// Char array to store the name of the loaded file. Used in load(), unload(), info(), and copy().

extern char infilename[TEXT_BUFFER];

// Char array to store (y/n) from user. Used in load() and copy().

extern char confirmation[TEXT_BUFFER];

// Pointer to loaded file. Every function call makes use of this pointer in some way or another.

extern SNDFILE *infile;

// Pointer to an output file. Used by copy().

extern SNDFILE *outfile;

// Struct storing SNDFILE information. Elements accessed using sfinfo.<element>. Used in almost all functions.

extern SF_INFO sfinfo;

// Flag tracking changes to loaded file when calling to waveform(). Eliminates excess accesses of waveFormArray
// in waveform() when no changes have been made.

extern int waveformReset;

// Number of samples. Value is set first in calls to load(), then additionally as needed.

extern long nsamples;

// stores number of frames read from calls to sf_read(). Should always return either NFRAMES or a number > 0 if successful.

extern long readcount;

// Buffer to store frames pulled from sf_read(). Main

extern float *buffer;

// Duration of loaded file in seconds. Calculated in load and adjusted when needed.

extern float duration;

// Starting frame of interval of effect, calculated by multiplying start by the sample rate.

extern int startFrame;

// End frame of interval of effect, calculated by multiplying end by the sample rate.

extern int endFrame;

// Tracks number of frames read when iterating through file using sf_read().

extern int sum;


/*
 * Declarations for main program functions.
 */


// Prints file info, including name, duration, and type.

void print_sfinfo(const SF_INFO *sfinfo);

// Gets extension of file.

int sf_extension(const char *filename);

// Change amplitude within time range of a loaded file.

void effect();

// Main effects window. Call to effect() allows calls to other effects functions.

void amplitude(float start, float end);

// Change fade values within time range of a loaded file.

void fade(float start, float end);

// Change speed within time range of a loaded file.

void speed(float start, float end);

// Cut audio file.

void cut(float start, float end);

// Add delay effect with decay.

void delay(float start, float end);

// Create copy of loaded audio file.

void copy();

// Loads file to be used by the program.

extern int load(char * infilename);

// Unloads previously loaded file.

int unload();

// Function to display representation of audio file to screen.

void waveform();

// Function to clear screen.

void reset();

// Helper function to remove newlines from user input.

void removeNewLine();


