#include <cs50.h>
#include <stdio.h>
#include <string.h>  // For string comparison functions

// Max number of candidates allowed in the election
#define MAX 9

// Define a candidate structure with name and vote count
typedef struct
{
    string name;   // Candidate's name
    int votes;     // Number of votes the candidate has
} candidate;

// Array to store all candidates in the election
candidate candidates[MAX];

// Variable to track the actual number of candidates
int candidate_count;

// Function prototypes - declarations before implementations
bool vote(string name);       // Record a vote for a valid candidate
void print_winner(void);      // Determine and print election winner(s)

int main(int argc, string argv[])
{
    // Check for invalid command-line usage (must have at least 1 candidate)
    if (argc < 2)
    {
        printf("Usage: plurality [candidate ...]\n");
        return 1;
    }

    // Set candidate_count based on command-line arguments
    // (argc - 1 because argv[0] is program name)
    candidate_count = argc - 1;

    // Ensure we don't exceed maximum candidate limit
    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }

    // Initialize candidates array with names from command line
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i].name = argv[i + 1];  // argv[1] is first candidate
        candidates[i].votes = 0;            // Start with zero votes
    }

    // Get number of voters from user
    int voter_count = get_int("Number of voters: ");

    // Process each voter's ballot
    for (int i = 0; i < voter_count; i++)
    {
        string name = get_string("Vote: ");

        // Check if vote is valid and count it
        if (!vote(name))
        {
            // Reject invalid candidate names
            printf("Invalid vote.\n");
        }
    }

    // Determine and announce election results
    print_winner();
}

// Process a single vote
bool vote(string name)
{
    // Iterate through all candidates
    for (int i = 0; i < candidate_count; i++)
    {
        // Check if candidate name matches vote (case-sensitive)
        if (strcmp(candidates[i].name, name) == 0)
        {
            // Valid candidate found - increment their vote count
            candidates[i].votes++;
            return true;  // Vote successfully recorded
        }
    }
    // No matching candidate found
    return false;  // Invalid vote
}

// Determine and print the election winner(s)
void print_winner(void)
{
    // First pass: Find maximum vote count
    int max_votes = 0;
    for (int i = 0; i < candidate_count; i++)
    {
        // Update max_votes if current candidate has more votes
        if (candidates[i].votes > max_votes)
        {
            max_votes = candidates[i].votes;
        }
    }

    // Second pass: Print all candidates with max_votes
    for (int i = 0; i < candidate_count; i++)
    {
        // Check if candidate has winning vote count
        if (candidates[i].votes == max_votes)
        {
            // Print winner's name (multiple winners possible)
            printf("%s\n", candidates[i].name);
        }
    }
}
