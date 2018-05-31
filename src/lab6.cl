#pragma OPENCL EXTENSION cl_khr_fp64 : enable
__kernel void map(
    __global const double *M1,
    __global double *c,
    int N)
{
    int gid = get_global_id(0);
    if (gid < N) {
        c[gid] = (pow(M_E, M1[gid]) + pow(M_E, -M1[gid])) / (pow(M_E, M1[gid]) - pow(M_E, -M1[gid])) + 1;
    }
}
