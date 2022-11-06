#include <cs50.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <math.h>

int count_letters(string);
int count_words(string);
int count_sentences(string);

int main(void)
{

    string text = get_string("Text: ");
    //printf("%s\n", text);

    int letters = count_letters(text);
    //printf("%i letters\n", letters);

    int words = count_words(text);
    //printf("%i words\n", words);

    int sentences = count_sentences(text);
    //printf("%i sentences\n", sentences);

    float index = (0.0588 * (float) letters / (float) words * 100.0) - (0.296 * (float) sentences / (float) words * 100.0) - 15.8;

    int grade = round(index);

    if (grade > 16)
    {
        printf("Grade 16+\n");
    }
    else if (grade < 1)
    {
        printf("Before Grade 1\n");
    }
    else
    {
        printf("Grade %i\n", grade);
    }
}


//counting letters
int count_letters(string text)
{
    int counter = 0;
    for (int i = 0, n = strlen(text); i < n; i++)
    {
        if  isalpha(text[i])
        {
            counter++;
        }
    }
    return counter;
}

//counting words
int count_words(string text)
{
    int counter = 0;
    for (int i = 0, n = strlen(text); i < n; i++)
    {
        if isspace(text[i])
        {
            counter++;
        }
    }
    return counter + 1;
}

//counting sentence
int count_sentences(string text)
{
    int counter = 0;
    for (int i = 0, n = strlen(text); i < n; i++)
    {
        if (text[i] == '.' || text[i] == '?' || text[i] == '!')
        {
            counter++;
        }
    }
    return counter;
}
