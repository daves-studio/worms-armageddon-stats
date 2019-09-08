#include <windows.h>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
using namespace std;
#include "guicon.h"
#include <iostream>
#include <sstream>

#include <sstream>
#include <iostream>
#include <string>

// Print Memory Address (DWORD addr) as Hex
// std::ostringstream ss;
// ss << std::hex << addr;
// const std::string tmp = ss.str();
// const char* cstr = tmp.c_str();
// MessageBox(0, cstr, "Dll Injector", MB_ICONEXCLAMATION | MB_OK);

bool bWantsExit;
bool bInLogMode = false;

DWORD retJMP = 0x4E67B8;

static void __stdcall printThingy(char* text) {
    std::cout << text << std::endl;
	MessageBox(0, text, "Dll Injector", MB_ICONEXCLAMATION | MB_OK);
}

// Mid Function Hook - adapted from
// - https://github.com/Hattiwatti/wkCrateEditor/blob/master/DllMain.cpp

static void __declspec(naked) hookFunc()
{
    char** esp_;
#if defined(_MSC_VER)
	__asm {
		push eax				// WA.exe+E67B3 - 50                    - push eax
		mov eax, [esp + 0x20]	// WA.exe+E67B4 - 8B 44 24 20           - mov eax,[esp+20]
		mov esp_, esp;
		pushad
	}
#else
    asm("push %eax");
	asm("mov 0x20(%esp),%eax");
    asm("mov %%esp,%0"
    : "=m"(esp_));
	asm("pushal;");
#endif // _MSC_VER

	printThingy(*esp_);

#if defined(_MSC_VER)
	__asm {
		popad
		jmp [retJMP]
	}
#else
    asm("popal;");
	asm("jmp *(%0)"
		:/*no output*/
		: "m"(retJMP) : "memory");

#endif // _MSC_VER
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
      MessageBox(0, "Ready to Hook (DEBUG)", "Dll Injector", MB_ICONEXCLAMATION | MB_OK);
#else
      MessageBox(0, "Ready to Hook! ", "Dll Injector", MB_ICONEXCLAMATION | MB_OK);
#endif // _DEBUG

    if(bInLogMode) {
        int hookAddress = reinterpret_cast<int>(&hookFunc);
#if defined(_MSC_VER)
        int relative = hookAddress - 0x4E67B8;
#else
        // +6 Due to the following extra operations in hookFunction due to gcc ignoring naked attribute
        // (i.e. it generates prolog and epilog code)
        // wkExample4.Z19RedirectIOToConsolev+2ED - 55                    - push ebp
        // wkExample4.Z19RedirectIOToConsolev+2EE - 89 E5                 - mov ebp,esp
        // wkExample4.Z19RedirectIOToConsolev+2F0 - 83 EC 28              - sub esp,28 { 40 }
        int relative = hookAddress - 0x4E67B8 + 6;
#endif // _MSC_VER

        std::ostringstream ss;
        ss << std::hex << hookAddress;
        const std::string tmp = ss.str();
        const char* cstr = tmp.c_str();
        MessageBox(0, cstr, "Dll Injector", MB_ICONEXCLAMATION | MB_OK);

		// WA.exe+E67B3 - 50                    - push eax
		// WA.exe+E67B4 - 8B 44 24 20           - mov eax,[esp+20]

        // 7 bytes overwrite
        writeMemory(0x4E67B3, "\xE9", 1); // jmp
        writeMemory(0x4E67B4, &relative, 4); // relative address to hookFunc
    }
#ifdef _DEBUG
      MessageBox(0, "Dll Injection Complete! (DEBUG)", "Dll Injector", MB_ICONEXCLAMATION | MB_OK);
#else
      MessageBox(0, "Dll Injection Complete! ", "Dll Injector", MB_ICONEXCLAMATION | MB_OK);
#endif // _DEBUG
      CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)MyThread, NULL, 0, 0 );
      return TRUE;
   case DLL_PROCESS_DETACH:
      bWantsExit = true;
      return TRUE;
   }
   return TRUE;
}
