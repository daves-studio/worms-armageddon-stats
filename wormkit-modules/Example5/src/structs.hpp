#pragma once

extern "C" {
typedef struct {
  char _unknown[0x300];
  char name[0x11];
  char team[0x11];
} worm_info;

typedef struct {
  char _unknown[0x558];
  int frames;
} holds_time;

typedef struct {
  char _unknown[0x34];
  holds_time *holds_time;
} game;

}
