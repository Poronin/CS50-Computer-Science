from cs50 import get_int


def main():
    while True:
        n = get_int("Height: ")
        if n > 0 and n < 9:
            piramid(n)
            break


def piramid(n, counter=0):
    hashtag = "#"
    space = " "

    if n == 0:
        return
    else:
        piramid(n-1, counter+1)
        text = space*counter + hashtag*n + space*2 + n*"#"
        print(f"{text}")


main()
