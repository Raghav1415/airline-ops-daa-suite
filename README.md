# Airline Crew, Flight & Gate Planning Suite (CSEG3053 DAA)

**Course:** CSEG3053 – Design and Analysis of Algorithms (Two-Phase Offline Hackathon)  
**Developer:** Raghav Sharma  
**Institution:** UPES Dehradun  
**Language & Standard:** C (ISO C99) | Menu-Driven Command-Line Interface (CLI)  

---

## 1. Project Overview & Problem Statement

### 1.1 Target User & Real-World Need
* **Target User:** Airline Network Operations Control (NOC) dispatchers, airport gate coordinators, and flight disruption recovery planners.
* **Real-World Need:** Commercial airports operate under tight turnaround windows where delayed arrivals, weather shocks, and limited gates or runways cause cascading schedule conflicts. Dispatchers require an offline, deterministic decision-support simulator that detects schedule overlaps and computes conflict-free runway sequences, minimum gate allocations, high-revenue cargo recovery plans, crew duty feasibility checks, and shortest flight routes.

### 1.2 Input, Output, Constraints & Success Criteria
* **Inputs:**
  * External flight schedule CSV (`sample_flights.csv`) or synthetic scenario generators (`id`, `dep_time`, `arr_time`, `weight`, `revenue`) for $N \in [0, 512]$.
  * Airport network adjacency matrices and edge lists (`MAX_NODES = 10`) representing fuel costs and transit times.
  * ACARS/NOTAM telemetry strings and periodic maintenance cycle moduli.
* **Outputs:**
  * Console recovery reports, empirical benchmark metrics, and automated test results.
  * Exported artifacts: `benchmark_report.csv`, `recovery_schedule.csv`, `recovery_plan.json`, and `execution_output.txt`.
* **Constraints & Validation Rules:**
  * Strictly offline execution in standard C99 with zero third-party libraries.
  * Input validation enforces $0 \le \text{dep\_time} < \text{arr\_time} \le 1440$, $\text{weight} > 0$, and $\text{revenue} \ge 0$.
  * Two flights conflict on a runway or gate if and only if $\text{dep}_i < \text{arr}_j$ and $\text{dep}_j < \text{arr}_i$.
* **Success Criteria:**
  * Zero gate or runway overlaps in the recovered schedule.
  * 100% agreement across exact 0/1 optimization paradigms (Dynamic Programming, Backtracking, and FIFO/LIFO/LC Branch-and-Bound).
  * 16/16 pass rate across Oracle, Integration, Boundary, Invalid-Input, and Regression test suites.

---

## 2. Repository Architecture

```text
airline_ops_daa_suite/
├── Phase_I/                   # Standalone Phase I evaluation snapshot & skeleton
├── Phase_II/                  # Complete Phase II evaluation snapshot & proof logs
├── aviation_types.h           # Core structs (Flight, Edge, Metrics) & 30 prototypes
├── sorting_searching.c        # Row 1: Binary Search, Merge Sort, Quick Sort
├── divide_conquer.c           # Row 1: Strassen 2x2, Max Subarray, Power, Celebrity Hub
├── greedy_graphs.c            # Row 2: Fractional Knapsack, Activity, Huffman, Gate, Dijkstra, Prim, Kruskal
├── dynamic_programming.c      # Row 3: 0/1 Knapsack, MCM, LCS, OBST, Bellman-Ford, TSP DP, Floyd-Warshall
├── backtracking_bnb.c         # Row 4: N-Queens, Subset Sum, Knapsack Bt, FIFO/LIFO/LC BnB, TSP BnB
├── string_number_theory.c     # Row 5: Chinese Remainder Theorem, Naive, Rabin-Karp, Automaton, KMP
├── test_suite.c               # 16 automated unit, oracle, boundary, invalid & regression tests
├── main.c                     # Conflict Recovery Engine, Menu CLI, Benchmark Runner & Exporters
├── sample_flights.csv         # Sample 10-flight input dataset
├── Makefile                   # Build configuration script
└── README.md                  # User guide, design guide, complexity report & test matrix
```

---

## 3. System Workflow & Architecture Diagram

```text
+-------------------------------------------------------------------------+
|                  INPUT LAYER (CSV File / Scenario Engine)               |
|  sample_flights.csv  |  1_Normal  |  2_Shock  |  3_Stress / Adversarial |
+------------------------------------+------------------------------------+
                                     |
                                     v
+-------------------------------------------------------------------------+
|               VALIDATION & CONFLICT DETECTION ENGINE (main.c)           |
|   Validates (dep < arr, weight > 0) & Scans Pairwise Time Overlaps      |
+------------------------------------+------------------------------------+
                                     |
        +----------------------------+----------------------------+
        |                            |                            |
        v                            v                            v
+---------------+          +-------------------+        +-----------------+
|  D&C MODULE   |          |   GREEDY MODULE   |        |  DP & BnB MOD   |
| - Merge Sort  |          | - Runway Activity |        | - 0/1 Knapsack  |
| - Quick Sort  |          | - Gate Partition  |        | - FIFO/LIFO/LC  |
| - Bin Search  |          | - Dijkstra / MST  |        | - Subset Sum    |
+-------+-------+          +---------+---------+        +--------+--------+
        |                            |                           |
        +----------------------------+---------------------------+
                                     |
                                     v
+-------------------------------------------------------------------------+
|           INTERFACE & EXPORT LAYER (Menu CLI + CSV/JSON Files)          |
|  benchmark_report.csv  |  recovery_schedule.csv  |  recovery_plan.json  |
+-------------------------------------------------------------------------+
```

---

## 4. Core Algorithm Pseudocode & Correctness Reasoning

### 4.1 Merge Sort + Greedy Runway Activity Selection
```text
ALGORITHM MergeSortAndRunwaySelection(Flights[0..n-1])
1. IF n <= 0 RETURN 0
2. MergeSort(Flights, 0, n - 1) by non-decreasing arr_time
3. selected_ids[0] <- Flights[0].id
4. count <- 1
5. last_finish <- Flights[0].arr_time
6. FOR i <- 1 TO n - 1 DO
7.     IF Flights[i].dep_time >= last_finish THEN
8.         selected_ids[count] <- Flights[i].id
9.         count <- count + 1
10.        last_finish <- Flights[i].arr_time
11. RETURN count
```
* **Correctness (Greedy Exchange Argument):** Picking the flight with the earliest finish time (`arr_time`) leaves the maximum remaining runway availability. If an optimal schedule $O$ starts with flight $f_k$ ($\text{arr}_k \ge \text{arr}_1$), replacing $f_k$ with $f_1$ yields a valid schedule of equal size.

### 4.2 Quick Sort + Greedy Gate Interval Partitioning
```text
ALGORITHM GateIntervalPartitioning(Flights[0..n-1])
1. QuickSort(Flights, 0, n - 1) by non-decreasing dep_time
2. gate_count <- 0
3. FOR i <- 0 TO n - 1 DO
4.     chosen_gate <- -1
5.     FOR g <- 0 TO gate_count - 1 DO
6.         IF gate_free_time[g] <= Flights[i].dep_time THEN
7.             chosen_gate <- g
8.             BREAK
9.     IF chosen_gate == -1 THEN
10.        chosen_gate <- gate_count
11.        gate_count <- gate_count + 1
12.    gate_free_time[chosen_gate] <- Flights[i].arr_time
13.    Flights[i].assigned_gate <- chosen_gate
14. RETURN gate_count
```
* **Correctness (Interval Depth Lower Bound):** When flight $i$ opens gate $d$, all $d-1$ existing gates have $\text{gate\_free\_time}[g] > \text{dep}_i$. Since flights are sorted by $\text{dep\_time}$, all $d$ flights overlap simultaneously at time $\text{dep}_i$, proving no valid assignment can use fewer than $d$ gates.

---

## 5. Recurrence Relations, Complexity Analysis & Empirical Benchmark

### 5.1 Master Theorem & Recurrence Solutions
* **Merge Sort & Max Subarray:** $T(n) = 2T(n/2) + \Theta(n) \implies \Theta(n \log n)$ time, $\Theta(n)$ auxiliary space (Master Theorem Case 2).
* **Binary Search & Powering Number:** $T(n) = T(n/2) + \Theta(1) \implies \Theta(\log n)$ time.
* **Strassen Matrix Multiplication:** $T(n) = 7T(n/2) + \Theta(n^2) \implies \Theta(n^{\log_2 7}) \approx \Theta(n^{2.81})$ time (Master Theorem Case 1).
* **Quick Sort:** Average $T(n) = 2T(n/2) + \Theta(n) \implies \Theta(n \log n)$; Adversarial Worst Case $T(n) = T(n-1) + \Theta(n) \implies \Theta(n^2)$ time, $\Theta(1)$ auxiliary array space.

### 5.2 Empirical Benchmark Results (`benchmark_report.csv`)
| Dataset Condition | Flights ($N$) | Merge Sort Comps | Quick Sort Comps | Merge Sort Aux Bytes | Quick Sort Aux Bytes | Max Runway Flights | Min Gates Needed |
|---|---|---|---|---|---|---|---|
| `1_Small_Normal_Ops` | 16 | 32 | 120 | 384 B | 0 B | 8 | 2 |
| `2_Constraint_Shock` | 16 | 44 | 57 | 384 B | 0 B | 1 | 16 |
| `3_Medium_Workload` | 64 | 274 | 468 | 1,536 B | 0 B | 15 | 5 |
| `4_Large_Stress_Adversarial` | 250 | 1,244 | 31,125 | 6,000 B | 0 B | 9 | 14 |

### 5.3 Multi-Paradigm Cargo Recovery Comparison ($N=6$, Capacity $= 50$)
* **Greedy Fractional Knapsack:** Revenue $= 433.3$ (splits boundary cargo lot; acts as upper bound).
* **Dynamic Programming 0/1 Knapsack:** Revenue $= 425$ (exact integer optimum in $\Theta(nW)$ pseudo-polynomial time).
* **Backtracking 0/1 Knapsack:** Revenue $= 425$ (explores unpruned feasible state tree).
* **Branch & Bound (FIFO / LIFO / Least-Cost):** Revenue $= 425$ across all three after ratio-sorting (`sort_cargo_by_ratio`), with Least-Cost expanding the fewest tree nodes.

### 5.4 Complexity Class Analysis & Reduction ($\text{Subset Sum} \le_P \text{0/1 Knapsack}$)
* **Class P:** Merge/Quick Sort, Binary Search, Activity Selection, Gate Partitioning, Dijkstra, Prim, Kruskal, Floyd-Warshall, KMP.
* **NP-Complete / NP-Hard:** Subset Sum, 0/1 Knapsack, N-Queens, and Travelling Salesperson Problem (TSP).
* **Polynomial-Time Reduction:** Given a Subset Sum instance $(S = \{h_1, \dots, h_n\}, T)$, construct a 0/1 Knapsack instance in $O(n)$ time by setting each item's weight $w_i = h_i$, revenue $v_i = h_i$, capacity $W = T$, and target profit $V = T$. Then a subset of $S$ sums to $T$ if and only if the knapsack achieves profit $\ge T$ within capacity $T$.

---

## 6. Test-Case Verification Table (16 Automated Checks + Stress Case)

| Test ID | Category | Test Case Description | Input Specification | Expected | Actual | Status |
|---|---|---|---|---|---|---|
| TC-01 | Oracle-R1 | Strassen 2x2 Matrix Multiplication | $A=[[1,2],[3,4]], B=[[5,6],[7,8]]$ | `19` | `19` | PASS |
| TC-02 | Oracle-R1 | D&C Maximum Subarray Delay | `[-5, 20, -4, 30, 10, -15]` | `56` | `56` | PASS |
| TC-03 | Oracle-R1 | Celebrity Sink Hub Discovery | 4 nodes, all edges into Hub `2` | `2` | `2` | PASS |
| TC-04 | Oracle-R2 | Prim vs. Kruskal MST Agreement | 4-node weighted graph (5 edges) | `19` | `19` | PASS |
| TC-05 | Oracle-R3 | Matrix Chain Multiplication Order | `dims = [10, 30, 5, 60]` | `4500` | `4500` | PASS |
| TC-06 | Oracle-R3 | LCS Flight Waypoint Adherence | `"DELBOMBLR"` vs `"DEBOMLR"` | `7` | `7` | PASS |
| TC-07 | Oracle-R4 | TSP DP vs. Branch & Bound Tour | 4-hub symmetric cost matrix | `80` | `80` | PASS |
| TC-08 | Oracle-R5 | 4 String Matchers Consensus | `"ACARS_WINDSHEAR_ALERT_WINDSHEAR"` | `2` | `2` | PASS |
| TC-09 | Integration| Sort + Runway + Gate Pipeline | 4 overlapping flights ($500..680$) | `3`, `2` | `3`, `2` | PASS |
| TC-10 | Boundary | Empty Flight Schedule ($N=0$) | `N = 0` in Runway Selection | `0` | `0` | PASS |
| TC-11 | Boundary | Single Flight Schedule ($N=1$) | `N = 1` in Gate Partitioning | `1` | `1` | PASS |
| TC-12 | Boundary | Powering Number Zero Exponent | `base = 99, exp = 0` | `1` | `1` | PASS |
| TC-13 | Invalid | Binary Search Missing Departure | `target_dep = 999` | `-1` | `-1` | PASS |
| TC-14 | Invalid | Knapsack Negative Capacity | `capacity = -25` | `0` | `0` | PASS |
| TC-15 | Invalid | Bellman-Ford Negative Cycle Detect | 3-edge cycle ($1 - 5 + 2 = -2$) | `-1` | `-1` | PASS |
| TC-16 | Regression | DP vs. FIFO/LIFO/LC BnB Knapsack | 4 cargo lots, `capacity = 40` | `320` | `320` | PASS |
| TC-17 | Adversarial| Reverse-Sorted 250-Flight Stress | $N = 250$, `dep = 1200 - 4*i` | `14` gates| `14` gates| PASS |

---

## 7. Progression After Phase I, Limitations & Error Analysis

* **Progression from Phase I to Phase II:** Upgraded the standalone Phase I sorting/greedy skeleton (`Phase_I/`) into a full conflict-detection and recovery simulator (`Phase_II/`) with CSV ingestion, JSON/CSV exporters, and a 16-test automated suite. Diagnosed and fixed early branch pruning in LIFO/LC Branch-and-Bound by sorting cargo items by descending revenue-to-weight ratio prior to bound calculation.
* **Limitations:**
  * Exact NP-hard solvers (`tsp_dp_tour` $\Theta(n^2 2^n)$, `bnb_tsp_tour` $O(n!)$, `n_queens_backtrack` $O(n!)$) are bounded to `MAX_NODES = 10` due to exponential state growth.
  * `dp_01_knapsack` uses a fixed stack array `dp[256]`, requiring weight scaling for capacities above 255.
  * `chinese_remainder_theorem` assumes pairwise coprime maintenance cycle moduli.

---

## 8. Offline Build & Execution Instructions

```powershell
# 1. Compile the complete application
gcc -std=c99 -Wall -Wextra -O2 sorting_searching.c divide_conquer.c greedy_graphs.c dynamic_programming.c backtracking_bnb.c string_number_theory.c test_suite.c main.c -o aviation_suite.exe

# 2. Run Interactive Menu CLI
.\aviation_suite.exe

# 3. Run Automated Batch Demo & Save Proof Logs
.\aviation_suite.exe --auto > execution_output.txt
```

---

## 9. Declaration of Originality

I, **Raghav Sharma**, declare that this project (**Airline Crew, Flight & Gate Planning Suite**) submitted for **CSEG3053 – Design and Analysis of Algorithms** is my own independent offline implementation. All core algorithms are written from scratch in C without external library shortcuts, and I am prepared to explain, trace, test, debug, and modify every component live.