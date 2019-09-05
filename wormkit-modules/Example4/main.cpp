#include <windows.h>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
using namespace std;
#include "guicon.h"
#include <iostream>
#include <sstream>

bool bWantsExit;
bool bInLogMode = false;

static void __stdcall printThingy() {
    printf("What What \n");
}

DWORD retJMP = 0x51BC3D;
unsigned my_addr = 0x51BC3D;

static void __declspec(naked) hookFunc()
{
    asm("pushal;");
    printThingy();
    asm("popal;");
    //asm("sub 0x408,%esp");
    asm("mov (%eax),%edx");
    asm("mov 0x08(%edx),%edx");
   // WA.exe+11BC38 - 8B 10                 - mov edx,[eax]
    // WA.exe+11BC3A - 8B 52 08              - mov edx,[edx+08]
    asm("jmp   %c[addr]");
}

static bool writeMemory(DWORD_PTR dwAddress, const void* cpvPatch, DWORD dwSize)
{
  DWORD dwProtect;
  if (VirtualProtect((void*)dwAddress, dwSize, PAGE_READWRITE, &dwProtect)) //Unprotect the memory
    memcpy((void*)dwAddress, cpvPatch, dwSize);
  else
    return false;

  return VirtualProtect((void*)dwAddress, dwSize, dwProtect, new DWORD); //Reprotect the memory
}

static void getWAMode() {
    LPWSTR *szArglist;
    int nArgs;
    int i;

    szArglist = CommandLineToArgvW(GetCommandLineW(), &nArgs);
    if( NULL == szArglist ) {
        printf("CommandLineToArgvW failed\n");
        exit(1);
    } else {
        for( i=0; i<nArgs; i++) {
            if (wcscmp(L"/getlog", szArglist[i]) == 0) {
                bInLogMode = true;
            }
            std::wcout << i << ": " << szArglist[i] << std::endl;
        }
    }

    // Free memory allocated for CommandLineToArgvW arguments.
    LocalFree(szArglist);
}

DWORD WINAPI MyThread()
{
    while( !bWantsExit )
    {

        Sleep( 4000 );
    }
    return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD dwMsg, LPVOID lpReserved)
{
   switch( dwMsg )
   {
   case DLL_PROCESS_ATTACH:
      DisableThreadLibraryCalls( hModule );
      bWantsExit = false;
#ifdef _DEBUG
    RedirectIOToConsole();
#endif // _DEBUG
    getWAMode();

#ifdef _DEBUG
    std::cout << "Generating Logs: " << bInLogMode << std::endl;
#endif // _DEBUG

#ifdef _DEBUG
      MessageBox(0, "Dll Injection Successful! (DEBUG)", "Dll Injector", MB_ICONEXCLAMATION | MB_OK);
#else
      MessageBox(0, "Dll Injection Successful! ", "Dll Injector", MB_ICONEXCLAMATION | MB_OK);
#endif // _DEBUG

    if(bInLogMode) {
        int hookAddress = reinterpret_cast<int>(&hookFunc);
        int relative = hookAddress - 0x51BC37;

        // WA.exe+11BC00 - 81 EC 08040000        - sub esp,00000408 { 1032 }
        // WA.exe+11BC38 - 8B 10                 - mov edx,[eax]
        // WA.exe+11BC3A - 8B 52 08              - mov edx,[edx+08]

        // 7 bytes overwrite
        writeMemory(0x51BC38, "\xE9", 1); // jmp
        writeMemory(0x51BC39, &relative, 4); // relative address to hookFunc
    }
#ifdef _DEBUG
      MessageBox(0, "Dll Injection Successful! (DEBUG)", "Dll Injector", MB_ICONEXCLAMATION | MB_OK);
#else
      MessageBox(0, "Dll Injection Successful! ", "Dll Injector", MB_ICONEXCLAMATION | MB_OK);
#endif // _DEBUG
      CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)MyThread, NULL, 0, 0 );
      return TRUE;
   case DLL_PROCESS_DETACH:
      bWantsExit = true;
      return TRUE;
   }
   return TRUE;
}
