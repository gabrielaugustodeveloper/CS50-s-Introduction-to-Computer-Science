import os
import requests
from flask import redirect, render_template, session
from functools import wraps

def apology(message, code=400):
    """Render message as an apology to user"""
    def escape(s):
        """
        Escape special characters.
        https://github.com/jacebrowning/memegen#special-characters
        """
        for old, new in [
            ("-", "--"), (" ", "-"), ("_", "__"), ("?", "~q"),
            ("%", "~p"), ("#", "~h"), ("/", "~s"), ('"', "''"),
        ]:
            s = s.replace(old, new)
        return s
    return render_template("apology.html", top=code, bottom=escape(message)), code

def login_required(f):
    """
    Decorate routes to require login.
    https://flask.palletsprojects.com/en/latest/patterns/viewdecorators/
    """
    @wraps(f)
    def decorated_function(*args, **kwargs):
        if session.get("user_id") is None:
            return redirect("/login")
        return f(*args, **kwargs)
    return decorated_function

def lookup(symbol):
    """Look up quote for symbol"""
    # Prepare API request
    symbol = symbol.upper()
    api_key = os.environ.get("API_KEY") or "pk_0c6b2d2c5c1f4c3b8d0d3b3f3c3b3f3c"
    url = f"https://api.iex.cloud/v1/data/core/quote/{symbol}?token={api_key}"

    try:
        # Make API request
        response = requests.get(url)
        response.raise_for_status()
        data = response.json()

        # Ensure response contains data
        if not data or not isinstance(data, list) or len(data) == 0:
            return None

        # Extract and return stock information
        return {
            "name": data[0]["companyName"],
            "price": float(data[0]["latestPrice"]),
            "symbol": data[0]["symbol"]
        }
    except (requests.RequestException, ValueError, KeyError, IndexError):
        return None

def usd(value):
    """Format value as USD"""
    return f"${value:,.2f}"
