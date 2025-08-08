#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    // Validate command-line arguments
    if (argc != 2)
    {
        printf("Usage: ./recover FILE\n");
        return 1;
    }

    // Open the memory card file for reading
    FILE *card = fopen(argv[1], "r");
    if (card == NULL)
    {
        printf("Could not open file.\n");
        return 1;
    }

    // Create buffer for 512-byte blocks (standard disk sector size)
    uint8_t buffer[512];

    // Counter for naming recovered JPEG files
    int jpeg_count = 0;

    // Output file pointer for current JPEG
    FILE *jpeg = NULL;

    // Buffer for filename (000.jpg to 999.jpg)
    char filename[8];

    // Read through the memory card in 512-byte blocks
    while (fread(buffer, sizeof(uint8_t), 512, card) == 512)
    {
        // Check for JPEG signature (first 4 bytes):
        // - 0xff: JPEG start marker
        // - 0xd8: SOI (Start of Image) marker
        // - 0xff: Another marker
        // - 0xe0-0xef: APPn marker indicating JPEG format
        if (buffer[0] == 0xff &&
            buffer[1] == 0xd8 &&
            buffer[2] == 0xff &&
            (buffer[3] & 0xf0) == 0xe0)
        {
            // Close previous JPEG if one is open
            if (jpeg != NULL)
            {
                fclose(jpeg);
            }

            // Create filename with sequential numbering
            sprintf(filename, "%03d.jpg", jpeg_count++);

            // Open new JPEG file for writing
            jpeg = fopen(filename, "w");
            if (jpeg == NULL)
            {
                fclose(card);
                printf("Could not create JPEG file.\n");
                return 1;
            }
        }

        // If we've found at least one JPEG, write the block to current file
        if (jpeg != NULL)
        {
            fwrite(buffer, sizeof(uint8_t), 512, jpeg);
        }
    }

    // Cleanup resources
    if (jpeg != NULL)
    {
        fclose(jpeg);
    }
    fclose(card);

    return 0;
}
