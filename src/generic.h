#ifndef __GENERIC_H__
#define __GENERIC_H__

typedef char bool;

typedef void* (*init_t)      ();
typedef void* (*clone_t)     (void*);
typedef bool  (*condition_t) (void*, void*);
typedef int   (*compare_t)   (void*, void*);
typedef void  (*free_t)      (void*);

#define true 1
#define false 0

#endif
