#include <nolibc89/futex.h>

pthread_mutex_t	g_nolibc89_futex_mutex	= PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t	g_nolibc89_futex_cond	= PTHREAD_COND_INITIALIZER;
