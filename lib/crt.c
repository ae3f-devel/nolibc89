#include <nolibc/crt.h>
#include <stddef.h>
#include <sys/syscall.h>
#include <nolibc/arch.h>
#include <ae2f/cc/qualifier.h>

#if	0
static void __stack_chk_init(void) {}
static void exit(int c) {
	(void)__nolibc_syscall1(SYS_exit, c);
}

static char *strrchr(const char * ae2f_restrict s, int c) {
	unsigned char target = (unsigned char)c;

	while (1) {
		if ((unsigned char)*s == target) {
			return (char *)s;
		}
		if (*s == '\0') {
			break;
		}
		s++;
	}

	return NULL;
}
#endif
