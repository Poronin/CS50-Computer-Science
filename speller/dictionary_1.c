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
    struct node *greater;
    struct node *lower;
}
node;

int find_node(node *nd, const char *word);

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
        *n->word = (char) index;
        n->greater = NULL;
        n->lower = NULL;
        table[index] = n;
    }

    find_node(table[index], word);

    return 0;
}

int find_node(node *nd, const char *word)
{
    // compare words
    int word_size = strcmp(word, nd->word);

    // the new word is greater than the one exising in the node
    if (word_size > 0)
    {
        if (nd->greater == NULL)
        {
            // create node
            node *new_node = malloc(sizeof(node));
            if(!new_node)
            {
                free(new_node);
                return 1;
            }
            strcpy(new_node->word, word);
            new_node->greater = NULL;
            new_node->lower = NULL;

            nd->greater = new_node;
        }
        else
        {
            find_node(nd->greater, word);
        }
    }
    else if (word_size < 0)
    {
        if (nd->lower == NULL)
        {
            // create node
            node *new_node = malloc(sizeof(node));
            if(!new_node)
            {
                free(new_node);
                return 1;
            }
            strcpy(new_node->word, word);
            new_node->greater = NULL;
            new_node->lower = NULL;

            //swap nodes
            new_node->lower = nd;
            nd = new_node;
        }
        else
        {
            find_node(nd->greater, word);
        }
    }

    printf("%p - %s\n", nd, word );
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
        return 1;
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

    return false;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    // TODO
    return 0;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    // TODO
    return false;
}
