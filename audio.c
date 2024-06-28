#include "audio.h"

/*
 * This is a function that clears the terminal. It acts a "refresh" after running other functions,
 * to give illusion of a static interface. It is called in the beginning of almost every function.
 */

void reset(){

    printf("\e[1;1H\e[2J");

}

/*
 * This function removes the newline from the end of user input. Used to clean inputs from the
 * user when passed to a function.
 */

void removeNewLine(char * string){

    string[strcspn(string, "\n")] = 0;

}

/*
 * Prints relevant information about the currently loaded file.
 */

void info(){

    reset();

    if(infile){

        printf("%s\nFILE INFO\n\nname - %s\nduration - %.2fs\n", seperator, infilename, duration);
        print_sfinfo(&sfinfo);
        printf("%s\n", seperator);

    }

    else{

        printf("No audio file has been loaded.\n");

    }

    printf("Press any key to continue.\n");

    fgetc(stdin);

}

/*
 * Function that takes an audio file path from user input, checks for validity,
 * and opens the file using sf_open(). After opening, pulls the number of samples
 * and duration from sfinfo to store in local variables. Eliminates need to calculate
 * samples and duration every time and effect function is called.
 *
 * Note: sf_open() must be called first in order to load number of channels and
 * format to sfinfo, otherwise all calls to load() will fail.
 */

void load(){

    reset();

    if(infile){

        printf("This will replace the currently loaded file. Continue (y/n) ?\n");

        fgets(confirmation, TEXT_BUFFER, stdin);

        removeNewLine(confirmation);

        if(!strcmp(confirmation, "n")){

            printf("File loading cancelled.\nPress any key to continue.\n");

            fgetc(stdin);

        }

    }

    printf("Enter name of audio file to be opened:\n");

    fgets(infilename, TEXT_BUFFER, stdin);

    removeNewLine(infilename);

    if ((infile = sf_open(infilename, SFM_RDWR, &sfinfo)) == NULL)
    {

        printf("File could not be opened.\n");

    }

    else if(sfinfo.channels != 1){

        printf("Input file must be mono.\n");

        free(infile);

    }

    else if(sf_extension(infilename) != SF_FORMAT_WAV){

        printf("File must be .wav.\n");

        free(infile);

    }

    else{

        duration =(float) sfinfo.frames / sfinfo.samplerate;

        nsamples = sfinfo.channels * NFRAMES;

        printf("SAMPLES: %ld\n", nsamples);

        waveformReset = 0;

        printf("File successfully loaded.\n");

    }

    printf("Press any key to continue.\n");

    fgetc(stdin);

}

/*
 * Function that closes currently loaded file, if one, sets the infile file descriptor
 * to NULL, and frees the file buffer. Ensures that all data connected to unloaded file
 * is removed before another file is loaded, eliminating the chance of a new file holding
 * leftover data.
 */

void unload(){

    reset();

    if(infile){

        if(sf_close(infile)){

            puts(sf_strerror(NULL));

        }

        infile = NULL;

        free(buffer);

        waveformReset = 0;

        printf("File has been unloaded.\nPress any key to continue.\n");

    }

    else {

        printf("No audio file has been loaded.\nPress any key to continue.\n");

    }

    fgetc(stdin);

}

/*
 * Function responsible for taking user input and executing the relevant effect function, if a valid file
 * has been loaded. Takes a valid time duration from user and calculates starting and ending frames to avoid having to
 * recalculate inside each effect function. If an unrecognized command is used, the function ends and
 * flow falls back to main().
 *
 * startFrame and endFrame are set inside this function, and do not have to be altered
 * in any other functions unless needed.
 */

void effect(){

    reset();

    if(infile){

        sf_seek(infile, 0, SEEK_SET);

        printf("%s\nEffects List\n\namp - alter amplitude\nfade - linear fade in/out\ndelay - add delay\nquit - return to main menu\n%s\n", seperator, seperator);

        fgets(command, TEXT_BUFFER, stdin);

        removeNewLine(command);

        printf("Enter start and end time (-1 -1 for entire track.): ");

        fgets(dur, TEXT_BUFFER, stdin);

        removeNewLine(dur);

        sscanf(dur, "%f %f", &start, &end);

        if(((start < end) && (start >= 0.00) && (end <= duration)) || (start == -1 && end == -1)){

            startFrame = (start == -1) ? 0.00 : start * (double) sfinfo.samplerate;
            endFrame = (end == -1) ? duration * (double) sfinfo.samplerate : end * (double) sfinfo.samplerate;

            if(!strcmp(command, "amp")){

                amplitude(start, end);

            }

            else if(!strcmp(command, "fade")){

                fade(start, end);

            }

            // INCOMPLETE.

//            else if(!strcmp(command, "speed")){
//
//                speed(start, end);
//
//            }
//
//            else if(!strcmp(command, "cut")){
//
//                cut(start, end);
//
//            }
//
//            else if(!strcmp(command, "delay")){
//
//                delay(start, end);
//
//            }

            else{

                printf("Unrecognized effect.\n");

            }

        }

        else{

            printf("Invalid start and end times.\n");

        }

        printf("Returning to main menu.\nPress any key to continue.\n");

        buffer = (float *)calloc(NFRAMES, sizeof(float *));

    }

    else{

        printf("No audio file has been loaded.\nPress any key to continue.\n");

    }

    waveformReset = 0;

    fgetc(stdin);

}

/*
 * Function to alter the amplitude of a section of the loaded file, with the intervals
 * of the effect being set beforehand by the effect() function. Iterates through the file,
 * multiplying each value by the given amplitude, then rewriting to the loaded file.
 */

void amplitude(float start, float end){

    float amplitude;

    printf("Enter change in amplitude (0.00 - 5.00)\n");

    fgets(command, TEXT_BUFFER, stdin);

    removeNewLine(command);

    amplitude = atof(command);

    if(amplitude < 0.00){

        printf("Invalid amplitude value,\n");

        return;

    }

    sum = 0;

    sf_seek(infile, startFrame, 0);

    while(((readcount = sf_read_float(infile, buffer, nsamples)) > 0) && sum < endFrame - startFrame){

        sum += readcount;

        for(int i=0;i<NFRAMES;i++){

            buffer[i] = buffer[i] * amplitude;

        }

        sf_write_float(infile, buffer, readcount);

    }

}

/*
 * Function to apply an increasing or decreasing linear fade to a section of the loaded file, as
 * specified by the user.
 *
 */

void fade(float start, float end){

    float level;

    printf("Enter 1 for 0.00 -> 1.00 fade in. Enter 0 for 1.0 -> 0.00 fade out.\n");

    fgets(command, TEXT_BUFFER, stdin);

    removeNewLine(command);

    level = atof(command);

    sum = 0;

    sf_seek(infile, startFrame, 0);

    while(((readcount = sf_read_float(infile, buffer, nsamples)) > 0) && sum < endFrame - startFrame){

        sum += readcount;

        for(int i=0;i<NFRAMES;i++){

            buffer[i] = (level) ? buffer[i] * sum / (double) (endFrame - startFrame) : buffer[i] * (1.00 - (sum / (double) (endFrame - startFrame)));

        }

        sf_write_float(infile, buffer, readcount);

    }

}

// INCOMPLETE.

/*
 * Function to alter the relative speed of the loaded file, by skipping over every
 * nth buffer value as defined by the user, and writing back to the input file,
 * reducing the duration and samples in sfinfo as needed.
 *
 */

void speed(float start, float end){

    sum = 0;

    speedBuffer = (float *) calloc(sfinfo.frames / 2, sizeof(float));

    sfinfo.frames = sfinfo.frames / 2;
    duration /= 2.00;

    sf_seek(infile, startFrame, 0);

    while(((readcount = sf_read_float(infile, buffer, nsamples)) > 0) && sum < (endFrame / 2.00) - startFrame){

        sum += readcount;

        for(int i=1;i<NFRAMES;i+=2){

            speedBuffer[i / 2] = buffer[i];

        }

        sf_write_float(infile, speedBuffer, readcount/2);

    }

}

// INCOMPLETE.

/*
 * This function deletes a section of the loaded file as specified by the user.
 * It shifts all remaining data to the left, overwriting the section to be deleted.
 */

void cut(float start, float end){

    reset();

    printf("%ld\n", sfinfo.frames / 2);

    speedBuffer = (float *) calloc((sfinfo.frames / 2), sizeof(float));

    if(!speedBuffer) printf("ERROR\n");

    // Test file.

    SNDFILE * out;

    if ((out = sf_open("tmp.wav", SFM_WRITE, &sfinfo)) != NULL)
    {

        // Shift all data from left of endFrame to startFrame position, overwriting section to be deleted.

        /*
         * for i in [endFrame, end of file]
         *  buffer[i] = buffer[i + (endFrame - startFrame)]
         *  write buffer to file
         */

    }

    else {

        puts(sf_strerror(NULL));

    }

    sf_close(out);

    printf("Press any key to continue.\n");

    printf("FINISHED\n");

    fgetc(stdin);

}

// INCOMPLETE.

/*
 * This function applies a delay effect to a section of the loaded file, as specified by the user.
 * Creates a second buffer to store the delayed samples, and adds the number of extra frames
 * needed to sfinfo.frames.
 *
 */

void delay(float start, float end){

    printf("Enter delay amount in seconds.\n");

    fgets(command, TEXT_BUFFER, stdin);

    removeNewLine(command);

    delayValue = atof(command);

    printf("Enter decay (0.00 - 1.00)\n");

    fgets(command, TEXT_BUFFER, stdin);

    removeNewLine(command);

    decayValue = atof(command);

    if((delayValue > 0.0) && (decayValue >= 0.0 && decayValue <= 1.00)){

        // Needs to be fixed.

        sf_seek(infile, startFrame, 0);

        delaySamples = (sfinfo.samplerate) * delayValue;

        delayBuffer = (float * )calloc(delaySamples, sizeof(float));

        sfinfo.frames += (int) delaySamples * 2;

        delayBufferIndex = 0;

        int count = 0;

        sum = 0;

        while(((readcount = sf_read_float(infile, buffer, nsamples)) > 0) && sum < endFrame - startFrame){

            for(int i=0;i<NFRAMES;i++){

                sum += readcount;

                delaySample = delayBuffer[delayBufferIndex];
                delayBuffer[delayBufferIndex] = decayValue * buffer[i];
                buffer[i] = buffer[i] + delaySample;
                delayBufferIndex = (delayBufferIndex + 1) % delaySamples;

                buffer[i] = buffer[i] + (float)(decayValue * delaySample);

                count++;

            }

            sf_write_float(infile, buffer, readcount);

        }

        free(delayBuffer);
        free(buffer);

        sf_write_float(infile, delayBuffer, delaySamples);

        printf("Finished.\n");

    }

    else{

        printf("Invalid inputs for delay effect.\n");

    }

}

/*
 * Function that copies the currently loaded file to a new file of the form 'Copy - <filename>'.
 * Note: This function does not load the copied file, only creates it. In order to use the created file,
 * it must be loaded through a call to load().
 */

void copy(){

    reset();

    if(infile){

        printf("copying %s. Continue (y/n) ?\n", infilename);

        fgets(confirmation, TEXT_BUFFER, stdin);

        removeNewLine(confirmation);

        if(!strcmp(confirmation, "n")){

            printf("Copying cancelled.\nPress any key to continue.\n");

        }

        else {

            // Construct copy file name.

            char * copyPrefix = "Copy - ";
            char * copyFileName;

            copyFileName = malloc(strlen(copyPrefix) + strlen(infilename) + 1);

            printf("NAME MALLOCED.\n");

            if(copyFileName != NULL){

                printf("NAME MADE.\n");

                copyFileName[0] = '\0';

                strcat(copyFileName, copyPrefix);
                strcat(copyFileName, infilename);

                printf("Copied file name - %s\n", copyFileName);

                if(!access(copyFileName, F_OK)) {

                    printf("File with that name already exists.\n");

                }

                else {

                    printf("CREATING NEW FILE.\n");

                    outfile = sf_open(copyFileName, SFM_WRITE, &sfinfo);

                    if(outfile == NULL) printf("ERROR CREATING FILE.\n");

                    if((outfile) != NULL){

                        sf_seek(infile, 0.0, 0);

                        sum = 0;

                        while(((readcount = sf_read_float(infile, buffer, nsamples)) > 0) && sum < (duration * (double) sfinfo.samplerate)){

                            sum += readcount;

                            for(int i=0;i<NFRAMES;i++){

                                buffer[i] = buffer[i];

                            }


                        }

                        printf("%s has been successfully copied to %s\n", infilename, copyFileName);

                    }

                }

            }

            else{

                printf("Error copying file.\n");

            }

        }

    }

    else{

        printf("No file has been loaded.\n");

    }

    printf("Press any key to continue.\n");

    fgetc(stdin);

}

/*
 * Function that prints a simplified representation of the loaded audio file to the screen, alongside
 * amplitude and duration markings. Variable waveFormReset is used to track whether any changes have
 * been made to the file. If no changes have been made, then the waveformArray does not need to be
 * recalculated, saving time when calling waveform() multiple times on an unchanged file.
 */

void waveform(){

    reset();

    sum = 0;

    sf_seek(infile, startFrame, 0);

    int range;

    int j = 0;

    int k = 0;

    int target = (int)((duration * (double) sfinfo.samplerate) / SCREEN_X);

    if(!waveformReset){

        for(int i=0;i<SCREEN_Y;i++){

            for(int j=0;j<SCREEN_X;j++){

                waveformArray[i][j] = 0;

            }

        }

        while(((readcount = sf_read_float(infile, buffer, nsamples)) > 0) && sum < (duration * (double) sfinfo.samplerate)){

            sum += readcount;

            for(int i=0;i<NFRAMES;i++){

                if(!(j % target)){

                    // MODIFY TO FIND AVERAGE OF ALL FRAMES IN TARGET RANGE INSTEAD OF SELECTING ONE FRAME PER TARGET.

                    range = ((buffer[i] * SCREEN_Y) < 0) ? (-(buffer[i] * SCREEN_Y) / 2) : ((buffer[i] * SCREEN_Y) / 2);

                    for(int l=0;l<SCREEN_Y;l++){

                        waveformArray[l][k] = ((l <= 32 - range) || (l >= 32 + range)) ? ' ' : '|';

                    }

                    k++;

                }

                j++;

            }

        }

    }

    waveformReset = 1;

    reset();

    waveformArray[1][0] = '1';
    waveformArray[16][0] = '/';
    waveformArray[32][0] = '0';
    waveformArray[48][0] = '/';
    waveformArray[63][0] = '1';

    for(int i=0;i<SCREEN_Y;i++){

        for(int j=0;j<SCREEN_X;j++){

            printf("%c", waveformArray[i][j]);

        }

        printf("\n");

    }

    printf("\n");

    for(int i=0;i<SCREEN_X - 4;i++){

        if(!(i % (SCREEN_X / 4))) printf("%.2fs", ((double)i / (double)SCREEN_X) * (duration));

        else printf(" ");

    }

    getc(stdin);

}

/*
 * Function that acts as body of program. Every function call is made through here, and this is where
 * the arguments to the program are checked for validity.
 */

int main(int argc, char *argv[])
{

    if (argc != ARG_NARGS)
    {
        printf("16-bit Mono Audio File Editor.\n");
        printf("Too many arguments.\n");
        printf("Usage: %s\n", argv[ARG_PROGNAME]);
        return 1;
    }

    memset(&sfinfo, 0, sizeof(sfinfo));

    buffer = (float *) calloc(NFRAMES, sizeof(float));

    while(1){

        reset();

        printf("%s\n16-bit Mono Audio File Editor.\n\n%s\n\nCommands\nquit, q - exit the program.\nload - load a 16-bit signed PCM mono audio file for editing."
               "\nunload - unload currently loaded file.\ninfo - get info on file if one is currently loaded.\ncopy - create a copy of the currently loaded file."
               "\neffect - apply effects to audio file.\nview - display current file waveform.\n%s\n", seperator, infilename != NULL ? infilename : "", seperator);

        fgets(command, TEXT_BUFFER, stdin);

        removeNewLine(command);

        if(!strcmp(command, "q") || !strcmp(command, "quit")){

            if(infile){

                unload(&buffer);

            }

            system("clear");
            return 0;

        }

        else if(!strcmp(command, "load")){

            load();

        }

        else if(!strcmp(command, "unload")){

            unload();

        }

        else if(!strcmp(command, "copy")){

            copy();

        }

        else if(!strcmp(command, "info")){

            info();

        }

        else if(!strcmp(command, "effect")){

            effect();

        }

        else if(!strcmp(command, "view")){

            waveform();

        }

    }

}

/*
 * Function that stores and prints the format and type of the currently
 * loaded file. Called through every call to info().
 */

void print_sfinfo(const SF_INFO *sfinfo)
{
    int majortype = sfinfo->format & SF_FORMAT_TYPEMASK; // find the major type
    int subtype = sfinfo->format & SF_FORMAT_SUBMASK;    // find the subtype
    int major_count;                                     // number of major types
    int sub_count;                                       // number of subtypes
    SF_FORMAT_INFO formatTemp;
    memset(&formatTemp, 0, sizeof(formatTemp)); // clear the SF_FORMWT_INFO struct

    // full format
    printf(" format: 0x%08x\n", sfinfo->format); // print the full format

    /* major type */
    sf_command(NULL, SFC_GET_FORMAT_MAJOR_COUNT, &major_count, sizeof(int));
    for (int i = 0; i < major_count; i++)
    {
        formatTemp.format = i;
        sf_command(NULL, SFC_GET_FORMAT_MAJOR, &formatTemp, sizeof(formatTemp));
        if (majortype == formatTemp.format)
        {
            printf(" %02d 0x%08x %s (extension \"%s\")\n", i, formatTemp.format, formatTemp.name, formatTemp.extension);
            break; // find the matching format
        }
    }

    /* subtype */
    memset(&formatTemp, 0, sizeof(formatTemp));
    sf_command(NULL, SFC_GET_FORMAT_SUBTYPE_COUNT, &sub_count, sizeof(int));
    for (int i = 0; i < sub_count; i++)
    {
        formatTemp.format = i;
        sf_command(NULL, SFC_GET_FORMAT_SUBTYPE, &formatTemp, sizeof(formatTemp));
        if (subtype == formatTemp.format)
        {
            printf(" %02d 0x%08x %s\n", i, formatTemp.format, formatTemp.name);
            // extension is not available for subtypes
            break; // find the matching format
        }
    }
}

/*
 * This function checks and returns the extension of the loaded file. Used in load() as a partial check
 * of a file's validity.
 */

int sf_extension(const char *filename)
{
    int filename_len = strlen(filename); // entire filename length (include the extension)
    if (strcmp((filename + (filename_len - 4)), ".wav") == 0)
    {
        return SF_FORMAT_WAV; // wav file type in hex
    }
    else if (strcmp((filename + (filename_len - 4)), ".aif") == 0 || strcmp((filename + (filename_len - 5)), ".aiff") == 0)
    {
        return SF_FORMAT_AIFF; // aiff or aif file type in hex
    }
    else
    {
        return -1; // extension is not wav, aiff, or aif
    }
}
