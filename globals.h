#ifndef GLOBAL_HEADER
#define GLOBAL_HEADER

#include "unistd.h"

typedef unsigned long long int uint64_t;

#ifndef DATA_STRUCT
#define DATA_STRUCT

#define DEFAULT_INIT_SIZE_RATIO    2
#define DEFAULT_MAX_HEAP_NODE_SIZE 4096

typedef enum{false, true} bool;

typedef struct _mem_opt {

        unsigned int  size_ratio;
        unsigned int  max_heap_node_size;

} mem_opt_t;


#endif


extern mem_opt_t mem_opt;


#endif
