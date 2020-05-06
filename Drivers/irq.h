#pragma once
#include "../common.h"
extern void irq_install_handler(int irq, void (*handler)(struct regs *r));
extern void irq_uninstall_handler(int irq);
extern void irq_install();
