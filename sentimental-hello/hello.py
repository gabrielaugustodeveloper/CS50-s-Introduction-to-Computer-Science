# Get User Input and Print Greeting
# This program demonstrates basic user input/output in Python

def main():
    """
    Main function that orchestrates the program flow:
    1. Prompt user for name
    2. Process input
    3. Print personalized greeting
    """
    # Prompt user for their name
    # input() displays the string argument and waits for user input
    # The entered text is stored in the 'name' variable
    name = input("What is your name? ").strip()

    """
    Why we use .strip():
    - Removes leading/trailing whitespace that might be accidentally entered
    - Prevents "hello,  John " with extra spaces
    - Handles cases where user presses spacebar before/after typing
    """

    # Print personalized greeting
    # Using f-string for formatted string literals (Python 3.6+ feature)
    # This embeds the {name} variable directly into the string
    print(f"hello, {name}")

    """
    Alternative printing methods:
    1. Concatenation: print("hello, " + name)
    2. Format method: print("hello, {}".format(name))
    3. Old-style: print("hello, %s" % name)

    f-strings are preferred for:
    - Readability
    - Performance (fastest execution)
    - Direct variable access
    """

# Standard Python idiom to check if script is run directly
# This prevents main() from running if imported as a module
if __name__ == "__main__":
    """
    __name__ is a special built-in variable:
    - When run directly: __name__ = "__main__"
    - When imported: __name__ = module's name

    This structure allows:
    - Direct execution: python hello.py
    - Safe module import: import hello (won't run main())
    """
    main()  # Start program execution
