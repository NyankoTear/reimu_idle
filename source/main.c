// Simple citro2d sprite drawing example
#include <citro2d.h>
#include <3ds/svc.h>
#include <3ds/types.h>

#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_SPRITES   14
#define SCREEN_WIDTH  400
#define SCREEN_HEIGHT 240

static C2D_SpriteSheet spriteSheet;
static C2D_Sprite reimu_sprite[MAX_SPRITES];
static uint64_t sprite_refresh_ms_time = 30;

//---------------------------------------------------------------------------------
static void initialize_reimu_sprite(C2D_Sprite* sprites) {
//---------------------------------------------------------------------------------	
	for (size_t index = 0; index < MAX_SPRITES; index++) {
		C2D_SpriteFromSheet(&sprites[index], spriteSheet, index);
		C2D_SpriteSetCenter(&sprites[index], 0.0f, 0.0f);
		C2D_SpriteSetPos(&sprites[index], 0, 0);
		C2D_SpriteSetRotation(&sprites[index], 0);
	}
}
//---------------------------------------------------------------------------------

//---------------------------------------------------------------------------------
int main(int argc, char* argv[]) {
//---------------------------------------------------------------------------------
	// Eslapsed time calculation values
	uint64_t start;
	uint64_t stop;
	uint64_t ms_time_elapsed = 0;

	// Animation loop sprite counter value
	int current_frame_index = 0;

	// Init libs
	romfsInit();
	gfxInitDefault();
	C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);
	C2D_Init(C2D_DEFAULT_MAX_OBJECTS);
	C2D_Prepare();
	consoleInit(GFX_BOTTOM, NULL);

	// Create screens
	C3D_RenderTarget* top = C2D_CreateScreenTarget(GFX_TOP, GFX_LEFT);

	// Load graphics
	spriteSheet = C2D_SpriteSheetLoad("romfs:/gfx/sprites.t3x");
	if (spriteSheet == NULL) {
		svcBreak(USERBREAK_PANIC);
	}

	// Initialize player sprites
	initialize_reimu_sprite(reimu_sprite);

	// Main loop
	while (aptMainLoop())
	{
		start = osGetTime();
		hidScanInput();

		// Respond to user input
		u32 kDown = hidKeysDown();
		if (kDown & KEY_START) {
			break; // break in order to return to hbmenu
		}

		// Congfigure a sprite refresh time [ms]
		u32 kHeld = hidKeysHeld();
		if ((kHeld & KEY_UP) && sprite_refresh_ms_time < 1000) {
			sprite_refresh_ms_time++;
		}
		if ((kHeld & KEY_DOWN) && sprite_refresh_ms_time > 3) {
			sprite_refresh_ms_time--;
		}
		
		// Print debug messages on the bottom screen
		printf("\x1b[2;1HCPU:     %6.2f%%\x1b[K", C3D_GetProcessingTime()*6.0f);
		printf("\x1b[3;1HGPU:     %6.2f%%\x1b[K", C3D_GetDrawingTime()*6.0f);
		printf("\x1b[4;1HCmdBuf:  %6.2f%%\x1b[K", C3D_GetCmdBufUsage()*100.0f);

		// Calculate elasped time [ms]
		stop = osGetTime();
		ms_time_elapsed += (stop - start);

		printf("\x1b[6;1HTime elapsed:  %lld ms\x1b[K", ms_time_elapsed);
		printf("\x1b[7;1HSprite refresh time:  %lld ms\x1b[K", sprite_refresh_ms_time);

		if (ms_time_elapsed >= sprite_refresh_ms_time) {
			ms_time_elapsed -= sprite_refresh_ms_time;
			
			// Making an animation loop sprite counter
			// This will indecate 0, 1, 2, ... 13, 0, 1, 2, ... 13, 0, ... sequentially.
			current_frame_index = (current_frame_index + 1) % MAX_SPRITES;

			// Render the scene
			C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
			C2D_TargetClear(top, C2D_Color32f(0.0f, 0.0f, 0.0f, 1.0f));
			C2D_SceneBegin(top);
			
			// Draw the player animation
			C2D_DrawSprite(&reimu_sprite[current_frame_index]);
		}

		C3D_FrameEnd(0);
	}

	// Delete graphics
	C2D_SpriteSheetFree(spriteSheet);

	// Deinit libs
	C2D_Fini();
	C3D_Fini();
	gfxExit();
	romfsExit();

	return 0;
}
