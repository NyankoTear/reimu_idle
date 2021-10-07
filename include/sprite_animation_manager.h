/**
 * @file sprite_animation_manager.h
 * @brief 2D object initialization and drawing on the screen
 */
#ifndef SPRITE_ANIMATION_MANAGER_H
#define SPRITE_ANIMATION_MANAGER_H

#include <citro2d.h>
#include <3ds/svc.h>
#include <3ds/types.h>

#include <stdbool.h>

#define ANIMATION_REFRESH_TIME_MIN  17   //  A minimum of the animation refresh time is 17ms

typedef struct sprite_velocity {
    float dx; ///< Sprite velocity in x direction
    float dy; ///< Sprite velocity in y direction
} sprite_velocity_t;

typedef struct sprite_pivot {
    float x; ///< Sprite x pivot point
    float y; ///< Sprite y pivot point
} sprite_pivot_t;

typedef struct sprite_position {
    float x; ///< Sprite x position in the Cartesian coordinate system
    float y; ///< Sprite y position in the Cartesian coordinate system
} sprite_position_t;

typedef struct sprite_refresh_info {
    uint64_t start; ///< Start time
    uint64_t stop; ///< Lap time
    uint64_t elapsed; ///< Elapsed time (`start` - `stop`)
    uint64_t refresh_time; ///< Next sprite update time [Unit: ms]
} sprite_refresh_info_t;

typedef struct sprite_frame_info {
    int current_frame_index; ///< Current sprite ID number
    size_t num_of_sprites; ///< Number of sprites
    bool loop_once; ///< Sprite animation loop information
} sprite_frame_info_t;

typedef struct object_2d_info {
    C2D_Sprite* object_sprite; ///< 2D object sprite information 
    C2D_SpriteSheet spritesheet; ///< 2D object Spritesheet information
    sprite_position_t position; ///<2D object position information
    sprite_velocity_t position_velocity; ///< 2D object position velocity information
    sprite_pivot_t pivot; ///< 2D object pivot information
    float rotation; ///< 2D object rotation information in degrees
    float rotation_velocity; ///< 2D object rotation velocity information
    sprite_refresh_info_t refresh_info; ///< 2D object refresh information
    sprite_frame_info_t frame_info; ///< 2D object frame information
} object_2d_info_t;

/**
 * @defgroup Initialization Initialization functions
 * @{
 */

/**
 * @brief Initialize 2D object.
 * 
 * @param[in] object 2D object
 * @param[in] sprites sprite array to contain from the spritesheet
 * @param[in] filname file path which contains spritesheet information
 * @param[in] pivot Initial pivot point
 * @param[in] position Initial position
 * @param[in] rotation Initial rotation
 * @param[in] animation_refresh_time Sprite animation refresh time [Unit: ms]
 * @param[in] loop_once Play sprite animation loop once or forever
 * @returns None
 */
void initialize_object(object_2d_info_t* object, C2D_Sprite* sprites, const char* filename, const sprite_pivot_t pivot, const sprite_position_t position, const float rotation, uint64_t animation_refresh_time, bool loop_once);

/**
 * @brief Deinitialize 2D object.
 * 
 * @param[in] object 2D object
 * @returns None
 */
void deinitialize_object(object_2d_info_t* object);

/**
 * Initialization functions group end
 * @}
 */

/**
 * @defgroup Update Update functions
 * @{ 
 */

/**
 * @brief Update object pose.
 * 
 * @param[in] object 2D object
 * @returns None
 */
void update_object(object_2d_info_t* object);

/**
 * @brief Draw sprite animation from the 2D object.
 * 
 * @param[in] object 2D object
 * @returns None
 */
void draw_sprite_animation(object_2d_info_t* object);

/**
 * Update Update functions group end
 * @}
 */

#endif /* SPRITE_ANIMATION_MANAGER_H */