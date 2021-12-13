#include <stdio.h>
#include <stdlib.h>

int dialog(char *msgs[])
{
char *errmsg = "";
int rc;
int i, n;
do{
    puts(errmsg);
    errmsg = "You are wrong. Repeate, please!";

    for(i = 0; i < 5; ++i)
        puts(msgs[i]);
    puts("Make your choice: --> ");
    n = getInt(&rc);
    if(n == 0)
        rc = 0;
} while(rc < 0 || rc >= 6);
return rc;
}

int getInt(int *a)
{
int n;
do{
n = scanf("%d", a, sizeof(int));
if (n < 0)
return 0;
if (n == 0){
printf("%s\n", "Error! Repeat input");
scanf("%*c", 0);
}
} while (n == 0);
return 1;
}


typedef struct list
{
    struct list* next;
    int data;
}list;


typedef struct tree
{
    list* info;
    int key;
    struct tree *left;
    struct tree *right;
}tree;


list * init(int a) // создает корень списка
{
  struct list *lst;// выделение памяти под корень списка
  lst = (struct list*)malloc(sizeof(struct list));
  lst->data = a;
  lst->next = NULL; // это последний узел списка
  return(lst);
}

tree *create(tree* root)//создает корень дерева
{
    tree *tmp =(tree*)malloc(sizeof(tree));// выделение памяти под корень дерева
    tmp -> info = init(13);
    tmp -> key = 50; // присваивание значения ключу
    tmp -> left = tmp -> right = NULL;// присваивание указателю на левое и правое поддерево значения NULL
    root = tmp;
    return root;
}



list * addelem(list *lst, int number)//добавляет элемент в список
{
  struct list *temp, *p;
  temp = (struct list*)malloc(sizeof(list));
  p = lst->next; // сохранение указателя на следующий узел
  lst->next = temp; // предыдущий узел указывает на создаваемый
  temp->data = number; // сохранение поля данных добавляемого узла
  temp->next = p; // созданный узел указывает на следующий элемент
  return(temp);
}

tree *add(tree *root)//добавляет элемент в дерево
{
    int key, info, duplicate=0;
    tree *root2 = root, *root3 = NULL;
    tree *tmp = (tree*)malloc(sizeof(tree));
    printf("Enter key: ");
    scanf("%d",&key);
    tmp -> key = key;
    printf("Enter information: ");
    scanf("%d",&info);
    while (root2 != NULL)
    {
        root3 = root2;
        if (key == root2 -> key)
        {
            duplicate = 1;
            break;
        }
        else
            if (key > root2 -> key)
            root2 = root2 -> right;
            else
                root2 = root2 -> left;
    }
    if (duplicate == 1)
    {
        addelem(root2->info, info);
    }
    else
    {
        tmp -> info = init(info);
        tmp -> left = NULL;
        tmp -> right = NULL;
        if (key > root3 -> key) root3 -> right = tmp;
        else root3 -> left = tmp;
    }
    return root;
}




tree *search(tree * root, int key)
{
    if ((root == NULL) || (root -> key == key))// если дерево пусто или ключ корня равен искомому ключу, то возвращается указатель на корень
        return root;
    if (key < root -> key)
        return search(root -> left, key);
    else return search(root -> right, key);
}




    void preorder(tree *root)//вывод дерево в прямом порядке
{
    /*int key, flag=1;
    tree* first;
    if (flag == 1)
    {
        printf("Enter the key that will start the tree output: ");
        scanf("%d", &key);
        first = search(root, key);
        flag = 0;
    }
    root = first;*/
    if (root == NULL)
        return;
    if (root -> key)
    {
        printf("Key: %d\n", root -> key);
        printf("Information: ");
        printf("%d ", root -> info -> data);
        while (root -> info -> next != NULL)
        {
            root -> info = root -> info -> next;
            printf("%d ", root -> info -> data);
        }
        printf("\n");
    }
    preorder(root -> left);
    preorder(root -> right);
}


/*tree *search(tree * root, int key)
{
    if ((root == NULL) || (root -> key == key))// если дерево пусто или ключ корня равен искомому ключу, то возвращается указатель на корень
        return root;
    if (key < root -> key)
        return search(root -> left, key);
    else return search(root -> right, key);
}*/


tree *searchparent(tree * root, int key) //функция поиска родительского элемента
{
     tree *parent = root;
     while(root != NULL)
     {
         if(key < root -> key)
         {
             parent = root;
             root = root -> left;
         }
         else if(key > root -> key)
         {
             parent = root;
             root = root -> right;
         }
         else
         {
             return parent;
         }
     }
     return NULL;
}


/*поиск родительского элемента через функцию search: условие на то, xnj root->left/right -> key == key и тогда root - это родительский элемент*/


void printfoundeditem (tree* root)
{
    int key;
    tree *tmp =(tree*)malloc(sizeof(tree));
    printf("Enter the key you are looking for: ");
    scanf("%d", &key);
    tmp = search(root, key);
    printf("Information: ");
    printf("%d ", tmp -> info -> data);
    while (tmp -> info -> next != NULL)
    {
        tmp -> info = tmp -> info -> next;
        printf("%d ", tmp -> info -> data);
    }
}



tree *min(tree *root) //поиск минимального элемента поддерева
{
    tree *l = root;
    while (l -> left != NULL)
        l = l -> left;
    return l;
}


tree *succ(tree *root) //функция поиска следующего за удаляемым элемента
{
    tree *p = root, *l = NULL;
// Если есть правое поддерево, то ищем минимальный элемент в этом поддереве
    if (p -> right != NULL)
        return min(p -> right);
/* Правое дерево пусто, идем по родителям до тех пор,
пока не найдем родителя, для которого наше поддерево левое */
    l = searchparent(root, p->key);
    while ((l != NULL) && (p == l -> right))
    {
        p = l;
        l = searchparent(root, l->key);
    }
    return l;
}



tree *delete(tree *root, int key)
{
// Поиск удаляемого узла по ключу
    tree *p = root, *l = NULL, *m = NULL, *mparent=NULL;
    l = search(root, key);
// 1 случай
    if ((l -> left == NULL) && (l -> right == NULL))
    {
        m = searchparent(root, l->key);
        if (l == m -> right) m -> right = NULL;
        else m -> left = NULL;
        free(l);
    }
    //return root;
// 2 случай, 1 вариант - поддерево справа
    if ((l -> left == NULL) && (l -> right != NULL))
    {
        m = searchparent(root, l->key);
        if (l == m -> right) m -> right = l -> right;
        else m -> left = l -> right;
        free(l);
    }
// 2 случай, 2 вариант - поддерево слева
    if ((l -> left != NULL) && (l -> right == NULL))
    {
        m = searchparent(root, l->key);
        if (l == m -> right) m -> right = l -> left;
        else m -> left = l -> left;
        free(l);
    }
// 3 случай
    if ((l -> left != NULL) && (l -> right != NULL))
    {
        m = succ(l);
        l -> key = m -> key;
        if (m -> right = NULL)
        {
            mparent = searchparent(root, m->key);
            mparent -> left = NULL;
        }
        else
        {
            mparent = searchparent(root, m->key);
            mparent -> left = m -> right;
        }
        free(m);
    }
    return root;
}








int main()
{
   int choice=-1;
   int key;
   tree* root;
   char *msgs[] = {"0. Quit", "1. Add", "2. Find", "3. Delete", "4. Show"};
   root = create(root);
   while (choice!=0)
   {
       choice = dialog(msgs);
       if (choice == 1)
       {
           add(root);
       }
       if (choice == 2)
       {
            printfoundeditem(root);
       }
       if (choice == 3)
       {
            printf("Enter the key of the item to remove: ");
            scanf("%d", &key);
            delete(root, key);
       }
       if (choice == 4)
       {
           printf("Tree: \n");
           preorder(root);
           //choice = 0;
       }

   }
   return(0);
}
