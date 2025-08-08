-- Keep a log of any SQL queries you execute as you solve the mystery.
/* INITIAL CRIME SCENE ANALYSIS */
-- Retrieve crime scene report for Humphrey Street on July 28, 2021
-- Focus on theft description and key details
SELECT *
FROM crime_scene_reports
WHERE
    street = 'Humphrey Street'
    AND year = 2021
    AND month = 7
    AND day = 28
    AND description LIKE '%duck%';  -- Filter for relevant incident


/* WITNESS INTERVIEWS */
-- Check witness interviews within 3 days of incident
-- Critical for modus operandi and suspect behavior
SELECT
    name,
    transcript
FROM interviews
WHERE
    year = 2021
    AND month = 7
    AND day BETWEEN 28 AND 30
    AND transcript LIKE '%bakery%';  -- Filter for bakery-related accounts


/* FINANCIAL INVESTIGATION */
-- Analyze ATM transactions on Leggett Street (per witness account)
-- Withdrawals only on theft morning
SELECT
    account_number,
    amount
FROM atm_transactions
WHERE
    year = 2021
    AND month = 7
    AND day = 28
    AND atm_location = 'Leggett Street'
    AND transaction_type = 'withdraw';


/* SUSPECT COMMUNICATIONS */
-- Identify short calls (<60s) on theft day (per witness)
-- Potential accomplice coordination
SELECT
    caller,
    receiver,
    duration
FROM phone_calls
WHERE
    year = 2021
    AND month = 7
    AND day = 28
    AND duration < 60;  -- Under 1 minute calls


/* ESCAPE ROUTE ANALYSIS */
-- Find earliest flight next day (July 29)
-- Per witness: thief planned early escape
SELECT
    flights.id,
    flights.hour,
    flights.minute,
    airports.city AS destination
FROM flights
JOIN airports ON flights.destination_airport_id = airports.id
WHERE
    flights.year = 2021
    AND flights.month = 7
    AND flights.day = 29
    AND flights.origin_airport_id = (
        SELECT id
        FROM airports
        WHERE city = 'Fiftyville'
    )
ORDER BY
    flights.hour,
    flights.minute
LIMIT 1;  -- Earliest flight only


/* BAKERY SURVEILLANCE */
-- Check security logs around theft time (10:15-10:25am)
-- Witness saw thief leave bakery parking lot
SELECT
    license_plate,
    activity,
    minute
FROM bakery_security_logs
WHERE
    year = 2021
    AND month = 7
    AND day = 28
    AND hour = 10
    AND minute BETWEEN 15 AND 25
    AND activity = 'exit';  -- Departures only


/* SUSPECT CROSS-REFERENCE */
-- Combine all clues to identify thief:
-- 1. Bank account holders near Leggett Street ATM
-- 2. Phone call participants
-- 3. Flight passengers
-- 4. License plates at bakery
SELECT
    people.name AS suspect,
    phone_calls.receiver AS accomplice_contact,
    airports.city AS escape_destination
FROM people
-- Bank account connection
JOIN bank_accounts ON people.id = bank_accounts.person_id
-- Phone records
JOIN phone_calls ON people.phone_number = phone_calls.caller
-- Flight passengers
JOIN passengers ON people.passport_number = passengers.passport_number
JOIN flights ON passengers.flight_id = flights.id
-- Bakery license plates
JOIN bakery_security_logs ON people.license_plate = bakery_security_logs.license_plate
-- ATM transactions
JOIN atm_transactions ON bank_accounts.account_number = atm_transactions.account_number
WHERE
    -- Theft day constraints
    atm_transactions.year = 2021 AND atm_transactions.month = 7 AND atm_transactions.day = 28
    AND atm_transactions.atm_location = 'Leggett Street'
    AND phone_calls.day = 28
    AND flights.day = 29
    AND bakery_security_logs.day = 28
    -- Witness timeline
    AND bakery_security_logs.minute BETWEEN 15 AND 25
    -- Flight criteria
    AND flights.id = (
        SELECT id
        FROM flights
        WHERE year = 2021 AND month = 7 AND day = 29
        ORDER BY hour, minute LIMIT 1
    );
