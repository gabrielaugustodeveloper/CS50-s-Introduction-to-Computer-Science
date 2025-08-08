import csv
import sys

def main():
    """
    Main function that identifies a person based on DNA sequence.
    Steps:
    1. Validate command-line arguments
    2. Read DNA database (CSV file)
    3. Read DNA sequence file
    4. Compute longest STR matches
    5. Identify matching profile in database
    6. Print result
    """

    # Step 1: Validate command-line arguments
    if len(sys.argv) != 3:
        print("Usage: python dna.py data.csv sequence.txt")
        sys.exit(1)

    # Step 2: Read DNA database (CSV file)
    database = []
    # Get STR names from first row (fieldnames)
    with open(sys.argv[1], "r") as csv_file:
        # Use DictReader for easy column access
        reader = csv.DictReader(csv_file)
        # Extract STR names (all fields except 'name')
        str_names = reader.fieldnames[1:]

        # Read all records into database list
        for row in reader:
            database.append(row)

    # Step 3: Read DNA sequence file
    with open(sys.argv[2], "r") as dna_file:
        dna_sequence = dna_file.read().strip()  # Remove any trailing newlines

    # Step 4: Compute longest matches for each STR
    str_counts = {}
    for subsequence in str_names:
        # Calculate longest consecutive run for current STR
        str_counts[subsequence] = longest_match(dna_sequence, subsequence)

    # Step 5: Check database for matching profiles
    found_match = False
    for person in database:
        match = True
        # Check all STR counts for current person
        for str_name in str_names:
            # Convert database value to integer for comparison
            # Note: CSV stores all values as strings
            if int(person[str_name]) != str_counts[str_name]:
                match = False
                break  # No need to check further STRs

        # If all STR counts match
        if match:
            print(person['name'])
            found_match = True
            break  # Exit after first match

    # Step 6: Handle no-match case
    if not found_match:
        print("No match")

def longest_match(sequence, subsequence):
    """
    Calculate the longest consecutive run of a DNA subsequence.

    Args:
        sequence: The full DNA sequence string
        subsequence: The STR pattern to search for (e.g., "AGAT")

    Returns:
        Integer representing the longest consecutive run
    """
    # Initialize tracking variables
    longest_run = 0
    sub_len = len(subsequence)
    seq_len = len(sequence)

    # Optimize: Skip if subsequence is empty
    if sub_len == 0:
        return 0

    # Iterate through each position in DNA sequence
    for i in range(seq_len):
        # Reset count for current starting position
        count = 0

        # Check for consecutive matches starting at position i
        while True:
            # Calculate next segment position
            start = i + count * sub_len
            end = start + sub_len

            # Prevent index overflow
            if end > seq_len:
                break

            # Check if current segment matches subsequence
            if sequence[start:end] == subsequence:
                count += 1
            else:
                break  # Sequence broken

        # Update longest run if current is longer
        if count > longest_run:
            longest_run = count

    return longest_run

if __name__ == "__main__":
    main()
