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
