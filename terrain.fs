#version 130
 
uniform vec3 camera_position;
 
// Output color
out vec4 fragcolor; //instead using build in variable gl_FragColor

// Variables from vertex to fragment shader
in vec4 _position; //position vector
in vec3 _normal; //normal vector
in vec3 _color;

// Light properties
vec3 light_position = vec3(0.,300.,300); // Position in world coordinates
//vec3 light_targetposition; // only important for spot lights
vec3 light_intensity = vec3(1, 1, 1);
//vec4 light_params;

vec3 material_specular = vec3(0.1,0,0);
float material_shininess = 0.0;


void main()
{
	vec3 material_diff = _color;
	vec3 position = _position.xyz;
	
	//calc diffuse lighting
	vec3 normal = normalize(_normal);
	float projection = clamp(dot(normal, normalize(light_position - position)),0.,1.);
	vec3 I_diff = material_diff * light_intensity * projection;
	
	////calc specular lighting
	vec3 H = normalize(light_position - position) 
		     + normalize(camera_position - position);
	H = normalize(H);
	float dotPow = pow(dot(normal,H),material_shininess);
	vec3 I_spec = material_specular*light_intensity*dotPow;

    vec3 I = (I_spec+I_diff);
	I = clamp(I, 0, 1);
	
	fragcolor = vec4(I, 1);
	
	////simplified lighting model
    //fragcolor = _normal.z*vec4(_color,1);
}
