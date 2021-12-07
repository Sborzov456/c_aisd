#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//------------------GET STRING FUNCTIONS-----------------
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
//------------------DIALOG FUNCTIONS-----------------
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
    n = getint(&rc);
    if(n == 0)
        rc = 0;
} while(rc < 0 || rc >= 5);
return rc;
}
int getint(int *a)
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
//------------------STRUCTS-----------------
typedef struct item
{
    int key1;
    long int offset;
    int len;
    int release;
}item;
typedef struct vector
{
    int vsize;//vector size
    int tsize;//table size
    item* first;
    FILE* fd;
}vector;
typedef struct hash_item
{
    int key2;
    struct hash_item* next;
}item_h;
typedef struct hash_table
{
    item_h** first;
    int hsize;
    FILE* fd;
}hash_table;
//------------------FILE FUNCTIONS-----------------
int load_from_file(vector* vector, char* filename)
{
    vector -> fd = fopen(filename, "r+b");
    if (vector -> fd == NULL) return 0;
    fread(&(vector -> vsize), sizeof(int), 1, vector -> fd);
    fread(&(vector -> tsize), sizeof(int), 1, vector -> fd);
    vector -> first = (item*)calloc(vector -> vsize, sizeof(item));
    fread(vector -> first, sizeof(item), vector -> tsize, vector -> fd);
    return 1;
}
int create(vector* vector, char* filename, int vsize)
{
    vector -> vsize = vsize;
    vector -> tsize = 0;
    if (fopen_s(&(vector -> fd), filename, "w+b") != 0)
    {
        vector -> fd = NULL;
        return 0;
    }
    vector -> first = (item*)calloc(vector -> vsize, sizeof(item));
    fwrite(&(vector -> vsize), sizeof(int), 1, vector -> fd);
    fwrite(&(vector -> tsize), sizeof(int), 1, vector -> fd);
    fwrite(vector -> first, sizeof(item), vector -> vsize, vector -> fd);
    return 1;
}
int d_load(vector* vector)
{
    int vsize;
    char* filename = NULL;
    printf("Enter filename: ");
    filename = get_string();
    if (filename == NULL) return 0;
    if(load_from_file(vector, filename) == 0)
    {
        printf("Enter size of vector: ");
        if (getint(&vsize) == 0) return 0;
        create(vector, filename, vsize);
    }
    free(filename);
    return 1;
}
d_save(vector* vector)
{
    fseek(vector -> fd, sizeof(int), SEEK_SET);
    fwrite(&(vector -> tsize), sizeof(int), 1, vector -> fd);
    fwrite(vector -> first, sizeof(item), vector -> tsize, vector -> fd);
    fclose(vector -> fd);
    vector -> fd = NULL;
    return 1;
}
int load_hash_table(hash_table* htab, char* filename)
{
    htab -> fd = fopen(filename, "w+b");
    if (htab -> fd == NULL)
    {
       printf("%d", errno);
       perror("Error\n");
       return 0;
    }
    if (fread(&(htab -> hsize), sizeof(int), 1, htab -> fd) == 0)
    {
       int hsize;
       printf("Enter size if vector: ");
       scanf("%d", &hsize);
       htab -> hsize = hsize;
    }
    htab -> first = (item**)calloc(htab -> hsize, sizeof(item*));
    for (int i = 0; i < htab -> hsize; i++)
    {
        htab -> first[i] = NULL;
    }
    for (int i = 0; i < htab -> hsize; i++)
    {
        htab -> first[i] = (hash_table*)malloc(sizeof(hash_table));
        fread(htab -> first[i], sizeof(item), 1, htab -> fd);
        item_h* temp = htab -> first[i];
        while (temp -> next != NULL)
        {
            temp -> next = (hash_table*)malloc(sizeof(hash_table));
            fread(temp -> next, sizeof(item), 1, htab -> fd);
            temp = temp -> next;
        }
    }
    return 1;
}
int d_load_hash(hash_table* htab)
{
    int flag = 1;
    char* filename = NULL;
    printf("Enter filename: ");
    filename = get_string();
    if (filename == NULL) return 0;
    flag = load_hash_table(htab, filename);
    if (flag == 0) printf("Oh...>_<\n");
    free(filename);
    return flag;
}
int save_hash_table(hash_table* htab)
{
    fwrite(&(htab -> hsize), sizeof(int), 1, htab -> fd);
    for (int i = 0; i < htab -> hsize; i++)
    {
        fwrite(htab -> first[i], sizeof(item), 1, htab -> fd);
        item_h* temp = htab -> first[i];
        while (temp -> next != NULL)
        {
            temp -> next = (hash_table*)malloc(sizeof(hash_table));
            fwrite(temp -> next, sizeof(item), 1, htab -> fd);
            temp = temp -> next;
        }
    }
    fclose(htab->fd);
    return 1;
}
//------------------ADD FUNCTION-----------------
int add_to_vector(vector* vector, int key, char* info)
{
    int n = vector -> tsize;
    vector -> first[n].release = 1;
    for (int i = 0; i < n; i++)
    {
        if (vector -> first[i].key1 == key) ++(vector -> first[n].release);
    }
    vector -> first[n].key1 = key;
    vector -> first[n].len = strlen(info) + 1;
    fseek(vector -> fd, 0, SEEK_END);
    vector -> first[n].offset = ftell(vector -> fd);
    fwrite(info, sizeof(char), vector -> first[n].len, vector -> fd);
    ++(vector -> tsize);
}
//------------------PRINT FUNCTION-----------------
void print(vector* vector)
{
    for (int i = 0; i < vector -> tsize; i++)
    {
        printf("Item %d:\n", i);
        printf("Key: %d\n", vector -> first[i].key1);
        printf("Release: %d\n", vector -> first[i].release);
        char* info = (char*)malloc(vector -> first[i].len);
        fseek(vector -> fd, vector -> first[i].offset, SEEK_SET);
        fread(info, sizeof(char), vector -> first[i].len, vector -> fd);
        printf("Len: %d\n", vector -> first[i].len);
        printf("Info: %s\n\n", info);
        free(info);
    }
}
//------------------FIND FUNCTION-----------------
int hash_function(int key, int hsize)
{
    key = key % hsize;
    return key;
}
hash_table* add_to_hash_table(hash_table* htab, int key)
{
    int h = hash_function(key, htab -> hsize);
    printf("debag h %d\n", h);
    if (htab -> first[h] == NULL)
    {
        printf("debag1\n");
        htab -> first[h] -> key2 = key;
        printf("debag2\n");
        htab -> first[h] -> next = NULL;
        printf("debag3\n");
    }
    else
    {
        printf("debag4\n");
        item_h* piligrim = htab -> first[h];
        printf("debag5\n");
        while (piligrim -> next != NULL) piligrim = piligrim -> next;
        printf("debag6\n");
        item_h* tmp = (item_h*)malloc(sizeof(item));
        printf("debag7\n");
        tmp -> key2 = key;
        printf("debag8\n");
        piligrim -> next = tmp;
        printf("debag9\n");
    }
    return htab;
}
void print_hash(hash_table* htab)
{
    for (int i; i < htab -> hsize; i++)
    {
        item_h* temp = htab -> first[i];
        printf("%d -> ", temp -> key2);
        while (temp != NULL)
        {
            temp = temp -> next;
            printf("%d ", temp -> key2);
        }
    }
}
//------------------MAIN-----------------
int main()
{
    vector* v = (vector*)calloc(1, sizeof(vector));
    v -> vsize = 0;
    v -> tsize = 0;
    v -> first = NULL;
    v -> fd = NULL;
    hash_table* htab = (hash_table*)calloc(1, sizeof(hash_table));
    htab -> fd = NULL;
    d_load(v);
    d_load_hash(htab);
    char* msgs[] = {"0. Quit", "1. Add", "2. Print", "3. Add to hash table", "4. Print hash table"};
    int choice = -1;
    while (choice != 0)
    {
        choice = dialog(msgs);
        if (choice == 1)
        {
            int key;
            char* info;
            printf("Enter key: ");
            scanf("%d", &key);
            printf("Enter information: ");
            scanf("%*c");
            info = get_string();
            add_to_vector(v, key, info);
        }
        if (choice == 2)
        {
            print(v);
        }
        if (choice == 3)
        {
            int key;
            printf("Enter key: ");
            scanf("%d", &key);
            htab = add_to_hash_table(htab, key);
        }
        if (choice == 4)
        {

        }

    }
    d_save(v);
    save_hash_table(htab);
    printf("Table saved ^_^\n");
    free(v);
    return 0;
}








