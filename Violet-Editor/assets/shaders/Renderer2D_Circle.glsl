#type vertex
#version 330 core

layout(location = 0) in vec4 a_worldPosition;
layout(location = 1) in vec4 a_localPosition;
layout(location = 2) in vec4 a_color;			
layout(location = 3) in float a_thickness;
layout(location = 4) in float a_fade;
layout(location = 5) in int a_entityID;

uniform mat4 u_viewProjection;

out vec4 v_localPosition;
out vec4 v_color;
out float v_thickness;
out float v_fade;
flat out int v_entityID;

void main(){
	gl_Position = u_viewProjection * a_worldPosition;

	v_localPosition = a_localPosition;
	v_color = a_color;
	v_thickness = a_thickness;
	v_fade = a_fade;
	v_entityID = a_entityID;
}

#type fragment
#version 330 core

layout(location = 0) out vec4 o_color;
layout(location = 1) out int o_entityID;

in vec4 v_localPosition;  //In range [-1.0f, 1.0f] on XY axes
in vec4 v_color;
in float v_thickness;
in float v_fade;
flat in int v_entityID;

void main(){
	
	float distanceFromCircleOrigin = 1.0f - length(vec3(v_localPosition));  //Inverse the distance, 1.0f is the origin and 0.0f is outside the circle boundary

	//Set any distance in range [1.0f to v_fade] to output 1.0f, then gradually drop until reaching 0.0f
	float circle = smoothstep(0.0f, v_fade, distanceFromCircleOrigin);    //For drawing filled in circle

	//Set any distance in range [1.0f to v_thickness + v_fade] to output 1.0f, then gradually drop until reaching v_thickness, then inverse the result
	circle *= 1.0f - smoothstep(v_thickness, v_thickness + v_fade, distanceFromCircleOrigin);    //For drawing dount shape circle

	//If out of the circle bounds
	if(circle == 0.0f)
		discard;  //Exit the fragment shader 
	/*
	* These will be excuted only if in circle boundary, as discard is called whenever we are outside the circle boundary
	*/
	o_color = v_color;    //Set circle color
	o_color.a *= circle;  //Apply the fade to the circle

	o_entityID = v_entityID;
}