#include <nolibc89/futex.h>

ae2f_extern pthread_mutex_t	g_nolibc89_futex_mutex	= PTHREAD_MUTEX_INITIALIZER;
ae2f_extern pthread_cond_t	g_nolibc89_futex_cond	= PTHREAD_COND_INITIALIZER;
