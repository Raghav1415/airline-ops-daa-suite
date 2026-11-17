#include "aviation_types.h"

/* Algorithm: Strassen Method of Matrix Multiplication
 * Role in Project: Multiplies two 2x2 airport sector transition matrices using 7 multiplications instead of 8.
 */
void strassen_multiply_2x2(int A[2][2], int B[2][2], int C[2][2], Metrics *m) {
    int p1 = (A[0][0] + A[1][1]) * (B[0][0] + B[1][1]);
    int p2 = (A[1][0] + A[1][1]) * B[0][0];
    int p3 = A[0][0] * (B[0][1] - B[1][1]);
    int p4 = A[1][1] * (B[1][0] - B[0][0]);
    int p5 = (A[0][0] + A[0][1]) * B[1][1];
    int p6 = (A[1][0] - A[0][0]) * (B[0][0] + B[0][1]);
    int p7 = (A[0][1] - A[1][1]) * (B[1][0] + B[1][1]);

    C[0][0] = p1 + p4 - p5 + p7;
    C[0][1] = p3 + p5;
    C[1][0] = p2 + p4;
    C[1][1] = p1 - p2 + p3 + p6;
    m->operations += 7;
}

/* Helper to find the maximum delay sum crossing the midpoint */
static int max_crossing_sum(const int delays[], int low, int mid, int high, Metrics *m) {
    int sum = 0;
    int left_sum = INT_MIN;
    for (int i = mid; i >= low; i--) {
        sum += delays[i];
        m->operations++;
        if (sum > left_sum) {
            left_sum = sum;
        }
    }

    sum = 0;
    int right_sum = INT_MIN;
    for (int i = mid + 1; i <= high; i++) {
        sum += delays[i];
        m->operations++;
        if (sum > right_sum) {
            right_sum = sum;
        }
    }
    return left_sum + right_sum;
}

/* Algorithm: Maximum Subarray Problem (Divide and Conquer)
 * Role in Project: Finds the contiguous time window with the highest accumulated flight delay.
 */
int max_subarray_sum(const int delays[], int low, int high, Metrics *m) {
    m->comparisons++;
    if (low == high) {
        return delays[low];
    }
    int mid = low + (high - low) / 2;
    int left_max = max_subarray_sum(delays, low, mid, m);
    int right_max = max_subarray_sum(delays, mid + 1, high, m);
    int cross_max = max_crossing_sum(delays, low, mid, high, m);

    int best = left_max;
    if (right_max > best) {
        best = right_max;
    }
    if (cross_max > best) {
        best = cross_max;
    }
    return best;
}

/* Algorithm: Powering a Number (Divide and Conquer)
 * Role in Project: Computes base^exp in O(log exp) steps to model compounding delay growth across flight legs.
 */
long long power_number(long long base, int exp, Metrics *m) {
    m->comparisons++;
    if (exp == 0) {
        return 1;
    }
    long long half = power_number(base, exp / 2, m);
    m->operations++;
    if (exp % 2 == 0) {
        return half * half;
    } else {
        m->operations++;
        return base * half * half;
    }
}

/* Algorithm: Celebrity Problem
 * Role in Project: Identifies a central sink hub airport that receives flights from all other airports but dispatches none.
 */
int find_celebrity_airport(int adj[MAX_NODES][MAX_NODES], int n, Metrics *m) {
    int cand = 0;
    for (int i = 1; i < n; i++) {
        m->comparisons++;
        if (adj[cand][i] == 1) {
            cand = i;
        }
    }
    for (int i = 0; i < n; i++) {
        if (i != cand) {
            m->comparisons++;
            if (adj[cand][i] == 1 || adj[i][cand] == 0) {
                return -1;
            }
        }
    }
    return cand;
}