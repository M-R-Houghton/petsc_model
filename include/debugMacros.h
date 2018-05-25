#ifndef DEBUG_MACROS_H
#define DEBUG_MACROS_H

#include <stdio.h>
#include <assert.h>
#include <errno.h>
#include <string.h>

#include <petscksp.h>


#ifdef NDEBUG
#define debug(M, ...)
#else
#define debug(M, ...) fprintf(stdout, "DEBUG %s:%d: " M "\n",\
		__FILE__, __LINE__, ##__VA_ARGS__)
#endif

#define clean_errno() (errno == 0 ? "None" : strerror(errno))

#define log_err(M, ...) fprintf(stdout,\
		"[ERROR] (%s:%d:%s: errno: %s) " M "\n", __FILE__, __LINE__,\
		__FUNCTION__, clean_errno(), ##__VA_ARGS__)

#define log_warn(M, ...) fprintf(stdout,\
		"[WARN] (%s:%d:%s: errno: %s) " M "\n",\
		__FILE__, __LINE__, __FUNCTION__, clean_errno(), ##__VA_ARGS__)

#define log_info(M, ...) fprintf(stdout, "[INFO] (%s:%d:%s:) " M "\n",\
		__FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__)

#define check(A, M, ...) if(!(A)) {\
	log_err(M, ##__VA_ARGS__); errno=0; goto error; }

#define sentinel(M, ...) { log_err(M, ##__VA_ARGS__);\
	errno=0; goto error; }

#define check_mem(A) check((A), "Out of memory.")

#define check_debug(A, M, ...) if(!(A)) { debug(M, ##__VA_ARGS__);\
	errno=0; goto error; }

#endif