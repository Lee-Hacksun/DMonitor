#include "Color.h"

Color GetDefalutTargetColor(int species)
{
    Color color[SPECIES_COUNT];

    color[CHERRY_BLOSSOM_TARGET_COLOR] = (Color){1, 2, 3};
    color[MAPLE_TARGET_COLOR] = (Color){4, 5, 6};

    return color[species];
}