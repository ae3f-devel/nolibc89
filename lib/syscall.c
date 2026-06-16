#define	NOLIBC_NO_RUNTIME

#include <ae2f/cc/profiles.h>
#if	_ae2f_gnuc(!)0 || _ae2f_clang(!)0
#define NOLIBC89	1
#else
#define	NOLIBC89	0
#endif

#if	!NOLIBC89
typedef int NOLIBC89_CANNOT_COMPILE[-1];
#endif

#include <nolibc89_syscall.h>
#include <stddef.h>
#include <nolibc/arch.h>

#define	wrd_t	nolibc89_wrd_t

wrd_t	nolibc89_syscall0(wrd_t c0) {
	return __nolibc_syscall0(c0);
}

wrd_t nolibc89_syscall1(wrd_t c0, wrd_t c_a0) {
	return __nolibc_syscall1(c0, c_a0);
}

wrd_t nolibc89_syscall2(wrd_t c0, wrd_t c_a0, wrd_t c_a1) {
	return __nolibc_syscall2(c0, c_a0, c_a1);
}

wrd_t nolibc89_syscall3(wrd_t c0, wrd_t c_a0, wrd_t c_a1, wrd_t c_a2) {
	return __nolibc_syscall3(c0, c_a0, c_a1, c_a2);
}

wrd_t nolibc89_syscall4(wrd_t c0, wrd_t c_a0, wrd_t c_a1, wrd_t c_a2, wrd_t c_a3) {
	return __nolibc_syscall4(c0, c_a0, c_a1, c_a2, c_a3);
}

wrd_t nolibc89_syscall5(wrd_t c0, wrd_t c_a0, wrd_t c_a1, wrd_t c_a2, wrd_t c_a3, wrd_t c_a4) {
	return __nolibc_syscall5(c0, c_a0, c_a1, c_a2, c_a3, c_a4);
}

wrd_t nolibc89_syscall6(wrd_t c0, wrd_t c_a0, wrd_t c_a1, wrd_t c_a2, wrd_t c_a3, wrd_t c_a4, wrd_t c_a5) {
	return __nolibc_syscall6(c0, c_a0, c_a1, c_a2, c_a3, c_a4, c_a5);
}
