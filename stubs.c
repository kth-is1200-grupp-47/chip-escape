/* From: https://canvas.kth.se/courses/31596/pages/chipkit-slash-mcb32-toolchain-faq */
void *stdin, *stdout, *stderr;

/* Non-Maskable Interrupt; something bad likely happened, so hang */
void _nmi_handler() {
	for(;;);
}

/* This function is called upon reset, before .data and .bss is set up */
void _on_reset() {

}

/* This function is called before main() is called, you can do setup here */
void _on_bootstrap() {

}