#ifndef	nolibc89_after_clone_thread_h
#define	nolibc89_after_clone_thread_h

#include <ae2f/c90/stddef/size_t.h>
#include <ae2f/Cxx/extern.h>

typedef void nolibc89_tdentry_t(void*);

ae2f_extern int	nolibc89_clone3_td(void* const rd_clone_args, ae2f_size_t c_sz_clone_args, nolibc89_tdentry_t* fn, void* h_fnarg);

#endif
