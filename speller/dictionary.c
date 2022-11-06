// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include "dictionary.h"
#include <strings.h>
#include <string.h>

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

int counter;

void count_nodes(node *nd);
void free_nodes(node *nd);
bool find_word(node *cursor, const char *word);

// TODO: Choose number of buckets in hash table
const unsigned int N = 26;

// Hash table
node *table[N];

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    int index_hash = hash(word);

    return find_word(table[index_hash], word);
}

bool find_word(node *cursor, const char *word)
{
    int i = 0;

    // empty index in hash table
    if (!cursor)
    {
        return false;
    }

    // check last node
    if (!cursor->next)
    {
        i = strcasecmp(cursor->word, word);
        if (i != 0)
        {
            return false;
        }
        else
        {
            return true;
        }
    }

    //check node lists
    i = strcasecmp(cursor->word, word);
    if (i != 0)
    {
        // if found return true.
        if (find_word(cursor->next, word))
        {
            return true;
        }
    }
    else
    {
        return true;
    }
    return false;
}


// Hashes word to a number
unsigned int hash(const char *word)
{
    int index = 0;
    int ascii_offset = 97;

    index = (int) tolower(word[0]);
    index = index - ascii_offset;

    return index;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // TODO
    int words = 0, index = 0;

    FILE *file = fopen(dictionary, "r");
    if (!file)
    {
        fclose(file);
        return false;
    }

    char c;
    char word[LENGTH + 1];

    while (fread(&c, sizeof(char), 1, file))
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

            // create a new node
            int index_hash = hash(word);
            node *n = malloc(sizeof(node));
            if (n == NULL)
            {
                free(n);
                return false;
            }
            strcpy(n->word, word);
            n->next = table[index_hash] ;
            table[index_hash] = n;

            index = 0;
        }
    }

    fclose(file);
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
    return counter;
}

void count_nodes(node *nd)
{
    if (nd)
    {
        counter++;
        count_nodes(nd->next);
    }
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

void free_nodes(node *root)
{
    if (root == NULL)
    {
        return;
    }
    free_nodes(root->next);
    free(root);
}
