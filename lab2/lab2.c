#include <stdlib.h>
#include <sys/time.h>
#include <stdio.h>
#include <math.h>

#include "ipp.h"

#define A 400
#define SEED 34
#define DEFAULT_N 20
#define DEFAULT_M 1

double* fill_array(size_t size, unsigned int min, unsigned int max);
int map(double *arr1, size_t size1, double *arr2, size_t size2);
double reduce(double *arr, size_t size);
int merge(double *arr1, double *arr2, size_t size2);
int stupid_sort(double *arr, size_t size);

int main(int argc, char* argv[]) {
    const IppLibraryVersion *lib;
//    IppStatus status;
//    Ipp64u mask, emask;
    ippSetNumThreads(1);

    /* Initialize Intel IPP library */
    ippInit();
    /* Get Intel IPP library version info */
    lib = ippGetLibVersion();
    printf("%s %s\n", lib->Name, lib->Version);

    struct timeval T1, T2;
    long time_ms, minimal_time_ms = -1;
    size_t N;
    if(argc > 1)
        N = (size_t) atoi(argv[1]); /* инициализировать число N первым параметром командной строки */
    else
        N = DEFAULT_N;
    if(argc > 2)
        ippSetNumThreads(atoi(argv[2]));
    else
        ippSetNumThreads(DEFAULT_M);

    double *m1, *m2, x;

    for (int i = 0; i < 10; ++i) {
        gettimeofday(&T1, NULL); /* запомнить текущее время T1 */
        m1 = fill_array(N, 1, A);
        m2 = fill_array(N/2, A, 10*A);
        map(m1, N, m2, N/2);
        merge(m1, m2, N/2);
        stupid_sort(m2, N/2);
        x = reduce(m2, N/2);
        gettimeofday(&T2, NULL); /* запомнить текущее время T2 */
        time_ms = 1000 * (T2.tv_sec - T1.tv_sec) + (T2.tv_usec - T1.tv_usec) / 1000;
        if ((minimal_time_ms == -1) || (time_ms < minimal_time_ms))
            minimal_time_ms = time_ms;
        free(m1);
        free(m2);
    }

    printf("Best time: %ld ms. N = %zu. X = %f.\n", minimal_time_ms, N, x); /* затраченное время */
    return 0;
}

double* fill_array(size_t size, unsigned int min, unsigned int max) {
    double *arr = malloc(sizeof(double) * size);
    unsigned int seed = SEED;
    int i;

    for (i = 0; i < size; i++)
        arr[i] = ((double) (rand_r(&seed)%(100*(max-min)))/100) + min;

    return arr;
}

int map(double *arr1, size_t size1, double *arr2, size_t size2) {
//    double x;
//    int i;
    ippsSqrt_64f_I(arr1, size1);
    ippsTan_64f_A50(arr1, arr1, size1);
//    for (i = 0; i < size1; i++) {
//        x = sqrt(arr1[i]);
//        arr1[i] = (pow(M_E, x) + pow(M_E, -x)) / (pow(M_E, x) - pow(M_E, -x));
//    }
//
//    x = 0;
//    for (i = 0; i < size2; i++) {
//        arr2[i] = fabs(sin(arr2[i] + x));
//        x = arr2[i];
//    }

    return 0;
}

double reduce(double *arr, size_t size) {
    double res = 0, min;
    int i = 0;

    if (size > 0)
        min = arr[0];
    else
        min = 0;
    for (i = 0; i < size; i++) {
        if (arr[i] < min && arr[i] != 0) {
            min = arr[i];
        }
    }

    for (i = 0; i < size; i++) {
        if ((int)(arr[i] / min) % 2 == 0) {
            res += sin(arr[i]);
        }
    }

    return res;
}

int merge(double *arr1, double *arr2, size_t size2) {
    int i;

    for (i = 0; i < size2; i++) {
        arr2[i] = pow(arr1[i], arr2[i]);
    }

    return 0;
}

int stupid_sort(double *arr, size_t size) {
    int i = 0;
    double tmp;
    while (i < size - 1) {
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