#include "libs.h"
#include "Workshop2.h"

Workshop2::Workshop2(InputState & inputstate) : inputstate(inputstate)
{
	wireFrameOn = false;	
	heightMultiplier = 1;
}

void Workshop2::toggleWireFrame(){
	if(wireFrameOn){
		glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
		wireFrameOn = false;
	}
	else{
		glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
		wireFrameOn = true;
	}
}

void Workshop2::makeTerrainRougher(){
	heightMultiplier += 0.1;
	loadTerrain();
}

void Workshop2::makeTerrainSmoother(){
	heightMultiplier -= 0.1;
	loadTerrain();
}

bool Workshop2::initialize()
{
	//attempt to load our terrain data
	if (!loadTerrain())
		return false;

	if (!terrainshader.loadShaderProgram("terrain"))
	{
		std::cout << terrainshader.getErrorMessage() << std::endl;
		return false;
	}

	//enable depth testing and set to "Less or Equal" mode
	//only pixels that are closer or equally close are shown
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_DEPTH_TEST);

	viewpoint = Vector3(-150.0f, -400.0f, 300.0f);
	viewtarget = Vector3(0.0f, 0.0f, 50.0f);

	return true;
}

void Workshop2::updateViewRatios(){
	Vector3 direction = viewtarget - viewpoint;
	float total = direction.x()+direction.y()+ direction.z();

	dx = direction.x() / total;
	dy = direction.y() / total;
	dz = direction.z() / total;
}

void Workshop2::update(int width, int height, double deltatime)//, double deltatime)
{
	this->width = width;
	this->height = height;
	
	if (inputstate.keysdown[SDLK_w]) {
		toggleWireFrame();
	} else if (inputstate.keysdown[SDLK_MINUS] || inputstate.keysdown[SDLK_KP_MINUS]) {
		makeTerrainSmoother();
	} else if (inputstate.keysdown[SDLK_EQUALS] || inputstate.keysdown[SDLK_KP_PLUS]) {
		makeTerrainRougher();
	}		
	//the viewpoint describes from where we look, we update it from movement key inputs here
	else if (inputstate.keysdown[SDLK_UP]) {//move forward
		updateViewRatios();
		float sensitivity = 0.05f*deltatime;
		//printf("forward\n %i %i %i\n",viewpoint.x(),viewpoint.y(),viewpoint.z());
		viewpoint.x() += sensitivity*dx;
		viewpoint.y() += sensitivity*dy;
		viewpoint.z() += sensitivity*dz;
	} else if (inputstate.keysdown[SDLK_DOWN]){//move back
		updateViewRatios();
		float sensitivity = 0.05f*deltatime;
		viewpoint.x() -= sensitivity*dx;
		viewpoint.y() -= sensitivity*dy;
		viewpoint.z() -= sensitivity*dz;
	} else if (inputstate.keysdown[SDLK_LEFT]) {
		makeTerrainSmoother();
	} else if (inputstate.keysdown[SDLK_RIGHT]) {
		makeTerrainRougher();
	}	
	//the viewtarget describes where we look at, it is dercribed here
	if (inputstate.mouse_moved){
		float sensitivity =-0.05f;
		float x = inputstate.mouse_rel_x * -1 * sensitivity;
		float y = inputstate.mouse_rel_y * -1 * sensitivity;
		inputstate.mouse_moved = false;
		
		// update camera angles, no roll support
		viewtarget = viewtarget - viewpoint;
		viewtarget = viewtarget.rotateZ(degreesToRadians(x));
		viewtarget = viewtarget.rotateX(degreesToRadians(y));
		viewtarget += viewpoint;
	}
}


void Workshop2::render()
{
	//set the size of the rendering area
	glViewport(0, 0, width, height);
	
	Matrix4 modelviewmatrix = Matrix4::lookAtMatrix(viewpoint,
	                                                viewtarget,
	                                                Vector3(0.0f, 0.0f, 1.0f));

	//projection transform matrix
	Matrix4 projectionmatrix = Matrix4::perspectiveMatrix((float)M_PI_4, (float)width / height, 1.0f, 10000.0f);


	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//use the terrain shader
	glUseProgram(terrainshader.getProgram());
	//set our matrices as "uniform" variables
	glUniformMatrix4fv(glGetUniformLocation(terrainshader.getProgram(), "matmodelview"), 1, GL_TRUE, modelviewmatrix.elements());
	glUniformMatrix4fv(glGetUniformLocation(terrainshader.getProgram(), "matprojection"), 1, GL_TRUE, projectionmatrix.elements());
	
	glUniform3f(glGetUniformLocation(terrainshader.getProgram(), "camera_position"), viewpoint.x(),viewpoint.y(),viewpoint.z());
	
	//set vertex position data
	glBindBuffer(GL_ARRAY_BUFFER, vertexbufferobject_position);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	
	//set vertex normal data
	glBindBuffer(GL_ARRAY_BUFFER, vertexbufferobject_normal);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

	//set vertex color data
	glBindBuffer(GL_ARRAY_BUFFER, vertexbufferobject_colors);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);


	//draw the data
	glDrawArrays(GL_TRIANGLES, 0, terrain_width * terrain_height * 6);
	
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
}


Vector3 Workshop2::getTerrainVertex(uint8_t* heightmap, size_t x, size_t y)
{
	//return the position of a point on the heightmap
	//The y axis is reversed here
	return Vector3(x - 256.0f, 256.0f - y, 
	              (float)heightmap[y * terrain_width + x] * 0.5f * heightMultiplier);
}
Vector3 Workshop2::getTerrainNormal(uint8_t* heightmap, size_t x, size_t y)
{
	//get the height difference along the X and Y directions
	size_t w = terrain_width;
	size_t h = terrain_height;
	float dx, dy;
	if (x == 0)
		dx = (float)heightmap[y * w + (x + 1)] * 0.5f - (float)heightmap[y * w + x] * 0.5f;
	else if (x == terrain_width-1)
		dx = (float)heightmap[y * w + x] * 0.5f - (float)heightmap[y * w + (x - 1)] * 0.5f;
	else
		dx = (float)heightmap[y * w + (x + 1)] * 0.5f - (float)heightmap[y * w + (x - 1)] * 0.5f;
	if (y == 0)
		dy = (float)heightmap[(y + 1) * w + x] * 0.5f - (float)heightmap[y * w + x] * 0.5f;
	else if (y == terrain_height-1)
		dy = (float)heightmap[y * w + x] * 0.5f - (float)heightmap[(y - 1) * w + x] * 0.5f;
	else
		dy = (float)heightmap[(y + 1) * w + x] * 0.5f - (float)heightmap[(y - 1) * w + x] * 0.5f;

	//use this to compute a normal vector:
	//u = (1, 0, dx); v = (0, 1, dy)
	//n = || (u x v) ||  ->  normalize(u cross v)
	//becomes: n = || (-dx, -dy, 1) ||
	float sizefactor = 1.0f / sqrt(dx * dx + dy * dy + 1.0f);
	return Vector3(-dx * sizefactor, -dy * sizefactor, 1.0f * sizefactor);
}

bool Workshop2::loadTerrain()
{
	//read heightmap image file into array "heightmap"
	struct pngImage image = load_png();
	uint8_t* heightmap = image.a.ptr;
	terrain_height = image.height;
	terrain_width = image.width;

	//large arrays of position and normal vectors
	//containing 512 x 512 tiles
	//each tile consists of 2 triangles (3 * 2 = 6 vectors)
	std::vector<Vector3> positions((terrain_height-1) * (terrain_width-1) * 6);
	std::vector<Vector3> normals((terrain_height-1) * (terrain_width-1) * 6);

	size_t x, y;
	size_t w = terrain_width-1;
	size_t h = terrain_height-1;
	for (y = 0; y < h; y++) {
		for (x = 0; x < w; x++) {
			float height = (float)heightmap[y * (w+1) + (x)];//50.0f;

			positions[(y * w + x) * 6 + 0] = getTerrainVertex(heightmap, x, y);
			positions[(y * w + x) * 6 + 1] = getTerrainVertex(heightmap, x + 1, y);
			positions[(y * w + x) * 6 + 2] = getTerrainVertex(heightmap, x + 1, y + 1);
			positions[(y * w + x) * 6 + 3] = getTerrainVertex(heightmap, x + 1, y + 1);
			positions[(y * w + x) * 6 + 4] = getTerrainVertex(heightmap, x, y + 1);
			positions[(y * w + x) * 6 + 5] = getTerrainVertex(heightmap, x, y);


			normals[(y * w + x) * 6 + 0] = getTerrainNormal(heightmap, x, y);
			normals[(y * w + x) * 6 + 1] = getTerrainNormal(heightmap, x + 1, y);
			normals[(y * w + x) * 6 + 2] = getTerrainNormal(heightmap, x + 1, y + 1);
			normals[(y * w + x) * 6 + 3] = getTerrainNormal(heightmap, x + 1, y + 1);
			normals[(y * w + x) * 6 + 4] = getTerrainNormal(heightmap, x, y + 1);
			normals[(y * w + x) * 6 + 5] = getTerrainNormal(heightmap, x, y);
		}
	}
	
	//Use the arrays "positions" and "normals" to fill two vertex buffer objects
	//These are essentially arrays of data to be stored in the GPU memory
	//First create a new vertex buffer object
	glGenBuffers(1, &vertexbufferobject_position);
	//then bind it, so that the next operation is performed on this object
	glBindBuffer(GL_ARRAY_BUFFER, vertexbufferobject_position);
	//send the data to the GPU
	//arguments after GL_ARRAY_BUFFER: size of data in bytes, pointer to start of data, expected method of updating the data
	glBufferData(GL_ARRAY_BUFFER, w * h * 6 * 3 * sizeof(float), &positions[0], GL_STATIC_DRAW);
	
	//repeat of the above, now for the normal array
	glGenBuffers(1, &vertexbufferobject_normal);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbufferobject_normal);
	glBufferData(GL_ARRAY_BUFFER, w * h * 6 * 3 * sizeof(float), &normals[0], GL_STATIC_DRAW);

	//repeat of the above, now for the color array
	glGenBuffers(1, &vertexbufferobject_colors);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbufferobject_colors);
	glBufferData(GL_ARRAY_BUFFER, w * h * 6 * 3 * sizeof(float), image.rgb.ptr, GL_STATIC_DRAW);
	return true;
}
