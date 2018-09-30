#include "InputState.h"
#include "ShaderProgram.h"
#include "geometry.h"
#include "iostream"

struct dynamicArray_f32 {
    size_t length;
	size_t capacity;
	float_t* ptr;
};

struct dynamicArray_u8 {
    size_t length;
	size_t capacity;
	uint8_t* ptr;
};

struct Image_rgb {
	size_t width;
    size_t height;
	dynamicArray_f32 rgb;
};

struct Image_seperate_channels {
	size_t width;
    size_t height;
	dynamicArray_u8 r;
	dynamicArray_u8 g;
	dynamicArray_u8 b;
};

extern "C" struct Image_rgb load_rgb_png();
extern "C" struct Image_seperate_channels load_channels_png();

class Workshop2
{
public:
	Workshop2(InputState & inputstate);
	//setup data
	bool initialize();
	//called before each render, deltatime is in seconds
	void update(int width, int height, double deltatime);
	void update(double deltatime);
	//render the scene
	void render();
	//toggle wireframe on off
	void toggleWireFrame();

	//adjust terrain multiplier to make smoother/rougher
	void makeTerrainRougher();
	void makeTerrainSmoother();

private:
	InputState & inputstate;

	int width, height;
	bool wireFrameOn;
	float heightMultiplier;

	Vector3 viewpoint, viewtarget;
	void updateViewRatios();
	float dx, dy, dz;

	//two buffers to store our vertex data
	GLuint vertexbufferobject_position;
	GLuint vertexbufferobject_normal;
	GLuint vertexbufferobject_colors;
	size_t terrain_width;
	size_t terrain_height;
	ShaderProgram terrainshader;

	Vector3 getTerrainVertex(uint8_t* heightmap, size_t x, size_t y);
	Vector3 getTerrainNormal(uint8_t* heightmap, size_t x, size_t y);
	bool loadTerrain();
};
