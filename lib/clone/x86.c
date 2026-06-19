#if defined(__x86_64__) || defined(__i386__) || defined(__i486__) || defined(__i586__) || defined(__i686__)

#include <nolibc89/clone.h>

#if defined(__x86_64__)

#ifndef __NR_clone3
#define __NR_clone3 435
#endif
#ifndef __NR_exit
#define __NR_exit 60
#endif

#define STR_(x) #x
#define STR(x)  STR_(x)

/*
 * nolibc89_clone3_td -- spawn a thread with clone3 and run fn(arg) on it.
 *
 * SysV/x86_64 hands us the arguments in:
 *     rdi = rd_clone_args   -> already where clone3 wants &clone_args
 *     rsi = c_sz_clone_args -> already where clone3 wants sizeof clone_args
 *     rdx = fn
 *     rcx = h_fnarg
 *
 * clone3 does not let us prime the child's stack with a return frame: the
 * kernel parks the child's SP at clone_args.stack + clone_args.stack_size and
 * the child resumes right here, sharing the parent's register file save for the
 * rcx/r11 that the syscall instruction clobbers. So we stash fn and arg in r9
 * and r8 -- caller-saved but untouched by the trap -- and the child reads them
 * back independently of wherever SP landed.
 *
 * Parent path: rax holds the new TID (or a negative -errno); we just ret it.
 * Child path (rax == 0): clear the frame pointer so unwinders stop here, align
 * the stack to the ABI, tail-call fn(arg), then feed exit(2) -- thread-local,
 * never exit_group -- so only this thread is torn down. fn returns void, so the
 * thread exits with status 0. The hlt is unreachable: exit never returns.
 */
__attribute__((naked)) int nolibc89_clone3_td(
		void* const rd_clone_args,
		size_t c_sz_clone_args,
		nolibc89_tdentry_t* fn,
		void* h_fnarg) {
	__asm__ volatile (
			"mov   %rdx, %r9\n"			/* fn  -> r9 (survives syscall)   */
			"mov   %rcx, %r8\n"			/* arg -> r8 (survives syscall)   */
			"mov   $" STR(__NR_clone3) ", %eax\n"	/* rdi/rsi already in place   */
			"syscall\n"

			"test  %rax, %rax\n"
			"jz    1f\n"				/* rax == 0 -> child              */
			"ret\n"					/* parent: rax = tid or -errno    */

			"1:\n"
			"xor   %ebp, %ebp\n"			/* outermost frame: stop unwinders */
			"mov   %r8, %rdi\n"			/* arg -> first integer argument  */
			"and   $-16, %rsp\n"			/* 16-byte align before the call  */
			"call  *%r9\n"				/* fn(arg)                        */
			"xor   %edi, %edi\n"			/* exit status 0 (fn is void)     */
			"mov   $" STR(__NR_exit) ", %eax\n"	/* __NR_exit (thread-local)   */
			"syscall\n"
			"hlt\n"					/* unreachable                    */
			);
}

#endif /* __x86_64__ */

#endif
