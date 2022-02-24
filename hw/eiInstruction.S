# This code runs the ei instruction to enable interrupts

.global enable_interrupt

.text

enable_interrupt:
    ei
    jr  $ra
    nop