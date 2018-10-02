#ifndef INPUT_H
#define INPUT_H

#include <fxcg/keyboard.h>

bool key_update();

bool is_key_pressed(int key_code);
bool is_key_held(int key_code);
bool is_key_new_pressed(int key_code);

#endif // INPUT_H
