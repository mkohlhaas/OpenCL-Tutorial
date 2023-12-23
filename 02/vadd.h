#include <CL/cl.h>

extern double wtime();
extern int output_device_info(cl_device_id);

const char *KernelSource = "__kernel void vadd(           \n"
                           "   __global float* a,         \n"
                           "   __global float* b,         \n"
                           "   __global float* c)         \n"
                           "{                             \n"
                           "   int i = get_global_id(0);  \n"
                           "   c[i] = a[i] + b[i];        \n"
                           "}                             \n";

#define TOL (0.001)               // tolerance used in floating point comparisons
#define LENGTH (10 * 1024 * 1024) // length of vectors a, b, and c

int err; // error code returned from OpenCL calls
cl_uint numPlatforms;
cl_device_id device_id;
cl_context context;
cl_command_queue commands;
cl_program program;
cl_kernel ko_vadd;
float *h_a; // host memory
float *h_b;
float *h_c;
cl_mem d_a; // device memory
cl_mem d_b;
cl_mem d_c;

void allocateHostMemory();
void getGPUDevice();
void createKernel();
void allocateDeviceMemory();
void copyHostMemoryToDeviceMemory();
void executeKernel();
void cleanUpDevice();
void testResults();
