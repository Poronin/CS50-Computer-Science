// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include "dictionary.h"
#include <string.h>

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

int counter;

int find_node(node *nd, const char *word);
void count_nodes(node *nd);
void free_nodes(node *nd);

// TODO: Choose number of buckets in hash table
const unsigned int N = 26;

// Hash table
node *table[N];

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    // TODO
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    int index = 0;
    int ascii_offset = 97;

    index = (int) tolower(word[0]);
    index = index - ascii_offset;

    if (!table[index])
    {
        node *n = malloc(sizeof(node));
        if(!n)
        {
            free(n);
            return 1;
        }
        strcpy(n->word, word);
        n->next = NULL;
        table[index] = n;
        printf("%p - %s\n", n, word );
    }
    else
    {
        find_node(table[index], word);
    }

    return 0;
}

int find_node(node *nd, const char *word)
{
    if (nd->next != NULL)
    {
        find_node(nd->next, word);
    }
    else
    {
        // create node
        node *new_node = malloc(sizeof(node));
        if(!new_node)
        {
            free(new_node);
            return 1;
        }
        strcpy(new_node->word, word);
        new_node->next = NULL;
        nd->next = new_node;
        printf("%p - %s \n", nd->next, word );
        return 0;
    }

    return 0;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // TODO
    int words = 0, index = 0;

    FILE *file = fopen(dictionary, "r");
    if (!file)
    {
        return false;
    }

    char c;
    char word[LENGTH + 1];

    while(fread(&c, sizeof(char), 1, file))
    {
        if (c != '\n')
        {
            // Append character to word
            word[index] = c;
            index++;
        }

        // We must have found a whole word
        else if (index > 0)
        {
            // Terminate current word
            word[index] = '\0';

            // Update counter
            words++;

            //
            hash(word);
            //printf("%i,\t%s, %p\n",words, word, table[2]);
            // Prepare for next word
            index = 0;
        }

    }

    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    counter = 0;
    for (int i = 0; i < N; i++)
    {
        count_nodes(table[i]);
    }

    printf("Counter: %i\n", counter);
    return counter;

}

void count_nodes(node *nd)
{
    if (nd)
    {
        counter++;
        count_nodes(nd->next);
    }

    return;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    for (int i = 0; i < N; i++)
    {
        free_nodes(table[i]);
    }

    return true;
}

void free_nodes(node *nd)
{

    if (nd == NULL)
    {
        return;
    }
    free(nd->next);
    free(nd);


    return;
}