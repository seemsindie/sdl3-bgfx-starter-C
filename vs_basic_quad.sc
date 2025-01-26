$input a_position, a_color0
$output v_color0, v_texcoord0

#include "./common.sh"

void main()
{
	gl_Position = vec4(a_position, 1.0);
	// v_texcoord0 = a_texcoord0;
	// v_texcoord0 = mul(u_model[0], vec4(a_position, 1.0)).xyz;
	// v_texcoord0 = a_position;
	v_color0 = a_color0;
	v_texcoord0 = a_position;
}