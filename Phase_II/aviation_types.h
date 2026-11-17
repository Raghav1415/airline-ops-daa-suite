#ifndef AVIATION_TYPES_H
#define AVIATION_TYPES_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <time.h>

#define MAX_NODES 10
#define MAX_FLIGHTS 512
#define INF 999999

/* Structure representing a single flight record */
typedef struct {
    int id;
    int dep_time;
    int arr_time;
    int weight;
    int revenue;
    int assigned_gate;
} Flight;

/* Structure representing a weighted graph edge */
typedef struct {
    int u;
    int v;
    int weight;
} Edge;

/* Structure to record empirical comparisons and operations */
typedef struct {
    long comparisons;
    long operations;
} Metrics;

/* Row 1: Sorting, Searching & Divide and Conquer */
int binary_search_flight(const Flight arr[], int low, int high, int target_dep, Metrics *m);
void merge_sort_flights(Flight arr[], int left, int right, Metrics *m);
void quick_sort_flights(Flight arr[], int low, int high, Metrics *m);
void strassen_multiply_2x2(int A[2][2], int B[2][2], int C[2][2], Metrics *m);
int max_subarray_sum(const int delays[], int low, int high, Metrics *m);
long long power_number(long long base, int exp, Metrics *m);
int find_celebrity_airport(int adj[MAX_NODES][MAX_NODES], int n, Metrics *m);

/* Row 2: Greedy Approach */
double greedy_knapsack_cargo(const Flight arr[], int n, int capacity, Metrics *m);
int activity_selection_runway(const Flight sorted_arr[], int n, int selected_ids[], Metrics *m);
int huffman_encoding_cost(const int freq[], int n, Metrics *m);
int interval_partitioning_gates(Flight arr[], int n, Metrics *m);
void dijkstra_shortest_path(int n, int graph[MAX_NODES][MAX_NODES], int src, int dist[], Metrics *m);
int prim_mst_cost(int n, int graph[MAX_NODES][MAX_NODES], Metrics *m);
int kruskal_mst_cost(int n, int edge_count, Edge edges[], Metrics *m);

/* Row 3: Dynamic Programming */
int dp_01_knapsack(const Flight arr[], int n, int capacity, Metrics *m);
int matrix_chain_order(const int dims[], int n, Metrics *m);
int longest_common_subsequence(const char X[], const char Y[], Metrics *m);
int optimal_bst_cost(const int freq[], int n, Metrics *m);
int bellman_ford_shortest_path(int n, int edge_count, const Edge edges[], int src, int dist[], Metrics *m);
int tsp_dp_tour(int n, int cost[MAX_NODES][MAX_NODES], Metrics *m);
void floyd_warshall_all_pairs(int n, int dist[MAX_NODES][MAX_NODES], Metrics *m);

/* Row 4: Backtracking & Branch and Bound */
int n_queens_backtrack(int n, Metrics *m);
int subset_sum_backtrack(const int hours[], int n, int target, Metrics *m);
int knapsack_backtrack(const Flight arr[], int n, int capacity, Metrics *m);
int bnb_fifo_knapsack(const Flight arr[], int n, int capacity, Metrics *m);
int bnb_lifo_knapsack(const Flight arr[], int n, int capacity, Metrics *m);
int bnb_lc_knapsack(const Flight arr[], int n, int capacity, Metrics *m);
int bnb_tsp_tour(int n, int cost[MAX_NODES][MAX_NODES], Metrics *m);

/* Automated Test Suite Runner */
int run_all_unit_tests(void);

#endif