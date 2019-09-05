#include <windows.h>

BOOL APIENTRY DllMain( HMODULE hModule,DWORD ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
        case DLL_PROCESS_ATTACH:
        /* The DLL is being loaded for the first time by a given process.
        Perform per-process initialization here.  If the initialization
        is successful, return TRUE; if unsuccessful, return FALSE. */
			MessageBoxA(0 , "DLL has been attached !" , "DLL Bot" , MB_ICONEXCLAMATION | MB_OK);
            break;
    }
    return TRUE;
}
