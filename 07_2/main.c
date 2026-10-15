
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define V 100
#define SPARSE_E 100
#define DENSE_E 4000
#define MAX_E (V * (V - 1) / 2)

typedef struct {
    int u, v;
} Edge;

typedef struct {
    int n;
    int e;
    int a[V][V];
} MatrixGraph;

typedef struct Node {
    int v;
    struct Node *next;
} Node;

typedef struct {
    int n;
    int e;
    Node *head[V];
} ListGraph;

typedef struct {
    size_t matrix_mem;
    size_t list_mem;

    int insert_u, insert_v;
    int delete_u, delete_v;
    int connect_u, connect_v;
    int adjacent_vertex;

    int matrix_insert_cmp;
    int list_insert_cmp;
    int matrix_delete_cmp;
    int list_delete_cmp;
    int matrix_connect_cmp;
    int list_connect_cmp;
    int matrix_adj_cmp;
    int list_adj_cmp;

    int connected;
    int neighbors[V];
    int neighbor_count;
} Result;

static void swap_edge(Edge *a, Edge *b)
{
    Edge t = *a;
    *a = *b;
    *b = t;
}

static int cmp_int(const void *a, const void *b)
{
    int x = *(const int *)a;
    int y = *(const int *)b;
    return (x > y) - (x < y);
}

static void matrix_init(MatrixGraph *g)
{
    g->n = V;
    g->e = 0;
    memset(g->a, 0, sizeof(g->a));
}

static void list_init(ListGraph *g)
{
    g->n = V;
    g->e = 0;
    memset(g->head, 0, sizeof(g->head));
}

static Node *new_node(int v)
{
    Node *p = (Node *)malloc(sizeof(Node));
    if (p == NULL) return NULL;

    p->v = v;
    p->next = NULL;
    return p;
}

static int list_add_raw(ListGraph *g, int u, int v)
{
    Node *a = new_node(v);
    Node *b = new_node(u);

    if (a == NULL || b == NULL) {
        free(a);
        free(b);
        return 0;
    }

    a->next = g->head[u];
    g->head[u] = a;

    b->next = g->head[v];
    g->head[v] = b;

    g->e++;
    return 1;
}

static void matrix_add_raw(MatrixGraph *g, int u, int v)
{
    if (!g->a[u][v]) {
        g->a[u][v] = 1;
        g->a[v][u] = 1;
        g->e++;
    }
}

static int generate_edges(int edge_count, Edge *edges, Edge *non_edge)
{
    Edge *all = (Edge *)malloc(sizeof(Edge) * MAX_E);
    int count = 0;
    int i, j;

    if (all == NULL) return 0;

    for (i = 0; i < V; i++) {
        for (j = i + 1; j < V; j++) {
            all[count].u = i;
            all[count].v = j;
            count++;
        }
    }

    for (i = MAX_E - 1; i > 0; i--) {
        j = rand() % (i + 1);
        swap_edge(&all[i], &all[j]);
    }

    for (i = 0; i < edge_count; i++) {
        edges[i] = all[i];
    }

    *non_edge = all[edge_count];

    free(all);
    return 1;
}

static int build_graphs(
    MatrixGraph *mg,
    ListGraph *lg,
    const Edge *edges,
    int edge_count
)
{
    int i;

    matrix_init(mg);
    list_init(lg);

    for (i = 0; i < edge_count; i++) {
        matrix_add_raw(mg, edges[i].u, edges[i].v);

        if (!list_add_raw(lg, edges[i].u, edges[i].v)) {
            return 0;
        }
    }

    return 1;
}

static void list_destroy(ListGraph *g)
{
    int i;

    for (i = 0; i < V; i++) {
        Node *p = g->head[i];

        while (p != NULL) {
            Node *next = p->next;
            free(p);
            p = next;
        }

        g->head[i] = NULL;
    }

    g->e = 0;
}

static size_t matrix_memory(void)
{
    return sizeof(MatrixGraph);
}

static size_t list_memory(const ListGraph *g)
{
    return sizeof(ListGraph) + (size_t)(2 * g->e) * sizeof(Node);
}

static int matrix_insert(MatrixGraph *g, int u, int v, int *cmp)
{
    *cmp = 1;

    if (g->a[u][v]) return 0;

    g->a[u][v] = 1;
    g->a[v][u] = 1;
    g->e++;

    return 1;
}

static int list_insert(ListGraph *g, int u, int v, int *cmp)
{
    Node *p;
    Node *a;
    Node *b;

    *cmp = 0;

    for (p = g->head[u]; p != NULL; p = p->next) {
        (*cmp)++;
        if (p->v == v) return 0;
    }

    a = new_node(v);
    b = new_node(u);

    if (a == NULL || b == NULL) {
        free(a);
        free(b);
        return 0;
    }

    a->next = g->head[u];
    g->head[u] = a;

    b->next = g->head[v];
    g->head[v] = b;

    g->e++;

    return 1;
}

static int remove_one(Node **head, int target, int *cmp)
{
    Node *p = *head;
    Node *prev = NULL;

    while (p != NULL) {
        (*cmp)++;

        if (p->v == target) {
            if (prev == NULL) {
                *head = p->next;
            } else {
                prev->next = p->next;
            }

            free(p);
            return 1;
        }

        prev = p;
        p = p->next;
    }

    return 0;
}

static int matrix_delete(MatrixGraph *g, int u, int v, int *cmp)
{
    *cmp = 1;

    if (!g->a[u][v]) return 0;

    g->a[u][v] = 0;
    g->a[v][u] = 0;
    g->e--;

    return 1;
}

static int list_delete(ListGraph *g, int u, int v, int *cmp)
{
    *cmp = 0;

    if (!remove_one(&g->head[u], v, cmp)) {
        return 0;
    }

    if (!remove_one(&g->head[v], u, cmp)) {
        return 0;
    }

    g->e--;
    return 1;
}

static int matrix_connected(
    const MatrixGraph *g,
    int u,
    int v,
    int *cmp
)
{
    *cmp = 1;
    return g->a[u][v] != 0;
}

static int list_connected(
    const ListGraph *g,
    int u,
    int v,
    int *cmp
)
{
    Node *p;

    *cmp = 0;

    for (p = g->head[u]; p != NULL; p = p->next) {
        (*cmp)++;

        if (p->v == v) {
            return 1;
        }
    }

    return 0;
}

static int matrix_neighbors(
    const MatrixGraph *g,
    int u,
    int out[],
    int *cmp
)
{
    int i;
    int count = 0;

    *cmp = 0;

    for (i = 0; i < V; i++) {
        (*cmp)++;

        if (g->a[u][i]) {
            out[count++] = i;
        }
    }

    return count;
}

static int list_neighbors(
    const ListGraph *g,
    int u,
    int out[],
    int *cmp
)
{
    Node *p;
    int count = 0;

    *cmp = 0;

    for (p = g->head[u]; p != NULL; p = p->next) {
        (*cmp)++;
        out[count++] = p->v;
    }

    return count;
}

static int run_case(int edge_count, Result *r)
{
    Edge *edges = (Edge *)malloc(sizeof(Edge) * edge_count);
    Edge non_edge;
    MatrixGraph mg;
    ListGraph lg;

    int mn[V], ln[V];
    int mc, lc;
    int tmp;
    int mconn, lconn;
    int i;

    if (edges == NULL) return 0;

    if (!generate_edges(edge_count, edges, &non_edge)) {
        free(edges);
        return 0;
    }

    if (!build_graphs(&mg, &lg, edges, edge_count)) {
        free(edges);
        list_destroy(&lg);
        return 0;
    }

    r->matrix_mem = matrix_memory();
    r->list_mem = list_memory(&lg);

    do {
        r->connect_u = rand() % V;
        r->connect_v = rand() % V;
    } while (r->connect_u == r->connect_v);

    mconn = matrix_connected(
        &mg,
        r->connect_u,
        r->connect_v,
        &r->matrix_connect_cmp
    );

    lconn = list_connected(
        &lg,
        r->connect_u,
        r->connect_v,
        &r->list_connect_cmp
    );

    if (mconn != lconn) {
        free(edges);
        list_destroy(&lg);
        return 0;
    }

    r->connected = mconn;

    r->adjacent_vertex = rand() % V;

    mc = matrix_neighbors(
        &mg,
        r->adjacent_vertex,
        mn,
        &r->matrix_adj_cmp
    );

    lc = list_neighbors(
        &lg,
        r->adjacent_vertex,
        ln,
        &r->list_adj_cmp
    );

    if (mc != lc) {
        free(edges);
        list_destroy(&lg);
        return 0;
    }

    qsort(mn, mc, sizeof(int), cmp_int);
    qsort(ln, lc, sizeof(int), cmp_int);

    for (i = 0; i < mc; i++) {
        if (mn[i] != ln[i]) {
            free(edges);
            list_destroy(&lg);
            return 0;
        }

        r->neighbors[i] = mn[i];
    }

    r->neighbor_count = mc;

    r->insert_u = non_edge.u;
    r->insert_v = non_edge.v;

    if (!matrix_insert(
        &mg,
        r->insert_u,
        r->insert_v,
        &r->matrix_insert_cmp))
    {
        free(edges);
        list_destroy(&lg);
        return 0;
    }

    if (!list_insert(
        &lg,
        r->insert_u,
        r->insert_v,
        &r->list_insert_cmp))
    {
        free(edges);
        list_destroy(&lg);
        return 0;
    }

    /* 삽입 측정 뒤 원상 복구 */
    matrix_delete(&mg, r->insert_u, r->insert_v, &tmp);
    list_delete(&lg, r->insert_u, r->insert_v, &tmp);

    {
        Edge d = edges[rand() % edge_count];
        r->delete_u = d.u;
        r->delete_v = d.v;
    }

    if (!matrix_delete(
        &mg,
        r->delete_u,
        r->delete_v,
        &r->matrix_delete_cmp))
    {
        free(edges);
        list_destroy(&lg);
        return 0;
    }

    if (!list_delete(
        &lg,
        r->delete_u,
        r->delete_v,
        &r->list_delete_cmp))
    {
        free(edges);
        list_destroy(&lg);
        return 0;
    }

    free(edges);
    list_destroy(&lg);

    return 1;
}

static void print_neighbors(const Result *r)
{
    int i;

    if (r->neighbor_count == 0) {
        printf("(none)");
        return;
    }

    for (i = 0; i < r->neighbor_count; i++) {
        if (i > 0) printf(", ");
        printf("%d", r->neighbors[i]);
    }
}

static void print_case(
    int case_no,
    const char *graph_name,
    const char *repr,
    const Result *r,
    int matrix
)
{
    printf("Case %d : %s - %s\n", case_no, graph_name, repr);

    printf(
        "Memory                         : %zu Bytes\n",
        matrix ? r->matrix_mem : r->list_mem
    );

    printf(
        "Edge insertion pair            : (%d, %d)\n",
        r->insert_u,
        r->insert_v
    );

    printf(
        "Edge insertion comparisons     : %d\n",
        matrix ? r->matrix_insert_cmp : r->list_insert_cmp
    );

    printf(
        "Edge deletion pair             : (%d, %d)\n",
        r->delete_u,
        r->delete_v
    );

    printf(
        "Edge deletion comparisons      : %d\n",
        matrix ? r->matrix_delete_cmp : r->list_delete_cmp
    );

    printf(
        "Connectivity pair              : (%d, %d)\n",
        r->connect_u,
        r->connect_v
    );

    printf(
        "Connectivity result            : %s\n",
        r->connected ? "Connected" : "Not Connected"
    );

    printf(
        "Connectivity check comparisons : %d\n",
        matrix ? r->matrix_connect_cmp : r->list_connect_cmp
    );

    printf("Adjacent vertex                : %d\n", r->adjacent_vertex);

    printf("Adjacent vertices              : ");
    print_neighbors(r);
    printf("\n");

    printf(
        "Adjacent vertices comparisons  : %d\n\n",
        matrix ? r->matrix_adj_cmp : r->list_adj_cmp
    );
}

int main(int argc, char *argv[])
{
    unsigned int seed;
    Result sparse;
    Result dense;

    if (argc >= 2) {
        seed = (unsigned int)strtoul(argv[1], NULL, 10);
    } else {
        seed = (unsigned int)time(NULL);
    }

    srand(seed);

    if (!run_case(SPARSE_E, &sparse)) {
        fprintf(stderr, "Error: sparse graph experiment failed.\n");
        return 1;
    }

    if (!run_case(DENSE_E, &dense)) {
        fprintf(stderr, "Error: dense graph experiment failed.\n");
        return 1;
    }

    printf("Random seed: %u\n\n", seed);

    printf("Sparse Graph\n");
    printf("Vertices : %d\n", V);
    printf("Edges    : %d\n\n", SPARSE_E);

    printf("Dense Graph\n");
    printf("Vertices : %d\n", V);
    printf("Edges    : %d\n\n", DENSE_E);

    print_case(
        1,
        "Sparse Graph",
        "Adjacency Matrix",
        &sparse,
        1
    );

    print_case(
        2,
        "Sparse Graph",
        "Adjacency List",
        &sparse,
        0
    );

    print_case(
        3,
        "Dense Graph",
        "Adjacency Matrix",
        &dense,
        1
    );

    print_case(
        4,
        "Dense Graph",
        "Adjacency List",
        &dense,
        0
    );

    return 0;
}
