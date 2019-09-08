#pragma once

extern "C" {
typedef struct {
  char _unknown[0x300];
  char name[0x11];
  char team[0x11];
} worm_info;
}
