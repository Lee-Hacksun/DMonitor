#include "Color.h"

Color GetDefalutTargetColor(int species)
{
    Color color[SPECIES_COUNT];

    color[CHERRY_BLOSSOM_TARGET_COLOR] = (Color){255, 245, 238};
    color[MAPLE_TARGET_COLOR] = (Color){139, 0, 0};

    return color[species];
}