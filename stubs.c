#include "debug.h"

void *stdin, *stdout, *stderr;

/* Non-Maskable Interrupt; something bad likely happened, so hang */
void _nmi_handler() {
	crash("Non-Maskable Interrupt");
}

/* This function is called upon reset, before .data and .bss is set up */
void _on_reset() {

}

/* This function is called before main() is called, you can do setup here */
void _on_bootstrap() {

}