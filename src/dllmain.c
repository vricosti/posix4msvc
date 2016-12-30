#define WIN32_LEAN_AND_MEAN
#include <windows.h>


HINSTANCE	        g_hInstance = NULL;
void init(void);
void cleanup(void);


BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
        GetModuleHandleEx(0, NULL, &g_hInstance);
        //init();
		break;
	case DLL_THREAD_ATTACH:
		break;

	case DLL_THREAD_DETACH:
		break;

	case DLL_PROCESS_DETACH:
        cleanup();
		break;
	}
	return TRUE;
}