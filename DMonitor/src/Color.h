#ifndef COLOR_H
#define COLOR_H

#define SPECIES_COUNT 2

#define CHERRY_BLOSSOM_TARGET_COLOR 0
#define MAPLE_TARGET_COLOR 1

typedef struct _color 
{
    unsigned char Red;
    unsigned char Green;
    unsigned char Blue;
} Color;

Color GetDefalutTargetColor(int species);

#endif