#include "sprite_animation_manager.h"

void initialize_object(object_2d_info_t* object, C2D_Sprite* sprites, const char* filename, const sprite_pivot_t pivot, const sprite_position_t position, const float rotation, uint64_t animation_refresh_time, bool loop_once)
{
    // Load graphics
    object->spritesheet = C2D_SpriteSheetLoad(filename);
    if (object->spritesheet == NULL) {
        svcBreak(USERBREAK_PANIC);
    }
    
    // Load sprites
    object->object_sprite = sprites;

    // Store number of sprites information
    object->frame_info.num_of_sprites = C2D_SpriteSheetCount(object->spritesheet);
    // Set the first sprite to the beginning of the spritesheet
    object->frame_info.current_frame_index = 0;

    // Set the object position
    object->position.x = position.x;
    object->position.y = position.y;

    // Set the object velocity to 0
    object->position_velocity.dx = 0.0f;
    object->position_velocity.dy = 0.0f;

    // Set the rotation velocity to 0
    object->rotation_velocity = 0.0f;

    // Set the object pivot
    object->pivot.x = pivot.x;
    object->pivot.y = pivot.y;

    // Set the object rotation
    object->rotation = rotation;

    // Set the object looping
    object->frame_info.loop_once = loop_once;

    for (size_t index = 0; index < object->frame_info.num_of_sprites; index++) {
        // Load the spriteheet to each sprites (or frames)
        C2D_SpriteFromSheet(&object->object_sprite[index], object->spritesheet, index);
        // Set the pivot, position, and rotation of the object
        C2D_SpriteSetCenter(&object->object_sprite[index], object->pivot.x, object->pivot.y);
        C2D_SpriteSetPos(&object->object_sprite[index], object->position.x, object->position.y);
        C2D_SpriteSetRotationDegrees(&object->object_sprite[index], object->rotation);
    }

    // Set initial value
    object->refresh_info.start = osGetTime();
    object->refresh_info.elapsed = 0;

    // Set a desired sprite refresh time (ms)
    if (animation_refresh_time < ANIMATION_REFRESH_TIME_MIN){
        object->refresh_info.refresh_time = ANIMATION_REFRESH_TIME_MIN;
    } else {
        object->refresh_info.refresh_time = animation_refresh_time;
    }
}

void deinitialize_object(object_2d_info_t* object)
{
    // Free 2D object graphic
    C2D_SpriteSheetFree(object->spritesheet);
}

void update_object(object_2d_info_t* object)
{
    object->position.x += object->position_velocity.dx;
    object->position.y += object->position_velocity.dy;

    object->rotation += object->rotation_velocity;

    for (size_t index = 0; index < object->frame_info.num_of_sprites; index++) {
        // Set the position, and rotation of the object
        C2D_SpriteSetPos(&object->object_sprite[index], object->position.x, object->position.y);
        C2D_SpriteSetRotationDegrees(&object->object_sprite[index], object->rotation);
    }
}

void draw_sprite_animation(object_2d_info_t* object)
{
    // Get an elapsed time
    object->refresh_info.stop = osGetTime();
    object->refresh_info.elapsed = object->refresh_info.stop - object->refresh_info.start;

    // Check the elapsed time which is greater than or equal to the refresh time
    if (object->refresh_info.elapsed >= object->refresh_info.refresh_time) {
        // Update next sprite
        if (object->frame_info.loop_once == false) {
            object->frame_info.current_frame_index = (object->frame_info.current_frame_index + 1) % object->frame_info.num_of_sprites;
        } else {
            if (object->frame_info.current_frame_index < object->frame_info.num_of_sprites - 1) {
                object->frame_info.current_frame_index++;
            }
        }
        object->refresh_info.start = osGetTime();
        C2D_DrawSprite(&object->object_sprite[object->frame_info.current_frame_index]);
    } else {
        // Draw current sprite
        C2D_DrawSprite(&object->object_sprite[object->frame_info.current_frame_index]);
    }
}