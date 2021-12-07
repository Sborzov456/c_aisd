#include <stdio.h>
#include <stdlib.h>
typedef enum {BLACK, RED} color;
typedef struct node
{
    struct node* left;
    struct node* right;
    struct node* parent;
    int key;
    color color;
}node;
static node* elist = {NULL, NULL, NULL, 0, BLACK};
