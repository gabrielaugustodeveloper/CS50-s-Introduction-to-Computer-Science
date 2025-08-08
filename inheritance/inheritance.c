// Simulate genetic inheritance of blood type
#define _DEFAULT_SOURCE  // Required for random number generation on some systems
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Each person has two parents and two alleles (genes for blood type)
typedef struct person
{
    struct person *parents[2];  // Pointer array for two parents
    char alleles[2];            // Array for two alleles (e.g., 'A', 'B', 'O')
} person;

// Constants for simulation
const int GENERATIONS = 3;     // Depth of family tree (child + parents + grandparents)
const int INDENT_LENGTH = 4;   // Spaces per generation for tree visualization

// Function prototypes
person *create_family(int generations);
void print_family(person *p, int generation);
void free_family(person *p);
char random_allele();

int main(void)
{
    // Seed random number generator with current time
    srandom(time(0));

    // Create family tree with specified generations
    person *child = create_family(GENERATIONS);

    // Print family blood type tree starting from child (generation 0)
    print_family(child, 0);

    // Free all allocated memory in family tree
    free_family(child);

    return 0;
}

// Creates a family tree with specified generations
person *create_family(int generations)
{
    // Allocate memory for new person
    person *new_person = malloc(sizeof(person));
    if (new_person == NULL)
    {
        printf("Memory allocation failed\n");
        exit(1);
    }

    // Create parents if more generations needed
    if (generations > 1)
    {
        // Recursively create parent generation (one level deeper)
        person *parent0 = create_family(generations - 1);
        person *parent1 = create_family(generations - 1);

        // Set parent pointers
        new_person->parents[0] = parent0;
        new_person->parents[1] = parent1;

        // Inherit alleles from parents:
        // - Randomly select one allele from each parent
        // - Parent 0 contributes to allele 0
        // - Parent 1 contributes to allele 1
        new_person->alleles[0] = parent0->alleles[random() % 2];
        new_person->alleles[1] = parent1->alleles[random() % 2];
    }
    else  // Base case: oldest generation
    {
        // Set parents to NULL (no further ancestors)
        new_person->parents[0] = NULL;
        new_person->parents[1] = NULL;

        // Randomly assign alleles for oldest generation
        new_person->alleles[0] = random_allele();
        new_person->alleles[1] = random_allele();
    }

    return new_person;
}

// Frees memory for person and all ancestors
void free_family(person *p)
{
    // Base case: stop recursion at NULL pointer
    if (p == NULL)
    {
        return;
    }

    // Recursively free both parents (if they exist)
    free_family(p->parents[0]);
    free_family(p->parents[1]);

    // Free current person after freeing ancestors
    free(p);
}

// Prints family tree with blood type information
void print_family(person *p, int generation)
{
    // Base case: stop recursion at NULL pointer
    if (p == NULL)
    {
        return;
    }

    // Print indentation proportional to generation depth
    for (int i = 0; i < generation * INDENT_LENGTH; i++)
    {
        printf(" ");
    }

    // Print person based on generation
    if (generation == 0)
    {
        printf("Child (Generation %i): blood type %c%c\n",
               generation, p->alleles[0], p->alleles[1]);
    }
    else if (generation == 1)
    {
        printf("Parent (Generation %i): blood type %c%c\n",
               generation, p->alleles[0], p->alleles[1]);
    }
    else
    {
        // Handle great-grandparents and beyond
        for (int i = 0; i < generation - 2; i++)
        {
            printf("Great-");
        }
        printf("Grandparent (Generation %i): blood type %c%c\n",
               generation, p->alleles[0], p->alleles[1]);
    }

    // Recursively print parents (next generation)
    print_family(p->parents[0], generation + 1);
    print_family(p->parents[1], generation + 1);
}

// Generates random blood type allele (A, B, or O)
char random_allele()
{
    // Get random number 0-2
    int r = random() % 3;

    // Return allele based on random value
    if (r == 0)
    {
        return 'A';
    }
    else if (r == 1)
    {
        return 'B';
    }
    else
    {
        return 'O';
    }
}
