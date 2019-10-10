.section .text

.global _hook_message
.global _on_message

_hook_message:
  pushal

  // 32 for pushal + 4 for return address
  movl 36(%esp), %eax
  push %eax
  call _on_message

  addl $4, %esp
  popal

  push $0x0051bc00
  ret

.global _hook_start
.global _on_start

_hook_start:
  pushal

  push %eax
  call _on_start

  addl $4, %esp
  popal

  push $0x004e6750
  ret

.global _hook_death
.global _on_death

_hook_death:
  pushal

  push %eax
  call _on_death

  addl $4, %esp
  popal

  push $0x004e6750
  ret

.global _hook_drown
.global _on_drown

_hook_drown:
  pushal

  push %eax
  call _on_drown

  addl $4, %esp
  popal

  push $0x004e6750
  ret

.global _hook_construct
.global _on_construct

_hook_construct:
  pushal

  // 32 for pushal + 4 for return address
  movl 36(%esp), %eax
  push %eax
  call _on_construct

  addl $4, %esp
  popal

  push $0x0052da40
  ret

.global _hook_play_sound
.global _on_play_sound

_hook_play_sound:
  pushal

  // 32 for pushal + 4 for return address
  movl 36(%esp), %eax
  push %eax
  call _on_play_sound

  addl $4, %esp
  popal

  // instructions replaced/skipped over by patch
  mov    0x34(%ecx),%eax
  mov    0x8078(%eax),%edx

  push $0x0051aa79
  ret

.global _hook_frame
.global _on_frame

_hook_frame:
  pushal

  call _on_frame

  popal

  push $0x00509160
  ret

.global _hook_loop
.global _on_loop

_hook_loop:
  pushal

  call _on_loop

  popal

  // instructions replaced/skipped over by patch
  push %ebp
  mov %esp,%ebp
  and $0xfffffff8,%esp

  push $0x004ff8e6
  ret
