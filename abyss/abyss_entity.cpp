
struct texture {
    u32* memory;
    vec2_u16 dim;
};

struct entity {
    texture* tex;
    vec3 pos;
};
