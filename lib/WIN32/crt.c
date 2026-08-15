#include <windows.h>
#include <ae2f/cc/callconv.h>
#include <ae2f/cc/qualifier.h>
#include <ae2f/cc/branches/naive.h>

extern int main(int c_argc, char* ae2f_restrict * ae2f_restrict rd_argv);

/*
 * Freestanding compiler-support routines.
 *
 * Even under -ffreestanding -nostdlib the compiler may auto-emit calls to
 * memset/memcpy/memmove/memcmp (e.g. to zero/copy structs and arrays). On the
 * LINUX target these come from nolibc's <string.h>; on WIN32 there is no libc,
 * so they must be provided here or every such call resolves to a bogus address
 * and faults at runtime. Implementations mirror nolibc (tools/include/nolibc).
 */

void *memset(void *dst, int b, size_t len);
__attribute__((weak, used))
void *memset(void *dst, int b, size_t len)
{
	char *p = (char *)dst;

	while (len--) {
		/* prevent the compiler from turning this into a memset() call */
		__asm__ volatile("");
		*(p++) = (char)b;
	}
	return dst;
}

void *memcpy(void *dst, const void *src, size_t len);
__attribute__((weak, used))
void *memcpy(void *dst, const void *src, size_t len)
{
	size_t pos = 0;

	while (pos < len) {
		__asm__ volatile("");
		((char *)dst)[pos] = ((const char *)src)[pos];
		pos++;
	}
	return dst;
}

void *memmove(void *dst, const void *src, size_t len);
__attribute__((weak, used))
void *memmove(void *dst, const void *src, size_t len)
{
	size_t dir, pos;

	pos = len;
	dir = (size_t)-1;

	if (dst < src) {
		pos = (size_t)-1;
		dir = 1;
	}

	while (len) {
		__asm__ volatile("");
		pos += dir;
		((char *)dst)[pos] = ((const char *)src)[pos];
		len--;
	}
	return dst;
}

int memcmp(const void *s1, const void *s2, size_t n);
__attribute__((weak, used))
int memcmp(const void *s1, const void *s2, size_t n)
{
	size_t ofs = 0;
	int c1 = 0;

	while (ofs < n &&
	       !(c1 = ((const unsigned char *)s1)[ofs] - ((const unsigned char *)s2)[ofs])) {
		ofs++;
	}
	return c1;
}

enum {
	R_GOOD,
	R_CMD_ARGV_W,
	R_WC_MB,
	ARGV_A_MALLOC,
	ARGVELM_A_MALLOC
};

ae2f_stdcall void mainCRTStartup(void);
ae2f_stdcall void mainCRTStartup(void) {

#define	retval(c)	{ ret = (UINT)(c); goto END; }
	int argc = 0;
	LPWSTR* argvW = 0;
	char* ae2f_restrict * ae2f_restrict argvA = 0;
	int i = 0;
	UINT ret = R_GOOD;

	argvW = CommandLineToArgvW(GetCommandLineW(), &argc);
	ae2f_ifezerr(argvW)		retval(-R_CMD_ARGV_W);
	ae2f_ifnezerr_strict(argc < 0)	retval(-R_CMD_ARGV_W);

	ae2f_ifezerr((argvA = (char* ae2f_restrict * ae2f_restrict)LocalAlloc(
					LPTR
					, argc * sizeof(char*))
				))
		retval(-ARGV_A_MALLOC);

	for(i = argc; i --> 0; ) {
		int len = WideCharToMultiByte(
				CP_UTF8
				, 0
				, argvW[i]
				, -1
				, 0
				, 0
				, 0
				, 0
				);

		ae2f_ifezerr((argvA[i] = (char*)LocalAlloc(LPTR, len)))
			retval(ARGVELM_A_MALLOC);

		WideCharToMultiByte(
				CP_UTF8
				, 0
				, argvW[i]
				, -1
				, argvA[i]
				, len
				, NULL
				, NULL
				);
	}

	retval(main(argc, argvA));

END:
	ae2f_ifneznrm(argvW) { LocalFree(argvW); }
	ae2f_ifneznrm(argvA) {
		int j = argc; ++i;

		while(j --> i) {
			ae2f_ifneznrm(argvA[j]) (void)LocalFree((void*)argvA[j]);
		}

		(void)LocalFree((void*)argvA);
	}

	ExitProcess(ret);
}
