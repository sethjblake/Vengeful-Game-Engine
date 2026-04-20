#include "Conversions.h"
#include "box2d/b2_math.h"

float VengefulGameEngine::degreesToRadians(float degrees)
{
    return degrees * (b2_pi / 180.0f);
}

float VengefulGameEngine::radiansToDegrees(float radians)
{
    return radians * (180.0f / b2_pi);
}
