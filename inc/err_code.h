#pragma once

#include <CL/cl.h>

#define handleError(E, S) _handleError(E, S, __FILE__, __LINE__)

const char *err_code(cl_int error);
void _handleError(cl_int err, const char *operation, char *filename, int line);
