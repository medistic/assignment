#include "graph.h"

#include <stdlib.h>
#include <string.h>

static ListNode *create_list_node(int vertex, int weight)
{
    ListNode *node = (ListNode *)malloc(sizeof(ListNode));
    if (node == NULL) return NULL;

    node->vertex = vertex;
    node->weight = weight;
    node->next = NULL;
    return node;
}

static void shuffle_vertices(int vertices[])
{
    int i;
    for (i = VERTEX_COUNT - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int temp = vertices[i];
        vertices[i] = vertices[j];
        vertices[j] = temp;
    }
}

int generate_connected_graph(int edge_count, Edge edges[])
{
    int used[VERTEX_COUNT][VERTEX_COUNT] = {{0}};
    int vertices[VERTEX_COUNT];
    int count = 0;
    int i;

    if (edges == NULL ||
        edge_count < VERTEX_COUNT - 1 ||
        edge_count > (VERTEX_COUNT * (VERTEX_COUNT - 1)) / 2) {
        return 0;
    }

    /* 먼저 spanning tree를 만들어 모든 정점의 연결을 보장한다. */
    for (i = 0; i < VERTEX_COUNT; i++) vertices[i] = i;
    shuffle_vertices(vertices);

    for (i = 1; i < VERTEX_COUNT; i++) {
        int parent_index = rand() % i;
        int u = vertices[i];
        int v = vertices[parent_index];

        edges[count].u = u;
        edges[count].v = v;
        edges[count].weight = 1 + rand() % MAX_WEIGHT;

        used[u][v] = 1;
        used[v][u] = 1;
        count++;
    }

    /* 목표 간선 수까지 self-loop/중복 없이 추가한다. */
    while (count < edge_count) {
        int u = rand() % VERTEX_COUNT;
        int v = rand() % VERTEX_COUNT;

        if (u == v || used[u][v]) continue;

        edges[count].u = u;
        edges[count].v = v;
        edges[count].weight = 1 + rand() % MAX_WEIGHT;

        used[u][v] = 1;
        used[v][u] = 1;
        count++;
    }

    return 1;
}

int build_list_graph(ListGraph *graph, const Edge edges[], int edge_count)
{
    int i;

    if (graph == NULL || edges == NULL) return 0;

    graph->vertex_count = VERTEX_COUNT;
    graph->edge_count = 0;
    memset(graph->heads, 0, sizeof(graph->heads));

    for (i = 0; i < edge_count; i++) {
        ListNode *uv = create_list_node(edges[i].v, edges[i].weight);
        ListNode *vu = create_list_node(edges[i].u, edges[i].weight);

        if (uv == NULL || vu == NULL) {
            free(uv);
            free(vu);
            destroy_list_graph(graph);
            return 0;
        }

        uv->next = graph->heads[edges[i].u];
        graph->heads[edges[i].u] = uv;

        vu->next = graph->heads[edges[i].v];
        graph->heads[edges[i].v] = vu;

        graph->edge_count++;
    }

    return 1;
}

void build_matrix_graph(MatrixGraph *graph, const Edge edges[], int edge_count)
{
    int i;

    graph->vertex_count = VERTEX_COUNT;
    graph->edge_count = edge_count;
    memset(graph->weight, 0, sizeof(graph->weight));

    for (i = 0; i < edge_count; i++) {
        int u = edges[i].u;
        int v = edges[i].v;
        int w = edges[i].weight;
        graph->weight[u][v] = w;
        graph->weight[v][u] = w;
    }
}

void destroy_list_graph(ListGraph *graph)
{
    int i;

    if (graph == NULL) return;

    for (i = 0; i < VERTEX_COUNT; i++) {
        ListNode *current = graph->heads[i];

        while (current != NULL) {
            ListNode *next = current->next;
            free(current);
            current = next;
        }

        graph->heads[i] = NULL;
    }

    graph->edge_count = 0;
}

size_t list_graph_memory_bytes(const ListGraph *graph)
{
    if (graph == NULL) return 0;

    return sizeof(ListGraph) +
           (size_t)(2 * graph->edge_count) * sizeof(ListNode);
}

size_t matrix_graph_memory_bytes(const MatrixGraph *graph)
{
    (void)graph;
    return sizeof(MatrixGraph);
}
