#include "aviation_types.h"

/* Algorithm: Binary Search
 * Role in Project: Searches a departure-sorted flight schedule to locate a flight at a target departure time.
 */
int binary_search_flight(const Flight arr[], int low, int high, int target_dep, Metrics *m) {
    while (low <= high) {
        m->comparisons++;
        int mid = low + (high - low) / 2;

        if (arr[mid].dep_time == target_dep) {
            return mid;
        }
        if (arr[mid].dep_time < target_dep) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return -1;
}

/* Helper function to merge two sorted halves of the flight schedule */
static void merge_arrays(Flight arr[], int left, int mid, int right, Metrics *m) {
    Flight temp[MAX_FLIGHTS];
    int i = left;
    int j = mid + 1;
    int k = left;

    while (i <= mid && j <= right) {
        m->comparisons++;
        if (arr[i].arr_time <= arr[j].arr_time) {
            temp[k++] = arr[i++];
        } else {
            temp[k++] = arr[j++];
        }
        m->operations++;
    }
    while (i <= mid) {
        temp[k++] = arr[i++];
        m->operations++;
    }
    while (j <= right) {
        temp[k++] = arr[j++];
        m->operations++;
    }
    for (i = left; i <= right; i++) {
        arr[i] = temp[i];
    }
}

/* Algorithm: Merge Sort
 * Role in Project: Stably sorts flights by arrival time so runway activity selection can schedule them optimally.
 */
void merge_sort_flights(Flight arr[], int left, int right, Metrics *m) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        merge_sort_flights(arr, left, mid, m);
        merge_sort_flights(arr, mid + 1, right, m);
        merge_arrays(arr, left, mid, right, m);
    }
}

/* Helper function to partition flights around a pivot departure time */
static int partition_array(Flight arr[], int low, int high, Metrics *m) {
    int pivot = arr[high].dep_time;
    int i = low - 1;

    for (int j = low; j < high; j++) {
        m->comparisons++;
        if (arr[j].dep_time <= pivot) {
            i++;
            Flight swap_temp = arr[i];
            arr[i] = arr[j];
            arr[j] = swap_temp;
            m->operations++;
        }
    }
    Flight swap_temp = arr[i + 1];
    arr[i + 1] = arr[high];
    arr[high] = swap_temp;
    m->operations++;
    return i + 1;
}

/* Algorithm: Quick Sort
 * Role in Project: Sorts flights in-place by departure time to prepare the schedule for gate interval partitioning.
 */
void quick_sort_flights(Flight arr[], int low, int high, Metrics *m) {
    if (low < high) {
        int pi = partition_array(arr, low, high, m);
        quick_sort_flights(arr, low, pi - 1, m);
        quick_sort_flights(arr, pi + 1, high, m);
    }
}