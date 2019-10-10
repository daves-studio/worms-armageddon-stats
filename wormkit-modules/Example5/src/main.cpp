#define _DEBUG
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <cassert>
#include <fstream>
using namespace std;
#include <iostream>
#include <sstream>
#include "guicon.h"
#include "hooks.hpp"

using address_t = long unsigned int;

typedef int (**fprintf_ptr)(FILE* stream, const char* format, ...);
fprintf_ptr _fprintf = (fprintf_ptr)0x0061c510;

FILE** log_file = (FILE**)0x008fc8f8;

bool bInLogMode = false;

extern "C" {

game* g = NULL;

void on_construct(game* constructed_game) {
  printf("on_construct\n");
  g = constructed_game;
}

double get_time() {
  if (g != NULL)
    return (double)g->holds_time->frames / 50.0;
  else
    return 0.0f;
}

void on_message(char* msg) { printf("message at %f: %s\n", get_time(), msg); }

void on_start(worm_info* wi) {
  printf("start: worm: %s, team: %s, time: %f\n", wi->name, wi->team,
         get_time());
}

void on_death(worm_info* wi) {
  printf("death: worm: %s, team: %s, time: %f\n", wi->name, wi->team,
         get_time());
}

void on_drown(worm_info* wi) {
  printf("drown: worm: %s, team: %s, time: %f\n", wi->name, wi->team,
         get_time());
}

void on_rope_attach() { printf("rope attach: time: %f\n", get_time()); }

void on_play_sound(int sound) {
  printf("sound: sound: %d, time: %f\n", sound, get_time());
  if (sound == 87) on_rope_attach();
}
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

  assert(writeMemory(call_addr, "\xE8", 1));  // call
  assert(writeMemory(call_addr + 1, &relative,
                     4));  // relative address to hook func
}

void patch_jmp(address_t jmp_addr, address_t hook) {
  uint32_t relative = (hook - jmp_addr) - 5;

  writeMemory(jmp_addr, "\xE9", 1);         // jmp
  writeMemory(jmp_addr + 1, &relative, 4);  // relative address to hook func
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
        patch_call(0x004ea1ac, (address_t)&hook_start);
        patch_call(0x004ee4b1, (address_t)&hook_death);
        patch_call(0x004edaf7, (address_t)&hook_drown);
        patch_call(0x004fe0d1, (address_t)&hook_construct);
        patch_jmp(0x0051aa70, (address_t)&hook_play_sound);
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
