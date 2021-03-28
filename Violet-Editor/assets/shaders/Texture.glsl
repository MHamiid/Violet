#type vertex
#version 330 core

layout(location = 0) in vec4 a_position;
layout(location = 1) in vec4 a_color;			
layout(location = 2) in vec2 a_texCoord;
layout(location = 3) in float a_texIndex;
layout(location = 4) in float a_textureSizeFactor;


uniform mat4 u_viewProjection;

out vec4 v_color;
out vec2 v_texCoord;
out float v_texIndex;

void main(){
	gl_Position = u_viewProjection * a_position;

	v_color = a_color;
	v_texCoord = a_texCoord * a_textureSizeFactor;
	v_texIndex = a_texIndex;
}
#type fragment
#version 330 core

layout(location = 0) out vec4 o_color;	

uniform sampler2D u_textures[32];

in vec4 v_color;
in vec2 v_texCoord;
in float v_texIndex;

void main(){
	o_color =  texture(u_textures[int(v_texIndex)], v_texCoord) * v_color;
	
}