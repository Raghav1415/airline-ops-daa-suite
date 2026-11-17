#include "aviation_types.h"

/* Helper to find modular multiplicative inverse of a modulo m using a simple loop */
static int find_mod_inverse(int a, int mod, Metrics *m) {
    a = a % mod;
    for (int x = 1; x < mod; x++) {
        m->comparisons++;
        if ((a * x) % mod == 1) {
            return x;
        }
    }
    return 1;
}

/* Algorithm: Chinese Remainder Theorem
 * Role in Project: Finds the exact future hour when multiple periodic aircraft maintenance cycles synchronize.
 */
int chinese_remainder_theorem(const int num[], const int rem[], int k, Metrics *m) {
    int prod = 1;
    for (int i = 0; i < k; i++) {
        prod *= num[i];
    }
    int result = 0;
    for (int i = 0; i < k; i++) {
        int pp = prod / num[i];
        int inv = find_mod_inverse(pp, num[i], m);
        result += rem[i] * pp * inv;
        m->operations++;
    }
    return result % prod;
}

/* Algorithm: Naive String-Matching Algorithm
 * Role in Project: Scans flight communication logs character by character to detect emergency alert codes.
 */
int naive_string_matcher(const char txt[], const char pat[], Metrics *m) {
    int n = (int)strlen(txt);
    int p = (int)strlen(pat);
    int matches = 0;

    for (int i = 0; i <= n - p; i++) {
        int j = 0;
        while (j < p) {
            m->comparisons++;
            if (txt[i + j] != pat[j]) {
                break;
            }
            j++;
        }
        if (j == p) {
            matches++;
        }
    }
    return matches;
}

/* Algorithm: Rabin-Karp Algorithm
 * Role in Project: Searches flight logs for alert patterns using a rolling hash to skip non-matching windows.
 */
int rabin_karp_matcher(const char txt[], const char pat[], Metrics *m) {
    int n = (int)strlen(txt);
    int p = (int)strlen(pat);
    int matches = 0;
    int d = 256;
    int q = 101;
    int h = 1;
    int p_hash = 0;
    int t_hash = 0;

    for (int i = 0; i < p - 1; i++) {
        h = (h * d) % q;
    }
    for (int i = 0; i < p; i++) {
        p_hash = (d * p_hash + pat[i]) % q;
        t_hash = (d * t_hash + txt[i]) % q;
        m->operations++;
    }
    for (int i = 0; i <= n - p; i++) {
        m->comparisons++;
        if (p_hash == t_hash) {
            int j = 0;
            while (j < p && txt[i + j] == pat[j]) {
                m->comparisons++;
                j++;
            }
            if (j == p) {
                matches++;
            }
        }
        if (i < n - p) {
            t_hash = (d * (t_hash - txt[i] * h) + txt[i + p]) % q;
            if (t_hash < 0) {
                t_hash += q;
            }
            m->operations++;
        }
    }
    return matches;
}

/* Algorithm: String Matching with Finite Automata
 * Role in Project: Builds a state transition table to scan flight telemetry streams in one pass without backup.
 */
int finite_automaton_matcher(const char txt[], const char pat[], Metrics *m) {
    int n = (int)strlen(txt);
    int p = (int)strlen(pat);
    int TF[32][256] = {{0}};

    for (int state = 0; state <= p; state++) {
        for (int ch = 0; ch < 256; ch++) {
            int next_state = (p < state + 1) ? p : state + 1;
            while (next_state > 0) {
                if (pat[next_state - 1] == ch) {
                    int i = 0;
                    while (i < next_state - 1 && pat[i] == pat[state - next_state + 1 + i]) {
                        i++;
                    }
                    if (i == next_state - 1) {
                        break;
                    }
                }
                next_state--;
            }
            TF[state][ch] = next_state;
        }
    }
    int state = 0;
    int matches = 0;
    for (int i = 0; i < n; i++) {
        m->comparisons++;
        state = TF[state][(unsigned char)txt[i]];
        if (state == p) {
            matches++;
        }
    }
    return matches;
}

/* Algorithm: Knuth-Morris-Pratt (KMP) Algorithm
 * Role in Project: Uses a precomputed prefix-suffix table (LPS) to find alert keywords in linear time.
 */
int kmp_matcher(const char txt[], const char pat[], Metrics *m) {
    int n = (int)strlen(txt);
    int p = (int)strlen(pat);
    int lps[32] = {0};

    int len = 0;
    int i = 1;
    while (i < p) {
        m->comparisons++;
        if (pat[i] == pat[len]) {
            len++;
            lps[i] = len;
            i++;
        } else if (len != 0) {
            len = lps[len - 1];
        } else {
            lps[i] = 0;
            i++;
        }
    }
    int matches = 0;
    int j = 0;
    i = 0;
    while (i < n) {
        m->comparisons++;
        if (pat[j] == txt[i]) {
            i++;
            j++;
        }
        if (j == p) {
            matches++;
            j = lps[j - 1];
        } else if (i < n && pat[j] != txt[i]) {
            if (j != 0) {
                j = lps[j - 1];
            } else {
                i++;
            }
        }
    }
    return matches;
}