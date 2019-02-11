#include "InputState.h"
#include "ShaderProgram.h"
#include "geometry.h"
#include "iostream"

#include "png_decode/png_decode.h"

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

	ShaderProgram terrainshader;
	//three buffers to store our vertex data
	GLuint vertexbufferobject_position;
	GLuint vertexbufferobject_normal;
	GLuint vertexbufferobject_colors;
	size_t terrain_width;
	size_t terrain_height;

	ShaderProgram watershader;
	GLuint vertexbufferobject_water_position;

	Vector3 getTerrainVertex(uint8_t* heightmap, size_t x, size_t y);
	Vector3 getTerrainNormal(uint8_t* heightmap, size_t x, size_t y);
	Vector3 getTerrainColor(float* array, size_t x, size_t y);
	bool loadTerrain();
	Vector3 getWaterVertex(size_t x, size_t y);
	bool loadWater();
};
