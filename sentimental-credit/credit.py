from cs50 import get_string
import numpy as np


def main():
    credit_card_raw = get_string("Number: ")
    credit_card = np.array(list(map(int, credit_card_raw)))

    index1 = [i for i in range(-2, -len(credit_card)-1, -2)]
    sum1 = credit_card[index1]*2

    digits = []
    for i in str(sum1):
        for j in str(i):
            if j in ['1', '2', '3', '4', '5', '6', '7', '8', '9']:
                digits.append(j)

    sum1 = np.sum(np.array(list(map(int, digits))))

    index2 = [i for i in range(-1, -len(credit_card)-1, -2)]
    sum2 = np.sum(credit_card[index2])

    if str(sum1 + sum2)[-1] != '0':
        print("INVALID")
    elif credit_card_raw.startswith(('34', '37')):
        print("AMEX")
    elif credit_card_raw.startswith(('51', '52', '53', '54', '55')):
        print("MASTERCARD")
    elif credit_card_raw.startswith(('4')):
        print("VISA")


main()