#include "audio.h"
#include "test.h"

/*
 * Helper function that takes a return value from a function called in audio.c,
 * and returns a string matching the return value's name in the return value enum.
 */

char * return_to_string(int return_value){

    switch(return_value){

        case 0:
            return "SUCCESS";
        case 1:
            return "WRONG_FORMAT";
        case 2:
            return "WRONG_CHANNELS";
        case 3:
            return "NONEXISTENT_FILE";
        case 4:
            return "NO_LOADED_FILE";
        default:
            return "UNDEFINED";

    }

}


/*
 * Function to test return values of calls to load() against expected return values.
 * Prints PASS/FAIL of each test to test.txt file.
 */

void load_test(){

    fprintf(test,"Running tests for load() function.\n%s%s\n", seperator, seperator);

    test_return_value = load("testfiles/dne.mp3");
    test_return_value_string = return_to_string(test_return_value);
    fprintf(test, "Testing load() with nonexistent file. Expected return value: NONEXISTENT_FILE. Actual return value: %s. %s.\n",
            return_to_string(test_return_value), strcmp("NONEXISTENT_FILE", test_return_value_string) == 0 ? "PASS" : "FAIL");

    test_return_value = load("testfiles/mono.mp3");
    test_return_value_string = return_to_string(test_return_value);
    fprintf(test, "Testing load() with mp3 file. Expected return value: WRONG_FORMAT. Actual return value: %s. %s.\n",
            return_to_string(test_return_value), strcmp("WRONG_FORMAT", test_return_value_string) == 0 ? "PASS" : "FAIL");

    test_return_value = load("testfiles/mono.wav");
    test_return_value_string = return_to_string(test_return_value);
    fprintf(test, "Testing load() with nonexistent file. Expected return value: SUCCESS. Actual return value: %s. %s.\n%s%s\n\n",
            return_to_string(test_return_value), strcmp("SUCCESS", test_return_value_string) == 0 ? "PASS" : "FAIL", seperator, seperator);

    fprintf(test,"Running tests for unload() function.\n%s%s\n", seperator, seperator);

    test_return_value = unload();
    test_return_value_string = return_to_string(test_return_value);
    fprintf(test, "Testing unload() with loaded file. Expected return value: SUCCESS. Actual return value: %s. %s.\n",
            return_to_string(test_return_value), (!strcmp("SUCCESS", test_return_value_string)) ? "PASS" : "FAIL");

    test_return_value = unload();
    test_return_value_string = return_to_string(test_return_value);
    fprintf(test,"Testing unload() with no loaded file. Expected return value: NO_LOADED_FILE. Actual return value: %s. %s.\n%s%s\n\n",
            return_to_string(test_return_value), (strcmp("NO_LOADED_FILE", test_return_value_string)) == 0 ? "PASS" : "FAIL", seperator, seperator);

}

/*
 * Function to test return values of calls to unload() against expected return values.
 * Prints PASS/FAIL of each test to test.txt file.
 */

void unload_test(){

    fprintf(test,"Running tests for unload() function.\n%s%s\n", seperator, seperator);

    test_return_value = unload();
    test_return_value_string = return_to_string(test_return_value);
    fprintf(test, "Testing unload() with loaded file. Expected return value: SUCCESS. Actual return value: %s. %s.\n",
            return_to_string(test_return_value), (!strcmp("SUCCESS", test_return_value_string)) ? "PASS" : "FAIL");

    test_return_value = unload();
    test_return_value_string = return_to_string(test_return_value);
    fprintf(test,"Testing unload() with no loaded file. Expected return value: NO_LOADED_FILE. Actual return value: %s. %s.\n%s%s\n\n",
            return_to_string(test_return_value), (strcmp("NO_LOADED_FILE", test_return_value_string)) == 0 ? "PASS" : "FAIL", seperator, seperator);

}

/*
 * Function that runs all sub-tests from within main() of audio.c.
 */

void audio_tests(){

//    load_test();

    unload_test();

    fprintf(test,"Running tests for unload() function.\n%s%s\n", seperator, seperator);

    fprintf(test, "%s%s\nRUNNING TESTS FOR AUDIO.C\n%s%s\n\n", seperator, seperator, seperator, seperator);

}

