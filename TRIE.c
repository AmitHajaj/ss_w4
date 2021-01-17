#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define NUM_OF_LETTERS 26 //extra one for the '/0' sign to know if some node is finishes a word.
#define BIGGEST_LETTER_SIZE 45 // the biggest word in english.(i think)

typedef enum
{
    FALSE = 0,  TRUE = 1
} boolean;

//represent a node in our TRIE.

typedef struct node
{
    char letter;
    struct node* parent;
    long unsigned int counter;
    struct node* children[NUM_OF_LETTERS];
    boolean is_word;
    char *word; // if is_word == true so we save the word till this node.
} node;

// we will build a tree.
//method will return pointer to the root we create.
node* build_node()
{
    node *root = (node*)calloc(1, sizeof(node));
    for(int i=0; i<NUM_OF_LETTERS; i++)
        {
            root->children[i] = (node*)calloc(1, sizeof(node));
            root->children[i]->letter = '&';
            root->is_word = false;
            root->parent = NULL;
            root->counter = 0;
        }
    return root;
}

// adds a node to some TRIE.
// check if there is enough memory, if not- allocate.
void add_word(char *word, struct node *root)
{
    node *temp = root;
    for(int i=0; i<strlen(word); i++)
    {
        //if the current letter don't have a pointer to the next letter in the tree.
        if(temp->children[word[i]-97]->letter == '&')
        {
            temp->children[word[i]-97]= build_node(); //build a node.
            temp->children[word[i]-97]->letter = word[i]; //give it the value of the current letter.
            temp->children[word[i]-97]->parent = temp; // set to be parent the one who came before the current.
        }
        // we also need to check if we finished with the word.
        // if we did, change the flag, and increase the counter.
        if(i+1 == strlen(word))
        {
            if(temp->children[word[i]-97]->is_word)
            {
                temp->children[word[i]-97]->counter ++;
            }
            else
            {
                temp->children[word[i]-97]->counter ++;
                temp->children[word[i]-97]->is_word = true;
                char *str = malloc(strlen(word)+1);
                strcpy(str, word);
                temp->children[word[i]-97]->word = str;
            }
        }
        // if the current node is exists nothing need's to be done, so we just need to continue.
        temp = temp->children[word[i]-97];
    }
}
// this method will print the word's that were given, lexicographic ordered.
void print_words_preorder(struct node *root)
{
    node *temp = root;
    if(temp != NULL)
	{
		if(temp->is_word)
        {
           printf("%s %ld\n",temp->word, temp->counter);
        }
		int i;
		for(i = 0; i < NUM_OF_LETTERS; i++)
        {
            if(temp->children[i]->letter != '&')
            {
                print_words_preorder(temp->children[i]);
            }
        }
	}
}

void print_words_reverse_inorder(struct node *root)
{
    node *temp = root;
    if(temp != NULL)
	{
        int i;
		for(i = (NUM_OF_LETTERS-1); i >= 0; i--)
        {
            if(temp->children[i]->letter != '&')
            {
                print_words_reverse_inorder(temp->children[i]);
            }
        }
        if(temp->is_word)
        {
           printf("%s %ld\n",temp->word, temp->counter);
        }
	}
}

int main(int argc, char *argv[])
{
    //this will be our root.
    node *root = build_node();

    //now we take the user choice.
    //we will use it before finish.

    char d;
    int counter = 0;//letter size will be initialize to 1.

    // our word will be start with size 2, and grow with the user's input.(if valid)
    char *word = (char*)calloc(1, sizeof(char));

    while((d = getchar()) != EOF)
    {
        //if d is a lower-case/
        if(d >= 97 && d <= 122)
        {
            counter = counter+1;
            word = (char*)realloc(word, (counter)*sizeof(char));
            word[counter-1] = d;
        }

        //if it is an upper-case.
        else if (d > 64 && d < 91)
        {
            counter = counter+1;
            d = tolower(d);
            word = (char*)realloc(word, (counter)*sizeof(char));
            word[counter-1] = d;
        }

        //if it is symbol, send the word to the tree, free the memory of the word and initialize variables.
        else
        {
            if(counter != 0)
            {
                add_word(word, root);
            }
            free(word);
            word = NULL;
            counter = 0;
        }
    }

    if(argv[0] == 'r')
    {
        print_words_reverse_inorder(root);
    }
    print_words_preorder(root);
}

