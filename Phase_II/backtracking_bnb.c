#include "aviation_types.h"

/* Helper to check if placing a radar queen at board[row][col] is conflict-free */
static int is_queen_safe(int board[MAX_NODES][MAX_NODES], int row, int col, int n, Metrics *m) {
    for (int i = 0; i < row; i++) {
        m->comparisons++;
        if (board[i][col] == 1) {
            return 0;
        }
    }
    for (int i = row - 1, j = col - 1; i >= 0 && j >= 0; i--, j--) {
        m->comparisons++;
        if (board[i][j] == 1) {
            return 0;
        }
    }
    for (int i = row - 1, j = col + 1; i >= 0 && j < n; i--, j++) {
        m->comparisons++;
        if (board[i][j] == 1) {
            return 0;
        }
    }
    return 1;
}

/* Recursive helper for N-Queen backtracking */
static void solve_nq_util(int board[MAX_NODES][MAX_NODES], int row, int n, int *solutions, Metrics *m) {
    if (row == n) {
        (*solutions)++;
        return;
    }
    for (int col = 0; col < n; col++) {
        if (is_queen_safe(board, row, col, n, m)) {
            board[row][col] = 1;
            m->operations++;
            solve_nq_util(board, row + 1, n, solutions, m);
            board[row][col] = 0;
        }
    }
}

/* Algorithm: N-Queen Problem (Backtracking)
 * Role in Project: Places N airfield radar transponders on an NxN grid so no two interfere along rows, columns, or diagonals.
 */
int n_queens_backtrack(int n, Metrics *m) {
    if (n <= 0 || n > MAX_NODES) {
        return 0;
    }
    int board[MAX_NODES][MAX_NODES] = {{0}};
    int solutions = 0;
    solve_nq_util(board, 0, n, &solutions, m);
    return solutions;
}

/* Algorithm: Sum of Subset Problem (Backtracking)
 * Role in Project: Checks if a combination of reserve crew shift blocks adds up to an exact target flight duty window.
 */
int subset_sum_backtrack(const int hours[], int n, int target, Metrics *m) {
    m->comparisons++;
    if (target == 0) {
        return 1;
    }
    if (n <= 0 || target < 0) {
        return 0;
    }
    m->operations++;
    if (subset_sum_backtrack(hours, n - 1, target - hours[n - 1], m)) {
        return 1;
    }
    return subset_sum_backtrack(hours, n - 1, target, m);
}

/* Recursive helper for 0/1 Knapsack Backtracking */
static void knap_bt_util(const Flight arr[], int n, int idx, int curr_w, int curr_v, int cap, int *best_v, Metrics *m) {
    m->comparisons++;
    if (curr_v > *best_v) {
        *best_v = curr_v;
    }
    if (idx == n) {
        return;
    }
    if (curr_w + arr[idx].weight <= cap) {
        m->operations++;
        knap_bt_util(arr, n, idx + 1, curr_w + arr[idx].weight, curr_v + arr[idx].revenue, cap, best_v, m);
    }
    knap_bt_util(arr, n, idx + 1, curr_w, curr_v, cap, best_v, m);
}

/* Algorithm: 0/1 Knapsack Problem (Backtracking)
 * Role in Project: Explores feasible cargo subsets via state-space backtracking to find the maximum flight revenue.
 */
int knapsack_backtrack(const Flight arr[], int n, int capacity, Metrics *m) {
    if (n <= 0 || capacity <= 0) {
        return 0;
    }
    int best_val = 0;
    knap_bt_util(arr, n, 0, 0, 0, capacity, &best_val, m);
    return best_val;
}

/* Node structure for 0/1 Knapsack Branch and Bound */
typedef struct {
    int level;
    int weight;
    int profit;
    double bound;
} KnapNode;

/* Helper to sort cargo items by revenue-to-weight ratio descending for valid BnB fractional bounds */
static void sort_cargo_by_ratio(const Flight src[], Flight dest[], int n, Metrics *m) {
    for (int i = 0; i < n; i++) {
        dest[i] = src[i];
    }
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            m->comparisons++;
            double r1 = (double)dest[j].revenue / dest[j].weight;
            double r2 = (double)dest[j + 1].revenue / dest[j + 1].weight;
            if (r1 < r2) {
                Flight temp = dest[j];
                dest[j] = dest[j + 1];
                dest[j + 1] = temp;
                m->operations++;
            }
        }
    }
}

/* Helper to calculate optimistic upper bound for Knapsack Branch and Bound */
static double calc_knap_bound(const Flight arr[], int n, int capacity, KnapNode u) {
    if (u.weight >= capacity) {
        return 0.0;
    }
    double profit_bound = u.profit;
    int j = u.level;
    int tot_weight = u.weight;

    while (j < n && tot_weight + arr[j].weight <= capacity) {
        tot_weight += arr[j].weight;
        profit_bound += arr[j].revenue;
        j++;
    }
    if (j < n) {
        profit_bound += (capacity - tot_weight) * ((double)arr[j].revenue / arr[j].weight);
    }
    return profit_bound;
}

/* Algorithm: FIFO Branch and Bound (0/1 Knapsack Problem)
 * Role in Project: Solves 0/1 cargo selection using a First-In-First-Out queue to prune unpromising subsets.
 */
int bnb_fifo_knapsack(const Flight arr[], int n, int capacity, Metrics *m) {
    if (n <= 0 || capacity <= 0) {
        return 0;
    }
    Flight items[MAX_FLIGHTS];
    sort_cargo_by_ratio(arr, items, n, m);

    KnapNode queue[512];
    int head = 0;
    int tail = 0;
    int max_profit = 0;

    KnapNode root = {0, 0, 0, 0.0};
    root.bound = calc_knap_bound(items, n, capacity, root);
    queue[tail++] = root;

    while (head < tail) {
        KnapNode u = queue[head++];
        m->comparisons++;
        if (u.level < n && u.bound > max_profit) {
            KnapNode take = {u.level + 1, u.weight + items[u.level].weight, u.profit + items[u.level].revenue, 0.0};
            if (take.weight <= capacity && take.profit > max_profit) {
                max_profit = take.profit;
            }
            take.bound = calc_knap_bound(items, n, capacity, take);
            if (take.bound > max_profit && tail < 512) {
                queue[tail++] = take;
                m->operations++;
            }
            KnapNode skip = {u.level + 1, u.weight, u.profit, 0.0};
            skip.bound = calc_knap_bound(items, n, capacity, skip);
            if (skip.bound > max_profit && tail < 512) {
                queue[tail++] = skip;
                m->operations++;
            }
        }
    }
    return max_profit;
}

/* Algorithm: LIFO Branch and Bound (0/1 Knapsack Problem)
 * Role in Project: Solves 0/1 cargo selection using a Last-In-First-Out stack to prune unpromising subsets.
 */
int bnb_lifo_knapsack(const Flight arr[], int n, int capacity, Metrics *m) {
    if (n <= 0 || capacity <= 0) {
        return 0;
    }
    Flight items[MAX_FLIGHTS];
    sort_cargo_by_ratio(arr, items, n, m);

    KnapNode stack[512];
    int top = 0;
    int max_profit = 0;

    KnapNode root = {0, 0, 0, 0.0};
    root.bound = calc_knap_bound(items, n, capacity, root);
    stack[top++] = root;

    while (top > 0) {
        KnapNode u = stack[--top];
        m->comparisons++;
        if (u.level < n && u.bound > max_profit) {
            KnapNode take = {u.level + 1, u.weight + items[u.level].weight, u.profit + items[u.level].revenue, 0.0};
            if (take.weight <= capacity && take.profit > max_profit) {
                max_profit = take.profit;
            }
            take.bound = calc_knap_bound(items, n, capacity, take);
            if (take.bound > max_profit && top < 512) {
                stack[top++] = take;
                m->operations++;
            }
            KnapNode skip = {u.level + 1, u.weight, u.profit, 0.0};
            skip.bound = calc_knap_bound(items, n, capacity, skip);
            if (skip.bound > max_profit && top < 512) {
                stack[top++] = skip;
                m->operations++;
            }
        }
    }
    return max_profit;
}

/* Algorithm: Least-Cost (LC) Branch and Bound (0/1 Knapsack Problem)
 * Role in Project: Solves 0/1 cargo selection by always expanding the node with the highest profit bound first.
 */
int bnb_lc_knapsack(const Flight arr[], int n, int capacity, Metrics *m) {
    if (n <= 0 || capacity <= 0) {
        return 0;
    }
    Flight items[MAX_FLIGHTS];
    sort_cargo_by_ratio(arr, items, n, m);

    KnapNode pq[512];
    int sz = 0;
    int max_profit = 0;

    KnapNode root = {0, 0, 0, 0.0};
    root.bound = calc_knap_bound(items, n, capacity, root);
    pq[sz++] = root;

    while (sz > 0) {
        int best_i = 0;
        for (int i = 1; i < sz; i++) {
            if (pq[i].bound > pq[best_i].bound) {
                best_i = i;
            }
        }
        KnapNode u = pq[best_i];
        pq[best_i] = pq[--sz];
        m->comparisons++;

        if (u.level < n && u.bound > max_profit) {
            KnapNode take = {u.level + 1, u.weight + items[u.level].weight, u.profit + items[u.level].revenue, 0.0};
            if (take.weight <= capacity && take.profit > max_profit) {
                max_profit = take.profit;
            }
            take.bound = calc_knap_bound(items, n, capacity, take);
            if (take.bound > max_profit && sz < 512) {
                pq[sz++] = take;
                m->operations++;
            }
            KnapNode skip = {u.level + 1, u.weight, u.profit, 0.0};
            skip.bound = calc_knap_bound(items, n, capacity, skip);
            if (skip.bound > max_profit && sz < 512) {
                pq[sz++] = skip;
                m->operations++;
            }
        }
    }
    return max_profit;
}

/* Recursive helper for Travelling Salesperson Branch and Bound */
static void tsp_bnb_util(int n, int cost[MAX_NODES][MAX_NODES], int curr_pos, int count, int curr_cost, int visited[], int *best_cost, Metrics *m) {
    m->comparisons++;
    if (count == n && cost[curr_pos][0] < INF) {
        if (curr_cost + cost[curr_pos][0] < *best_cost) {
            *best_cost = curr_cost + cost[curr_pos][0];
        }
        return;
    }
    for (int next = 0; next < n; next++) {
        if (!visited[next] && cost[curr_pos][next] < INF) {
            int next_cost = curr_cost + cost[curr_pos][next];
            if (next_cost < *best_cost) {
                visited[next] = 1;
                m->operations++;
                tsp_bnb_util(n, cost, next, count + 1, next_cost, visited, best_cost, m);
                visited[next] = 0;
            }
        }
    }
}

/* Algorithm: Travelling Salesperson Problem (Branch and Bound)
 * Role in Project: Finds the minimum-cost aircraft ferry tour across airports by pruning expensive partial routes.
 */
int bnb_tsp_tour(int n, int cost[MAX_NODES][MAX_NODES], Metrics *m) {
    if (n <= 0 || n > MAX_NODES) {
        return 0;
    }
    int visited[MAX_NODES] = {0};
    int best_cost = INF;
    visited[0] = 1;
    tsp_bnb_util(n, cost, 0, 1, 0, visited, &best_cost, m);
    return best_cost;
}