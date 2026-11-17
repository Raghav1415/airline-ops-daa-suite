#include "aviation_types.h"

int main(void) {
    Flight schedule[8] = {
        {1, 480, 520, 20, 150, -1}, {2, 490, 550, 30, 200, -1},
        {3, 530, 580, 15, 120, -1}, {4, 510, 570, 25, 180, -1},
        {5, 585, 630, 10, 100, -1}, {6, 540, 600, 20, 160, -1},
        {7, 610, 660, 35, 250, -1}, {8, 640, 700, 15, 130, -1}
    };
    Flight quick_copy[8];
    int runway_ids[8];
    for (int i = 0; i < 8; i++) quick_copy[i] = schedule[i];

    Metrics m_ms = {0, 0}, m_qs = {0, 0}, m_bs = {0, 0}, m_act = {0, 0}, m_gate = {0, 0};
    merge_sort_flights(schedule, 0, 7, &m_ms);
    quick_sort_flights(quick_copy, 0, 7, &m_qs);

    int idx = binary_search_flight(quick_copy, 0, 7, 540, &m_bs);
    int r_cnt = activity_selection_runway(schedule, 8, runway_ids, &m_act);
    int g_cnt = interval_partitioning_gates(quick_copy, 8, &m_gate);

    printf("=== PHASE I CORE SKELETON (SORTING, D&C & GREEDY SCHEDULING) ===\n");
    printf("MergeSort Comps=%ld | QuickSort Comps=%ld | BinarySearch(540) Idx=%d\n",
           m_ms.comparisons, m_qs.comparisons, idx);
    printf("Conflict-Free Runway Flights=%d | Minimum Gates Required=%d\n", r_cnt, g_cnt);
    return 0;
}
