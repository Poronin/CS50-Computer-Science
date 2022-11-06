-- Keep a log of any SQL queries you execute as you solve the mystery.

-- Read the reports:
SELECT * FROM crime_scene_reports
WHERE year = 2021
AND month = 7
AND day = 28;
-- 10:15AM / Humphrey street backery / 3 Witnesses


-- See the interviews info:
SELECT * FROM interviews WHERE transcript LIKE "%bakery%";
-- Ruth - Leaves the parking lot in 10 min time frame.
-- Eugene - before 10:15 / ATM Leggett Street Withdrawing money
-- Raymond - Earliest Flight on 29 07 2021 from Fiftyville / someone buying the ticket from 10:15


-- get all license plate
SELECT license_plate FROM bakery_security_logs
WHERE activity = "exit"
AND year = 2021
AND month = 7
AND day = 28
AND hour = 10 AND minute BETWEEN 15 AND 45;

-- get account & amount
SELECT account_number FROM atm_transactions
WHERE year = 2021
AND month = 7
AND day = 28
AND transaction_type = "withdraw"
AND atm_location LIKE "%Leggett%";

-- Phone "caller" number / "receiver" number
SELECT caller, receiver FROM phone_calls
WHERE year = 2021
AND month = 7
AND day = 28
AND duration < 60;

-- Get passaport numbers
SELECT *
FROM passengers
WHERE flight_id IN
(
    SELECT flights.id
    FROM airports
    JOIN flights ON airports.id= flights.origin_airport_id
    WHERE year = 2021
    AND month = 7
    AND day = 29
    ORDER BY flights.hour DESC
    LIMIT 1
);

-- Put all together!!
SELECT * FROM bank_accounts
JOIN people ON bank_accounts.person_id=people.id
WHERE
account_number IN           (
                            SELECT account_number FROM atm_transactions
                            WHERE year = 2021
                            AND month = 7
                            AND day = 28
                            AND transaction_type = "withdraw"
                            AND atm_location LIKE "%Leggett%"
                            )
AND phone_number IN         (
                            SELECT caller FROM phone_calls
                            WHERE year = 2021
                            AND month = 7
                            AND day = 28
                            AND duration < 60
                            )
AND license_plate IN        (
                            SELECT license_plate FROM bakery_security_logs
                            WHERE activity = "exit"
                            AND year = 2021
                            AND month = 7
                            AND day = 28
                            AND hour = 10 AND minute BETWEEN 15 AND 25
                            )
AND passport_number IN      (
                            SELECT passport_number
                            FROM passengers
                            WHERE flight_id IN
                            (
                                SELECT flights.id
                                FROM airports
                                JOIN flights ON airports.id= flights.origin_airport_id
                                WHERE year = 2021
                                AND month = 7
                                AND day = 29
                                ORDER BY flights.hour ASC
                                LIMIT 1
                            )
);
--+----------------+-----------+---------------+--------+--------+----------------+-----------------+---------------+
--| account_number | person_id | creation_year |   id   |  name  |  phone_number  | passport_number | license_plate |
--+----------------+-----------+---------------+--------+--------+----------------+-----------------+---------------+
--| 49610011       | 686048    | 2010          | 686048 | Bruce  | (367) 555-5533 | 5773159633      | 94KL13X       |
--+----------------+-----------+---------------+--------+--------+----------------+-----------------+---------------+

-- Get the receiver call.
SELECT caller, receiver FROM phone_calls
WHERE year = 2021
AND month = 7
AND day = 28
AND duration < 60
AND caller = "(367) 555-5533";
---+----------------+----------------+
--|     caller     |    receiver    |
--+----------------+----------------+
--| (367) 555-5533 | (375) 555-8161 |
--+----------------+----------------+

-- Get name of accomplice.
SELECT * FROM people
WHERE phone_number = "(375) 555-8161";
--+--------+-------+----------------+-----------------+---------------+
--|   id   | name  |  phone_number  | passport_number | license_plate |
--+--------+-------+----------------+-----------------+---------------+
--| 864400 | Robin | (375) 555-8161 |                 | 4V16VO0       |
--+--------+-------+----------------+-----------------+---------------+

-- Get city.
SELECT city
FROM airports
JOIN flights ON airports.id= flights.destination_airport_id
JOIN passengers ON passengers.flight_id=flights.id
WHERE passport_number = 5773159633;
--+---------------+
--|     city      |
--+---------------+
--| New York City |
--+---------------+
