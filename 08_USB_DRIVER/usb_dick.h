#ifndef QUERRY_IO_H
#define QUERRY_IO_H
#include <linux/ioctl.h>

typedef struct {
    int status, dignity, ego;
} query_arg_t;

#define QUERY_GET_VARIABLES _IOR('q',1, query_arg_t *)
#define QUERY_CLR_VARIABLES _IO('q',2)
//#define QUERY_CLR_VARIABLES _IO('q',2)
#define QUERY_SET_VARIABLES _IOW('q',1, query_arg_t *)

#endif
