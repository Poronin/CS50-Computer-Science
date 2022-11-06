#include <cs50.h>
#include <stdio.h>

int n;
int space;

int main(void)
{
    do
    {
        n = get_int("Height: ");
    }
    while (n < 1 | n > 8);

    for (int i = 1; i <= n; i++)
    {
        space = n - i;
        for (int j = 0 ; j < space ; j++)
        {
            printf(" ");
        }

        for (int k = 0 ; k < i; k++)
        {
            printf("#");
        }

        printf("  ");

        for (int k = 0 ; k < i; k++)
        {
            printf("#");
        }

        printf("\n");
    }
}