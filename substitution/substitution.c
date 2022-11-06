#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

int keyLenghtValidation(string);
int keyCharacterValidation(string);
void printCypherLowerCase(char, string);
void printCypherUpperCase(char, string);

//https://cs50.harvard.edu/x/2022/psets/2/substitution/

int main(int argc, string argv[])
{

    int returnValue = 0;

    if (argc != 2)
    {
        printf("One parameter expected!\n");
        returnValue = 1;
    }
    else
    {

        if (returnValue == 0)
        {
            returnValue = keyLenghtValidation(argv[1]);
        }

        if (returnValue == 0)
        {
            returnValue = keyCharacterValidation(argv[1]);
        }

        if (returnValue == 0)
        {
            string text = get_string("plaintext: ");
            printf("ciphertext: ");
            for (int i = 0; i < strlen(text); i++)
            {
                if (islower(text[i]))
                {
                    //printf("it is lower.\n");
                    printCypherLowerCase(text[i], argv[1]);
                }
                else
                {
                    //printf("it is upper.\n");
                    printCypherUpperCase(text[i], argv[1]);
                }
            }
            printf("\n");
        }
    }

    return returnValue;

}


// If the key is invalid (as by not containing 26 characters, containing any character that is not an alphabetic character,
// or not containing each letter exactly once), your program should print an error message of your choice (with printf) and
// return from main a value of 1 immediately.

int keyLenghtValidation(string key)
{
    if (strlen(key) != 26)
    {
        printf("ERROR: Key '%s' is not 26 character long!\n", key);
        return 1;
    }
    return 0;
}

// Key validation
int keyCharacterValidation(string key)
{
    int returnValue = 0;

    for (int i = 0; i < strlen(key); i++)
    {
        int character = key[i];
        int counter = 0;

        if (!(isalpha(key[i])))
        {
            printf("ERROR: Character '%c' is not alphabetical character.\n", key[i]);
            returnValue = 1;
            break;
        }

        for (int j = 0; j < strlen(key); j++)
        {
            if (character == key[j])
            {
                counter++;
            }
        }

        if (counter >= 2)
        {
            printf("ERROR: Letter '%c' is duplicated %i times\n", key[i], counter);
            returnValue = 1;
            break;
        }

    }

    return returnValue;
}

// Funciton lower
void printCypherLowerCase(char character, string key)
{
    if (isalpha(character))
    {
        int indexKey = (int) character - 97;
        char cypheredCharacter = tolower(key[indexKey]);
        printf("%c", cypheredCharacter);
    }
    else
    {
        printf("%c", character);
    }
}

// Funciton upper
void printCypherUpperCase(char character, string key)
{
    if (isalpha(character))
    {
        int indexKey = (int) character - 65;
        char cypheredCharacter = toupper(key[indexKey]);
        printf("%c", cypheredCharacter);
    }
    else
    {
        printf("%c", character);
    }
}