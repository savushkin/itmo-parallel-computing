#include <stdlib.h>
#include <sys/time.h>
#include <stdio.h>
#include <math.h>
#include <unistd.h>

#ifdef _OPENMP
#include <omp.h>
#else
int omp_get_max_threads()
{
    return 1;
}

int omp_get_num_procs()
{
    return 1;
}

int omp_get_thread_num()
{
    return 0;
}

void omp_set_num_threads(int thrds)
{
    return;
}

double omp_get_wtime()
{
    struct timeval T;
    double time_ms;

    gettimeofday(&T, NULL);
    time_ms = (1000.0 * ((double) T.tv_sec) + ((double)T.tv_usec) / 1000.0);
    return (double)(time_ms / 1000.0);
}

void omp_set_nested(int b){
    return;
}
#endif

#define A 400
#define SEED 34
#define DEFAULT_N 20
#define DEFAULT_M 1

double* fill_array(double *arr, size_t size, unsigned int min, unsigned int max, int seed);
int map(double *arr1, size_t size1, double *arr2, size_t size2);
double reduce(double *arr, size_t size);
int merge(double *arr1, double *arr2, size_t size2);
int stupid_sort(double *arr, size_t from, size_t to);
void sort(double **array, size_t size);
void compare_time(double start_time, double end_time, double* min_time);

int main(int argc, char* argv[]) {
    size_t N;
    if(argc > 1)
        N = (size_t) atoi(argv[1]); /* инициализировать число N первым параметром командной строки */
    else
        N = DEFAULT_N;
    omp_set_nested(1);
    if(argc > 2)
        omp_set_num_threads(atoi(argv[2]));
    else
        omp_set_num_threads(DEFAULT_M);
    double x, *m1 = malloc(sizeof(double) * N), *m2 = malloc(sizeof(double) * (N / 2)),
            t1, t2, minimal_time_ms = -1.0,
            step_t1, step_t2,
            minimal_generate_time = -1.0,
            minimal_map_time = -1.0,
            minimal_merge_time = -1.0,
            minimal_sort_time = -1.0,
            minimal_reduce_time = -1.0;

    int i, max_iterarions = 10;
    omp_set_nested(1);
    #pragma omp parallel shared(i, max_iterarions)
    #pragma omp sections
    {
        #pragma omp section
        {
            for (i = 0; i < max_iterarions; ++i) {
                t1 = omp_get_wtime();
                step_t1 = omp_get_wtime();
                fill_array(m1, N, 0, A, i);
                fill_array(m2, N/2, A, 10*A, i);
                step_t2 = omp_get_wtime();
                compare_time(step_t1, step_t2, &minimal_generate_time);

                step_t1 = omp_get_wtime();
                map(m1, N, m2, N/2);
                step_t2 = omp_get_wtime();
                compare_time(step_t1, step_t2, &minimal_map_time);

                step_t1 = omp_get_wtime();
                merge(m1, m2, N/2);
                step_t2 = omp_get_wtime();
                compare_time(step_t1, step_t2, &minimal_merge_time);


                step_t1 = omp_get_wtime();
                sort(&m2, N/2);
                step_t2 = omp_get_wtime();
                compare_time(step_t1, step_t2, &minimal_sort_time);

                step_t1 = omp_get_wtime();
                x = reduce(m2, N/2);
                step_t2 = omp_get_wtime();
                compare_time(step_t1, step_t2, &minimal_reduce_time);

                t2 = omp_get_wtime();
                compare_time(t1, t2, &minimal_time_ms);
            }
        }

//        #pragma omp section
//        {
//            printf("Task is completed for:\n0  %%");
//            while (i < max_iterarions) {
//                printf("%c[2K\r%2.0f %%", 27, (max_iterarions/100.0)*(i+1)*100.0);
//                fflush(stdout);
//                usleep(800);
//            }
//            printf("\n");
//        }
    }

    free(m1);
    free(m2);

    printf("Best time: %f ms; N = %zu; X = %f ; threads: %d; generate: %f ms; map: %f ms; merge: %f ms; sort: %f ms; reduce: %f ms\n",
           minimal_time_ms,
           N,
           x,
           omp_get_max_threads(),
           minimal_generate_time,
           minimal_map_time,
           minimal_merge_time,
           minimal_sort_time,
           minimal_reduce_time); /* затраченное время */
    return 0;
}

double* fill_array(double *arr, size_t size, unsigned int min, unsigned int max, int seed) {
    int i, tmp_seed;

    #pragma omp parallel for default(none) shared(size, arr, min, max) private(i, seed, tmp_seed) schedule(SCHEDULE, CHUNK)
    for (i = 0; i < size; i++) {
        tmp_seed = sqrt((i + 2) * 100);
        arr[i] = ((double) (rand_r(&tmp_seed) % (100 * (max - min))) / 100) + min;
    }

    return arr;
}

int map(double *arr1, size_t size1, double *arr2, size_t size2) {
    double x;
    int i;

    #pragma omp parallel for default(none) shared(size1, arr1) private(i) schedule(SCHEDULE, CHUNK)
    for (i = 0; i < size1; i++) {
        arr1[i] = (pow(M_E, arr1[i]) + pow(M_E, -arr1[i])) / (pow(M_E, arr1[i]) - pow(M_E, -arr1[i])) + 1;
    }

    x = 0;

    for (i = 0; i < size2; i++) {
        arr2[i] = fabs(sin(arr2[i] + x));
        x = arr2[i];
    }

    return 0;
}

double reduce(double *arr, size_t size) {
    double res = 0, min = arr[0];
    int i = 0;

    for (i = 0; i < size; i++) {
        if (arr[i] != 0) {
            min = arr[i];
            break;
        }
    }

    #pragma omp parallel for default(none) shared(arr, size, min) private(i) reduction(+: res) schedule(SCHEDULE, CHUNK)
    for (i = 0; i < size; i++) {
        if ((int)(arr[i] / min) % 2 == 0) {
            res += sin(arr[i]);
        }
    }

    return res;
}

int merge(double *arr1, double *arr2, size_t size2) {
    int i;

    #pragma omp parallel for default(none) shared(size2, arr1, arr2) private(i) schedule(SCHEDULE, CHUNK)
    for (i = 0; i < size2; i++) {
        arr2[i] = pow(arr1[i], arr2[i]);
    }

    return 0;
}

void merge_arrays(double *array_old, double *array_new, unsigned int n, int num, int chunk) {
    unsigned int i, m;
    unsigned int *arr_i = calloc(num, (sizeof(unsigned int)));
    unsigned int min;

    for (i = 0; i < n; i++) {
        min = 0;
        for (m = 0; m < num; m++) {
            if (arr_i[m] < chunk) {
                array_new[i] = array_old[m*chunk + arr_i[m]];
                min = m;
                break;
            }
        }
        for (m = 0; m < num; m++) {
            if((m*chunk + arr_i[m] < n) && (arr_i[m] < chunk) && array_old[m*chunk + arr_i[m]] < array_new[i]) {
                array_new[i] = array_old[m*chunk + arr_i[m]];
                min = m;
            }
        }
        arr_i[min]++;
    }
}

int stupid_sort(double *arr, size_t from, size_t to) {
    int i = from + 1;
    double tmp;
    while (i < to - 1) {
        if (arr[i + 1] < arr[i]) {
            tmp = arr[i];
            arr[i] = arr[i + 1];
            arr[i + 1] = tmp;
            i = 0;
        } else
            i++;
    }
    return 0;
}

void sort(double **array, size_t size)
{
    int from, to;
    int count_threads = omp_get_max_threads();
    int curr_chunk = size % count_threads ? size / count_threads + 1 : size / count_threads;
    unsigned int i;
    double *array_new = malloc(sizeof(double) * size);

#pragma omp parallel for default(none) shared(array, size, curr_chunk, count_threads) private(i, from, to) schedule(SCHEDULE, CHUNK)
    for (i = 0; i < count_threads; i++) {
        from = i * curr_chunk;
        to = i * curr_chunk  + curr_chunk - 1;
        stupid_sort(*array, from < size - 1 ? from : size - 1, to < size - 1 ? to : size - 1);
    }

    merge_arrays(*array, array_new, size, count_threads, curr_chunk);
    free(*array);
    *array = array_new;
}

void compare_time(double start_time, double end_time, double* min_time) {
    double step_time = 1000 * (end_time - start_time);
    if ((*min_time == -1.0) || (step_time < *min_time))
        *min_time = step_time;
}