
uniform mat4 projection_matrix;
uniform mat4 modelview_matrix;

attribute vec3 vertex_position;
attribute vec3 vertex_colour;
attribute vec2 vertex_texcoords;

varying vec3 varying_colour;
varying vec2 varying_texcoords;

void main()
{
	varying_colour = vertex_colour; 
	varying_texcoords = vertex_texcoords;
	
	gl_Position = projection_matrix * modelview_matrix * vec4(vertex_position, 1.0);
}

%%

uniform sampler2D texture_diffuse;

varying vec3 varying_colour;
varying vec2 varying_texcoords;

void main(void)
{
	gl_FragColor = texture2D(texture_diffuse, varying_texcoords);
	gl_FragColor *= vec4(varying_colour, 1.0);
}
