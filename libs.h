#define GLEW_STATIC
#define _USE_MATH_DEFINES
 
#include <GL/glew.h>
#include <GL/glu.h>
#include <SDL/SDL.h>

#include <cstdlib>
#include <cmath>
#include <vector>
#include <iostream>
#include <fstream>
#include <string>


#define degreesToRadians(angleDegrees) ((angleDegrees) * M_PI / 180.0)
#define radiansToDegrees(angleRadians) ((angleRadians) * 180.0 / M_PI)
