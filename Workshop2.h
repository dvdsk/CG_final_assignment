#include "InputState.h"
#include "ShaderProgram.h"
#include "geometry.h"
 

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

	//two buffers to store our vertex data
	GLuint vertexbufferobject_position;
	GLuint vertexbufferobject_normal;

	ShaderProgram terrainshader;


	Vector3 getTerrainVertex(std::vector<unsigned char> & heightmap, size_t x, size_t y);
	Vector3 getTerrainNormal(std::vector<unsigned char> & heightmap, size_t x, size_t y);
	bool loadTerrain();
};
