#ifndef MST_H
#define MST_H

#include "graph.h"

#include <stddef.h>

typedef struct {
    Edge edges[VERTEX_COUNT - 1];
    int edge_count;
    int total_weight;
    double execution_time_us;
    size_t memory_bytes;
} MSTResult;

int kruskal_mst(const ListGraph *graph, MSTResult *result);
int prim_mst(const MatrixGraph *graph, MSTResult *result);

#endif
