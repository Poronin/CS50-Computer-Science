from cs50 import get_string
import re

text = get_string("Text: ")

words = len(text.split(" "))

sentences = re.split(r"[!?.]", text)
sentences = [w for w in sentences if w != '' and w != '"']

sentences = len(sentences)
letters = text.split()

letters = [l for l in text if l.isalpha()]
letters = len(letters)

index = (0.0588 * letters / words * 100.0) - (0.296 * sentences / words * 100.0) - 15.8
grade = round(index)

if grade > 16:
    print("Grade 16+")
elif grade < 1:
    print(f"Before Grade 1")
else:
    print(f"Grade {grade}")