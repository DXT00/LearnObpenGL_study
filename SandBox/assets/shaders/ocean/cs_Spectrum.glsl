#type compute
#version 460 core

#define COMPUTE_WORK_GROUP_DIM 32
#define RESOLUTION 512

const float PI = 3.14159265359f;
const float g = 9.81f;
const float KM = 370.f;

layout (local_size_x = COMPUTE_WORK_GROUP_DIM, local_size_y = COMPUTE_WORK_GROUP_DIM) in;

layout (binding = 0, r32f) readonly uniform image2D u_phases;
layout (binding = 1, r32f) readonly uniform image2D u_initial_spectrum;
layout (binding = 2, rgba32f) writeonly uniform image2D u_spectrum;

uniform int u_ocean_size;
uniform float u_choppiness;

vec2 multiplyComplex(vec2 a, vec2 b)
{
    return vec2(a[0] * b[0] - a[1] * b[1], a[1] * b[0] + a[0] * b[1]);
}

vec2 multiplyByI(vec2 z)
{
    return vec2(-z[1], z[0]);
}

float omega(float k)
{
    return sqrt(g * k * (1.0 + k * k / KM * KM));
}

void main()
{
    ivec2 pixel_coord = ivec2(gl_GlobalInvocationID.xy);

    float n = (pixel_coord.x < 0.5f * RESOLUTION) ? pixel_coord.x : pixel_coord.x - RESOLUTION;
	float m = (pixel_coord.y < 0.5f * RESOLUTION) ? pixel_coord.y : pixel_coord.y - RESOLUTION;
    vec2 wave_vector = (2.f * PI * vec2(n, m)) / u_ocean_size;

    float phase = imageLoad(u_phases, pixel_coord).r;
    vec2 phase_vector = vec2(cos(phase), sin(phase));

    vec2 h0 = vec2(imageLoad(u_initial_spectrum, pixel_coord).r, 0.f);
    vec2 h0Star = vec2(imageLoad(u_initial_spectrum, (RESOLUTION - pixel_coord) % (RESOLUTION - 1)).r, 0.f);
    h0Star.y *= -1.f;

    vec2 h = multiplyComplex(h0, phase_vector) + multiplyComplex(h0Star, vec2(phase_vector.x, -phase_vector.y));

    vec2 hX = -multiplyByI(h * (wave_vector.x / length(wave_vector))) * u_choppiness;
    vec2 hZ = -multiplyByI(h * (wave_vector.y / length(wave_vector))) * u_choppiness;

    // No DC term
    if (wave_vector.x == 0.0 && wave_vector.y == 0.0)
    {
        h = vec2(0.f);
        hX = vec2(0.f);
        hZ = vec2(0.f);
    }

    imageStore(u_spectrum, pixel_coord, vec4(hX + multiplyByI(h), hZ));
}
