// Simple citro2d sprite drawing example
#include <citro2d.h>
#include <3ds/svc.h>
#include <3ds/types.h>

#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "sprite_animation_manager.h"

#define MAX_SPRITES   14
#define SCREEN_WIDTH  400
#define SCREEN_HEIGHT 240

static object_2d_info_t reimu_object;
static C2D_Sprite reimu_sprite[MAX_SPRITES];
static sprite_pivot_t reimu_pivot = {0.0f, 0.0f};
static sprite_position_t reimu_position = {0.0f, 0.0f};
static float reimu_roation = 0.0f;
static uint64_t sprite_refresh_ms_time = 33;

int main(int argc, char* argv[]) 
{
	// Init libs
	romfsInit();
	gfxInitDefault();
	C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);
	C2D_Init(C2D_DEFAULT_MAX_OBJECTS);
	C2D_Prepare();
	consoleInit(GFX_BOTTOM, NULL);

	// Create screens
	C3D_RenderTarget* top = C2D_CreateScreenTarget(GFX_TOP, GFX_LEFT);

	// Load graphics and initialize player sprites
	initialize_object(&reimu_object, \
					  reimu_sprite,	\
					  "romfs:/gfx/sprites.t3x", \
					  reimu_pivot, \
					  reimu_position, \
					  reimu_roation, \
					  sprite_refresh_ms_time, \
					  false);

	// Main loop
	while (aptMainLoop())
	{
		hidScanInput();

		// Respond to user input
		u32 kDown = hidKeysDown();
		if (kDown & KEY_START) {
			break; // break in order to return to hbmenu
		}

		// Congfigure a sprite refresh time [ms]
		u32 kHeld = hidKeysHeld();
		if ((kHeld & KEY_A) && reimu_object.refresh_info.refresh_time < 1000) {
			reimu_object.refresh_info.refresh_time++;
		}
		if ((kHeld & KEY_B) && reimu_object.refresh_info.refresh_time > ANIMATION_REFRESH_TIME_MIN) {
			reimu_object.refresh_info.refresh_time--;
		}
		if (kHeld & KEY_X) {
			reimu_object.frame_info.loop_once = true;
		}
		if (kHeld & KEY_Y) {
			reimu_object.frame_info.loop_once = false;
		}
		if ((kHeld & KEY_LEFT)) {
			reimu_object.position_velocity.dx = -0.4f;
			reimu_object.position_velocity.dy = 0.0f;
		}
		if ((kHeld & KEY_RIGHT)) {
			reimu_object.position_velocity.dx = 0.4f;
			reimu_object.position_velocity.dy = 0.0f;
		}
		if ((kHeld & KEY_UP)) {
			reimu_object.position_velocity.dx = 0.0f;
			reimu_object.position_velocity.dy = -0.4f;
		}
		if ((kHeld & KEY_DOWN)) {
			reimu_object.position_velocity.dx = 0.0f;
			reimu_object.position_velocity.dy = 0.4f;
		}
		if (kHeld == 0) {
			reimu_object.position_velocity.dx = 0.0f;
			reimu_object.position_velocity.dy = 0.0f;
		}

		// Print debug messages on the bottom screen
		printf("\x1b[2;1HCPU:     %6.2f%%\x1b[K", C3D_GetProcessingTime()*6.0f);
		printf("\x1b[3;1HGPU:     %6.2f%%\x1b[K", C3D_GetDrawingTime()*6.0f);
		printf("\x1b[4;1HCmdBuf:  %6.2f%%\x1b[K", C3D_GetCmdBufUsage()*100.0f);

		printf("\x1b[6;1HTime elapsed:  %lld ms\x1b[K", reimu_object.refresh_info.elapsed);
		printf("\x1b[7;1HSprite refresh time:  %lld ms\x1b[K", reimu_object.refresh_info.refresh_time);
		printf("\x1b[8;1HKey held:  0x%lx\x1b[K", hidKeysHeld());
		printf("\x1b[9;1HKey down:  0x%lx\x1b[K", hidKeysDown());

		// Render the scene
		update_object(&reimu_object);
		C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
		C2D_TargetClear(top, C2D_Color32f(0.0f, 0.0f, 0.0f, 1.0f));
		C2D_SceneBegin(top);
		draw_sprite_animation(&reimu_object);

		C3D_FrameEnd(0);
	}

	// Delete graphics
	deinitialize_object(&reimu_object);
	
	// Deinit libs
	C2D_Fini();
	C3D_Fini();
	gfxExit();
	romfsExit();

	return 0;
}
