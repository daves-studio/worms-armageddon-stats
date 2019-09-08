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
