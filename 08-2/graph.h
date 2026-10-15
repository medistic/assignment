#ifndef GRAPH_H
#define GRAPH_H

#include <stddef.h>

#define VERTEX_COUNT 50
#define MAX_WEIGHT 1000

typedef struct {
    int u;
    int v;
    int weight;
} Edge;

typedef struct ListNode {
    int vertex;
    int weight;
    struct ListNode *next;
} ListNode;

typedef struct {
    int vertex_count;
    int edge_count;
    ListNode *heads[VERTEX_COUNT];
} ListGraph;

typedef struct {
    int vertex_count;
    int edge_count;
    int weight[VERTEX_COUNT][VERTEX_COUNT];
} MatrixGraph;

int generate_connected_graph(int edge_count, Edge edges[]);
int build_list_graph(ListGraph *graph, const Edge edges[], int edge_count);
void build_matrix_graph(MatrixGraph *graph, const Edge edges[], int edge_count);
void destroy_list_graph(ListGraph *graph);
size_t list_graph_memory_bytes(const ListGraph *graph);
size_t matrix_graph_memory_bytes(const MatrixGraph *graph);

#endif
