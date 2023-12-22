#include "err_code.h"
#include <CL/cl.h>

#define STR_LENGTH 1024
#define KB 1024
#define MB (1024 * 1024)

cl_uint numPlatforms() {
  cl_uint num_platforms;
  cl_int err = clGetPlatformIDs(0, NULL, &num_platforms);
  handleError(err, "Finding platforms");
  return num_platforms;
}

cl_platform_id *platforms() {
  cl_platform_id *platforms = malloc(numPlatforms() * sizeof(cl_platform_id *));
  cl_int err = clGetPlatformIDs(numPlatforms(), platforms, NULL);
  handleError(err, "Getting platforms");
  return platforms;
}

char *platformName(cl_platform_id p) {
  char *name = malloc(STR_LENGTH);
  cl_int err = clGetPlatformInfo(p, CL_PLATFORM_NAME, STR_LENGTH, name, NULL);
  handleError(err, "Getting platform name");
  return name;
}

char *platformVendor(cl_platform_id p) {
  char *vendor = malloc(STR_LENGTH);
  cl_int err =
      clGetPlatformInfo(p, CL_PLATFORM_VENDOR, STR_LENGTH, vendor, NULL);
  handleError(err, "Getting platform vendor");
  return vendor;
}

char *platformVersion(cl_platform_id p) {
  char *version = malloc(STR_LENGTH);
  cl_int err =
      clGetPlatformInfo(p, CL_PLATFORM_VERSION, STR_LENGTH, version, NULL);
  handleError(err, "Getting platform OpenCL version");
  return version;
}

cl_uint numDevices(cl_platform_id p) {
  cl_uint numDevices;
  cl_int err = clGetDeviceIDs(p, CL_DEVICE_TYPE_ALL, 0, NULL, &numDevices);
  if (err != CL_SUCCESS) {
    return 0;
  }
  return numDevices;
}

cl_device_id *devices(cl_platform_id p) {
  cl_uint nDev = numDevices(p);
  cl_device_id *devices = malloc(nDev * sizeof(cl_device_id *));
  cl_int err = clGetDeviceIDs(p, CL_DEVICE_TYPE_ALL, nDev, devices, NULL);
  handleError(err, "Getting devices");
  return devices;
}

char *deviceName(cl_device_id d) {
  const size_t strLength = STR_LENGTH;
  char *name = malloc(strLength);
  cl_int err = clGetDeviceInfo(d, CL_DEVICE_NAME, strLength, name, NULL);
  handleError(err, "Getting device name");
  return name;
}

char *deviceVersion(cl_device_id d) {
  const size_t strLength = STR_LENGTH;
  char *version = malloc(strLength);
  cl_int err =
      clGetDeviceInfo(d, CL_DEVICE_OPENCL_C_VERSION, strLength, version, NULL);
  handleError(err, "Getting device OpenCL C version");
  return version;
}

cl_uint deviceMaxComputeUnits(cl_device_id d) {
  cl_uint num;
  cl_int err = clGetDeviceInfo(d, CL_DEVICE_MAX_COMPUTE_UNITS, sizeof(cl_uint),
                               &num, NULL);
  handleError(err, "Getting device max compute units");
  return num;
}

cl_ulong deviceLocalMemSize(cl_device_id d) {
  cl_ulong mem_size;
  cl_int err = clGetDeviceInfo(d, CL_DEVICE_LOCAL_MEM_SIZE, sizeof(cl_ulong),
                               &mem_size, NULL);
  handleError(err, "Getting device local memory size");
  return mem_size;
}

cl_ulong deviceGlobalMemSize(cl_device_id d) {
  cl_ulong mem_size;
  cl_int err = clGetDeviceInfo(d, CL_DEVICE_GLOBAL_MEM_SIZE, sizeof(cl_ulong),
                               &mem_size, NULL);
  handleError(err, "Getting device global memory size");
  return mem_size;
}

cl_ulong deviceMaxMemAllocSize(cl_device_id d) {
  cl_ulong mem_size;
  cl_int err = clGetDeviceInfo(d, CL_DEVICE_MAX_MEM_ALLOC_SIZE,
                               sizeof(cl_ulong), &mem_size, NULL);
  handleError(err, "Getting device max allocation size");
  return mem_size;
}

size_t deviceMaxWorkGroupSize(cl_device_id d) {
  size_t size;
  cl_int err = clGetDeviceInfo(d, CL_DEVICE_MAX_WORK_GROUP_SIZE, sizeof(size_t),
                               &size, NULL);
  handleError(err, "Getting device max work-group size");
  return size;
}

cl_uint deviceMaxWorkItemDimensions(cl_device_id d) {
  size_t num;
  cl_int err = clGetDeviceInfo(d, CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS,
                               sizeof(size_t), &num, NULL);
  handleError(err, "Getting device max work-item dims");
  return num;
}

size_t *deviceMaxWorkItemSizes(cl_device_id d) {
  size_t dimSize = sizeof(size_t) * deviceMaxWorkItemDimensions(d);
  size_t *dims = malloc(dimSize);
  cl_int err = clGetDeviceInfo(d, CL_DEVICE_MAX_WORK_GROUP_SIZE, dimSize, dims, NULL);
  handleError(err, "Getting device max work-item sizes");
  return dims;
}

void printPlatformName(cl_platform_id p) {
  printf("Platform: %s\n", platformName(p));
}

void printPlatformVendor(cl_platform_id p) {
  printf("Vendor: %s\n", platformVendor(p));
}

void printPlatformVersion(cl_platform_id p) {
  printf("Version: %s\n", platformVersion(p));
}

void printNumberOfDevices(cl_platform_id p) {
  cl_uint nDev = numDevices(p);
  printf("Number of devices: %d\n", nDev);
}

void printDeviceName(cl_device_id d) { printf("  Name: %s\n", deviceName(d)); }

void printDeviceVersion(cl_device_id d) {
  printf("  Version: %s\n", deviceVersion(d));
}

void printDeviceMaxComputeUnits(cl_device_id d) {
  printf("  Max. Compute Units: %d\n", deviceMaxComputeUnits(d));
}

void printDeviceLocalMemSize(cl_device_id d) {
  printf("  Local Memory Size: %lu KB\n", deviceLocalMemSize(d) / KB);
}

void printDeviceGlobalMemSize(cl_device_id d) {
  printf("  Global Memory Size: %lu MB\n", deviceGlobalMemSize(d) / MB);
}

void printDeviceMaxMemAllocSize(cl_device_id d) {
  printf("  Max Alloc Size: %lu MB\n", deviceMaxMemAllocSize(d) / MB);
}

void printDeviceMaxWorkGroupSize(cl_device_id d) {
  printf("  Max Work-group Total Size: %ld\n", deviceMaxWorkGroupSize(d));
}

void printMaxWorkGroupDims(cl_device_id d) {
  cl_uint num = deviceMaxWorkItemDimensions(d);
  size_t *dims = deviceMaxWorkItemSizes(d);

  printf("  Max Work-group Dims: ( ");
  for (size_t k = 0; k < num; k++) {
    printf("%ld ", dims[k]);
  }
  printf(")\n");
}

void printNumberOfPlatforms() {
  printf("Number of OpenCL platforms: %d\n", numPlatforms());
}

void printDevices(cl_platform_id p) {
  cl_uint numOfDevices = numDevices(p);
  if (numOfDevices > 0) {
    cl_device_id *device = devices(p);
    for (int j = 0; j < numOfDevices; j++) {
      cl_device_id d = device[j];
      printf("--\n");
      printDeviceName(d);
      printDeviceVersion(d);
      printDeviceMaxComputeUnits(d);
      printDeviceLocalMemSize(d);
      printDeviceGlobalMemSize(d);
      printDeviceMaxMemAllocSize(d);
      printDeviceMaxWorkGroupSize(d);
      printMaxWorkGroupDims(d);
    }
  }
}

void printPlatforms() {
  cl_uint numOfPlatforms = numPlatforms();
  if (numOfPlatforms > 0) {
    cl_platform_id *platform = platforms();
    for (int i = 0; i < numOfPlatforms; i++) {
      cl_platform_id p = platform[i];
      printf("-------------------------\n");
      printPlatformName(p);
      printPlatformVendor(p);
      printPlatformVersion(p);
      printNumberOfDevices(p);
      printDevices(p);
    }
  }
}
