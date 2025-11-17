#include <stdint.h>

uint8_t inb(uint16_t port);
void outb(uint16_t port, uint8_t val);
void cpuid(uint32_t eax_in, uint32_t *eax, uint32_t *ebx, uint32_t *ecx, uint32_t *edx);

typedef struct {
  char vendor[13];
  uint32_t max_basic;
  uint32_t max_extended;
  uint32_t features_ecx;
  uint32_t features_edx;
  uint32_t eax1, ebx1, ecx1, edx1;
} cpu_info_t;
