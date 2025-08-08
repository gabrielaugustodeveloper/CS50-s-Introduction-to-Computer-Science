def main():
    """
    Main function that controls the pyramid creation process.
    Steps:
    1. Get valid height input from user
    2. Print pyramid with proper alignment
    3. Handle edge cases and errors
    """

    # Loop until valid input is received
    while True:
        try:
            # Prompt user for pyramid height
            height = int(input("Height: "))

            """
            Input validation criteria:
            - Must be integer (handled by int() conversion)
            - Must be between 1 and 8 inclusive
            - We re-prompt on invalid input using continue
            """
            if height < 1 or height > 8:
                print("Height must be between 1 and 8")
                continue  # Skip to next iteration

            break  # Exit loop on valid input

        except ValueError:
            # Handle non-integer input
            print("Please enter a whole number between 1 and 8")

    """
    Pyramid printing logic:
    - For each row from 1 to height:
        1. Print leading spaces: (height - row) spaces
        2. Print hashes: (row) '#' characters
        3. Print newline

    Example (height=3):
        Row 1: 2 spaces + 1 hash  -> "  #"
        Row 2: 1 space  + 2 hashes -> " ##"
        Row 3: 0 spaces + 3 hashes -> "###"
    """
    for row in range(1, height + 1):
        # Print leading spaces
        print(" " * (height - row), end="")

        # Print hashes
        print("#" * row)

"""
Python execution guard:
- Prevents running when imported as module
- Allows direct execution: python pyramid.py
- Standard Python idiom for main function execution
"""
if __name__ == "__main__":
    main()
