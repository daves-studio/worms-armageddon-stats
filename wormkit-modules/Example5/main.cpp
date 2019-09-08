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

using address_t = long unsigned int;

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

void patch_call(address_t call_addr, address_t hook) {
  uint32_t relative = (hook - call_addr) - 5;

  writeMemory(call_addr, "\xE8", 1);         // call
  writeMemory(call_addr + 1, &relative, 4);  // relative address to hook func
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
        patch_call(0x004e67bb, (address_t)&hook_message);
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
