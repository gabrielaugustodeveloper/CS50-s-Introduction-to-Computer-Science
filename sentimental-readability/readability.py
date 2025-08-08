def main():
    """
    Main function that calculates text readability using Coleman-Liau formula.
    Steps:
    1. Prompt user for text input
    2. Count letters, words, and sentences
    3. Calculate readability index
    4. Output grade level
    """

    # Prompt user for text input
    text = input("Enter text: ")

    """
    Initialize counters:
    - letters: alphabetic characters (a-z, A-Z)
    - words: separated by spaces (initialized to 1 to account for first word)
    - sentences: terminated by '.', '!', or '?'
    """
    letters = 0
    words = 1 if text else 0  # Handle empty string case
    sentences = 0

    """
    Text processing loop:
    - Iterate through each character in the input text
    - Update counters based on character type
    - Uses string methods for efficient character classification
    """
    for char in text:
        if char.isalpha():
            letters += 1
        elif char.isspace():
            words += 1
        elif char in ['.', '!', '?']:
            sentences += 1

    """
    Coleman-Liau formula variables:
    - L: average letters per 100 words
    - S: average sentences per 100 words

    Formula: index = 0.0588 * L - 0.296 * S - 15.8

    Special case handling:
    - Avoid division by zero if no words
    - Round to nearest integer for grade level
    """
    if words == 0:  # Prevent division by zero
        grade = 0
    else:
        # Calculate averages per 100 words
        L = (letters / words) * 100
        S = (sentences / words) * 100

        # Apply Coleman-Liau formula
        grade = round(0.0588 * L - 0.296 * S - 15.8)

    """
    Grade level interpretation:
    - Below 1: "Before Grade 1"
    - 16+: "Grade 16+"
    - Others: "Grade X"
    """
    if grade < 1:
        print("Before Grade 1")
    elif grade >= 16:
        print("Grade 16+")
    else:
        print(f"Grade {grade}")

"""
Python execution guard:
- Standard practice for script execution
- Ensures main() runs only when file is executed directly
"""
if __name__ == "__main__":
    main()
