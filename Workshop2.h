#include "InputState.h"
#include "ShaderProgram.h"
#include "geometry.h"
#include "iostream"

struct dynamicArray {
    size_t length;
	size_t capacity;
	uint8_t* ptr;
};

struct pngImage {
	dynamicArray r;
	dynamicArray g;
	dynamicArray b;
	dynamicArray a;
};

extern "C" struct pngImage load_png
();

class Workshop2
{
public:
	Workshop2(InputState & inputstate);
	//setup data
	bool initialize();
	//called before each render, deltatime is in seconds
	void update(int width, int height, double deltatime);
	void update();
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
	float yaw, pitch;  

	//two buffers to store our vertex data
	GLuint vertexbufferobject_position;
	GLuint vertexbufferobject_normal;

	ShaderProgram terrainshader;


	Vector3 getTerrainVertex(uint8_t* heightmap, size_t x, size_t y);
	Vector3 getTerrainNormal(uint8_t* heightmap, size_t x, size_t y);
	bool loadTerrain();
};
