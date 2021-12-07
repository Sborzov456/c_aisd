#include <stdlib.h>
#include <string.h>
#include <stdio.h>
//----------------AVL TREE ^_^------------------------
//----------------MENU------------------------
int getint(int *a)
{
    int n;
    do{
        n = scanf("%d", a);
        if (n < 0)
            return 0;
        if (n == 0){
            printf("%s\n", "Error! Repeat input");
            scanf("%*c");
        }
    } while (n == 0);
    return 1;
}
int dialog(char *msgs[])
{
    char *errmsg = "";
    int rc;
    int i, n;
    do{
        puts(errmsg);
        errmsg = "You are wrong. Repeate, please!";

        for(i = 0; i < 7; ++i)
            puts(msgs[i]);
        puts("Make your choice: --> ");
        n = getint(&rc);
        if(n == 0)
            rc = 0;
    } while(rc < 0 || rc >= 8);
    return rc;
}
//----------------GET_STRING FUNCTION------------------------
char* get_string(){
    int n, len, len_res = 0;
    char buf[51];
    char *res = NULL;
    do{
        n = scanf("%50[^\n]", buf);
        if (n < 0){
            if (!res) return NULL;
        }
        else if (n > 0) {
            len = (int)strlen(buf);
            len_res += len;
            res = realloc(res, (len_res + 1) * sizeof(char));
            memcpy(res + len_res - len, buf, len);
        }
        else{
            scanf("%*c");
        }
    } while (n > 0);
    if (len_res > 0){
        res[len_res] = '\0';
    }
    else {
        res = calloc(1, sizeof(char));
    }
    return res;
}
//----------------STRUCTS------------------------
typedef struct info
{
    char* info;
    struct info* next;
}info;
typedef struct tree
{
    struct tree* parent;
    struct tree* left;
    struct tree* right;
    info* info;
    int key;
    int balance;
}tree;
//----------------CREATE TREE------------------------
tree *create(tree* root)
{
    tree *tmp =(tree*)malloc(sizeof(tree));
    tmp -> parent = NULL;
    tmp -> key = 50;
    tmp -> balance = 0;
    tmp -> left = tmp -> right = NULL;
    root = tmp;
    return root;
}
//----------------SEARCH FUNCTION------------------------
tree* search_by_key(tree* root, int key)
{
    if ((root == NULL) || (root -> key == key))
        return root;
    if (key < root -> key)
        return search_by_key(root -> left, key);
    else return search_by_key(root -> right, key);
}
//----------------BALANCE FUNCTIONS------------------------
tree* serch_imbalanced_item(tree* leaf)
{
    int flag = 0;
    while ((flag == 0) && (leaf -> parent != NULL))
    {
        leaf = leaf -> parent;
        if ((leaf -> balance == 2) || (leaf -> balance == -2)) flag = 1;
    }
    if (flag == 1)
    {
        return leaf;
    }
    else
    {
        return NULL;
    }
}
int max(int a, int b)
{
    if (a >= b) return a;
    else return b;
}
int height_of_tree(tree* root)
{
    if(root == NULL) return 0;
    else return 1 + max(height_of_tree(root -> left), height_of_tree(root -> right));
}
int balance_of_item(tree* item)
{
    int balance = height_of_tree(item -> right) - height_of_tree(item -> left);
    return balance;
}
tree* rotate_right(tree* p, tree** root)
{
    tree* q = p -> left;
    p -> left = q -> right;
    q -> right = p;
    if (p -> parent){
        if (p -> parent->left == p){
            p -> parent->left = q;
        }
        else {
            p -> parent->right = p;
        }
    }
    q -> parent = p -> parent;
    p -> parent = q;
    q -> balance = balance_of_item(q);
    p -> balance = balance_of_item(p);
    if (!q -> parent){
        *root = q;
    }
    return q;
}
tree* rotate_left(tree* q, tree** root)
{
    tree* p = q -> right;
    q -> right = p -> left;
    p -> left = q;
    if (q -> parent){
        if (q -> parent -> right == q){
            q -> parent -> right = p;
        }
        else {
            q -> parent -> left = p;
        }
    }
    p -> parent = q -> parent;
    q -> parent = p;
    q -> balance = balance_of_item(q);
    p -> balance = balance_of_item(p);
    if (!p -> parent){
        *root = p;
    }
    return p;
}
tree* balance(tree* p, tree** root)
{
    if(balance_of_item(p) == 2)
    {
        if(balance_of_item(p -> right) < 0)
        {
            p -> right = rotate_right(p -> right, root);
        }
        return rotate_left(p, root);
    }
    else if(balance_of_item(p) == -2)
    {
        if(balance_of_item(p -> left) > 0)
        {
            p -> left = rotate_left(p -> left, root);
        }
        return rotate_right(p, root);
    }
    else return p;
}
void recount_balance(tree* root)
{
    if (root == NULL) return;
    else
    {
        root -> balance = balance_of_item(root);
        recount_balance(root->left);
        recount_balance(root->right);
    }
}
//----------------ADD FUNCTION------------------------
info* add_to_info_list(info* inf, char* information)
{
    info* tmp = (info*)malloc(sizeof(info));
    tmp -> info = (char*)malloc(strlen(information)*sizeof(char));
    tmp -> info = strcpy(tmp -> info, information);
    tmp -> next = NULL;
    info* head = inf;
    while (head -> next != NULL) head = head -> next;
    head -> next = tmp;
    return inf;
}
tree *add(tree *root, int key, char* information)
{
    tree *root2 = root, *root3 = NULL;
    tree* unb = NULL;
    tree *tmp = (tree*)malloc(sizeof(tree));
    tmp -> key = key;
    tmp -> balance = 0;
    while (root2 != NULL)
    {
        root3 = root2;
        if (key < root2 -> key)
        {
            root2 = root2 -> left;
        }
        else if (key > root2 -> key)
        {
            root2 = root2 -> right;
        }
        else break;
    }
    if (root2 == NULL)
    {
        tmp -> info = (info*)malloc(sizeof(info));
        tmp -> info -> info = strcpy(tmp -> info -> info, information);
        tmp -> info -> next = NULL;
        tmp -> parent = root3;
        tmp -> left = NULL;
        tmp -> right = NULL;
        if ((root3) && (key <= root3 -> key)) root3 -> left = tmp;
        else if (root3) root3 -> right = tmp;
        recount_balance(root);
        unb = serch_imbalanced_item(tmp);
        if (unb != NULL)
        {
            printf("Tree is imbalanced in %d key with %d balance factor\n",unb -> key, unb -> balance);
            balance(unb, &root);
            recount_balance(root);
        }
        else printf("Tree is balanced\n");
    }
    else
    {
        root2 -> info = add_to_info_list(root2 -> info, information);
    }
    return root;
}
//----------------PRINT TREE FUNCTION(PREORDER)------------------------
void preorder(tree *root)
{
    if (root == NULL)
        return;
    if (root -> key)
    {
        printf("Key: %d\n", root -> key);
        printf("Balance: %d\n", root -> balance);
        printf("Information: ");
        info* tmp = root -> info;
        while (tmp != NULL)
        {
            printf("%s, ", tmp -> info);
            tmp = tmp -> next;
        }
    }
    preorder(root -> left);
    preorder(root -> right);
}
void tree_print(tree* root, int lvl){

    if (root){
    tree_print(root -> right, lvl + 1);
    for(int i = 0; i < lvl; i++){
        printf(" ");
    }
    printf("%d(%d)\n", root -> key, root -> balance);
    tree_print(root -> left, lvl + 1);
    }
}
//----------------DELETE FUNCTION------------------------
tree* find_min(tree* root)
{
    tree* tmp = root -> right;
    while (tmp -> left != NULL) tmp = tmp -> left;
    return tmp;
}
tree* delete(tree* root, int key)
{
    tree* item = search_by_key(root, key);
    tree* item_parent = item -> parent;
    tree* unb;
    if ((item -> right == NULL) && (item -> left == NULL))
    {
        if (item_parent -> right == item) item_parent -> right = NULL;
        else item_parent -> left = NULL;
        free(item);
        recount_balance(root);
        unb = serch_imbalanced_item(item_parent);
        if (unb != NULL)
        {
            printf("Tree is imbalanced in %d key with %d balance factor\n",unb -> key, unb -> balance);
            balance(unb, &root);
        }
    }
    else if ((item -> left == NULL) && (item -> right != NULL))
    {
         if (item_parent -> right == item) item_parent -> right = item -> right;
         else item_parent -> left = item -> right;
         free(item);
         recount_balance(root);
         unb = serch_imbalanced_item(item_parent -> left);
         if (unb != NULL)
         {
            printf("Tree is imbalanced in %d key with %d balance factor\n",unb -> key, unb -> balance);
            balance(unb, &root);
         }
    }
    else if ((item -> right == NULL) && (item -> left != NULL))
    {
         if (item_parent -> left == item) item_parent -> left = item -> left;
         else item_parent -> right = item -> left;
         recount_balance(root);
         unb = serch_imbalanced_item(item_parent -> left);
         if (unb != NULL)
         {
             printf("Tree is imbalanced in %d key with %d balance factor\n",unb -> key, unb -> balance);
             balance(unb, &root);
         }
    }
    else if ((item -> right != NULL) && (item -> left != NULL))
    {
        tree* min = find_min(item);
        tree* min_parent = min -> parent;
        tree* item_right = item -> right;
        tree* item_left = item -> left;
        if(item_parent -> left == item) item_parent -> left = min;
        else item_parent -> right = min;
        item = min;
        min -> right = item_right;
        min -> left = item_left;
        min -> parent = item_parent;
        min_parent -> left = NULL;
        recount_balance(root);
        unb = serch_imbalanced_item(min_parent);
        if (unb != NULL)
        {
            printf("Tree is imbalanced in %d key with %d balance factor\n",unb -> key, unb -> balance);
            balance(unb, &root);
        }
    }
    return root;
}
//----------------SEARCH THE FURTHEST ITEM FUNCTION------------------------
int min_in_tree(tree* root)
{
    tree* tmp = root;
    while (tmp -> left != NULL) tmp = tmp -> left;
    return (tmp -> key);
}
int max_in_tree(tree* root)
{
    tree* tmp = root;
    while (tmp -> right != NULL) tmp = tmp -> right;
    return (tmp -> key);
}
int search_the_furthest_item(tree* root, int key)
{
    if ((key - min_in_tree(root)) >= (max_in_tree(root) - key)) return (min_in_tree(root));
    else return (max_in_tree(root));
}
//----------------MAIN------------------------
int main()
{
    int choice=-1;
    int key, a;
    tree* root = NULL, *unb = NULL;
    char *msgs[] = {"0. Quit", "1. Add", "2. Find", "3. Delete", "4. Show", "5. Find an farthest item", "6. Test tree"};
    root = create(root);
    while (choice!=0)
    {
        choice = dialog(msgs);
        if (choice == 1)
        {
            printf("Enter key: ");
            scanf("%d", &key);
            printf("Enter information: ");
            scanf("%*c");
            char* information = get_string();
            root = add(root, key, information);
        }
       if (choice == 2)
        {
            tree* tmp;
            int key;
            printf("Enter key: ");
            scanf("%d", &key);
            tmp = search_by_key(root, key);
            if (tmp == NULL) printf("Key not found");
            else
            {
                info* info_tmp = tmp -> info;
                int c;
                printf("1. All versions of this key\n");
                printf("2. Chosen version of this key\n");
                scanf("%d", &c);
                if (c == 1)
                {
                    while (info_tmp != NULL)
                    {
                        printf("%s ", info_tmp -> info);
                        info_tmp = info_tmp -> next;
                    }
                }
                else if (c == 2)
                {
                    int i = 1;
                    int k;
                    printf("Enter version of item: ");
                    scanf("%d", &k);
                    info* info_tmp = tmp -> info;
                    while (info_tmp != NULL)
                    {
                        if (i == k)
                        {
                            printf("%s ", info_tmp -> info);
                            break;
                        }
                        i++;
                        info_tmp = info_tmp -> next;
                    }
                    if (info_tmp == NULL) printf("Version not found\n");
                }
            }
        }
        if (choice == 3)
        {
            int key;
            printf("Enter key you want to remove: ");
            scanf("%d", &key);
            root = delete(root, key);
        }
        if (choice == 4)
        {
            printf("1. Preorder print\n");
            printf("2. Beautiful print\n");
            int i;
            scanf("%d", &i);
            if (i == 1) preorder(root);
            else if (i == 2) tree_print(root, 0);
        }
        if (choice == 5)
        {
            int key, a;
            printf("Enter key: ");
            scanf("%d", &key);
            a = search_the_furthest_item(root, key);
            printf("The furthest key: %d", a);
        }
        if (choice == 6)
        {
            add(root, 55, "abc");
            add(root, 40, "abc");
            add(root, 60, "abc");
            add(root, 30, "abc");
            add(root, 45, "abc");
            add(root, 43, "abc");
        }
    }
    return(0);
}
