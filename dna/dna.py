import csv
from sys import argv


def main():

    # Check for command-line usage
    if len(argv) != 3:
        print("Comand-line arguments error")
        exit()

    people = []
    # Read database file into a variable
    with open(argv[1], "r") as file:
        reader = csv.DictReader(file)
        for row in reader:
            people.append(row)

    str = []
    for k, v in people[0].items():
        str.append(k)
    str = str[1:]

    # Read DNA sequence file into a variable
    with open(argv[2], "r") as file:
        dna = file.readline()

    # Find longest match of each STR in DNA sequence
    for person in people:
        for i in str:
            n = longest_match(dna, i)
            if int(person[i]) == n:
                found = person["name"]
            else:
                found = "No match"
                break

        if found != "No match":
            break

    print(found)

    return


def longest_match(sequence, subsequence):
    """Returns length of longest run of subsequence in sequence."""

    # Initialize variables
    longest_run = 0
    subsequence_length = len(subsequence)
    sequence_length = len(sequence)

    # Check each character in sequence for most consecutive runs of subsequence
    for i in range(sequence_length):

        # Initialize count of consecutive runs
        count = 0

        # Check for a subsequence match in a "substring" (a subset of characters) within sequence
        # If a match, move substring to next potential match in sequence
        # Continue moving substring and checking for matches until out of consecutive matches
        while True:

            # Adjust substring start and end
            start = i + count * subsequence_length
            end = start + subsequence_length

            # If there is a match in the substring
            if sequence[start:end] == subsequence:
                count += 1

            # If there is no match in the substring
            else:
                break

        # Update most consecutive matches found
        longest_run = max(longest_run, count)

    # After checking for runs at each character in seqeuence, return longest run found
    return longest_run


main()
