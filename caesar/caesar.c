#include <cs50.h>   // For get_string()
#include <stdio.h>   // For input/output functions
#include <stdlib.h>  // For atoi() and exit codes
#include <string.h>  // For strlen()
#include <ctype.h>   // For isdigit(), isupper(), islower()

int main(int argc, string argv[])
{
    // Validate command-line arguments
    // Must have exactly 2 arguments: program name + key
    if (argc != 2)
    {
        printf("Usage: ./caesar key\n");
        return 1;  // Exit with error code
    }

    // Validate key format - must be all digits
    string key_str = argv[1];  // Get key string from command line
    for (int i = 0, n = strlen(key_str); i < n; i++)
    {
        // Check each character is a digit
        if (!isdigit(key_str[i]))
        {
            printf("Usage: ./caesar key\n");
            return 1;  // Exit with error code
        }
    }

    // Convert key string to integer
    int key = atoi(key_str);

    // Get plaintext input from user
    string plaintext = get_string("plaintext:  ");

    // Begin ciphertext output
    printf("ciphertext: ");

    // Process each character in plaintext
    for (int i = 0, n = strlen(plaintext); i < n; i++)
    {
        char c = plaintext[i];  // Current character

        // Handle uppercase letters (ASCII 65-90)
        if (isupper(c))
        {
            /*
            Encryption formula for uppercase:
            1. Convert character to alphabet index (0-25): c - 'A'
            2. Apply Caesar shift: (index + key) % 26
            3. Convert back to ASCII: add 'A'
            */
            printf("%c", 'A' + (c - 'A' + key) % 26);
        }
        // Handle lowercase letters (ASCII 97-122)
        else if (islower(c))
        {
            /*
            Encryption formula for lowercase:
            1. Convert character to alphabet index (0-25): c - 'a'
            2. Apply Caesar shift: (index + key) % 26
            3. Convert back to ASCII: add 'a'
            */
            printf("%c", 'a' + (c - 'a' + key) % 26);
        }
        // Non-alphabetic characters remain unchanged
        else
        {
            printf("%c", c);
        }
    }

    // End with new line for clean output
    printf("\n");
    return 0;  // Successful execution
}
