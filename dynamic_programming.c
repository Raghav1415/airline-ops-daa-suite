#include "aviation_types.h"

/* Algorithm: 0/1 Knapsack Problem (Dynamic Programming)
 * Role in Project: Selects indivisible high-priority cargo containers to maximize flight revenue under a weight limit.
 */
int dp_01_knapsack(const Flight arr[], int n, int capacity, Metrics *m) {
    int dp[256] = {0};
    if (capacity > 255) {
        capacity = 255;
    }
    for (int i = 0; i < n; i++) {
        for (int w = capacity; w >= arr[i].weight; w--) {
            m->comparisons++;
            int take = dp[w - arr[i].weight] + arr[i].revenue;
            if (take > dp[w]) {
                dp[w] = take;
                m->operations++;
            }
        }
    }
    return dp[capacity];
}

/* Algorithm: Matrix Chain Multiplication Problem
 * Role in Project: Finds the parenthesization of flight transition matrices that minimizes scalar multiplications.
 */
int matrix_chain_order(const int dims[], int n, Metrics *m) {
    int dp[MAX_NODES][MAX_NODES] = {{0}};

    for (int len = 2; len < n; len++) {
        for (int i = 1; i < n - len + 1; i++) {
            int j = i + len - 1;
            dp[i][j] = INF;
            for (int k = i; k < j; k++) {
                m->comparisons++;
                int cost = dp[i][k] + dp[k + 1][j] + dims[i - 1] * dims[k] * dims[j];
                if (cost < dp[i][j]) {
                    dp[i][j] = cost;
                    m->operations++;
                }
            }
        }
    }
    return dp[1][n - 1];
}

/* Algorithm: Longest Common Subsequence (LCS) Problem
 * Role in Project: Measures route adherence by finding the longest waypoint sequence shared by planned and actual logs.
 */
int longest_common_subsequence(const char X[], const char Y[], Metrics *m) {
    int r = (int)strlen(X);
    int c = (int)strlen(Y);
    int dp[64][64] = {{0}};

    for (int i = 1; i <= r; i++) {
        for (int j = 1; j <= c; j++) {
            m->comparisons++;
            if (X[i - 1] == Y[j - 1]) {
                dp[i][j] = dp[i - 1][j - 1] + 1;
            } else if (dp[i - 1][j] >= dp[i][j - 1]) {
                dp[i][j] = dp[i - 1][j];
            } else {
                dp[i][j] = dp[i][j - 1];
            }
            m->operations++;
        }
    }
    return dp[r][c];
}

/* Algorithm: Optimal Binary Search Tree (OBST) Problem
 * Role in Project: Organizes flight code lookup keys into a tree that minimizes expected dispatch search operations.
 */
int optimal_bst_cost(const int freq[], int n, Metrics *m) {
    int cost[MAX_NODES][MAX_NODES] = {{0}};
    for (int i = 0; i < n; i++) {
        cost[i][i] = freq[i];
    }
    for (int L = 2; L <= n; L++) {
        for (int i = 0; i <= n - L; i++) {
            int j = i + L - 1;
            cost[i][j] = INF;
            int freq_sum = 0;
            for (int s = i; s <= j; s++) {
                freq_sum += freq[s];
            }
            for (int r = i; r <= j; r++) {
                m->comparisons++;
                int left_cost = (r > i) ? cost[i][r - 1] : 0;
                int right_cost = (r < j) ? cost[r + 1][j] : 0;
                int total = left_cost + right_cost + freq_sum;
                if (total < cost[i][j]) {
                    cost[i][j] = total;
                    m->operations++;
                }
            }
        }
    }
    return cost[0][n - 1];
}

/* Algorithm: Bellman-Ford Algorithm for Single Source Shortest Path
 * Role in Project: Finds shortest flight routes when tailwind fuel rebates create negative edge weights.
 */
int bellman_ford_shortest_path(int n, int edge_count, const Edge edges[], int src, int dist[], Metrics *m) {
    for (int i = 0; i < n; i++) {
        dist[i] = INF;
    }
    dist[src] = 0;

    for (int step = 1; step <= n - 1; step++) {
        for (int j = 0; j < edge_count; j++) {
            int u = edges[j].u;
            int v = edges[j].v;
            int w = edges[j].weight;
            m->comparisons++;
            if (dist[u] != INF && dist[u] + w < dist[v]) {
                dist[v] = dist[u] + w;
                m->operations++;
            }
        }
    }
    /* Check for negative-weight cycles */
    for (int j = 0; j < edge_count; j++) {
        int u = edges[j].u;
        int v = edges[j].v;
        int w = edges[j].weight;
        if (dist[u] != INF && dist[u] + w < dist[v]) {
            return -1;
        }
    }
    return 0;
}

/* Algorithm: Travelling Salesman Problem (Dynamic Programming)
 * Role in Project: Computes the minimum-cost tour for a maintenance crew to inspect all regional airports and return.
 */
int tsp_dp_tour(int n, int cost[MAX_NODES][MAX_NODES], Metrics *m) {
    int total_masks = (1 << n);
    int dp[64][MAX_NODES];

    for (int mask = 0; mask < total_masks; mask++) {
        for (int i = 0; i < n; i++) {
            dp[mask][i] = INF;
        }
    }
    dp[1][0] = 0;

    for (int mask = 1; mask < total_masks; mask++) {
        for (int u = 0; u < n; u++) {
            if ((mask & (1 << u)) && dp[mask][u] < INF) {
                for (int v = 0; v < n; v++) {
                    if (!(mask & (1 << v)) && cost[u][v] < INF) {
                        int next_mask = mask | (1 << v);
                        m->comparisons++;
                        if (dp[mask][u] + cost[u][v] < dp[next_mask][v]) {
                            dp[next_mask][v] = dp[mask][u] + cost[u][v];
                            m->operations++;
                        }
                    }
                }
            }
        }
    }
    int best_tour = INF;
    for (int u = 1; u < n; u++) {
        if (dp[total_masks - 1][u] + cost[u][0] < best_tour) {
            best_tour = dp[total_masks - 1][u] + cost[u][0];
        }
    }
    return best_tour;
}

/* Algorithm: Floyd-Warshall Algorithm for All-Pairs Shortest Path
 * Role in Project: Precomputes the shortest transit time between every pair of airports in the network.
 */
void floyd_warshall_all_pairs(int n, int dist[MAX_NODES][MAX_NODES], Metrics *m) {
    for (int k = 0; k < n; k++) {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                m->comparisons++;
                if (dist[i][k] < INF && dist[k][j] < INF && dist[i][k] + dist[k][j] < dist[i][j]) {
                    dist[i][j] = dist[i][k] + dist[k][j];
                    m->operations++;
                }
            }
        }
    }
}