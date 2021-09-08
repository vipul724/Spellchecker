// Implements a dictionary's functionality

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>

#include "dictionary.h"


int word_count = 0;

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Number of buckets in hash table
const unsigned int N = (LENGTH + 1) * 'z';

// Hash table
node *table[N];

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    unsigned int hash_code = hash(word);
    node *cursor = table[hash_code];

    while (cursor != NULL)
    {
        if (strcasecmp(word, cursor->word) == 0)
        {
            return true;
        }
        
        cursor = cursor->next;
    }
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word) 
{
    int sum = 0;
    for (int i = 0; i < strlen(word); i++)
    {
        sum += tolower(word[i]);
    }
    
    return sum % N;
}

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    FILE *file = fopen(dictionary, "r"); //opens dictionary file
    
    if (file == NULL)
    {
        return false;
    }

    char dict_word[LENGTH + 1]; //creates string for the word

    while (fscanf(file, "%s", dict_word) != EOF) //loop that scans for each word until end of file
    {
        node *new_node = malloc(sizeof(node));
        
        if (new_node == NULL)
        {
            return false;
        }

        strcpy(new_node->word, dict_word); //copies word into node
        new_node->next = NULL;
        
        unsigned int hash_code = hash(dict_word);
        
        if (table[hash_code] == NULL) // inserts node into hash table if there is nothing yet in the hash table
        {
            table[hash_code] = new_node;
        }
        else
        {
            new_node->next = table[hash_code]; // changes pointers to allow for chaining to the linked list
            table[hash_code] = new_node;
        }
        
        word_count++;
    }
    
    fclose(file);
    return true;
    
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    return word_count; 
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    for (int i = 0; i < N; i++)
    {
        node *cursor = table[i];
        
        while (cursor != NULL)
        {
            node *tmp = cursor;
            cursor = cursor->next;
            free(tmp);
        }
    }
    return true;
}
