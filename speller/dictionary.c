// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <strings.h>
#include <stdlib.h>

#include "dictionary.h"

// Represents number of buckets in a hash table
#define N 26
unsigned int dict_size = 0;
// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

void insert(node **head, char *word)
{
    node *n = malloc(sizeof(node));
    strcpy(n->word, word);
    n->next = *head;
    *head = n;
    dict_size++;
}

// Represents a hash table
node *hashtable[N];

// Hashes word to a number between 0 and 25, inclusive, based on its first letter
unsigned int hash(const char *word)
{
    return tolower(word[0]) - 'a';
}

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    // Initialize hash table
    for (int i = 0; i < N; i++)
    {
        hashtable[i] = NULL;
    }

    // Open dictionary
    FILE *file = fopen(dictionary, "r");
    if (file == NULL)
    {
        unload();
        return false;
    }

    // Buffer for a word
    char word[LENGTH + 1];

    // Insert words into hash table
    while (fscanf(file, "%s", word) != EOF)
    {
        insert(&hashtable[hash(word)], word);
    }

    // Close dictionary
    fclose(file);

    // Indicate success
    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    return dict_size;
}

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    node *n = hashtable[hash(word)];
    while (n != NULL)
    {
        if (strcasecmp(word, n->word))
        {
            return true;
        }
        n = n->next;
    }
    
    return false;
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    for (size_t i = 0; i < N; i++)
    {
       node *n = hashtable[i];
       while (n != NULL)
       {
           node *next = n->next;
           free(n);
           dict_size--;
           n = next;
       }
    }
    
    return dict_size == 0;
}
