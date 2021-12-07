#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define N 3
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

    for(i = 0; i < 14; ++i)
        puts(msgs[i]);
    puts("Make your choice: --> ");
    n = getint(&rc);
    if(n == 0)
        rc = 0;
} while(rc < 0 || rc >= 14);
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
typedef struct info
{
    float numb1;
    float numb2;
    char* str;
}info_t;

typedef struct item_s
{
    info_t* info;
    char* key2;
    int key1;
}item_t;

typedef struct key_space1
{
    int busy;
    int key1;
    int release;
    item_t* info;
}key_s1;

typedef struct key_space2
{
   char* key2;
   item_t* info;
   struct key_space2 *next;
}key_s2;

typedef struct table_s
{
    key_s1* ks1;
    key_s2** ks2;
    int msize1;
    int msize2;
}table_t;
//------------------FUNCTIONS FOR KEY SPACE 1-----------------
key_s1* create_key_space_1(int msize1)
{
    key_s1* p = (key_s1*)malloc(sizeof(key_s1)*msize1);
    for (int i = 0; i < msize1; i++)
    {
        p[i].busy = 0;
        p[i].release = 1;
    }
    return p;
}
int is_complete(key_s1* ks1, int msize1)
{
    int i = 0;
    int flag = -1;
    for (int i = 0; i < msize1; i++)
    {
        if (ks1[i].busy == 0) flag = 1;
    }
    return flag;
}
int max_release(key_s1* ks1, int msize1, int key1)
{
    int release = 0;
    for(int i = 0; i < msize1; i++)
    {
        if (key1 == ks1[i].key1) release++;
    }
    return release;
}//ДЛЯ СБОРКИ МУСОРА: можно в дырку копировать последний элемент
key_s1* reorganistion(key_s1* ks1, int msize1)
{
    for (int i = 0; i < msize1; i++)
    {
        if (ks1[i].busy == 0)
        {
            for (int j = i; j < msize1; j++) ks1[j] = ks1[j+1];
        }
    }
    return ks1;
}
key_s1* add_1(key_s1* ks1, int msize1, item_t* info, int key)//новый элемент добавляется в конец таблицы, если места не осталось, то включается функция сборки мусора
{
    //просматривать таблицу с конца, чтобы найти максимальный резил (можно без функции)
    //при удалении и удалении пересчитать версии (удалили 8 элементов, релизы 9 и 10 менять не надо) при вставке выичлсять максимальный релиз
    if (is_complete(ks1, msize1) == -1)
    {
        printf("Key space 1 is complete\n");
        return ks1;
    }
    else
    {
        int i = msize1 - 1;
        if (ks1[i].busy == 1) reorganistion(ks1, msize1);
        while ((ks1[i].busy != 1) && (i != 0)) i = i - 1;
        if (ks1[i].busy == 1)
        {
            ks1[i+1].key1 = key;
            ks1[i+1].busy = 1;
            ks1[i+1].release = max_release(ks1, msize1, key);
            ks1[i+1].info = info;
        }
        else
        {
            ks1[i].key1 = key;
            ks1[i].busy = 1;
            ks1[i].release = max_release(ks1, msize1, key);
            ks1[i].info = info;
        }
    }
    return ks1;
}
key_s1* delete_all_releases(key_s1* ks1, int msize1)
{
    int key;
    printf("Enter key you want to remove: ");
    scanf("%d", &key);
    for (int i = 0; i < msize1; i++)
    {
        if (key == ks1[i].key1)
        {
            ks1[i].busy = 0;
            free(ks1[i].info);
        }
    }
    return ks1;
}
key_s1* delete_chosen_release(key_s1* ks1, int msize1, int release, int key)
{
    int i = 0;
    for (i; i < msize1; i++)
    {
        if ((key == ks1[i].key1) && (release == ks1[i].release))
        {
            ks1[i].busy = 0;
            free(ks1[i].info);
            break;
        }
    }
    //if (i == (msize1 - 1)) printf("Key or release not found ");
    return ks1;
}
int find_in_key_space_1(int key1, int release, key_s1* ks1, int msize1)
{
    int i;
    for (i = 0; i <= msize1; i++)
    {
        if ((key1 == ks1[i].key1) && (release == ks1[i].release)) return i;
    }
    if (i == msize1)
    {
        printf("This key not exist in key space 1\n");
        return -3;
    }
}
void print_founded_index_in_key_space_1(key_s1* ks1, int msize1)
{
    int key, release, index;
    printf("Enter key: ");
    scanf("%*c");
    scanf("%d", &key);
    printf("Enter release: ");
    scanf("%d", &release);
    index = find_in_key_space_1(key, release, ks1, msize1);
    if (index >= 0)
    {
        printf("Index: %d\n", index);
        printf("Info: \n");
        printf("First float: %f\n", ks1[index].info->info->numb1);
        printf("Second float: %f\n", ks1[index].info->info->numb2);
        printf("String: %s\n", ks1[index].info->info->str);
    }
}
void print_key_space_1(key_s1* ks1, int msize1)
{
    printf("Key space 1:\n");
    int i = 0;
    while (i < msize1)
    {
        if (ks1[i].busy != 0) printf("%d(%d)\n", ks1[i].key1, ks1[i].release);
        else printf("\n");
        i++;
    }
}
//------------------FUNCTIONS FOR KEY SPACE 2-----------------
key_s2** create_key_space_2(int msize2)
{
    key_s2** q = (key_s2**)malloc(sizeof(key_s2*)*msize2);
    for (int i = 0; i <= msize2; i++)
    {
        q[i] = (key_s2*)malloc(sizeof(key_s2));
        q[i] -> next = NULL;
        q[i] -> key2 = NULL;
    }
    return q;
}
int is_empty(key_s2** ks2, int msize2)
{
    int flag = 1;//пуста
    for (int i = 0; i < msize2; i++)
    {
        if (ks2[i]->key2 != NULL) flag = -1;//не пуста
    }
    return flag;
}
int find_in_key_space_2(key_s2** ks2, char* key, int msize2)//?
{
    int res = -1;
    int h = hash_function(key, msize2);
    if (ks2[h] -> key2 == NULL)
    {
        //printf("This key not exist in key space 2\n");
        return -1;
    }
    else if ((ks2[h] -> next == NULL) && (strcmp(ks2[h] -> key2, key) == 0))
    {
        return h;
    }
    else
    {
        key_s2* tmp = ks2[h];
        while ((tmp != NULL) && (strcmp(tmp -> key2, key) != 0)) tmp = tmp -> next;
        if (tmp != NULL) return h;
        else return -2;
    }
}
int hash_function(char* key, int msize2)
{
    int c, b = 0, i = 0;
    while (*(key+i) != '\0')
    {
        c = (int)*(key+i);
        b = b + c - 97;
        i++;
    }
    b = b % msize2;
    return b;
}
key_s2** add_2(key_s2** ks2, item_t* info, char* key2, int msize2)//проверка на уникальность
{
   if (is_empty(ks2, msize2) == -1)
   {
       if (find_in_key_space_2(ks2, key2, msize2) >= 0)
       {
           printf("This key is already exist is key space 2");
           return ks2;
       }
   }
   int h = hash_function(key2, msize2);
   key_s2* pilgrim = ks2[h];
   if (ks2[h] -> key2 != NULL)
   {
       while (pilgrim -> next != NULL) pilgrim = pilgrim -> next;
       key_s2* tmp = (key_s2*)malloc(sizeof(key_s2));
       tmp -> key2 = (char*)malloc(sizeof(char)*strlen(key2));
       tmp -> key2 = strcpy(tmp -> key2, key2);
       tmp -> info = info;
       tmp -> next = NULL;
       pilgrim -> next = tmp;
   }
   else
   {
       ks2[h] -> key2 = (char*)malloc(sizeof(char)*strlen(key2));
       ks2[h] -> key2 = strcpy(ks2[h] -> key2, key2);
       ks2[h] -> info = info;
   }
   return ks2;
}
void print_founded_index_in_key_space_2(key_s2** ks2, int msize2)
{
    printf("Enter key: ");
    scanf("%*c");
    char* key = get_string();
    int index;
    index = find_in_key_space_2(ks2, key, msize2);
    if (index >= 0) printf("Index of list: %d", index);
}
key_s2** delete_from_array(key_s2** ks2, int index)
{
    ks2[index] -> key2 = NULL;
    ks2[index] -> info -> key2 = NULL;
    free(ks2[index] -> key2);
    free(ks2[index] -> info);
    free(ks2[index]);
    ks2[index] = NULL;
    return ks2;
}
key_s2* delete_from_list(key_s2** ks2, int index, char* key)
{
    if (strcmp(ks2[index] -> key2, key) == 0)
    {
        key_s2* tmp1 = ks2[index];
        tmp1 = ks2[index] -> next;
        ks2[index] -> key2 = NULL;
        ks2[index] -> info -> key2 = NULL;
        free(ks2[index] -> key2);
        free(ks2[index] -> info);
        free(ks2[index]);
        ks2[index] = NULL;
        return tmp1;
    }
    else
    {
       key_s2* tmp2 = ks2[index];
       key_s2* prev;
       while (tmp2 -> next != NULL)
            {
                if (strcmp(tmp2 -> key2, key) == 0) break;
                prev = tmp2;
                tmp2 = tmp2 -> next;
            }
       prev -> next = tmp2 -> next;
       tmp2 -> key2 = NULL;
       tmp2 -> info -> key2 = NULL;
       free(tmp2 -> key2);
       free(tmp2 -> info);
       free(tmp2);
       return ks2[index];
    }
}
key_s2** delete_ks2 (key_s2** ks2, int msize2, char* key)
{
    int index;
    index = find_in_key_space_2(ks2, key, msize2);
    if (index >= 0)
    {
        if (ks2[index] -> next == NULL)
        {
            ks2 = delete_from_array(ks2, index);
            return ks2;
        }
        else
        {
            ks2[index] = delete_from_list(ks2, index, key);
            return ks2;
        }
    }
    else return ks2;
}
void print_key_space_2(key_s2** ks2, int msize2)
{
    for (int i = 0; i <= msize2; i++)
    {
        key_s2* tmp = ks2[i];
        printf("%d: ", i);
        while (tmp != NULL)
        {
            printf("%s, ", tmp -> key2);
            tmp = tmp -> next;
        }
        printf("\n");
    }
}
//------------------FUNCTIONS FOR TABLE-----------------
table_t* create_table(int msize1, int msize2)
{
    table_t* table = (table_t*)malloc(sizeof(table_t));
    key_s1* ks1 = create_key_space_1(msize1);
    key_s2** ks2 = create_key_space_2(msize2);
    table -> ks1 = ks1;
    table -> ks2 = ks2;
    table -> msize1 = msize1;
    table -> msize2 = msize2;
    return table;
}
table_t* add_table(table_t* table, int msize1, int msize2)
{
    float f1;
    float f2;
    char* str;
    int key1;
    char* key2 = NULL;//переписать через указатель
    printf("Insert key1: ");
    scanf("%d", &key1);
    printf("Insert key2: ");
    scanf("%*c");
    while (1)
    {
        key2 = get_string();
        if (strlen(key2) <= N) break;
        else printf("Error: Repeat input\n");
    }
    printf("Insert information: \n");
    printf("Insert first float: ");
    scanf("%f", &f1);
    printf("Insert second float: ");
    scanf("%f", &f2);
    scanf("%*c");
    printf("Insert string: ");
    str = get_string();
    item_t* it = (item_t*)malloc(sizeof(item_t));
    it -> info = (info_t*)malloc(sizeof(info_t));
    it -> info -> numb1 = f1;
    it -> info -> numb2 = f2;
    it -> info -> str = str;
    it -> key1 = key1;
    it -> key2 = (char*)malloc(sizeof(char)*strlen(key2));
    it -> key2 = strcpy(it -> key2, key2);
    table -> ks1 = add_1(table -> ks1, msize1, it, key1);
    table -> ks2 = add_2(table -> ks2, it, key2, msize2);
    return table;
}
void print_table(table_t* table, int msize1, int msize2)
{
    key_s1* ks1 = table -> ks1;
    key_s2** ks2 = table -> ks2;
    int i = 0, j = 0;
    while (i < msize1)
    {
        if ((ks1[i].busy != 0) && (ks1[i].info -> key2 != NULL))
        {
            printf("Item %d\n", i);
            printf("Key1: %d\n", ks1[i].key1);
            printf("Key2: %s\n", ks1[i].info -> key2);
            printf("Information\n");
            printf("  Float 1: %f1\n",ks1[i].info -> info -> numb1);
            printf("  Float 2: %f1\n",ks1[i].info -> info -> numb2);
            printf("  String: %s\n\n",ks1[i].info -> info -> str);
        }
        i++;
    }
}
item_t* find_item_by_double_key(key_s2** ks2, key_s1* ks1, int msize2, int msize1)
{
    int key1;
    int index;
    char* key2;
    printf("Insert key1: ");
    scanf("%d", &key1);
    scanf("%*c");
    printf("Insert key2: ");
    key2 = get_string();
    index = find_in_key_space_2(ks2, key2, msize2);
    if (index < 0)
    {
        printf("Key not found");
        return NULL;
    }
    int chek = find_in_key_space_1(key1, 1, ks1, msize1);
    key_s2* tmp = ks2[index];
    while ((tmp != NULL) && (strcmp(tmp -> key2, key2) != 0)) tmp = tmp -> next;
    if ((tmp -> info -> key1 == key1) && (chek != -3))
    {
        printf("\nInformation of founded item: \n");
        printf("First float: %f.1\n",tmp -> info -> info -> numb1);
        printf("Second float: %f.1\n",tmp -> info -> info -> numb2);
        printf("String: %s",tmp -> info -> info -> str);
        return tmp -> info;
    }
    else
    {
        printf("Key not found");
        return NULL;
    }
}
table_t* delete_item_by_double_key(table_t* table, int msize1, int msize2)
{
    key_s1* ks1 = table -> ks1;
    key_s2** ks2 = table -> ks2;
    int index1, index2;
    int key1;
    char* key2;
    printf("Insert key1: ");
    scanf("%d", &key1);
    scanf("%*c");
    printf("Insert key2: ");
    key2 = get_string();
    index1 = find_in_key_space_1(key1, 1, ks1, msize1);
    index2 = find_in_key_space_2(ks2, key2, msize2);
    if ((index1 >= 0) && (index2 >= 0))
    {
        key_s2* tmp = ks2[index2];
        while ((tmp != NULL) && (strcmp(tmp -> key2, key2) != 0)) tmp = tmp -> next;
        if (tmp -> info -> key1 == key1)
        {
            int release = ks1[index1].release;
            ks2 = delete_ks2(ks2, msize2, key2);
            ks1 = delete_chosen_release(ks1, msize1, release, key1);
        }
        else printf("Key not found");
    }
    else printf("Key not found\n");
    return table;
}
//------------------FILE FUNCTIONS-----------------
//1.Инормация, связанная с элементами таблицы сразу заносится в файл (ну короче инфо)
//2.По окончании работы программы простарнства ключей загружаются в файл
//3.ИЗ ПРЕДЫДЦУЩЕЙ ПРОГРАММЫ пространства ключей вводятся из файла
//файлы можно отличать по суффиксам
//пространства ключей лучше сохранять в двоичные файлы
//выгрузка из двочиного файла работает с теми же функциями, что и обычно
int load_table_to_file(table_t* table, const char* table_file)
{
    printf("debag1\n");
    FILE* description = fopen(table_file, "wb");
    printf("debag2\n");
    fwrite(&(table -> msize1), sizeof(int), 1, description);
    printf("debag3\n");
    fwrite(&(table -> msize2), sizeof(int), 1, description);
    printf("debag4\n");
    fwrite(table->ks1, sizeof(key_s1), msize1, description);
    /*for (int i = 0; i < table -> msize1; i++)
    {
        printf("debag5\n");
        fwrite(&(table -> ks1[i].busy), sizeof(int), 1, description);
        printf("debag6\n");
        if (table -> ks1[i].busy == 1) fwrite(&(table -> ks1[i].key1), sizeof(int), 1, description);
        printf("debag7\n");
        fwrite(&(table -> ks1[i].release), sizeof(int), 1, description);
        printf("debag8\n");
    }
    printf("debag9\n");
    //item_t* item;
    for (int i = 0; i < table -> msize1; i++)
    {
        printf("debag10\n");
        if (table -> ks1[i].busy == 1)
        {
            //item = table -> ks1[i].info;
            fwrite(table -> ks1[i].info -> info, sizeof(info_t), 1, description);
            fwrite(&(table -> ks1[i].info -> key1), sizeof(int), 1, description);
            fwrite(table -> ks1[i].info -> key2, sizeof(char), strlen(table -> ks1[i].info -> key2), description);
        }
    }
    printf("debag11\n");
    key_s2** ks2 = table -> ks2;
    for (int i = 0; i < table -> msize2; i++)
    {
        if (ks2[i]->key2 != NULL)
        {
            while(ks2[i] != NULL)
            {
                printf("debag13\n");
                printf("%s\n", ks2[i] -> key2);
                fwrite(ks2[i] -> key2, sizeof(char), strlen(ks2[i] -> key2), description);
                printf("debag14\n");
                ks2[i] = ks2[i] -> next;
            }
        }
    }*/
    fclose(description);
    return 0;
}
int load_table_from_file(table_t* table, const char* table_file)
{
    FILE* description = fopen(table_file, "rb");
    fread(&(table -> msize1), sizeof(int), 1, description);
    fread(&(table -> msize2), sizeof(int), 1, description);
    table -> ks1 = (key_s1*)malloc((table -> msize1)*sizeof(key_s1));
    fread(table->ks1, sizeof(key_s1), msize1, description);
    for (int i = 0; i < table -> msize1; i++)
    {
        printf("%d\n", table -> ks1[i].key1);
    }
    /*for(int i = 0; i < table -> msize1; i++)
    {
        table -> ks1[i].busy = 0;
        table -> ks1[i].release = 1;
        table -> ks1[i].info = NULL;
    }
    for (int i = 0; i < table -> msize1; i++)
    {
        fread(&(table -> ks1[i].busy), sizeof(int), 1, description);
        if (table -> ks1[i].busy == 1) fread(&(table -> ks1[i].key1), sizeof(int), 1, description);
        fread(&(table -> ks1[i].release), sizeof(int), 1, description);
    }
    printf("debag1\n");
    //item_t* item;
    for (int i = 0; i < table -> msize1; i++)
    {
        if (table -> ks1[i].busy == 1)
        {
            table -> ks1[i].info = (item_t*)malloc(sizeof(item_t));
            table -> ks1[i].info -> key2 = (char*)malloc(strlen(table -> ks1[i].info -> key2)*sizeof(char));
        }
    }
    for (int i = 0; i < table -> msize1; i++)
    {
        printf("debag2\n");
        if (table -> ks1[i].busy == 1)
        {
            printf("debag3\n");
             printf("debag4\n");
            fread(table -> ks1[i].info -> info, sizeof(info_t), 1, description);
             printf("debag5\n");
            fread(&(table -> ks1[i].info -> key1), sizeof(int), 1, description);
             printf("debag6\n");
            fread(table -> ks1[i].info -> key2, sizeof(char), strlen(table -> ks1[i].info -> key2), description);
             printf("debag7\n");
        }
        printf("degag\n");
    }
    printf("%d\n", table -> ks1[0].info -> key1);
    printf("%s\n", table -> ks1[0].info -> key2);
    printf("%d\n", table -> ks1[1].info -> key1);
    printf("%s\n", table -> ks1[1].info -> key2);
    printf("debag8\n");*/
    fclose(description);
    return 0;
}
//------------------MAIN-----------------
int main()
{
    char table_file[5];
    int test;
    int choice = -1;
    char *msgs[] = {"0. Quit","1. Add", "2. Find", "3. Delete", "4. Print table", "5. Find in key space 1", "6. Find in key space 2", "7. Delete all releases in key space 1", "8. Delete chosen release in key space 1","9. Reorganization key space 1", "10. Delete in key space 2", "11. Print key space 1", "12. Print key space 2", "13. Load table to file"};
    int msize1, msize2;
    table_t* table;
//--------------------------------------------------------
    int l;
    printf("Would you like to load table from file ? (1: Yes, 2: No)\n");
    scanf("%d", &l);
    if (l == 1)
    {
        printf("Enter filename for table: ");
        scanf("%s", table_file);
        table = create_table(0,0);
        load_table_from_file(table, table_file);
    }
    else if (l == 2)
    {
        printf("Enter size of new table: ");
        scanf("%d %d", &msize1, &msize2);
        table = create_table(msize1, msize2);
    }
    key_s1* ks1 = table -> ks1;
    key_s2** ks2 = table -> ks2;
    msize1 = table -> msize1;
    msize2 = table -> msize2;
//--------------------------------------------------------
    while (choice!=0)
   {
       choice = dialog(msgs);
       if (choice == 1)
       {
            table = add_table(table, msize1, msize2);
       }
       if (choice == 2)
       {
            find_item_by_double_key(ks2, ks1, msize2, msize1);
       }
       if (choice == 3)
       {
            table = delete_item_by_double_key(table, msize1, msize2);
       }
       if (choice == 4)
       {
            print_table(table, msize1, msize2);
       }
       if (choice == 5)
       {
            print_founded_index_in_key_space_1(ks1, msize1);
       }
       if (choice == 6)
       {
            print_founded_index_in_key_space_2(ks2, msize2);
       }
       if (choice == 7)
       {
            ks1 = delete_all_releases(ks1, msize1);
       }
       if (choice == 8)
       {
            int key, release;
            printf("Enter key you want to remove: ");
            scanf("%d", &key);
            printf("Enter release of key: ");
            scanf("%d", &release);
            ks1 = delete_chosen_release(ks1, msize1, release, key);
       }
       if (choice == 9)
       {
            ks1 = reorganistion(ks1, msize1);//можно улучшить, рассмотрев случай, когда нихрена не удаляли
       }
       if (choice == 10)
       {
            printf("Enter key you want to remove: ");
            scanf("%*c");
            char* key = get_string();
            ks2 = delete_ks2(ks2, msize2, key);
       }
       if (choice == 11)
       {
           print_key_space_1(ks1, msize1);
       }
       if (choice == 12)
       {
           print_key_space_2(ks2, msize2);
       }
       if (choice == 13)
       {
           printf("Enter table file: ");
           scanf("%s", table_file);
           load_table_to_file(table, table_file);
       }
   }
   return(0);
}














