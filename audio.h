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
    ARG_NARGS
};

// Seperator to frame menu items.

char * seperator = "------------------------------------------------------------";

// Char array holding user input. This is the main array to hold almost all input from the effects() function
// and its calling functions.

char command[TEXT_BUFFER];

// Char array to store the name of the loaded file. Used in load(), unload(), info(), and copy().

char infilename[TEXT_BUFFER];

// Char array to store (y/n) from user. Used in load() and copy().

char confirmation[TEXT_BUFFER];

// Char array to store start and end durations from user input. Used in effect().

char dur[TEXT_BUFFER];

// Pointer to loaded file. Every function call makes use of this pointer in some way or another.

SNDFILE *infile = NULL;

// Pointer to an output file. Used by copy().

SNDFILE *outfile = NULL;

// Struct storing SNDFILE information. Elements accessed using sfinfo.<element>. Used in almost all functions.

SF_INFO sfinfo;

// 2D character array storing graphical representation of loaded file. Populated and used in waveform().

int waveformArray[SCREEN_Y][SCREEN_X];

// Flag tracking changes to loaded file when calling to waveform(). Eliminates excess accesses of waveFormArray
// in waveform() when no changes have been made.

int waveformReset;

//int outfile_major_type;  // output major type in hex

// Number of samples. Value is set first in calls to load(), then additionally as needed.

long nsamples;

// stores number of frames read from calls to sf_read(). Should always return either NFRAMES or a number > 0 if successful.

long readcount;

// Buffer to store frames pulled from sf_read(). Main

float *buffer;

// Buffer to store remaining frames after removal.

float *speedBuffer;

// Buffer to store frames for delay effect.

float *delayBuffer;

// Length of delay in seconds. Must be a positive number.

float delayValue;

// Strength of delay. Must be between 0.00 and 1.00 inclusive.

float decayValue;

// Duration of loaded file in seconds. Calculated in load and adjusted when needed.

float duration;

// Stores delay sample.

float delaySample;

// Stores start time of interval of effect. Must be between 0.00 and duration, and cannot be greater than end.

float start;

// End time of interval of effect. Must be between 0.00 and duration, and cannot be less than start.

float end;

// Starting frame of interval of effect, calculated by multiplying start by the sample rate.

int startFrame;

// End frame of interval of effect, calculated by multiplying end by the sample rate.

int endFrame;

// Number of channels of loaded file.

int numberOfChannels;

// Number of samples of loaded file. Used to determine duration.

int numberOfSamples;

// Stores current spot in delayBuffer.

int delayBufferIndex;

// Stores delay samples.

int delaySamples;


// Flag checking for input file validity.

//int isValidFile;

// Tracks number of frames read when iterating through file using sf_read().

int sum;

// Note: A more detailed explanation of each function can be found in audio.c.

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

void load();

// Unloads previously loaded file.

void unload();

// Function to display representation of audio file to screen.

void waveform();

// Function to clear screen.

void reset();

// Helper function to remove newlines from user input.

void removeNewLine();

