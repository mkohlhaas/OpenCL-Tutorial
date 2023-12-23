#include <CL/cl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "err_code.h"

void output_device_info(cl_device_id device_id) {
  int err;

  // TODO: Querying is all library code. See 01.
  cl_char device_name[1024] = {0};
  err = clGetDeviceInfo(device_id, CL_DEVICE_NAME, sizeof(device_name), &device_name, NULL);
  handleError(err, "Failed to access device name!");

  cl_char vendor_name[1024] = {0};
  err = clGetDeviceInfo(device_id, CL_DEVICE_VENDOR, sizeof(vendor_name), &vendor_name, NULL);
  handleError(err, "Failed to access device vendor name!");

  cl_uint comp_units;
  err = clGetDeviceInfo(device_id, CL_DEVICE_MAX_COMPUTE_UNITS, sizeof(cl_uint), &comp_units, NULL);
  handleError(err, "Failed to access device number of compute units !");

  printf("Device is a %s %s %d.", device_name, vendor_name, comp_units);
}
