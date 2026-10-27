#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct {
    int key1;
    int key2;
    int original_index;
} DataItem;

int get_random_key() {
    long long r = ((long long)rand() << 15) ^ rand();
    return (int)(r % 1000001);
}

void generate_random_data(DataItem arr[], int n) {
    for (int i = 0; i < n; i++) {
        arr[i].key1 = get_random_key();
        arr[i].key2 = get_random_key();
        arr[i].original_index = i;
    }
}

void copy_data(DataItem dest[], DataItem src[], int n) {
    for (int i = 0; i < n; i++) {
        dest[i] = src[i];
    }
}

void selection_sort(DataItem arr[], int n, long long *comparisons, long long *movements) {
    *comparisons = 0;
    *movements = 0;
    DataItem temp;
    for (int i = 0; i < n; i++) {
        int min_idx = i;
        for (int j = i + 1; j < n; j++) {
            (*comparisons)++;
            if (arr[j].key1 < arr[min_idx].key1) {
                min_idx = j;
            }
        }
        if (min_idx != i) {
            temp = arr[i];        
            arr[i] = arr[min_idx];   
            arr[min_idx] = temp;    
            (*movements) += 3;
        }
    }
}

void bubble_sort(DataItem arr[], int n, long long *comparisons, long long *movements) {
    *comparisons = 0;
    *movements = 0;
    DataItem temp;
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - 1 - i; j++) {
            (*comparisons)++;
            if (arr[j].key1 > arr[j + 1].key1) {
                temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
                (*movements) += 3;
            }
        }
    }
}

void bubble_sort_early(DataItem arr[], int n, long long *comparisons, long long *movements) {
    *comparisons = 0;
    *movements = 0;
    DataItem temp;
    for (int i = 0; i < n - 1; i++) {
        int swapped = 0;
        for (int j = 0; j < n - 1 - i; j++) {
            (*comparisons)++;
            if (arr[j].key1 > arr[j + 1].key1) {
                temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
                (*movements) += 3;
                swapped = 1;
            }
        }
        if (!swapped) {
            break;
        }
    }
}

void insertion_sort(DataItem arr[], int n, int use_greater_strict, long long *comparisons, long long *movements) {
    *comparisons = 0;
    *movements = 0;
    for (int i = 1; i < n; i++) {
        DataItem temp = arr[i];
        (*movements)++; 
        int j = i - 1;
        while (j >= 0) {
            (*comparisons)++;
            int condition = use_greater_strict ? (arr[j].key1 > temp.key1) : (arr[j].key1 >= temp.key1);
            if (condition) {
                arr[j + 1] = arr[j];
                (*movements)++; 
                j--;
            } else {
                break;
            }
        }
        arr[j + 1] = temp;
        (*movements)++; 
    }
}

const char* verify_stability(DataItem arr[], int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = i + 1; j < n; j++) {
            if (arr[i].key1 == arr[j].key1) {
                if (arr[i].original_index > arr[j].original_index) {
                    return "Unstable";
                }
            }
        }
    }
    return "Stable";
}

int compare_items(const void *a, const void *b) {
    DataItem *item1 = (DataItem *)a;
    DataItem *item2 = (DataItem *)b;
    if (item1->key1 < item2->key1) return -1;
    if (item1->key1 > item2->key1) return 1;
    return 0;
}

int main() {
    srand(42); 
    int n_data = 10000;
    int num_trials = 100;

    DataItem *base_data = (DataItem*)malloc(sizeof(DataItem) * n_data);
    DataItem *work_data = (DataItem*)malloc(sizeof(DataItem) * n_data);

    long long sel_total_comp = 0, sel_total_mov = 0;
    long long bub_total_comp = 0, bub_total_mov = 0;
    long long bub_e_total_comp = 0, bub_e_total_mov = 0;
    long long ins_total_comp = 0, ins_total_mov = 0;

    for (int t = 0; t < num_trials; t++) {
        generate_random_data(base_data, n_data);
        long long c, m;

        copy_data(work_data, base_data, n_data);
        selection_sort(work_data, n_data, &c, &m);
        sel_total_comp += c; sel_total_mov += m;

        copy_data(work_data, base_data, n_data);
        bubble_sort(work_data, n_data, &c, &m);
        bub_total_comp += c; bub_total_mov += m;

        copy_data(work_data, base_data, n_data);
        bubble_sort_early(work_data, n_data, &c, &m);
        bub_e_total_comp += c; bub_e_total_mov += m;

        copy_data(work_data, base_data, n_data);
        insertion_sort(work_data, n_data, 1, &c, &m); // op '>'
        ins_total_comp += c; ins_total_mov += m;
    }

    double sel_avg_comp = (double)sel_total_comp / num_trials;
    double sel_avg_mov = (double)sel_total_mov / num_trials;
    double bub_avg_comp = (double)bub_total_comp / num_trials;
    double bub_avg_mov = (double)bub_total_mov / num_trials;
    double bub_e_avg_comp = (double)bub_e_total_comp / num_trials;
    double bub_e_avg_mov = (double)bub_e_total_mov / num_trials;
    double ins_avg_comp = (double)ins_total_comp / num_trials;
    double ins_avg_mov = (double)ins_total_mov / num_trials;

    DataItem *sample_base = (DataItem*)malloc(sizeof(DataItem) * n_data);
    generate_random_data(sample_base, n_data);

    DataItem *sorted_data = (DataItem*)malloc(sizeof(DataItem) * n_data);
    copy_data(sorted_data, sample_base, n_data);
    qsort(sorted_data, n_data, sizeof(DataItem), compare_items);

    long long sc_s, sm_s, bc_s, bm_s, bec_s, bem_s, ic_s, im_s;
    copy_data(work_data, sorted_data, n_data); selection_sort(work_data, n_data, &sc_s, &sm_s);
    copy_data(work_data, sorted_data, n_data); bubble_sort(work_data, n_data, &bc_s, &bm_s);
    copy_data(work_data, sorted_data, n_data); bubble_sort_early(work_data, n_data, &bec_s, &bem_s);
    copy_data(work_data, sorted_data, n_data); insertion_sort(work_data, n_data, 1, &ic_s, &im_s);

    DataItem *reverse_data = (DataItem*)malloc(sizeof(DataItem) * n_data);
    for (int i = 0; i < n_data; i++) {
        reverse_data[i] = sorted_data[n_data - 1 - i];
    }

    long long sc_r, sm_r, bc_r, bm_r, bec_r, bem_r, ic_r, im_r;
    copy_data(work_data, reverse_data, n_data); selection_sort(work_data, n_data, &sc_r, &sm_r);
    copy_data(work_data, reverse_data, n_data); bubble_sort(work_data, n_data, &bc_r, &bm_r);
    copy_data(work_data, reverse_data, n_data); bubble_sort_early(work_data, n_data, &bec_r, &bem_r);
    copy_data(work_data, reverse_data, n_data); insertion_sort(work_data, n_data, 1, &ic_r, &im_r);

    DataItem *partial_data = (DataItem*)malloc(sizeof(DataItem) * n_data);
    copy_data(partial_data, sorted_data, n_data);
    int swap_count = n_data * 0.05; 
    for (int k = 0; k < swap_count; k++) {
        int idx1 = rand() % n_data;
        int idx2 = rand() % n_data;
        DataItem temp = partial_data[idx1];
        partial_data[idx1] = partial_data[idx2];
        partial_data[idx2] = temp;
    }

    long long sc_p, sm_p, bc_p, bm_p, bec_p, bem_p, ic_p, im_p;
    copy_data(work_data, partial_data, n_data); selection_sort(work_data, n_data, &sc_p, &sm_p);
    copy_data(work_data, partial_data, n_data); bubble_sort(work_data, n_data, &bc_p, &bm_p);
    copy_data(work_data, partial_data, n_data); bubble_sort_early(work_data, n_data, &bec_p, &bem_p);
    copy_data(work_data, partial_data, n_data); insertion_sort(work_data, n_data, 1, &ic_p, &im_p);

    DataItem test_stab[5] = {
        {527, 100, 0},
        {527, 200, 1},
        {125, 300, 2},
        {892, 400, 3},
        {527, 50,  4}
    };
    DataItem t_sel[5], t_bub[5], t_ins_gt[5], t_ins_gte[5];
    for(int i=0; i<5; i++) { 
        t_sel[i]=test_stab[i]; 
        t_bub[i]=test_stab[i]; 
        t_ins_gt[i]=test_stab[i]; 
        t_ins_gte[i]=test_stab[i]; 
    }

    long long dummy_c, dummy_m;
    selection_sort(t_sel, 5, &dummy_c, &dummy_m);
    bubble_sort(t_bub, 5, &dummy_c, &dummy_m);
    insertion_sort(t_ins_gt, 5, 1, &dummy_c, &dummy_m);   // Case A: '>'
    insertion_sort(t_ins_gte, 5, 0, &dummy_c, &dummy_m);  // Case B: '>='

    const char* stab_sel = verify_stability(t_sel, 5);
    const char* stab_bub = verify_stability(t_bub, 5);
    const char* stab_ins_gt = verify_stability(t_ins_gt, 5);
    const char* stab_ins_gte = verify_stability(t_ins_gte, 5);

    printf("Data Count : %d\n\n", n_data);
    printf("Random Data\n");
    printf("Number of Trials : %d\n\n", num_trials);

    printf("Selection Sort\n");
    printf("Total Comparisons   : %lld\n", sel_total_comp);
    printf("Average Comparisons : %.2f\n", sel_avg_comp);
    printf("Average Movements   : %.2f\n\n", sel_avg_mov);

    printf("Bubble Sort\n");
    printf("Total Comparisons   : %lld\n", bub_total_comp);
    printf("Average Comparisons : %.2f\n", bub_avg_comp);
    printf("Average Movements   : %.2f\n\n", bub_avg_mov);

    printf("Bubble Sort (Early Termination)\n");
    printf("Total Comparisons   : %lld\n", bub_e_total_comp);
    printf("Average Comparisons : %.2f\n", bub_e_avg_comp);
    printf("Average Movements   : %.2f\n\n", bub_e_avg_mov);

    printf("Insertion Sort\n");
    printf("Total Comparisons   : %lld\n", ins_total_comp);
    printf("Average Comparisons : %.2f\n", ins_avg_comp);
    printf("Average Movements   : %.2f\n\n", ins_avg_mov);

    printf("------------------------------------\n\n");
    printf("Sorted Data\n\n");
    printf("Selection Sort\n");
    printf("Comparisons : %lld\n", sc_s);
    printf("Movements   : %lld\n\n", sm_s);
    printf("Bubble Sort\n");
    printf("Comparisons : %lld\n", bc_s);
    printf("Movements   : %lld\n\n", bm_s);
    printf("Bubble Sort (Early Termination)\n");
    printf("Comparisons : %lld\n", bec_s);
    printf("Movements   : %lld\n\n", bem_s);
    printf("Insertion Sort\n");
    printf("Comparisons : %lld\n", ic_s);
    printf("Movements   : %lld\n\n", im_s);

    printf("------------------------------------\n\n");
    printf("Reverse-Sorted Data\n\n");
    printf("Selection Sort\n");
    printf("Comparisons : %lld\n", sc_r);
    printf("Movements   : %lld\n\n", sm_r);
    printf("Bubble Sort\n");
    printf("Comparisons : %lld\n", bc_r);
    printf("Movements   : %lld\n\n", bm_r);
    printf("Bubble Sort (Early Termination)\n");
    printf("Comparisons : %lld\n", bec_r);
    printf("Movements   : %lld\n\n", bem_r);
    printf("Insertion Sort\n");
    printf("Comparisons : %lld\n", ic_r);
    printf("Movements   : %lld\n\n", im_r);

    printf("------------------------------------\n\n");
    printf("Partially Sorted Data\n\n");
    printf("Selection Sort\n");
    printf("Comparisons : %lld\n", sc_p);
    printf("Movements   : %lld\n\n", sm_p);
    printf("Bubble Sort\n");
    printf("Comparisons : %lld\n", bc_p);
    printf("Movements   : %lld\n\n", bm_p);
    printf("Bubble Sort (Early Termination)\n");
    printf("Comparisons : %lld\n", bec_p);
    printf("Movements   : %lld\n\n", bem_p);
    printf("Insertion Sort\n");
    printf("Comparisons : %lld\n", ic_p);
    printf("Movements   : %lld\n\n", im_p);

    printf("------------------------------------\n\n");
    printf("Stability Verification\n\n");
    printf("Selection Sort       : %s\n", stab_sel);
    printf("Bubble Sort          : %s\n", stab_bub);
    printf("Insertion Sort (>)   : %s\n", stab_ins_gt);
    printf("Insertion Sort (>=)  : %s\n", stab_ins_gte);

    // 메모리 해제
    free(base_data);
    free(work_data);
    free(sample_base);
    free(sorted_data);
    free(reverse_data);
    free(partial_data);

    return 0;
}
