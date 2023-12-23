#include <CL/cl.h>
#include <stdio.h>

#include "err_code.h"
#include "vadd.h"

int main(int argc, char **argv) {
  allocateHostMemory();
  getGPUDevice();
  createKernel();
  allocateDeviceMemory();
  copyHostMemoryToDeviceMemory();
  executeKernel();
  cleanUpDevice();
  testResults();
}

void allocateHostMemory() {
  h_a = malloc(LENGTH * sizeof(float));
  h_b = malloc(LENGTH * sizeof(float));
  h_c = malloc(LENGTH * sizeof(float));

  // Fill vectors a and b with random float values.
  for (int i = 0; i < LENGTH; i++) {
    h_a[i] = (float)rand() / RAND_MAX;
    h_b[i] = (float)rand() / RAND_MAX;
    h_c[i] = (float)rand() / RAND_MAX;
  }
}

void getGPUDevice() {
  // Find number of platforms.
  err = clGetPlatformIDs(0, NULL, &numPlatforms);
  handleError(err, "Finding platforms");
  if (numPlatforms == 0) {
    printf("Found no platforms!\n");
    exit(EXIT_FAILURE);
  }

  // Get all platforms.
  cl_platform_id Platform[numPlatforms];
  handleError(clGetPlatformIDs(numPlatforms, Platform, NULL), "Getting platforms");

  // Get a GPU.
  for (int i = 0; i < numPlatforms; i++) {
    err = clGetDeviceIDs(Platform[i], CL_DEVICE_TYPE_GPU, 1, &device_id, NULL);
    if (!err) {
      break;
    }
  }

  if (!device_id) {
    handleError(err, "Finding a device");
  }

  // Print device info.
  output_device_info(device_id);
}

// Create Context and command queue, build program, then create kernel.
void createKernel() {
  context = clCreateContext(NULL, 1, &device_id, NULL, NULL, &err);
  handleError(err, "Creating context");
  commands = clCreateCommandQueueWithProperties(context, device_id, NULL, &err);
  handleError(err, "Creating command queue");
  program = clCreateProgramWithSource(context, 1, (const char **)&KernelSource, NULL, &err);

  handleError(err, "Creating program");
  err = clBuildProgram(program, 1, &device_id, NULL, NULL, NULL);
  if (err) {
    char buffer[2048];
    printf("Error: Failed to build program executable!\n%s\n", err_code(err));
    clGetProgramBuildInfo(program, device_id, CL_PROGRAM_BUILD_LOG, sizeof(buffer), buffer, NULL);
    printf("%s\n", buffer);
    exit(EXIT_FAILURE);
  }
  ko_vadd = clCreateKernel(program, "vadd", &err);
  handleError(err, "Creating kernel");
}

void allocateDeviceMemory() {
  d_a = clCreateBuffer(context, CL_MEM_READ_ONLY, sizeof(float) * LENGTH, NULL, &err);
  handleError(err, "Creating buffer d_a on device");
  d_b = clCreateBuffer(context, CL_MEM_READ_ONLY, sizeof(float) * LENGTH, NULL, &err);
  handleError(err, "Creating buffer d_b on device");
  d_c = clCreateBuffer(context, CL_MEM_WRITE_ONLY, sizeof(float) * LENGTH, NULL, &err);
  handleError(err, "Creating buffer d_c on device");
}

void copyHostMemoryToDeviceMemory() {
  handleError(clEnqueueWriteBuffer(commands, d_a, CL_BLOCKING, 0, sizeof(float) * LENGTH, h_a, 0, NULL, NULL),
              "Copying h_a to device at d_a");
  handleError(clEnqueueWriteBuffer(commands, d_b, CL_BLOCKING, 0, sizeof(float) * LENGTH, h_b, 0, NULL, NULL),
              "Copying h_b to device at d_b");
}

void executeKernel() {
  // Set kernel arguments.
  err = clSetKernelArg(ko_vadd, 0, sizeof(cl_mem), &d_a);
  err |= clSetKernelArg(ko_vadd, 1, sizeof(cl_mem), &d_b);
  err |= clSetKernelArg(ko_vadd, 2, sizeof(cl_mem), &d_c);
  handleError(err, "Setting kernel arguments");

  // Start timer.
  double rtime = wtime();

  // Run kernel.
  size_t global = LENGTH; // global work size
  handleError(clEnqueueNDRangeKernel(commands, ko_vadd, 1, NULL, &global, NULL, 0, NULL, NULL), "Enqueueing kernel");

  // Wait for commands to finish.
  handleError(clFinish(commands), "Waiting for kernel to finish");

  // Stop timer and print execution time.
  rtime = wtime() - rtime;
  printf("\nThe kernel ran in %lf seconds\n", rtime);

  // Read back the results from the device.
  handleError(clEnqueueReadBuffer(commands, d_c, CL_BLOCKING, 0, sizeof(float) * LENGTH, h_c, 0, NULL, NULL),
              "Failed to read output array!");

  rtime = wtime() - rtime;
  printf("The kernel run and copy between host and device took %lf seconds\n", rtime);
}

void cleanUpDevice() {
  clReleaseMemObject(d_a);
  clReleaseMemObject(d_b);
  clReleaseMemObject(d_c);
  clReleaseProgram(program);
  clReleaseKernel(ko_vadd);
  clReleaseCommandQueue(commands);
  clReleaseContext(context);
}

void testResults() {
  unsigned int correct = 0;
  float tmp;

  for (int i = 0; i < LENGTH; i++) {
    tmp = h_a[i] + h_b[i];
    tmp -= h_c[i];
    if (tmp * tmp < TOL * TOL) {
      correct++;
    } else {
      printf("tmp: %f | h_a: %f | h_b: %f | h_c: %f \n", tmp, h_a[i], h_b[i], h_c[i]);
    }
  }

  printf("C = A+B: %d out of %d results were correct.\n", correct, LENGTH);
}
