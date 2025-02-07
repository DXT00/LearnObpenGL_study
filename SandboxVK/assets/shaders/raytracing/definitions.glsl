
struct onb {
    vec3 u;
    vec3 v;
    vec3 w;
};

struct material {
    uint materialType;
    vec3 albedo;
};

struct triangle {
    vec3 v0;
    vec3 v1;
    vec3 v2;
    uint materialIndex;
    uint objIndex;
};

struct light {
    uint triangleIndex;
    float area;
};

struct sphere {
    vec4 s;
    uint materialIndex;
};

struct bvhNode {
    vec3 min;
    vec3 max;
    int leftNodeIndex;
    int rightNodeIndex;
    int objectIndex;
};

/*
#define LIGHT_MATERIAL 0
#define LAMBERTIAN_MATERIAL 1
#define METAL_MATERIAL 2
#define GLASS_MATERIAL 3
*/

#define	RTX_AMBIENT 0
#define	RTX_METALLIC 1
#define	RTX_DIELECTRIC 2
#define	RTX_DIFFUSE 3
#define	RTX_SPECULAR 4
#define	RTX_EMISSION 5
#define	RTX_ROUGHNESS 6
#define	RTX_AO 7


onb Onb(vec3 n) {
    onb res;
    res.w = normalize(n);
    vec3 a = (abs(res.w.x) > 0.9) ? vec3(0,1,0) : vec3(1,0,0);
    res.v = normalize(cross(res.w, a));
    res.u = cross(res.w, res.v);
    return res;
}

vec3 onbLocal(vec3 a, onb o) {
    return a.x * o.u + a.y * o.v + a.z * o.w;
}