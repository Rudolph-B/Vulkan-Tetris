/*
 * CREATED BY RUDOLPH ON 2023-05-27.
 */

#ifndef VULKAN_TETRIS_CONSTANTS_H
#define VULKAN_TETRIS_CONSTANTS_H

#include <string>

/* WINDOW SIZE */
const int WIDTH = 400;
const int HEIGHT = 800;

/* MODEL AND TEXTURE PATHS */
const std::string MODEL_PATH = "mdl/temp.obj";
const std::string TEXTURE_PATH = "tex/temp.png";

const int MAX_FRAMES_IN_FLIGHT = 2;

/* ENABLES/DISABLES VALIDATION LAYERS*/
#ifdef NDEBUG
const bool E_VALIDATION_LAYERS = false;
#else
const bool E_VALIDATION_LAYERS = true;
#endif

#endif //VULKAN_TETRIS_CONSTANTS_H
