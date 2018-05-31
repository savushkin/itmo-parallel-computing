#include <stdlib.h>
#include <sys/time.h>
#include <stdio.h>
#include <math.h>
#include <unistd.h>
#include <pthread.h>
#include <CL/cl.h>
#include <string.h>

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
#define PLATFORM_ID 0
#define KERNEL_COMPILE_LOG 0
#define QUEUE_PROFILING_ENABLE (1 << 1)

struct thread_stupid_sort_args {
    double * array;
    size_t from;
    size_t to;
};

struct thread_time_args {
    int max_iterations;
    int* iteration;
};

char *read_file(const char *name);
cl_program parse_program(cl_context context, char const *file_name, char const *options);

double* fill_array(double *arr, size_t size, unsigned int min, unsigned int max, int seed);
//int map(double *arr1, size_t size1, double *arr2, size_t size2);
double reduce(double *arr, size_t size);
int merge(double *arr1, double *arr2, size_t size2);
void * thread_stupid_sort(void *args);
void sort(double **array, size_t size);
void compare_time(double start_time, double end_time, double* min_time);
void * time_thread(void * arg);

int main(int argc, char* argv[]) {
    cl_context context;
    cl_command_queue queue;
    cl_int status;

    size_t returned_size = 0;
    cl_char device_name[1024] = {0};
    cl_uint platform_count;
    clGetPlatformIDs(0, NULL, &platform_count);
    cl_platform_id *platforms = (cl_platform_id*) malloc(platform_count * sizeof(cl_platform_id));
    clGetPlatformIDs(platform_count, platforms, NULL);

    cl_uint device_count;
    clGetDeviceIDs(platforms[PLATFORM_ID], CL_DEVICE_TYPE_ALL, 0, NULL, &device_count);
    cl_device_id *devices = (cl_device_id*) malloc(device_count * sizeof(cl_device_id));
    clGetDeviceIDs(platforms[PLATFORM_ID], CL_DEVICE_TYPE_ALL, device_count, devices, NULL);

    clGetDeviceInfo(devices[0], CL_DEVICE_NAME, sizeof(device_name), device_name, NULL);
//    printf("Connecting to %s...\n", device_name);

    cl_device_id device_id = devices[0];

    free(devices);
    free(platforms);

    context = clCreateContext(0, 1, &device_id, NULL, NULL, &status);

    cl_command_queue_properties queue_properties = 0;
    if (QUEUE_PROFILING_ENABLE)
        queue_properties |= CL_QUEUE_PROFILING_ENABLE;
    if (queue)
        queue = clCreateCommandQueueWithProperties(
                context,
                device_id,
                &queue_properties,
                &status);

    cl_program program = parse_program(context, "src/lab6.cl", NULL);
    cl_kernel map = clCreateKernel(program, "map", &status);

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

    size_t local_work_size = 256;
    int num_work_groups = (int) ((N + local_work_size - 1) / local_work_size);
    size_t global_work_size = num_work_groups * local_work_size;

    cl_mem buf_M1 = clCreateBuffer(context, CL_MEM_READ_ONLY, sizeof(double) * N, 0, &status);
    cl_mem buf_resM1 = clCreateBuffer(context, CL_MEM_READ_WRITE, sizeof(double) * N, 0, &status);

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

    pthread_t thread;

    struct thread_time_args thread_time_args;
    thread_time_args.max_iterations = max_iterarions;
    thread_time_args.iteration = &i;

//    pthread_create(&thread, NULL, time_thread, (void*)&thread_time_args);

    for (i = 0; i < max_iterarions; ++i) {
        t1 = omp_get_wtime();
        step_t1 = omp_get_wtime();
        fill_array(m1, N, 0, A, i);
        fill_array(m2, N/2, A, 10*A, i);
        step_t2 = omp_get_wtime();
        compare_time(step_t1, step_t2, &minimal_generate_time);

        step_t1 = omp_get_wtime();
        clEnqueueWriteBuffer(queue, buf_M1, CL_TRUE, 0, N * sizeof(double), m1, 0, NULL, NULL);
        clFinish(queue);
        clSetKernelArg(map, 0, sizeof(cl_mem), (void *)&buf_M1);
        clSetKernelArg(map, 1, sizeof(cl_mem), (void *)&buf_resM1);
        clSetKernelArg(map, 2, sizeof(int), (void *)&N);
        clEnqueueNDRangeKernel(queue, map, 1, NULL, &global_work_size, &local_work_size, 0, NULL, NULL);
        clFinish(queue);

        clEnqueueReadBuffer(queue, buf_resM1, CL_TRUE, 0, N * sizeof(double), m1, 0, NULL, NULL);

        double x = 0;
        for (i = 0; i < N/2; i++) {
            m2[i] = fabs(sin(m2[i] + x));
            x = m2[i];
        }
//        map(m1, N, m2, N/2);
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

    free(m1);
    free(m2);
    pthread_join(thread, NULL);
    clReleaseKernel(map);
    clReleaseCommandQueue(queue);
    clReleaseContext(context);

    printf("Best time: %f ms; N = %zu; X = %f ; threads: %d; generate: %f ms; map: %f ms; merge: %f ms; sort: %f ms; reduce: %f ms\n",
           minimal_time_ms,
           N,
           x,
           omp_get_max_threads(),
           minimal_generate_time,
           minimal_map_time,
           minimal_merge_time,
           minimal_sort_time,
           minimal_reduce_time);
    return 0;
}

void * time_thread(void * arg) {
    struct thread_time_args * thread_time_args = arg;

    printf("Task is completed for:\n0  %%");
    while (*thread_time_args->iteration < thread_time_args->max_iterations) {
        printf("%c[2K\r%2.0f %%", 27,
               (thread_time_args->max_iterations / 100.0) * (*thread_time_args->iteration+1) * 100.0);
        fflush(stdout);
        usleep(800);
    }
    printf("\n");
    return NULL;
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

//int map(double *arr1, size_t size1, double *arr2, size_t size2) {
//    double x;
//    int i;
//
//#pragma omp parallel for default(none) shared(size1, arr1) private(i) schedule(SCHEDULE, CHUNK)
//    for (i = 0; i < size1; i++) {
//        arr1[i] = (pow(M_E, arr1[i]) + pow(M_E, -arr1[i])) / (pow(M_E, arr1[i]) - pow(M_E, -arr1[i])) + 1;
//    }
//
//    x = 0;
//
//    for (i = 0; i < size2; i++) {
//        arr2[i] = fabs(sin(arr2[i] + x));
//        x = arr2[i];
//    }
//
//    return 0;
//}

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

void * thread_stupid_sort(void *args) {
    struct thread_stupid_sort_args * sort_args = args;
    int i = sort_args->from + 1;
    double tmp;
    while (i < sort_args->to - 1) {
        if (sort_args->array[i + 1] < sort_args->array[i]) {
            tmp = sort_args->array[i];
            sort_args->array[i] = sort_args->array[i + 1];
            sort_args->array[i + 1] = tmp;
            i = 0;
        } else
            i++;
    }
    pthread_exit(NULL);
}

void sort(double **array, size_t size)
{
    int from, to;
    int count_threads = omp_get_max_threads();
    int curr_chunk = size % count_threads ? size / count_threads + 1 : size / count_threads;
    unsigned int i;
    double *array_new = malloc(sizeof(double) * size);
    pthread_t threads[count_threads];

    for (i = 0; i < count_threads; i++) {
        from = i * curr_chunk;
        to = i * curr_chunk  + curr_chunk - 1;
        struct thread_stupid_sort_args sort_args;
        sort_args.from = from < size - 1 ? from : size - 1;
        sort_args.to = to < size - 1 ? to : size - 1;
        sort_args.array = *array;
        pthread_create(&threads[i], NULL, thread_stupid_sort, (void*)&sort_args);
    }
    for (i = 0; i < count_threads; i++) {
        pthread_join(threads[i], NULL);
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

char *read_file(const char *name) {
    FILE *file = fopen(name, "r");
    char *result;
    if (file) {
        fseek(file, 0, SEEK_END);
        size_t size = (size_t) ftell(file);
        fseek(file, 0, SEEK_SET);
        result = (char *)malloc(size + 1);
        if (result) {
            size_t count = fread(result, 1, size, file);
        }

        result[size] = '\0';
    }
    fclose(file);
    return result;
}

cl_program parse_program(cl_context context, char const *file_name, char const *options) {
    const char *kernel_source = read_file(file_name);
    size_t sizes[] = {strlen(kernel_source)};

    cl_device_id device_id;
    cl_int status;
    size_t log_size;
    char device_name[1000];

    cl_program program = clCreateProgramWithSource(context, 1, &kernel_source, sizes, &status);
    status = clBuildProgram(program, 0, NULL, options, NULL, NULL);
    clGetProgramInfo(program, CL_PROGRAM_DEVICES, sizeof(device_id), &device_id, NULL);
    clGetProgramBuildInfo(program, device_id, CL_PROGRAM_BUILD_LOG, 0, NULL, &log_size);

    if (KERNEL_COMPILE_LOG == 1) {
        char *log = (char *)malloc(log_size);
        clGetDeviceInfo(device_id, CL_DEVICE_NAME, sizeof(device_name), device_name, NULL);
        clGetProgramBuildInfo(program, device_id, CL_PROGRAM_BUILD_LOG, log_size, log, NULL);
        fprintf(stderr, "*** build of '%s' on '%s' said:\n%s\n*** (end of message)\n", file_name, device_name, log);
    }

//    free(kernel_source);

    return program;
}