#ifndef DEBUG_H
#define DEBUG_H

/* Crash the game and show an error on the OLED display. */
void crash(const char* reason);

/* Enable showing crashes on the OLED display. */
void debug_enable_visual_crashes();
/* Called every frame for debug helpers */
void debug_on_frame();
/* Called if SW4 == 1 */
void debug_draw_overlay();

#endif