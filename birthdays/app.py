# Import necessary modules
import os  # Operating system interface
from cs50 import SQL  # CS50 database library for simplified SQL interaction
from flask import (  # Flask web framework components
    Flask,  # Core Flask application class
    flash,  # For displaying temporary messages to users
    jsonify,  # For returning JSON responses
    redirect,  # For redirecting to different routes
    render_template,  # For rendering HTML templates
    request,  # For accessing incoming request data
    session  # For storing user session information
)

# Initialize Flask application instance
app = Flask(__name__)

# Configure application settings
app.config["TEMPLATES_AUTO_RELOAD"] = True  # Reload templates on change for development

# Set up database connection using CS50's SQL wrapper
# Uses SQLite database file named birthdays.db
db = SQL("sqlite:///birthdays.db")

# Register after_request hook to modify responses
@app.after_request
def after_request(response):
    """
    Post-request processing to prevent caching of pages.
    Ensures users always see the most recent content.
    """
    # Set HTTP headers to disable caching
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response

# Define main route that handles both GET and POST requests
@app.route("/", methods=["GET", "POST"])
def index():
    """
    Main controller function for the birthday application:
    - Handles form submission via POST
    - Displays birthday records via GET
    """
    
    if request.method == "POST":
        # Process form submission
        
        # Extract form data using Flask's request object
        name = request.form.get("name")        # Get name from form
        month = request.form.get("month")       # Get month from form
        day = request.form.get("day")           # Get day from form

        # Validate presence of all required fields
        if name and month and day:
            try:
                # Convert to integers for validation
                month = int(month)
                day = int(day)
                
                # Validate date ranges
                if 1 <= month <= 12 and 1 <= day <= 31:
                    # Insert valid data into database
                    db.execute(
                        "INSERT INTO birthdays (name, month, day) VALUES (?, ?, ?)",
                        name, month, day
                    )
                else:
                    # Handle invalid date ranges
                    flash("Invalid date: Month must be 1-12, Day must be 1-31")
                    
            except ValueError:
                # Handle non-numeric input for month/day
                flash("Please enter valid numbers for month and day")
        else:
            # Handle missing form fields
            flash("Please fill all fields")

        # Redirect to homepage after processing POST request
        # PRG pattern (Post/Redirect/Get) prevents form resubmission
        return redirect("/")

    else:
        # Handle GET request
        
        # Retrieve all birthday records from database
        birthdays = db.execute("SELECT * FROM birthdays")
        
        # Render HTML template with database records
        return render_template("index.html", birthdays=birthdays)

# Application entry point (if run directly)
if __name__ == "__main__":
    app.run(debug=True)  # Start development server with debug mode