#ifndef	nolibc89_arch_h
#define	nolibc89_arch_h

#define	NOLIBC89_ARCH_X86	0
#define	NOLIBC89_ARCH_ARM	0
#define	NOLIBC89_ARCH_ARM64	0
#define	NOLIBC89_ARCH_MIPS	0
#define	NOLIBC89_ARCH_POWERPC	0
#define	NOLIBC89_ARCH_RISCV	0
#define	NOLIBC89_ARCH_S390	0
#define	NOLIBC89_ARCH_LOONGARCH	0
#define	NOLIBC89_ARCH_SPARC	0
#define	NOLIBC89_ARCH_M68K	0
#define	NOLIBC89_ARCH_SH	0
#define	NOLIBC89_ARCH_OPENRISC	0
#define	NOLIBC89_ARCH_PARISC	0

#if defined(__x86_64__) || defined(__i386__) || defined(__i486__) || defined(__i586__) || defined(__i686__)

#undef	NOLIBC89_ARCH_X86
#define	NOLIBC89_ARCH_X86	1

#elif defined(__ARM_EABI__)

#undef	NOLIBC89_ARCH_ARM
#define	NOLIBC89_ARCH_ARM	1

#elif defined(__aarch64__)

#undef	NOLIBC89_ARCH_ARM64
#define	NOLIBC89_ARCH_ARM64	1

#elif defined(__mips__)

#undef	NOLIBC89_ARCH_MIPS
#define	NOLIBC89_ARCH_MIPS	1

#elif defined(__powerpc__)

#undef	NOLIBC89_ARCH_POWERPC
#define	NOLIBC89_ARCH_POWERPC	1

#elif defined(__riscv)

#undef	NOLIBC89_ARCH_RISCV
#define	NOLIBC89_ARCH_RISCV	1

#elif defined(__s390x__)

#undef	NOLIBC89_ARCH_S390
#define	NOLIBC89_ARCH_S390	1

#elif defined(__loongarch__)

#undef	NOLIBC89_ARCH_LOONGARCH
#define	NOLIBC89_ARCH_LOONGARCH	1

#elif defined(__sparc__)

#undef	NOLIBC89_ARCH_SPARC
#define NOLIBC89_ARCH_SPARC	1

#elif defined(__m68k__)

#undef	NOLIBC89_ARCH_M68K
#define NOLIBC89_ARCH_M68K	1

#elif defined(__sh__)

#undef	NOLIBC89_ARCH_SH
#define	NOLIBC89_ARCH_SH	1

#elif defined(__or1k__)

#undef	NOLIBC89_ARCH_OPENRISC
#define	NOLIBC89_ARCH_OPENRISC	1

#elif defined(__hppa__)

#undef	NOLIBC89_ARCH_PARISC
#define	NOLIBC89_ARCH_PARISC	1

#endif

#endif
