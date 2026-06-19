#ifndef	nolibc89_after_clone_thread_h
#define	nolibc89_after_clone_thread_h

#include <stddef.h>

typedef void nolibc89_tdentry_t(void*);

int	nolibc89_clone3_td(void* const rd_clone_args, size_t c_sz_clone_args, nolibc89_tdentry_t* fn, void* h_fnarg);

#endif
