#include <cs50.h>    // CS50 library for get_string()
#include <ctype.h>   // For character type functions (isalpha)
#include <math.h>    // For rounding function (round)
#include <stdio.h>   // For input/output functions
#include <string.h>  // For string length function (strlen)

// Function prototypes
int count_letters(string text);
int count_words(string text);
int count_sentences(string text);

int main(void)
{
    // Prompt user for text input
    string text = get_string("Text: ");

    // Calculate text statistics
    int letters = count_letters(text);
    int words = count_words(text);
    int sentences = count_sentences(text);

    // Calculate averages per 100 words
    float L = (float) letters / words * 100;   // Letters per 100 words
    float S = (float) sentences / words * 100; // Sentences per 100 words

    // Compute Coleman-Liau index
    float index_value = 0.0588 * L - 0.296 * S - 15.8;
    int index = (int) round(index_value);  // Round to nearest integer

    // Output grade level based on index
    if (index < 1)
    {
        printf("Before Grade 1\n");
    }
    else if (index >= 16)
    {
        printf("Grade 16+\n");
    }
    else
    {
        printf("Grade %d\n", index);
    }
}

// Counts alphabetical characters in text
int count_letters(string text)
{
    int count = 0;
    // Iterate through each character in the text
    for (int i = 0, len = strlen(text); i < len; i++)
    {
        // Increment count if character is a letter
        if (isalpha(text[i]))
        {
            count++;
        }
    }
    return count;
}

// Counts words in text (words separated by spaces)
int count_words(string text)
{
    int count = 0;
    // Iterate through each character in the text
    for (int i = 0, len = strlen(text); i < len; i++)
    {
        // Count spaces to determine word count
        if (text[i] == ' ')
        {
            count++;
        }
    }
    // Return word count (number of spaces + 1)
    return count + 1;
}

// Counts sentences in text (ended by '.', '!', or '?')
int count_sentences(string text)
{
    int count = 0;
    // Iterate through each character in the text
    for (int i = 0, len = strlen(text); i < len; i++)
    {
        // Check for sentence-ending punctuation
        if (text[i] == '.' || text[i] == '!' || text[i] == '?')
        {
            count++;
        }
    }
    return count;
}
