#include "graph.h"
#include "mst.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SPARSE_EDGES 100
#define DENSE_EDGES 600

static void print_mst_edges(const MSTResult *result)
{
    int i;

    for (i = 0; i < result->edge_count; i++) {
        printf("  %2d - %2d  weight = %4d\n",
               result->edges[i].u,
               result->edges[i].v,
               result->edges[i].weight);
    }
}

static void print_case(
    int case_number,
    const char *graph_name,
    const char *representation,
    const char *algorithm,
    int original_edges,
    const MSTResult *result)
{
    printf("Case %d : %s - %s - %s\n",
           case_number, graph_name, representation, algorithm);
    printf("Vertices      : %d\n", VERTEX_COUNT);
    printf("Edges         : %d\n", original_edges);
    printf("MST Edges     : %d\n", result->edge_count);
    printf("MST Cost      : %d\n", result->total_weight);
    printf("Memory        : %zu Bytes\n", result->memory_bytes);
    printf("Execution Time: %.3f us\n", result->execution_time_us);
    printf("MST edge list:\n");
    print_mst_edges(result);
    printf("\n");
}

int main(int argc, char *argv[])
{
    unsigned int seed;

    Edge sparse_edges[SPARSE_EDGES];
    Edge dense_edges[DENSE_EDGES];

    ListGraph sparse_list;
    ListGraph dense_list;
    MatrixGraph sparse_matrix;
    MatrixGraph dense_matrix;

    MSTResult sparse_kruskal;
    MSTResult dense_kruskal;
    MSTResult sparse_prim;
    MSTResult dense_prim;

    if (argc >= 2)
        seed = (unsigned int)strtoul(argv[1], NULL, 10);
    else
        seed = (unsigned int)time(NULL);

    srand(seed);

    if (!generate_connected_graph(SPARSE_EDGES, sparse_edges)) {
        fprintf(stderr, "Error: sparse graph generation failed.\n");
        return 1;
    }

    if (!generate_connected_graph(DENSE_EDGES, dense_edges)) {
        fprintf(stderr, "Error: dense graph generation failed.\n");
        return 1;
    }

    if (!build_list_graph(&sparse_list, sparse_edges, SPARSE_EDGES)) {
        fprintf(stderr, "Error: sparse adjacency list build failed.\n");
        return 1;
    }

    if (!build_list_graph(&dense_list, dense_edges, DENSE_EDGES)) {
        fprintf(stderr, "Error: dense adjacency list build failed.\n");
        destroy_list_graph(&sparse_list);
        return 1;
    }

    build_matrix_graph(&sparse_matrix, sparse_edges, SPARSE_EDGES);
    build_matrix_graph(&dense_matrix, dense_edges, DENSE_EDGES);

    if (!kruskal_mst(&sparse_list, &sparse_kruskal) ||
        !kruskal_mst(&dense_list, &dense_kruskal) ||
        !prim_mst(&sparse_matrix, &sparse_prim) ||
        !prim_mst(&dense_matrix, &dense_prim)) {
        fprintf(stderr, "Error: MST calculation failed.\n");
        destroy_list_graph(&sparse_list);
        destroy_list_graph(&dense_list);
        return 1;
    }

    if (sparse_kruskal.total_weight != sparse_prim.total_weight) {
        fprintf(stderr, "Error: sparse MST costs do not match.\n");
        destroy_list_graph(&sparse_list);
        destroy_list_graph(&dense_list);
        return 1;
    }

    if (dense_kruskal.total_weight != dense_prim.total_weight) {
        fprintf(stderr, "Error: dense MST costs do not match.\n");
        destroy_list_graph(&sparse_list);
        destroy_list_graph(&dense_list);
        return 1;
    }

    printf("Random seed: %u\n\n", seed);

    print_case(1, "Sparse Graph", "Adjacency List", "Kruskal",
               SPARSE_EDGES, &sparse_kruskal);
    print_case(2, "Dense Graph", "Adjacency List", "Kruskal",
               DENSE_EDGES, &dense_kruskal);
    print_case(3, "Sparse Graph", "Adjacency Matrix", "Prim",
               SPARSE_EDGES, &sparse_prim);
    print_case(4, "Dense Graph", "Adjacency Matrix", "Prim",
               DENSE_EDGES, &dense_prim);

    printf("Validation\n");
    printf("  Sparse MST cost match : %d == %d\n",
           sparse_kruskal.total_weight, sparse_prim.total_weight);
    printf("  Dense MST cost match  : %d == %d\n",
           dense_kruskal.total_weight, dense_prim.total_weight);

    destroy_list_graph(&sparse_list);
    destroy_list_graph(&dense_list);
    return 0;
}
