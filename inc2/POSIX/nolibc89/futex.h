#ifndef	nolibc89_posix_futex_h
#define nolibc89_posix_futex_h

#include <pthread.h>
#include <ae2f/Keys.h>

ae2f_extern pthread_mutex_t	g_nolibc89_futex_mutex;
ae2f_extern pthread_cond_t	g_nolibc89_futex_cond;

#endif
