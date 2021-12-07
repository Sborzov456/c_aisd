#include "functions.h"

int main()
{
    node* root;
    root = create(root);
    int choice = 1;
    while (choice > 0)
    {
        printf("0. Quit\n");
        printf("1. Add\n");
        printf("2. Print\n");
        scanf("%d", &choice);
        if (choice == 1)
        {
            int key;
            printf("Enter key: ");
            scanf("%d", &key);
            root = insert(root, key);
        }
        if (choice == 2)
        {
            print(root, 0);
        }

    }
    printf("Bye bye, senpai ^_^");
    return 0;
}
