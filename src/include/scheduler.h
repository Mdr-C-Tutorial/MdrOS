#pragma once

#include "isr.h"
#include "pcb.h"

void default_scheduler(registers_t *reg,pcb_t* next);
void scheduler_process(registers_t *reg);