#pragma once

void terminal_init();
void terminal_destroy();
void terminal_flush();
void terminal_advance_state_single(char c);
void terminal_advance_state(char* s);
void terminal_set_auto_flush(unsigned int auto_flush);