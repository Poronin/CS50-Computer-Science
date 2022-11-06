import os
import re

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""

    total_current_wallet = 0.0
    total_benefit = 0.0
    benefit = 0.0

    rows = db.execute(
        "SELECT symbol, sum(shares) shares FROM history WHERE id=? GROUP BY symbol HAVING sum(shares) > 0", session["user_id"])
    for row in rows:

        # current stock price
        current_stock = lookup(row["symbol"])
        current_stock_price = float(current_stock["price"])
        row["current_stock_price"] = current_stock_price

        # current wallet
        shares = int(row["shares"])
        current_wallet = current_stock_price * shares
        row["current_wallet"] = current_wallet

        # current wallet
        total_current_wallet = total_current_wallet + current_wallet

    # get cash
    rows_1 = db.execute("SELECT cash FROM users WHERE id=?", session["user_id"])
    cash = float(rows_1[0]["cash"])
    total = cash + total_current_wallet

    return render_template("index.html", rows=rows, total=total, cash=cash)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("symbol"):
            return apology("must provide stock", 400)

        elif found_not_character(request.form.get("symbol")):
            return apology("Invalid data type", 400)

        elif detect_sql_injection_symbol(request.form.get("symbol")):
            return apology("Invalid input", 400)

        elif not lookup(request.form.get("symbol")):
            return apology("Stock symbol not found", 400)

        elif found_not_digit(request.form.get("shares")):
            return apology("Invalid data type input", 400)

        elif 0 >= int(request.form.get("shares")):
            return apology("Incorrect amount of shares to buy", 400)

        else:

            shares_to_buy = int(request.form.get("shares"))
            stock_info = lookup(request.form.get("symbol"))
            current_share_price = float(stock_info["price"])

            row = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])
            current_cash = float(row[0]["cash"])

            updated_cash = current_cash - shares_to_buy * current_share_price

            if current_cash < 0:
                return apology("Not enough money", 400)
            else:
                db.execute("UPDATE users SET cash = ? WHERE id = ?", updated_cash, session["user_id"])
                db.execute("INSERT INTO history (id, symbol, shares, value, wallet) VALUES (?,?,?,?,?)",
                           session["user_id"], stock_info["symbol"], shares_to_buy, current_share_price,  current_share_price * shares_to_buy)

            # Redirect user to home page
            return redirect("/")
    else:
        return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""

    rows = db.execute("SELECT symbol, shares, value , wallet , time FROM history WHERE id=?", session["user_id"])

    return render_template("history.html", rows=rows)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        elif detect_sql_injection_symbol(request.form.get("username")):
            return apology("Invalid input", 400)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        elif detect_sql_injection_symbol(request.form.get("password")):
            return apology("Invalid input", 400)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():

    if request.method == "POST":
        """Get stock quote."""

        # Ensure username was submitted
        if not request.form.get("symbol"):
            return apology("must provide stock", 400)

        elif not lookup(request.form.get("symbol")):
            return apology("Stock symbol not found", 400)

        elif detect_sql_injection_symbol(request.form.get("symbol")):
            return apology("Invalid input", 400)

        return render_template("/quoted.html", stock=lookup(request.form.get("symbol")))

    else:
        return render_template("/quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 400)

        elif detect_sql_injection_symbol(request.form.get("username")):
            return apology("Invalid input", 400)

        # Ensure username does not exist
        elif 0 < len(db.execute("SELECT * FROM users WHERE username = ?",
                                request.form.get("username"))):
            return apology("must provide different username", 400)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 400)

        # Ensure password was submitted
        elif not request.form.get("confirmation"):
            return apology("must provide confirmation", 400)

        # Ensure password was submitted
        elif not (request.form.get("password") == request.form.get("confirmation")):
            return apology("Pasword do not match", 400)

        elif not validate_password(request.form.get("password")):
            password_validation = validate_password(request.form.get("password"))
            print(password_validation)
            return render_template("register.html", password_validation=password_validation)

        db.execute("INSERT INTO users (username, hash) VALUES (?, ?)",
                   request.form.get("username"),
                   generate_password_hash(request.form.get("password"),
                                          method='pbkdf2:sha256',
                                          salt_length=8))

        # Query database for username
        user_name = request.form.get("username")
        rows = db.execute("SELECT * FROM users WHERE username = ?", user_name)
        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        return redirect("/")

    else:
        password_validation = True
        return render_template("register.html", password_validation=password_validation)


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""

    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("symbol"):
            return apology("must provide stock", 400)

        elif found_not_character(request.form.get("symbol")):
            return apology("Incorrect data type", 400)

        elif detect_sql_injection_symbol(request.form.get("symbol")):
            return apology("Invalid input", 400)

        elif found_not_digit(request.form.get("shares")):
            return apology("Incorrect data type", 400)

        elif 0 >= int(request.form.get("shares")):
            return apology("Incorrect amount of shares to buy", 400)

        else:

            shares_to_sell = int(request.form.get("shares"))
            stock_info = lookup(request.form.get("symbol"))
            current_share_price = float(stock_info["price"])

            rows = db.execute("SELECT id, symbol, sum(shares) shares, sum(shares * value) wallet FROM history WHERE id = ? AND symbol=? GROUP BY id, symbol",
                              session["user_id"], request.form.get("symbol"))

            current_wallet = float(rows[0]["wallet"])
            current_shares = float(rows[0]["shares"])

            if shares_to_sell > current_shares:
                return apology("Not enough shares", 400)

            bought_share_price = current_wallet / current_shares
            index = current_share_price / bought_share_price
            margen = index * shares_to_sell * bought_share_price

            rows_1 = db.execute("SELECT cash FROM users WHERE id=?", session["user_id"])
            current_cash = float(rows_1[0]['cash'])
            updated_cash = current_cash + margen

            db.execute("INSERT INTO history (id, symbol, shares, value, wallet) VALUES (?,?,?,?,?)",
                       rows[0]["id"], rows[0]["symbol"], -1 * shares_to_sell, current_share_price, current_share_price * shares_to_sell)
            db.execute("UPDATE users SET cash = ? WHERE id=?",
                       updated_cash, session["user_id"])

            # Redirect user to home page
            return redirect("/")
    else:
        rows = db.execute("SELECT DISTINCT symbol FROM history WHERE id=?", session["user_id"])
        return render_template("sell.html", rows=rows)


def detect_sql_injection_symbol(symbol):
    return bool(re.search("[';]", symbol))


def found_not_digit(number):
    return bool(re.search("[\D]", number))


def found_not_character(word):
    return bool(re.search("[^a-zA-Z]", word))


def validate_password(password):
    return bool(re.search("^.*(?=.{8,})(?=.*[a-zA-Z])(?=.*\d).*$", password))