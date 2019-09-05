#include <windows.h>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
using namespace std;
#include "guicon.h"
#include <iostream>

#include <sstream>

bool bWantsExit;

#ifdef _DEBUG
void printArgs() {
    LPWSTR *szArglist;
    int nArgs;
    int i;

    szArglist = CommandLineToArgvW(GetCommandLineW(), &nArgs);
    if( NULL == szArglist ) {
        printf("CommandLineToArgvW failed\n");
    } else {
        for( i=0; i<nArgs; i++) std::wcout << i << ": " << szArglist[i] << std::endl;
    }

    // Free memory allocated for CommandLineToArgvW arguments.
    LocalFree(szArglist);
}
#endif // _DEBUG

DWORD WINAPI MyThread()
{
#ifdef _DEBUG
    RedirectIOToConsole();
    fprintf(stdout, "Test output to stdout\n");
    fprintf(stderr, "Test output to stderr\n");
    printArgs();
#endif // _DEBUG
   while( !bWantsExit )
   {
#ifdef _DEBUG
      printf("Sleepy Timer\n");
#endif // _DEBUG
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

#ifdef _DEBUG
      MessageBox(0, "Dll Injection Successful! (DEBUG)", "Dll Injector", MB_ICONEXCLAMATION | MB_OK);
#else
      MessageBox(0, "Dll Injection Successful! ", "Dll Injector", MB_ICONEXCLAMATION | MB_OK);
#endif // _DEBUG
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
