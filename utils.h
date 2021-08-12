#ifndef __UTILS_H__
#define __UTILS_H__

#include <stdlib.h>
#include <sys/time.h>

#define SUCCESS_STATUS_CODE 0
#define FAILURE_STATUS_CODE 1

typedef unsigned int _uint;
typedef const _uint  _cuint;

// Certifica que um ponteiro foi alocado
void must_alloc (void *ptr, const char* desc);

// Checa se n é um número inválido
void check_exception (long double n, const char *desc);

#endif