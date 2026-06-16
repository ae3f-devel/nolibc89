#ifndef	nolibc89_syscall_h
#define	nolibc89_syscall_h

#include <ae2f/c90/stdint.h>
#include <ae2f/Cxx/extern.h>

#ifndef	nolibc89_wrd_t
typedef	intmax_t nolibc89_wrd_t;
#define	nolibc89_wrd_t	nolibc89_wrd_t
#endif

ae2f_extern nolibc89_wrd_t nolibc89_syscall0(nolibc89_wrd_t c0);
ae2f_extern nolibc89_wrd_t nolibc89_syscall1(
		nolibc89_wrd_t c0,
		nolibc89_wrd_t a0
		);
ae2f_extern nolibc89_wrd_t nolibc89_syscall2(
		nolibc89_wrd_t c0,
		nolibc89_wrd_t a0,
		nolibc89_wrd_t a1
		);
ae2f_extern nolibc89_wrd_t nolibc89_syscall3(
		nolibc89_wrd_t c0,
		nolibc89_wrd_t a0,
		nolibc89_wrd_t a1,
		nolibc89_wrd_t a2
		);
ae2f_extern nolibc89_wrd_t nolibc89_syscall4(
		nolibc89_wrd_t c0,
		nolibc89_wrd_t a0,
		nolibc89_wrd_t a1,
		nolibc89_wrd_t a2,
		nolibc89_wrd_t a3
		);
ae2f_extern nolibc89_wrd_t nolibc89_syscall5(
		nolibc89_wrd_t c0,
		nolibc89_wrd_t a0,
		nolibc89_wrd_t a1,
		nolibc89_wrd_t a2,
		nolibc89_wrd_t a3,
		nolibc89_wrd_t a4
		);
ae2f_extern nolibc89_wrd_t nolibc89_syscall6(
		nolibc89_wrd_t c0,
		nolibc89_wrd_t a0,
		nolibc89_wrd_t a1,
		nolibc89_wrd_t a2,
		nolibc89_wrd_t a3,
		nolibc89_wrd_t a4,
		nolibc89_wrd_t a5
		);

#endif
