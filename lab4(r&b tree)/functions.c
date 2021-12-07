#include "functions.h"
node* create (node* root)
{
    node* tmp = (node*)malloc(sizeof(node));
    tmp->color = BLACK;
    tmp->key = 1;
    tmp->left = elist;
    tmp->right = elist;
    tmp->parent = elist;
    return tmp;
}
//-------------BALANCE-------------
node* rotate_right(node* p, node** root)
{
    node* q = p->left;
    p->left = q->right;
    q->right = p;
    if (p->parent)
    {
        if (p -> parent -> left == p) p -> parent->left = q;
        else p -> parent -> right = q;
    }
    q->parent = p->parent;
    p->parent = q;
    if (!q -> parent) *root = q;
    return q;
}
node* rotate_left(node* q, node** root)
{
    node* p = q->right;
    q->right = p->left;
    p->left = q;
    if (q->parent)
    {
        if (q -> parent -> right == q) q -> parent -> right = p;
        else q -> parent -> left = p;
    }
    p->parent = q->parent;
    q->parent = p;
    if (!p -> parent) *root = p;
    return p;
}
node* find_uncle(node* lst)
{
    node* dad = lst->parent;
    node* g_dad = dad->parent;
    if(g_dad->left == dad) return g_dad->right;
    else return g_dad->left;
}
int balance(node* lst, node** root)
{
    node* dad = lst->parent;
    node* g_dad = dad->parent;
    node* uncle = find_uncle(lst);
    color c;
    if (uncle == elist) c = BLACK;
    else c = uncle->color;
    if ((c == RED) && (dad->color == RED))
    {
        printf("Case 1 fix\n");
        g_dad->color = RED;
        uncle->color = BLACK;
        dad->color = BLACK;
        if (g_dad != *root) balance(g_dad, root);
        else g_dad->color = BLACK;
    }
    else if ((dad->right == lst) && (g_dad->right == dad) && (c == BLACK))
    {
        printf("Case 3 fix\n");
        dad->color = BLACK;
        g_dad->color = RED;
        rotate_left(g_dad, root);
    }
    else if ((dad->left == lst) && (g_dad->left == dad) && (c == BLACK))
    {
        printf("Case 3 fix\n");
        dad->color = BLACK;
        g_dad->color = RED;
        rotate_right(g_dad, root);
    }
    else if ((dad->right == lst) && (g_dad->left == dad) && (c == BLACK))
    {
        printf("Case 2 fix\n");
        rotate_left(dad, root);
        lst->color = BLACK;
        g_dad->color = RED;
        rotate_right(g_dad, root);
    }
    else if ((dad->left == lst) && (g_dad->right == dad) && (c == BLACK))
    {
        printf("Case 2 fix\n");
        rotate_right(dad, root);
        lst->color = BLACK;
        g_dad->color = RED;
        rotate_left(g_dad, root);
    }
    return 0;
}
//---------------------------------
node* insert (node* root, int key)
{
    node* tmp = (node*)malloc(sizeof(node));
    tmp->key = key;
    tmp->color = RED;
    tmp->right = elist;
    tmp->left = elist;
    node* cur = root;
    node* prev = elist;
    while (cur != elist)
    {
        prev = cur;
        if (cur->key > key) cur = cur->left;
        else cur = cur->right;
    }
    tmp->parent = prev;
    if (prev->key > key) prev->left = tmp;
    else prev->right = tmp;
    if(prev != root)
    {
        balance(tmp, &root);
    }
    return root;
}
void print(node* root, int lvl)
{
    if (root)
    {
        print(root -> right, lvl + 1);
        for(int i = 0; i < lvl; i++) printf(" ");
        char c;
        if(root->color == BLACK) c = 'B';
        else if (root->color == RED) c = 'R';
        printf("%d(%c)\n", root -> key, c);
        print(root -> left, lvl + 1);
    }
}
