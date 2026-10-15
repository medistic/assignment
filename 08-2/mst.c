#include "mst.h"

#include <limits.h>
#include <stdlib.h>
#include <time.h>

static double elapsed_us(const struct timespec *start, const struct timespec *end)
{
    double seconds = (double)(end->tv_sec - start->tv_sec);
    double nanoseconds = (double)(end->tv_nsec - start->tv_nsec);
    return seconds * 1000000.0 + nanoseconds / 1000.0;
}

static int edge_compare(const void *a, const void *b)
{
    const Edge *ea = (const Edge *)a;
    const Edge *eb = (const Edge *)b;

    if (ea->weight != eb->weight)
        return (ea->weight > eb->weight) - (ea->weight < eb->weight);
    if (ea->u != eb->u)
        return (ea->u > eb->u) - (ea->u < eb->u);
    return (ea->v > eb->v) - (ea->v < eb->v);
}

static int uf_find(int parent[], int x)
{
    if (parent[x] != x)
        parent[x] = uf_find(parent, parent[x]);
    return parent[x];
}

static void uf_union(int parent[], int rank[], int a, int b)
{
    int root_a = uf_find(parent, a);
    int root_b = uf_find(parent, b);

    if (root_a == root_b) return;

    if (rank[root_a] < rank[root_b]) {
        parent[root_a] = root_b;
    }
    else if (rank[root_a] > rank[root_b]) {
        parent[root_b] = root_a;
    }
    else {
        parent[root_b] = root_a;
        rank[root_a]++;
    }
}

static int collect_unique_edges(const ListGraph *graph, Edge out_edges[])
{
    int count = 0;
    int u;

    for (u = 0; u < VERTEX_COUNT; u++) {
        ListNode *current = graph->heads[u];

        while (current != NULL) {
            if (u < current->vertex) {
                out_edges[count].u = u;
                out_edges[count].v = current->vertex;
                out_edges[count].weight = current->weight;
                count++;
            }
            current = current->next;
        }
    }

    return count;
}

int kruskal_mst(const ListGraph *graph, MSTResult *result)
{
    Edge *working_edges;
    int parent[VERTEX_COUNT];
    int rank[VERTEX_COUNT] = {0};
    int collected;
    int i;
    struct timespec start;
    struct timespec end;

    if (graph == NULL || result == NULL) return 0;

    result->edge_count = 0;
    result->total_weight = 0;
    result->execution_time_us = 0.0;

    result->memory_bytes =
        list_graph_memory_bytes(graph) +
        (size_t)graph->edge_count * sizeof(Edge) +
        sizeof(parent) +
        sizeof(rank) +
        sizeof(result->edges);

    timespec_get(&start, TIME_UTC);

    working_edges = (Edge *)malloc(sizeof(Edge) * graph->edge_count);
    if (working_edges == NULL) return 0;

    collected = collect_unique_edges(graph, working_edges);
    if (collected != graph->edge_count) {
        free(working_edges);
        return 0;
    }

    qsort(working_edges, graph->edge_count, sizeof(Edge), edge_compare);

    for (i = 0; i < VERTEX_COUNT; i++) parent[i] = i;

    for (i = 0;
         i < graph->edge_count && result->edge_count < VERTEX_COUNT - 1;
         i++) {
        int u = working_edges[i].u;
        int v = working_edges[i].v;
        int root_u = uf_find(parent, u);
        int root_v = uf_find(parent, v);

        if (root_u != root_v) {
            result->edges[result->edge_count++] = working_edges[i];
            result->total_weight += working_edges[i].weight;
            uf_union(parent, rank, root_u, root_v);
        }
    }

    free(working_edges);
    timespec_get(&end, TIME_UTC);
    result->execution_time_us = elapsed_us(&start, &end);

    return result->edge_count == VERTEX_COUNT - 1;
}

int prim_mst(const MatrixGraph *graph, MSTResult *result)
{
    int key[VERTEX_COUNT];
    int parent[VERTEX_COUNT];
    int in_mst[VERTEX_COUNT];
    int count;
    int i;
    struct timespec start;
    struct timespec end;

    if (graph == NULL || result == NULL) return 0;

    result->edge_count = 0;
    result->total_weight = 0;
    result->execution_time_us = 0.0;

    result->memory_bytes =
        matrix_graph_memory_bytes(graph) +
        sizeof(key) +
        sizeof(parent) +
        sizeof(in_mst) +
        sizeof(result->edges);

    timespec_get(&start, TIME_UTC);

    for (i = 0; i < VERTEX_COUNT; i++) {
        key[i] = INT_MAX;
        parent[i] = -1;
        in_mst[i] = 0;
    }

    key[0] = 0;

    for (count = 0; count < VERTEX_COUNT; count++) {
        int min_key = INT_MAX;
        int u = -1;
        int v;

        for (v = 0; v < VERTEX_COUNT; v++) {
            if (!in_mst[v] && key[v] < min_key) {
                min_key = key[v];
                u = v;
            }
        }

        if (u == -1) break;
        in_mst[u] = 1;

        for (v = 0; v < VERTEX_COUNT; v++) {
            int w = graph->weight[u][v];

            if (w > 0 && !in_mst[v] && w < key[v]) {
                key[v] = w;
                parent[v] = u;
            }
        }
    }

    for (i = 1; i < VERTEX_COUNT; i++) {
        if (parent[i] == -1) {
            timespec_get(&end, TIME_UTC);
            result->execution_time_us = elapsed_us(&start, &end);
            return 0;
        }

        result->edges[result->edge_count].u = parent[i];
        result->edges[result->edge_count].v = i;
        result->edges[result->edge_count].weight = graph->weight[parent[i]][i];
        result->total_weight += graph->weight[parent[i]][i];
        result->edge_count++;
    }

    timespec_get(&end, TIME_UTC);
    result->execution_time_us = elapsed_us(&start, &end);

    return result->edge_count == VERTEX_COUNT - 1;
}
