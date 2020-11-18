// Implements a dictionary's functionality

#include <stdlib.h>
#include <stdio.h>
#include <strings.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <ctype.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Number of buckets in hash table
const unsigned int N = 17576;

// Hash table
node *table[N];

// Number of loaded words
unsigned int dicSize = 0;

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    unsigned int index = hash(word);

    if (table[index] != NULL)
    {
        node *currentNode = table[index];
        while (currentNode != NULL)
        {
            if (strcasecmp(word, currentNode -> word) == 0)
            {
                return true;
            }

            currentNode = currentNode -> next;
        }
    }
    return false;
}

// Hashes word to a number, takes first 3 letters into account;
unsigned int hash(const char *word)
{
    char hashed[3];

    unsigned int hash = 1;

    for (int i = 0; i < 3; i++)
    {
        if (strlen(word) < 2)
        {
            hash = (int) tolower(word[0]) -
                   96;                                  //this turns the charchter to lower case then substracts 96 to get a value between 1 - 26;
            return hash - 1;
        }

        if (isalpha(word[i]))
        {
            hashed[i] = tolower(word[i]);
            hash = hash * ((int)hashed[i] - 96);        //hash index maximum value would be zzz which is 17576
        }

    }
    //printf("%i %s %i \n", hash,hashed,dicSize);
    return (hash - 1);             //-1 to have a value at the 0 index;
}

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    // open the dictionary;
    FILE *dic = fopen(dictionary, "r");
    if (dic == NULL)
    {
        return false;
    }


    char dicword[LENGTH + 1];


    while (fscanf(dic, "%s", dicword) != EOF)
    {
        dicSize++;
        node *n = malloc(sizeof(node));
        strcpy(n -> word, dicword);
        n -> next = NULL;
        int index = hash(n -> word);
        if (table[index] == NULL)
        {
            table[index] = n;
        }
        else
        {
            n -> next = table[index];
            table[index] = n;
        }

    }
    fclose(dic);
    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    return dicSize;
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    for (int i = 0; i < N; i++)
    {
        if (table[i] != NULL)
        {
            node *currentNode = table[i];
            node *temp = currentNode -> next;
            while (currentNode != NULL)
            {
                if (temp == NULL)
                {
                    free(currentNode);
                    currentNode = NULL;
                }
                else
                {
                    free(currentNode);
                    currentNode = temp;
                    temp = temp -> next;
                }

            }
        }
    }

    return true;
}
