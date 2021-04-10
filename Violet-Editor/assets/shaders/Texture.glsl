#type vertex
#version 330 core

layout(location = 0) in vec4 a_position;
layout(location = 1) in vec4 a_color;			
layout(location = 2) in vec2 a_texCoord;
layout(location = 3) in float a_texIndex;
layout(location = 4) in float a_textureSizeFactor;
layout(location = 5) in int a_entityID;

uniform mat4 u_viewProjection;

out vec4 v_color;
out vec2 v_texCoord;
flat out float v_texIndex;
flat out int v_entityID;

void main(){
	gl_Position = u_viewProjection * a_position;

	v_color = a_color;
	v_texCoord = a_texCoord * a_textureSizeFactor;
	v_texIndex = a_texIndex;
	v_entityID = a_entityID;
}
#type fragment
#version 330 core

layout(location = 0) out vec4 o_color;
layout(location = 1) out int o_int;

uniform sampler2D u_textures[32];

in vec4 v_color;
in vec2 v_texCoord;
flat in float v_texIndex;
flat in int v_entityID;

void main(){
	o_color =  texture(u_textures[int(v_texIndex)], v_texCoord) * v_color;
	o_int = v_entityID;
}