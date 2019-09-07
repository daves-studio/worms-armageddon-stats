#define _DEBUG
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <fstream>
using namespace std;
#include <iostream>
#include <sstream>
#include "guicon.h"
#include "hooks.hpp"

bool bInLogMode = false;

extern "C" {
void on_message(char* msg) { printf("message: %s\n", msg); }
}

static bool writeMemory(DWORD_PTR dwAddress, const void* cpvPatch,
                        DWORD dwSize) {
  DWORD dwProtect;
  if (VirtualProtect((void*)dwAddress, dwSize, PAGE_READWRITE,
                     &dwProtect))  // Unprotect the memory
    memcpy((void*)dwAddress, cpvPatch, dwSize);
  else
    return false;

  return VirtualProtect((void*)dwAddress, dwSize, dwProtect,
                        new DWORD);  // Reprotect the memory
}

static void getWAMode() {
  LPWSTR* szArglist;
  int nArgs;
  int i;

  szArglist = CommandLineToArgvW(GetCommandLineW(), &nArgs);
  if (NULL == szArglist) {
    printf("CommandLineToArgvW failed\n");
    exit(1);
  } else {
    for (i = 0; i < nArgs; i++) {
      if (wcscmp(L"/getlog", szArglist[i]) == 0) {
        bInLogMode = true;
      }
      std::wcout << i << ": " << szArglist[i] << std::endl;
    }
  }

  // Free memory allocated for CommandLineToArgvW arguments.
  LocalFree(szArglist);
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD dwMsg, LPVOID lpReserved) {
  switch (dwMsg) {
    case DLL_PROCESS_ATTACH:
      DisableThreadLibraryCalls(hModule);
#ifndef NDEBUG
      RedirectIOToConsole();
#endif
      getWAMode();

#ifndef NDEBUG
      std::cout << "Generating Logs: " << bInLogMode << std::endl;
      MessageBox(0, "Dll Injection Successful! (DEBUG)", "Dll Injector",
                 MB_ICONEXCLAMATION | MB_OK);
#endif

      if (bInLogMode) {
        int hookAddress = reinterpret_cast<int>(&hook_message);
        int relative = (hookAddress - 0x004e67bb) - 5;

        writeMemory(0x004e67bb, "\xE8", 1);     // call
        writeMemory(0x004e67bc, &relative, 4);  // relative address to hook func
      }
#ifndef NDEBUG
      MessageBox(0, "Dll Injection Successful! (DEBUG)", "Dll Injector",
                 MB_ICONEXCLAMATION | MB_OK);
#endif
      return TRUE;
      break;
    case DLL_PROCESS_DETACH:
      return TRUE;
      break;
  }
  return TRUE;
}
