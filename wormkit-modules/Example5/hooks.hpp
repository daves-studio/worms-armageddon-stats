#pragma once

#include "structs.hpp"

extern "C" {
extern void hook_message(void);
extern void hook_start(worm_info *wi);
extern void hook_death(worm_info *wi);
extern void hook_drown(worm_info *wi);
}
