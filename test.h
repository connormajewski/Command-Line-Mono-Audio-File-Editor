#ifndef COMMAND_LINE_MONO_AUDIO_FILE_EDITOR_TEST_H
#define COMMAND_LINE_MONO_AUDIO_FILE_EDITOR_TEST_H

    // Variables to store the return value of functions called in audio.c.

    int test_return_value;
    char * test_return_value_string;

    // Pointer to file containing output of test functions.

    FILE * test = fopen("test.txt", "w");

    /*
    * Declarations for testing functions.
    */

    void audio_tests();

    void load_test();

    void unload_test();

    char * return_to_string(int return_value);

#endif
