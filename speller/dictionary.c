// Implements a dictionary's functionality

#include <ctype.h>       // For character handling functions
#include <stdbool.h>    // For boolean types
#include <stdio.h>      // For file operations
#include <stdlib.h>     // For memory allocation
#include <string.h>     // For string operations
#include <strings.h>    // For case-insensitive string comparison (strcasecmp)

#include "dictionary.h" // Contains definitions for our dictionary functions

// Represents a node in a hash table (linked list implementation)
typedef struct node
{
    char word[LENGTH + 1];  // Stores the word (LENGTH defined in dictionary.h)
    struct node *next;      // Pointer to next node in the chain
} node;

// Number of buckets in the hash table (prime number reduces collisions)
const unsigned int N = 10007;  // Prime number larger than dictionary size

// Hash table (array of linked list pointers)
node *table[N];

// Global word counter
unsigned int word_count = 0;

// Hashes word to a number using the djb2 algorithm
unsigned int hash(const char *word)
{
    // DJB2 hash algorithm by Dan Bernstein
    unsigned long hash = 5381;  // Initial prime seed value

    // Process each character in the word
    int c;
    while ((c = *word++))
    {
        // Update hash: hash * 33 + current character
        // Using bit shift (<< 5) for efficient multiplication by 32
        hash = ((hash << 5) + hash) + tolower(c);  // Case-insensitive
    }

    // Ensure hash value fits within table size
    return hash % N;
}

// Loads dictionary into memory, returning true if successful
bool load(const char *dictionary)
{
    // Open dictionary file for reading
    FILE *file = fopen(dictionary, "r");
    if (file == NULL)
    {
        return false;  // File not found
    }

    // Buffer for reading words
    char word[LENGTH + 1];

    // Read words until end of file
    while (fscanf(file, "%45s", word) != EOF)  // Limit to LENGTH characters
    {
        // Create new node for the word
        node *n = malloc(sizeof(node));
        if (n == NULL)
        {
            fclose(file);
            return false;  // Memory allocation failed
        }

        // Copy word into node (using strcpy for safe copying)
        strcpy(n->word, word);

        // Hash word to get bucket index
        unsigned int index = hash(word);

        // Insert node at beginning of linked list
        n->next = table[index];
        table[index] = n;

        // Increment word counter
        word_count++;
    }

    // Close dictionary file
    fclose(file);
    return true;  // Successfully loaded
}

// Returns true if word is in dictionary (case-insensitive)
bool check(const char *word)
{
    // Hash word to get bucket index
    unsigned int index = hash(word);

    // Search through linked list at this index
    for (node *cursor = table[index]; cursor != NULL; cursor = cursor->next)
    {
        // Compare words case-insensitively
        if (strcasecmp(cursor->word, word) == 0)
        {
            return true;  // Word found
        }
    }
    return false;  // Word not found
}

// Returns number of words in loaded dictionary
unsigned int size(void)
{
    return word_count;  // Return global counter
}

// Unloads dictionary from memory
bool unload(void)
{
    // Iterate through all buckets
    for (unsigned int i = 0; i < N; i++)
    {
        // Start at the head of linked list
        node *cursor = table[i];

        // Traverse linked list freeing nodes
        while (cursor != NULL)
        {
            node *temp = cursor;    // Save current node
            cursor = cursor->next;   // Move to next node
            free(temp);              // Free saved node
        }

        // Set bucket to NULL after freeing
        table[i] = NULL;
    }
    return true;  // Successfully unloaded
}
