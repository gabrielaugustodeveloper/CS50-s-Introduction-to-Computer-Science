// Modifies the volume of a WAV audio file
#include <stdint.h>   // Provides fixed-width integer types (uint8_t, int16_t)
#include <stdio.h>    // Standard I/O functions (fopen, fread, fwrite, fclose)
#include <stdlib.h>   // Standard library functions (atof for string to float conversion)

// Number of bytes in standard .wav file header
// WAV headers contain metadata like audio format, sample rate, channels, etc.
const int HEADER_SIZE = 44;

int main(int argc, char *argv[])
{
    /*********************************************
     * COMMAND-LINE ARGUMENT HANDLING
     *********************************************/
    // Validate correct number of arguments
    if (argc != 4)
    {
        printf("Usage: ./volume input.wav output.wav factor\n");
        return 1;  // Return error code
    }

    /*********************************************
     * FILE OPERATIONS
     *********************************************/
    // Open input file in read-binary mode ("r" works but "rb" is more explicit for binaries)
    FILE *input = fopen(argv[1], "rb");
    if (input == NULL)
    {
        printf("Could not open input file.\n");
        return 1;
    }

    // Open output file in write-binary mode ("w" works but "wb" is better for binaries)
    FILE *output = fopen(argv[2], "wb");
    if (output == NULL)
    {
        printf("Could not open output file.\n");
        fclose(input);  // Cleanup already opened input file
        return 1;
    }

    /*********************************************
     * VOLUME FACTOR PROCESSING
     *********************************************/
    // Convert factor argument from string to float
    // atof returns 0.0 on conversion failure, which mutes the audio
    float factor = atof(argv[3]);

    /*********************************************
     * WAV HEADER PROCESSING
     *********************************************/
    // Create buffer for WAV header (44 bytes of metadata)
    uint8_t header[HEADER_SIZE];  // uint8_t = unsigned 1-byte integers

    // Read entire header in one operation:
    // fread(buffer, element_size, element_count, file_pointer)
    size_t header_read = fread(header, sizeof(uint8_t), HEADER_SIZE, input);
    if (header_read != HEADER_SIZE)
    {
        printf("Incomplete header read.\n");
        fclose(input);
        fclose(output);
        return 1;
    }

    // Write header unchanged to output file
    size_t header_written = fwrite(header, sizeof(uint8_t), HEADER_SIZE, output);
    if (header_written != HEADER_SIZE)
    {
        printf("Header write failed.\n");
        fclose(input);
        fclose(output);
        return 1;
    }

    /*********************************************
     * AUDIO SAMPLE PROCESSING
     *********************************************/
    // Audio samples are 16-bit signed integers (-32,768 to 32,767)
    int16_t sample;  // int16_t = signed 2-byte integer

    // Process audio samples until end of file:
    // fread returns number of items read (1 = success, 0 = EOF/error)
    while (fread(&sample, sizeof(int16_t), 1, input) == 1)
    {
        // Apply volume adjustment by scaling factor:
        // - Multiplying float by int16_t automatically casts to float
        // - Result truncated when stored back to int16_t
        sample = (int16_t)(sample * factor);

        // Write modified sample to output file
        size_t samples_written = fwrite(&sample, sizeof(int16_t), 1, output);
        if (samples_written != 1)
        {
            printf("Sample write error.\n");
            fclose(input);
            fclose(output);
            return 1;
        }
    }

    /*********************************************
     * CLEANUP OPERATIONS
     *********************************************/
    // Close file handles to ensure data is flushed
    fclose(input);
    fclose(output);

    return 0;  // Indicate successful execution
}
