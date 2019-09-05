#include <windows.h>

bool bWantsExit;

DWORD WINAPI MyThread()
{
   while( !bWantsExit )
   {
      MessageBox(0, "Sleepy Timer", "Dll Injector", MB_ICONEXCLAMATION | MB_OK);
      Sleep( 4000 );
   }
   return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD dwMsg, LPVOID lpReserved)
{
   switch( dwMsg )
   {
   case DLL_PROCESS_ATTACH:
      MessageBox(0, "Dll Injection Successful! ", "Dll Injector", MB_ICONEXCLAMATION | MB_OK);
      DisableThreadLibraryCalls( hModule );
      bWantsExit = false;
      CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)MyThread, NULL, 0, 0 );
      return TRUE;
   case DLL_PROCESS_DETACH:
      bWantsExit = true;
      return TRUE;
   }
   return TRUE;
}
