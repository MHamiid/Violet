#type vertex
#version 330 core

layout(location = 0) in vec4 a_position;
layout(location = 1) in vec4 a_color;			
layout(location = 2) in int a_entityID;

uniform mat4 u_viewProjection;

out vec4 v_color;
flat out int v_entityID;

void main(){
	gl_Position = u_viewProjection * a_position;

	v_color = a_color;
	v_entityID = a_entityID;
}
#type fragment
#version 330 core

layout(location = 0) out vec4 o_color;
layout(location = 1) out int o_entityID;

in vec4 v_color;
flat in int v_entityID;

void main(){
	o_color = v_color;
	o_entityID = v_entityID;
}