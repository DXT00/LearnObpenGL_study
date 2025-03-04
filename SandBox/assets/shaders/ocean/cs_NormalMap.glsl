#type compute
#version 460 core

#define WORK_GROUP_DIM 32

layout(local_size_x = WORK_GROUP_DIM, local_size_y = WORK_GROUP_DIM) in;

layout(binding = 0, rgba32f) readonly uniform image2D u_displacement_map;
layout(binding = 1, rgba32f) writeonly uniform image2D u_normal_map;

uniform int u_resolution;
uniform int u_ocean_size;

void main()
{
	ivec2 pixel_coord = ivec2(gl_GlobalInvocationID.xy);

	float texel = 1.f / u_resolution;
	float texel_size = u_ocean_size * texel;

	vec3 center = imageLoad(u_displacement_map, pixel_coord).xyz;
	vec3 right = vec3(texel_size, 0.f, 0.f) + imageLoad(u_displacement_map, ivec2(clamp(pixel_coord.x + 1, 0, u_resolution - 1), pixel_coord.y)).xyz - center;
	vec3 left = vec3(-texel_size, 0.f, 0.f) + imageLoad(u_displacement_map, ivec2(clamp(pixel_coord.x - 1, 0, u_resolution - 1), pixel_coord.y)).xyz - center;
	vec3 top = vec3(0.f, 0.f, -texel_size) + imageLoad(u_displacement_map, ivec2(pixel_coord.x, clamp(pixel_coord.y - 1, 0, u_resolution - 1))).xyz - center;
	vec3 bottom = vec3(0.f, 0.f, texel_size) + imageLoad(u_displacement_map, ivec2(pixel_coord.x, clamp(pixel_coord.y + 1, 0, u_resolution - 1))).xyz - center;

	vec3 top_right = cross(right, top);
	vec3 top_left = cross(top, left);
	vec3 bottom_left = cross(left, bottom);
	vec3 bottom_right = cross(bottom, right);

	imageStore(u_normal_map, pixel_coord, vec4(normalize(top_right + top_left + bottom_right + bottom_left), 1.f));
}