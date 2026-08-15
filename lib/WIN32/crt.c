#include <windows.h>
#include <ae2f/cc/callconv.h>
#include <ae2f/cc/qualifier.h>
#include <ae2f/cc/branches/naive.h>

extern int main(int c_argc, char* ae2f_restrict * ae2f_restrict rd_argv);

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
