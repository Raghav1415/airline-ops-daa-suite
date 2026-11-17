#include "aviation_types.h"

/* Algorithm: Greedy Knapsack Problem (Fractional Knapsack)
 * Role in Project: Loads divisible aviation fuel and bulk cargo by highest revenue-to-weight ratio first.
 */
double greedy_knapsack_cargo(const Flight arr[], int n, int capacity, Metrics *m) {
    Flight items[MAX_FLIGHTS];
    for (int i = 0; i < n; i++) {
        items[i] = arr[i];
    }
    /* Sort items by revenue-to-weight ratio in descending order */
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            m->comparisons++;
            double r1 = (double)items[j].revenue / items[j].weight;
            double r2 = (double)items[j + 1].revenue / items[j + 1].weight;
            if (r1 < r2) {
                Flight temp = items[j];
                items[j] = items[j + 1];
                items[j + 1] = temp;
                m->operations++;
            }
        }
    }
    double total_value = 0.0;
    int remaining = capacity;
    for (int i = 0; i < n && remaining > 0; i++) {
        if (items[i].weight <= remaining) {
            total_value += items[i].revenue;
            remaining -= items[i].weight;
        } else {
            total_value += items[i].revenue * ((double)remaining / items[i].weight);
            remaining = 0;
        }
        m->operations++;
    }
    return total_value;
}

/* Algorithm: Activity Selection Problem
 * Role in Project: Selects the maximum number of non-overlapping flights that can use a single runway.
 */
int activity_selection_runway(const Flight sorted_arr[], int n, int selected_ids[], Metrics *m) {
    if (n <= 0) {
        return 0;
    }
    int count = 1;
    selected_ids[0] = sorted_arr[0].id;
    int last_finish = sorted_arr[0].arr_time;

    for (int i = 1; i < n; i++) {
        m->comparisons++;
        if (sorted_arr[i].dep_time >= last_finish) {
            selected_ids[count] = sorted_arr[i].id;
            count++;
            last_finish = sorted_arr[i].arr_time;
            m->operations++;
        }
    }
    return count;
}

/* Algorithm: Huffman Encoding
 * Role in Project: Merges least-frequent flight telemetry symbols greedily to minimize total transmission bits.
 */
int huffman_encoding_cost(const int freq[], int n, Metrics *m) {
    int nodes[MAX_NODES * 2];
    int sz = 0;
    for (int i = 0; i < n; i++) {
        if (freq[i] > 0) {
            nodes[sz++] = freq[i];
        }
    }
    int total_bits = 0;
    while (sz > 1) {
        for (int i = 0; i < sz - 1; i++) {
            for (int j = i + 1; j < sz; j++) {
                m->comparisons++;
                if (nodes[i] > nodes[j]) {
                    int temp = nodes[i];
                    nodes[i] = nodes[j];
                    nodes[j] = temp;
                }
            }
        }
        int combined = nodes[0] + nodes[1];
        total_bits += combined;
        nodes[0] = combined;
        nodes[1] = nodes[sz - 1];
        sz--;
        m->operations++;
    }
    return total_bits;
}

/* Algorithm: Interval Partitioning Problem
 * Role in Project: Assigns every flight to an airport gate using the minimum total number of gates.
 */
int interval_partitioning_gates(Flight arr[], int n, Metrics *m) {
    int gate_free_time[MAX_FLIGHTS];
    int gate_count = 0;

    for (int i = 0; i < n; i++) {
        int chosen_gate = -1;
        for (int g = 0; g < gate_count; g++) {
            m->comparisons++;
            if (gate_free_time[g] <= arr[i].dep_time) {
                chosen_gate = g;
                break;
            }
        }
        if (chosen_gate == -1) {
            chosen_gate = gate_count;
            gate_count++;
        }
        gate_free_time[chosen_gate] = arr[i].arr_time;
        arr[i].assigned_gate = chosen_gate;
        m->operations++;
    }
    return gate_count;
}

/* Algorithm: Dijkstra's Algorithm for Single Source Shortest Path
 * Role in Project: Computes the minimum-fuel flight path from a source airport to all other airports.
 */
void dijkstra_shortest_path(int n, int graph[MAX_NODES][MAX_NODES], int src, int dist[], Metrics *m) {
    int visited[MAX_NODES] = {0};
    for (int i = 0; i < n; i++) {
        dist[i] = INF;
    }
    dist[src] = 0;

    for (int step = 0; step < n - 1; step++) {
        int u = -1;
        int min_val = INF;
        for (int v = 0; v < n; v++) {
            m->comparisons++;
            if (!visited[v] && dist[v] < min_val) {
                min_val = dist[v];
                u = v;
            }
        }
        if (u == -1) {
            break;
        }
        visited[u] = 1;
        for (int v = 0; v < n; v++) {
            m->comparisons++;
            if (!visited[v] && graph[u][v] > 0 && dist[u] + graph[u][v] < dist[v]) {
                dist[v] = dist[u] + graph[u][v];
                m->operations++;
            }
        }
    }
}

/* Algorithm: Prim's Algorithm for Minimum Cost Spanning Tree
 * Role in Project: Grows a minimum-cost ground communication network tree starting from terminal 0.
 */
int prim_mst_cost(int n, int graph[MAX_NODES][MAX_NODES], Metrics *m) {
    int key[MAX_NODES];
    int in_mst[MAX_NODES] = {0};
    for (int i = 0; i < n; i++) {
        key[i] = INF;
    }
    key[0] = 0;
    int total_cost = 0;

    for (int step = 0; step < n; step++) {
        int u = -1;
        int min_val = INF;
        for (int v = 0; v < n; v++) {
            m->comparisons++;
            if (!in_mst[v] && key[v] < min_val) {
                min_val = key[v];
                u = v;
            }
        }
        if (u == -1) {
            break;
        }
        in_mst[u] = 1;
        total_cost += key[u];
        for (int v = 0; v < n; v++) {
            m->comparisons++;
            if (graph[u][v] > 0 && !in_mst[v] && graph[u][v] < key[v]) {
                key[v] = graph[u][v];
                m->operations++;
            }
        }
    }
    return total_cost;
}

/* Helper to find the connected component root of a vertex */
static int find_set_root(int parent[], int i) {
    while (parent[i] != i) {
        i = parent[i];
    }
    return i;
}

/* Algorithm: Kruskal's Algorithm for Minimum Cost Spanning Tree
 * Role in Project: Builds a minimum-cost airport network by adding the cheapest non-cycling links first.
 */
int kruskal_mst_cost(int n, int edge_count, Edge edges[], Metrics *m) {
    for (int i = 0; i < edge_count - 1; i++) {
        for (int j = 0; j < edge_count - i - 1; j++) {
            m->comparisons++;
            if (edges[j].weight > edges[j + 1].weight) {
                Edge temp = edges[j];
                edges[j] = edges[j + 1];
                edges[j + 1] = temp;
                m->operations++;
            }
        }
    }
    int parent[MAX_NODES];
    for (int i = 0; i < n; i++) {
        parent[i] = i;
    }
    int mst_cost = 0;
    int edges_taken = 0;
    for (int i = 0; i < edge_count && edges_taken < n - 1; i++) {
        int root_u = find_set_root(parent, edges[i].u);
        int root_v = find_set_root(parent, edges[i].v);
        m->comparisons++;
        if (root_u != root_v) {
            parent[root_u] = root_v;
            mst_cost += edges[i].weight;
            edges_taken++;
            m->operations++;
        }
    }
    return mst_cost;
}