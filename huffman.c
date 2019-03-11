//huffman.c
//oliver mulcahy and eric huang
# include <stdint.h>
# include <stdbool.h>
# include <stdlib.h>
# include <ctype.h>
# include <fcntl.h>
# include <unistd.h>
# include <stdio.h>
# include "huffman.h"
# include "code.h"
# include "stack.h"

# ifndef NIL
# define NIL (void *) 0
# endif

treeNode *newNode(uint8_t s, bool l, uint64_t c)
{
	treeNode *n = calloc(1, sizeof(treeNode));
	n -> leaf = l;
	n -> count = c;
	n -> symbol = s;
	n -> left = NIL;
	n -> right = NIL;
	return n;
}

void dumpTree (treeNode *t, int file)
{
    if (t -> leaf)
    {
        uint8_t s = t -> symbol;
        uint8_t l = 'L';
        write(file, &l, sizeof(l)); //leaf
        write(file, &s, sizeof(s)); //symbol
    }
    if (t -> symbol == '$') //interior node
    {
        uint8_t I = 'I';
        write(file, &I, sizeof(I));
        dumpTree(t -> left, file);
        dumpTree(t -> right, file);
    }
}

// Build a tree from the saved tree
treeNode *loadTree (uint8_t savedTree [], uint16_t treeBytes)
{
    stack *stackTree = newStack();
    for(uint16_t i = 0; i < treeBytes; i++)
    {
        if(savedTree[i] == 'L')
        {
            i += 1;
            uint8_t symbol = savedTree[i]; //symbol comes after leaf
            treeNode *n = newNode(symbol, true, 0);
            push(stackTree, n);
        }
        else if(savedTree[i] == 'I') //is interior node
        {
            treeNode *right = pop(stackTree); //pop twice to get left and right
            treeNode *left =  pop(stackTree);
            treeNode *combined = join(left, right);
            push(stackTree, combined);
        }
    }
    treeNode *root = pop(stackTree);
    delStack(stackTree);
    return root;
}

//Step through a tree following the code
int32_t stepTree (treeNode *root, treeNode **t, uint32_t code)
{
    if(!code) //if 0 go left
    {
        *t = (*t)->left;
    }
    if(code) //if 1 go right
    {
        *t = (*t)->right;
    }
    if((*t)->leaf) //if its a leaf
    {
        int32_t sym = (*t)->symbol;
	*t = root; //go back to root
	return sym;  //return symbol
    }
    return -1; //no leaf reached
}

void buildCode (treeNode *t, code s, code table[256])
{
    if (t -> leaf)
    {
        table[t -> symbol] = s;
    }
    if (t -> symbol == '$')
    {
        pushCode(&s, 0);
        buildCode(t -> left, s, table);
        popCode(&s, NULL);
        pushCode(&s, 1);
        buildCode(t -> right, s, table);
    }
    return;
}

void delTree (treeNode *t)
{
    if(t!= NIL)
    {
        delTree(t->left);
        delTree(t->right);
        delNode(t);
    }
    return;
} 

treeNode *join (treeNode *l, treeNode *r)
{
    bool leaf = false;
    uint8_t sym = '$';
    uint64_t combCount = l->count + r->count;
    treeNode *combined = newNode(sym, leaf, combCount);
    combined->left = l;
    combined->right = r;
    return combined;
}

//Credits to Darrell Long
void printTree(treeNode *t, int depth)
{
        if (t && t->leaf)
        {
                if (isalnum(t->symbol))
                {
                        spaces(4 * depth); printf("%c (%lu)\n", t->symbol, t->count);
                }
                else
                {
                        spaces(4 * depth); printf("%X (%lu)\n", t->symbol, t->count);
                }
        }
        else if (t)
        {
                spaces(4 * depth); printf("$ (%lu)\n", t->count);
                printTree(t->left, depth + 1);
                printTree(t->right, depth + 1);
        }
        return;
} 

