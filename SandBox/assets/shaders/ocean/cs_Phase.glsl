#type compute
#version 460 core

#define COMPUTE_WORK_GROUP_DIM 32

const float PI = 3.14159265359;
const float g = 9.81;
const float KM = 370.0;

layout(local_size_x = COMPUTE_WORK_GROUP_DIM, local_size_y = COMPUTE_WORK_GROUP_DIM) in;

layout(binding = 0, r32f) readonly uniform image2D u_phases;
layout(binding = 1, r32f) writeonly uniform image2D u_delta_phases;

uniform float u_delta_time;
uniform int u_ocean_size;
uniform int u_resolution;

float omega(float k)
{
    return sqrt(g * k * (1.0 + k * k / KM * KM));
}

void main()
{
    ivec2 pixel_coord = ivec2(gl_GlobalInvocationID.xy);

    float n = (pixel_coord.x < 0.5f * u_resolution) ? pixel_coord.x : pixel_coord.x - u_resolution;
    float m = (pixel_coord.y < 0.5f * u_resolution) ? pixel_coord.y : pixel_coord.y - u_resolution;

    vec2 wave_vector = (2.f * PI * vec2(n, m)) / u_ocean_size;
    float k = length(wave_vector);

    // Note: An ad-hoc factor to make the phase change per-frame slow
    float slowdown_factor = 1.f;

    float delta_phase = omega(k) * u_delta_time * slowdown_factor;
    float phase = imageLoad(u_phases, pixel_coord).r;
    phase = mod(phase + delta_phase, 2.f * PI);
    //    phase = mod(phase + delta_phase, 2.f * PI);

    imageStore(u_delta_phases, pixel_coord, vec4(phase, 0.f, 0.f, 0.f));
}