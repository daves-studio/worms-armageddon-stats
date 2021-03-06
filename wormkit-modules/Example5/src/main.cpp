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
#include "json11.hpp"
using Json = json11::Json;

using address_t = long unsigned int;

typedef int (**fprintf_ptr)(FILE* stream, const char* format, ...);
fprintf_ptr _fprintf = (fprintf_ptr)0x0061c510;

FILE** log_file = (FILE**)0x008fc8f8;

bool bInLogMode = false;

// enable debug printing
const bool debug_print = false;
// enable message box before and after hooks
const bool debug_msgbox = false;
// break into debugger after hooking
const bool debug_break = false;
// call RedirectIOToConsole?
const bool redirect_io = false;

extern "C" {

game* g = NULL;

void on_construct(game* constructed_game) {
  if (debug_print) printf("on_construct\n");
  g = constructed_game;
}

int get_frames() {
  if (g != NULL)
    return g->holds_time->frames;
  else
    return 0;
}

double get_time() {
  return (double)get_frames() / 50.0;
}

void write_log_json(const Json& json) {
  std::string json_str = json.dump();
  (*_fprintf)(*log_file, "%s\n", json_str.c_str());
}

void on_message(char* msg) {
  if (debug_print) printf("message at %f: %s\n", get_time(), msg);

  if (bInLogMode)
    write_log_json(Json::object{
        {"type", "message"},
        {"frames", get_frames()},
        {"message", msg},
    });
}

void on_start(worm_info* wi) {
  if (debug_print)
    printf("start: worm: %s, team: %s, time: %f\n", wi->name, wi->team,
           get_time());

  if (bInLogMode)
    write_log_json(Json::object{
        {"type", "start"},
        {"frames", get_frames()},
        {"worm", wi->name},
        {"team", wi->team},
    });
}

void on_death(worm_info* wi) {
  if (debug_print)
    printf("death: worm: %s, team: %s, time: %f\n", wi->name, wi->team,
           get_time());

  if (bInLogMode)
    write_log_json(Json::object{
        {"type", "death"},
        {"frames", get_frames()},
        {"worm", wi->name},
        {"team", wi->team},
    });
}

void on_drown(worm_info* wi) {
  if (debug_print)
    printf("drown: worm: %s, team: %s, time: %f\n", wi->name, wi->team,
           get_time());

  if (bInLogMode)
    write_log_json(Json::object{
        {"type", "drown"},
        {"frames", get_frames()},
        {"worm", wi->name},
        {"team", wi->team},
    });
}

void on_rope_attach() {
  if (debug_print) printf("rope attach: time: %f\n", get_time());

  if (bInLogMode)
    write_log_json(Json::object{
        {"type", "rope_attach"},
        {"frames", get_frames()},
    });
}

void on_play_sound(int sound) {
  if (debug_print) printf("sound: sound: %d, time: %f\n", sound, get_time());

  if (bInLogMode)
    write_log_json(Json::object{
        {"type", "sound"},
        {"frames", get_frames()},
        {"sound_id", sound},
    });
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
  assert(szArglist);
  for (i = 0; i < nArgs; i++) {
    if (wcscmp(L"/getlog", szArglist[i]) == 0) {
      bInLogMode = true;
    }
  }

  // Free memory allocated for CommandLineToArgvW arguments.
  LocalFree(szArglist);
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD dwMsg, LPVOID lpReserved) {
  switch (dwMsg) {
    case DLL_PROCESS_ATTACH:
      DisableThreadLibraryCalls(hModule);
      if (redirect_io) RedirectIOToConsole();

      getWAMode();

      if (debug_print)
        std::cout << "Generating Logs: " << bInLogMode << std::endl;
      if (debug_msgbox)
        MessageBox(0, "Dll Injection Successful! (DEBUG)", "Dll Injector",
                   MB_ICONEXCLAMATION | MB_OK);

      if (bInLogMode) {
        patch_call(0x004e67bb, (address_t)&hook_message);
        patch_call(0x004ea1ac, (address_t)&hook_start);
        patch_call(0x004ee4b1, (address_t)&hook_death);
        patch_call(0x004edaf7, (address_t)&hook_drown);
        patch_call(0x004fe0d1, (address_t)&hook_construct);
        patch_jmp(0x0051aa70, (address_t)&hook_play_sound);
      }

      if (debug_break) __asm__("int3");
      if (debug_msgbox)
        MessageBox(0, "Dll Injection Successful! (DEBUG)", "Dll Injector",
                   MB_ICONEXCLAMATION | MB_OK);
      return TRUE;
      break;
    case DLL_PROCESS_DETACH:
      return TRUE;
      break;
  }
  return TRUE;
}
