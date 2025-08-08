#include <cs50.h>
#include <stdio.h>
#include <string.h>  // Required for string comparison (strcmp)

// Maximum limits to prevent resource exhaustion
#define MAX_VOTERS 100     // Maximum number of voters allowed
#define MAX_CANDIDATES 9   // Maximum number of candidates allowed

// Global 2D array tracking voter preferences
// preferences[i][j] stores candidate index for voter i's jth preference
int preferences[MAX_VOTERS][MAX_CANDIDATES];

// Candidate data structure definition
typedef struct
{
    string name;       // Candidate name
    int votes;         // Current vote count
    bool eliminated;   // Track if candidate has been eliminated
} candidate;

// Global array storing all candidate data
candidate candidates[MAX_CANDIDATES];

// Global counters for election participants
int voter_count;      // Number of voters participating
int candidate_count;   // Number of candidates running

// Function prototypes - detailed descriptions below implementations
bool vote(int voter, int rank, string name);
void tabulate(void);
bool print_winner(void);
int find_min(void);
bool is_tie(int min);
void eliminate(int min);

int main(int argc, string argv[])
{
    /*******************************************
     * INITIAL ELECTION SETUP
     *******************************************/

    // Validate command-line arguments
    if (argc < 2)
    {
        printf("Usage: runoff [candidate ...]\n");
        return 1;  // Exit: no candidates provided
    }

    // Initialize candidates from command-line arguments
    candidate_count = argc - 1;  // First argument is program name

    // Ensure candidate count doesn't exceed maximum
    if (candidate_count > MAX_CANDIDATES)
    {
        printf("Maximum number of candidates is %i\n", MAX_CANDIDATES);
        return 2;  // Exit: too many candidates
    }

    // Populate candidate array with names and default values
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i].name = argv[i + 1];  // Skip program name
        candidates[i].votes = 0;            // Initialize votes to zero
        candidates[i].eliminated = false;   // All start active
    }

    // Get number of voters from user
    voter_count = get_int("Number of voters: ");

    // Ensure voter count doesn't exceed maximum
    if (voter_count > MAX_VOTERS)
    {
        printf("Maximum number of voters is %i\n", MAX_VOTERS);
        return 3;  // Exit: too many voters
    }

    /*******************************************
     * VOTE COLLECTION PROCESS
     *******************************************/

    // Collect ranked preferences from each voter
    for (int voter = 0; voter < voter_count; voter++)
    {
        // Collect each rank preference (1st, 2nd, 3rd, etc.)
        for (int rank = 0; rank < candidate_count; rank++)
        {
            string name = get_string("Rank %i: ", rank + 1);  // rank+1 for 1-based display

            // Validate and record vote
            if (!vote(voter, rank, name))
            {
                printf("Invalid vote.\n");
                return 4;  // Exit: invalid candidate name
            }
        }
        printf("\n");  // Separate voter entries
    }

    /*******************************************
     * RUNOFF ELECTION PROCESSING
     *******************************************/

    // Continue runoff rounds until winner is determined
    while (true)
    {
        // Count votes for current active candidates
        tabulate();

        // Check for immediate winner
        bool won = print_winner();
        if (won) break;  // Exit loop if winner found

        // Find lowest vote count among active candidates
        int min = find_min();

        // Check if all remaining candidates are tied
        bool tie = is_tie(min);

        // Handle tie scenario
        if (tie)
        {
            // Print all remaining candidates as winners
            for (int i = 0; i < candidate_count; i++)
            {
                if (!candidates[i].eliminated)
                {
                    printf("%s\n", candidates[i].name);
                }
            }
            break;  // Exit election process
        }

        // Eliminate candidate(s) with minimum votes
        eliminate(min);

        // Reset votes for next runoff round
        for (int i = 0; i < candidate_count; i++)
        {
            candidates[i].votes = 0;
        }
    }

    return 0;  // Successful election completion
}

/*******************************************
 * FUNCTION IMPLEMENTATIONS
 *******************************************/

// Records a voter's preference if valid
// voter: voter index (0 to voter_count-1)
// rank: preference level (0 = highest, candidate_count-1 = lowest)
// name: candidate name entered
// Returns: true if valid candidate, false otherwise
bool vote(int voter, int rank, string name)
{
    // Search candidate list for matching name
    for (int i = 0; i < candidate_count; i++)
    {
        // Case-sensitive name comparison
        if (strcmp(candidates[i].name, name) == 0)
        {
            // Record candidate index in preferences array
            preferences[voter][rank] = i;
            return true;  // Valid vote recorded
        }
    }
    return false;  // Candidate not found
}

// Tabulates votes for current active candidates
// Each voter's vote counts for their highest-ranked active candidate
void tabulate(void)
{
    // Process each voter's ballot
    for (int voter = 0; voter < voter_count; voter++)
    {
        int rank = 0;  // Start with highest preference

        // Find highest-ranked active candidate
        while (rank < candidate_count)
        {
            int candidate_idx = preferences[voter][rank];

            // Check if candidate is still in race
            if (!candidates[candidate_idx].eliminated)
            {
                // Count vote for candidate
                candidates[candidate_idx].votes++;
                break;  // Vote counted, move to next voter
            }
            rank++;  // Candidate eliminated, check next preference
        }
    }
}

// Checks and prints winner if majority exists
// Majority = 50% + 1 of votes
// Returns: true if winner found, false otherwise
bool print_winner(void)
{
    // Calculate majority threshold
    int majority = (voter_count / 2) + 1;

    // Check all candidates
    for (int i = 0; i < candidate_count; i++)
    {
        // Candidate must be active and have majority
        if (!candidates[i].eliminated && candidates[i].votes >= majority)
        {
            printf("%s\n", candidates[i].name);  // Announce winner
            return true;  // Winner found
        }
    }
    return false;  // No majority winner
}

// Finds minimum vote count among active candidates
// Returns: minimum vote count value
int find_min(void)
{
    // Initialize with maximum possible votes
    int min_votes = voter_count;

    // Check all candidates
    for (int i = 0; i < candidate_count; i++)
    {
        // Only consider active candidates
        if (!candidates[i].eliminated)
        {
            // Update min_votes if current candidate has fewer votes
            if (candidates[i].votes < min_votes)
            {
                min_votes = candidates[i].votes;
            }
        }
    }
    return min_votes;
}

// Checks if all active candidates are tied
// min: the current minimum vote count (should be same for all if tied)
// Returns: true if all active candidates have 'min' votes, false otherwise
bool is_tie(int min)
{
    // Check all candidates
    for (int i = 0; i < candidate_count; i++)
    {
        // Skip eliminated candidates
        if (candidates[i].eliminated) continue;

        // If any active candidate doesn't have 'min' votes → not a tie
        if (candidates[i].votes != min) return false;
    }
    return true;  // All active candidates have same vote count
}

// Eliminates candidate(s) with specified vote count
// min: the vote count threshold for elimination
void eliminate(int min)
{
    // Check all candidates
    for (int i = 0; i < candidate_count; i++)
    {
        // Eliminate if active and has minimum votes
        if (!candidates[i].eliminated && candidates[i].votes == min)
        {
            candidates[i].eliminated = true;  // Mark as eliminated
        }
    }
}
