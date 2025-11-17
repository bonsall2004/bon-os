global gdt_load

gdt_load:
  lgdt [rdi]

  mov ax, 0x10
  mov ds, ax
  mov es, ax
  mov fs, ax
  mov gs, ax
  mov ss, ax

  ; Far jump to reload CS
  push 0x08
  lea rax, [rel reload]
  push rax
  retfq

reload:
    ret
