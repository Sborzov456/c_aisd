#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
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

        for(i = 0; i < 10; ++i)
            puts(msgs[i]);
        puts("Make your choice: --> ");
        n = getint(&rc);
        if(n == 0)
            rc = 0;
    } while(rc < 0 || rc >= 10);
    return rc;
}
//-------------------STRUCTS---------------------
struct edge;
struct vertex;
typedef struct vertex
{
    int x;
    int y;
    int key;
    int index;//the number of vertex
    int color;//-1 - white, 1 - gray [FOR DFS]
    struct edge* edge;
    struct vertex* next;
    struct vertex* prev;//[FOR DFS]
}vertex;
typedef struct edge
{
    vertex* to;
    int weight;
    struct edge* next;
}edge;
typedef struct graph
{
    int v;
    int e;
    vertex* head;
}graph;
//-------------------HELPFUL FUNCTIONS--------------------
edge* delete_from_edge_list(edge* edge_list_head, int key_to)
{
    edge* tmp = edge_list_head;
    edge* prev;
    while ((tmp != NULL) && (tmp -> to -> key != key_to))
    {
        prev = tmp;
        tmp = tmp -> next;
    }
    if (tmp == edge_list_head)
    {
        free(edge_list_head);
        return tmp -> next;
    }
    else
    {
        prev -> next = tmp -> next;
        free(tmp);
        return edge_list_head;
    }
}
graph* delete_from_vertex_list(graph* g, int key)
{
    vertex* head = g -> head;
    vertex* prev;
    while ((head != NULL) && (head -> key != key))
    {
        prev = head;
        head = head -> next;
    }
    if (head == g -> head)
    {
        free(head);
        head = head -> next;
        return g;
    }
    else
    {
        prev -> next = head -> next;
        free(head);
        return g;
    }
}
//-------------------ADD/DELETE/FIND FUNCTIONS---------------------
graph* create()
{
    graph* g = (graph*)malloc(sizeof(graph));
    g -> head = NULL;
    g -> v = 0;
    g -> e = 0;
    return g;
}
graph* add_vertex(graph* g, int x, int y, int key)
{
    vertex* v = (vertex*)malloc(sizeof(vertex));
    v -> x = x;
    v -> y = y;
    v -> key = key;
    v -> next = NULL;
//--------FOR DFS AND TOPOLOGICAL SORT--------
    v -> color = -1;
//--------FOR BELLMAN FORD ALGORITHM----------
    v -> index = (g -> v) + 1;
//--------------------------------------------
    v -> edge = NULL;
    if (g -> head == NULL)
    {
        g -> head = v;
    }
    else
    {
       v -> next = g -> head;
       g -> head = v;
    }
    (g -> v)++;
    return g;
}
vertex* find_vertex_by_key(graph* g, int key)
{
    vertex* head = g -> head;
    while ((head != NULL) && (head -> key != key)) head = head -> next;
    if (head != NULL) return head;
    else return NULL;
}
graph* add_edge(graph* g, int key1, int key2, int weight)
{
    vertex* v_from = find_vertex_by_key(g, key1);
    vertex* v_to = find_vertex_by_key(g, key2);
    edge* e = (edge*)malloc(sizeof(edge));
    e -> weight = weight;
    if (v_from -> edge == NULL)
    {
        v_from -> edge = e;
        v_from -> edge -> next = NULL;
        e -> to = v_to;
    }
    else
    {
        e -> next = v_from -> edge;
        v_from -> edge = e;
        e -> to = v_to;
    }
    (g -> e)++;
    return g;
}
graph* delete_edge(graph* g, int key1, int key2)
{
    vertex* v1 = find_vertex_by_key(g, key1);
    vertex* v2 = find_vertex_by_key(g, key2);
    int flag = -1;
    edge* tmp = v1 -> edge;
    if (v1 -> edge != NULL)
    {
        while (tmp != NULL)
        {
            if (tmp -> to -> key == v2 -> key)
            {
                flag = 1;
            }
            tmp = tmp -> next;
        }
    }
   else return g;
   if (flag == 1) v1 -> edge = delete_from_edge_list(v1 -> edge, key2);
   return g;
}
graph* delete_vertex(graph* g, int key)
{
    vertex* v = find_vertex_by_key(g, key);
    vertex* head = g -> head;
    while (head != NULL)
    {
        edge* e_list = head -> edge;
        while(e_list != NULL)
        {
            if (e_list -> to -> key == v -> key)
            {
                g = delete_edge(g, head -> key, key);
            }
            e_list = e_list -> next;
        }
        head = head -> next;
    }
    g = delete_from_vertex_list(g, key);
    free(v -> edge);
    return g;
}
//-------------------PRINT FUNCTION---------------------
void print_graph(graph* g)
{
    vertex* head = g -> head;
    printf("Graph: \n");
    while (head != NULL)
    {
        printf("%d(%d) -> ", head -> key, head -> index);
        edge* e_head = head -> edge;
        if (e_head != NULL)
        {
            while(e_head != NULL)
            {
                printf("%d ", e_head -> to -> key);
                e_head = e_head -> next;
            }
        }
        head = head -> next;
        printf("\n");
    }
}
//-------------------DFS---------------------
int dfs(vertex* v_start, vertex* prev, int finish_key)//ÌÀÑÑÈÂ ÏÐÅÄØÅÑÒÂÓÞÙÈÕ ÂÅÐØÈÍ
{
     printf("KEY: %d\n", v_start -> key);
     if (prev != NULL) printf("PREV KEY: %d\n", prev -> key);
     if (v_start -> prev != NULL) printf("NEIGHOBIOR PREV KEY: %d\n", v_start -> prev -> key);
     else printf("PREV KEY: NULL\n");
     printf("COLOR: %d\n", v_start -> color);
     printf("---------\n");
     if(v_start -> key == finish_key) return 1;
     if(v_start -> color == 1) return -1;
     v_start -> color = 1;
     prev = v_start;
     edge* edge_list = prev -> edge;
     if (edge_list == NULL);
     while (edge_list != NULL)
     {
         vertex* neighbour = edge_list -> to;
         neighbour->prev = prev;
         int flag = dfs(neighbour, prev, finish_key);
         if (flag == 1) return 1;
         else edge_list = edge_list -> next;
     }
     return -1;
}
void print_result_dfs(graph* g, int start_key, int finish_key)
{
    vertex* head = g -> head;
    for (int i = 1; i <= g -> v; i++)
    {
        head -> color = -1;
        head -> prev = NULL;
        head = head -> next;
    }
    vertex* v_start = find_vertex_by_key(g, start_key);
    int flag = dfs(v_start, NULL, finish_key);
    if (flag == 1)
    {
         printf("The vertex is reachable\n");
         vertex* v_finish = find_vertex_by_key(g, finish_key);
         vertex* tmp = v_finish;
         printf("Way: \n");
         while (tmp != NULL)
         {
             printf("%d ",tmp -> key);
             tmp = tmp -> prev;
         }
    }
    else printf("The vertex is NOT reachable");
}
//-------------------BELLMAN FORD---------------------
int bellman_ford(graph* g, int start_key, int finish_key)
{
    int* d = (int*)malloc((g -> v + 1)*sizeof(int));
    vertex** res = (vertex**)malloc((g -> v + 1)*sizeof(vertex*));
    vertex* v_start = find_vertex_by_key(g, start_key);
    vertex* v_finish = find_vertex_by_key(g, finish_key);
    for (int i = 0; i < (g -> v + 1); i++) res[i] = NULL;
    for (int i = 0; i < (g -> v + 1); i++) d[i] = -1;
    d[v_start -> index] = 0;
    for (int i = 0; i < g -> v; i++)
    {
        vertex* buf = g -> head;
        for (int j = 0; j < g -> v + 1; j++)
        {
            edge* buf1 = buf -> edge;
            while (buf1 != NULL)
            {
                if ((d[buf1 -> to -> index] == -1) && (d[buf -> index] != -1))
                {
                    d[buf1 -> to -> index] = d[buf -> index] + buf1 -> weight;
                    res[buf1 -> to -> index] = buf;
                }
                else if((d[buf1 -> to -> index] != -1) && (d[buf -> index] != -1) && (d[buf1 -> to -> index] > d[buf -> index] + buf1 -> weight))
                {
                    d[buf1 -> to -> index] = d[buf -> index] + buf1 -> weight;
                    res[buf1 -> to -> index] = buf;
                }
                buf1 = buf1 -> next;
            }
            buf = buf -> next;
            if (!buf) break;
        }
    }
    printf("Distance: %d\n", d[v_finish -> index]);
    vertex* buf = v_finish;
    if (d[v_finish -> index] != -1)
    {
        printf("Way: ");
        do
        {
            printf("%d ", v_finish -> key);
            v_finish = res[v_finish -> index];
        }while((res[v_finish -> index]!=NULL) && (res[v_finish -> index]->key != buf->key));
        printf("%d\n", v_start -> key);
    }
    else
    {
        printf("There is no way");
        return -1;
    }
    return 0;
}
//-------------------TOPOLOGICAL SORT---------------------
int is_headspring(graph* g, vertex* v)
{
    vertex* head = g -> head;
    if (v -> color != 1)
    {
        while (head != NULL)
        {
            edge* e_list = head -> edge;
            while(e_list != NULL)
            {
                if ((e_list -> to -> key == v -> key) && (head -> color != 1)) return -1; //not the headspring
                e_list = e_list -> next;
            }
            head = head -> next;
        }
        //v -> color = 1;
        return 1;//the headspring
    }

}
void topological_sort(graph* g)//ÏÐÎÂÅÐÈÒÜ ÄËß ÃÐÀÔÀ ÈÇ ËÅÊÖÈÉ
{
    int* sorted_array = (int*)malloc(g -> v*sizeof(int));
    for (int i = 1; i <= g -> v; i++)
    {
        vertex* head = g -> head;
        while(head != NULL)
        {
            if ((is_headspring(g, head) == 1) && (head -> color != 1))
            {
                head -> color = 1;
                sorted_array[i] = head -> index;
                break;
            }
            head = head -> next;
        }
    }
    vertex* head = g -> head;
    for (int i = g -> v; i >= 1; i--)
    {
        head -> index = sorted_array[i];
        head = head -> next;
    }
}
//-------------------MAIN---------------------
int main()
{
    int choice = -1;
    char *msgs[] = {"0. Quit", "1. Add vertex", "2. Add edge", "3. Delete vertex", "4. Delete edge", "5. Print graph", "6. DFS", "7. Bellman-Ford algorithm", "8. Topological sort", "9. Test graph"};
    graph* g = create();
    while (choice!=0)
    {
        choice = dialog(msgs);
        if (choice == 1)
        {
            int x, y, key;
            printf("Enter x: ");
            scanf("%d", &x);
            printf("Enter y: ");
            scanf("%d", &y);
            printf("Enter key: ");
            scanf("%d", &key);
            g = add_vertex(g, x, y, key);
        }
       if (choice == 2)
        {
            int key1, key2, weight;
            printf("Enter key from: ");
            scanf("%d", &key1);
            printf("Enter key to: ");
            scanf("%d", &key2);
            printf("Enter weight of edge: ");
            scanf("%d", &weight);
            g = add_edge(g, key1, key2, weight);
        }
        if (choice == 3)
        {
            int key;
            printf("Enter key of vertex you want to remove: ");
            scanf("%d", &key);
            g = delete_vertex(g, key);
        }
        if (choice == 4)
        {
            int key1, key2;
            printf("Enter key of first vertex: ");
            scanf("%d", &key1);
            printf("Enter key of second vertex: ");
            scanf("%d", &key2);
            g = delete_edge(g, key1, key2);
        }
        if (choice == 5)
        {
            print_graph(g);
        }
        if (choice == 6)
        {
            int key1, key2;
            printf("Enter key from: ");
            scanf("%d", &key1);
            printf("Enter key to: ");
            scanf("%d", &key2);
            print_result_dfs(g, key1, key2);
        }
        if (choice == 7)
        {
            int key1, key2, a;
            printf("Enter key from: ");
            scanf("%d", &key1);
            printf("Enter key to: ");
            scanf("%d", &key2);
            a = bellman_ford(g, key1, key2);
        }
        if (choice == 8)
        {
            topological_sort(g);
        }
        if (choice == 9)
        {
            add_vertex(g,1,1,1);
            add_vertex(g,1,1,2);
            add_vertex(g,1,1,3);
            add_vertex(g,1,1,4);
            add_vertex(g,1,1,5);
            add_vertex(g,1,1,6);
            add_vertex(g,1,1,7);
            add_vertex(g,1,1,8);
            add_edge(g, 1, 2, 3);
            add_edge(g, 1, 4, 1);
            add_edge(g, 2, 3, 2);
            add_edge(g, 2, 5, 7);
            add_edge(g, 4, 6, 3);
            add_edge(g, 4, 2, 11);
            add_edge(g, 6, 7, 5);
            add_edge(g, 6, 8, 6);
        }

    }
    return(0);
}









