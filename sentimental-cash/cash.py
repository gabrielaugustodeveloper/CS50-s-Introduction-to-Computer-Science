def main():
    """
    Main function that orchestrates the coin calculation process.
    Steps:
    1. Get valid change amount from user (in dollars)
    2. Convert dollars to cents (integer to avoid float precision issues)
    3. Calculate minimum coins using greedy algorithm
    4. Print result
    """

    # Loop until valid input is received
    while True:
        try:
            # Prompt user for change amount in dollars
            dollars = float(input("Change owed: "))

            """
            Input validation criteria:
            - Must be a valid number (handled by float() conversion)
            - Must be non-negative
            - We re-prompt on invalid input
            """
            if dollars < 0:
                print("Amount must be non-negative")
                continue  # Skip to next iteration

            break  # Exit loop on valid input

        except ValueError:
            # Handle non-numeric input
            print("Please enter a valid number")

    """
    Convert dollars to cents:
    - Multiply by 100 to convert dollars to cents
    - Round to nearest integer to avoid floating-point precision issues
    - Example: $0.41 becomes 41 cents
    """
    cents = round(dollars * 100)

    """
    Coin calculation using greedy algorithm:
    - Start with largest coin value (quarter = 25¢)
    - Use as many as possible before moving to next smaller coin
    - Continue until all change is accounted for

    Coin values in cents:
    Quarter = 25¢, Dime = 10¢, Nickel = 5¢, Penny = 1¢
    """
    coins = 0
    denominations = [25, 10, 5, 1]  # Coin values in descending order

    for denom in denominations:
        # Calculate how many of current denomination can be used
        count = cents // denom

        # Add to total coin count
        coins += count

        # Update remaining cents
        cents -= count * denom

        # Early exit if no cents left
        if cents <= 0:
            break

    # Print final coin count
    print(coins)

"""
Python execution guard:
- Standard practice to prevent execution when imported as module
- Ensures main() only runs when script is executed directly
"""
if __name__ == "__main__":
    main()
