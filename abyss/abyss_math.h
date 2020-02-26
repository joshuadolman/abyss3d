#ifndef ABYSS_MATH_H
#define ABYSS_MATH_H

union v4f {
	struct {
		f32 r;
		f32 g;
		f32 b;
		f32 a;
	};
	struct {
		f32 x;
		f32 y;
		f32 z;
		f32 w;
	};
	struct {
		f32 f[4];
	};
};

typedef v4f colour;

internal colour
ColourRGBA(f32 r, f32 g, f32 b)
{
	colour result = {};
	result.r = r;
	result.g = g;
	result.b = b;
	result.a = 1.0f;

	return result;
}

internal colour
ColourRGBA(f32 r, f32 g, f32 b, f32 a)
{
	colour result = {};
	result.r = r;
	result.g = g;
	result.b = b;
	result.a = a;

	return result;
}

internal v4f
V4F(f32 x, f32 y, f32 z, f32 w)
{
	v4f result = {};
	result.x = x;
	result.y = y;
	result.z = z;
	result.w = w;

	return result;
}

union vec2_u16 {
	struct {
		u16 x;
		u16 y;
	};
	struct {
		u16 w;
		u16 h;
	};
	struct {
		u16 width;
		u16 height;
	};
};

//U64
union vec2_u64 {
	struct {
		u64 x;
		u64 y;
	};
	struct {
		u64 w;
		u64 h;
	};
	struct {
		u64 width;
		u64 height;
	};
};

union vec3_u64 {
	struct {
		u64 x;
		u64 y;
		u64 z;
	};
	struct {
		u64 r;
		u64 g;
		u64 b;
	};
	struct {
		u64 width;
		u64 height;
		u64 depth;
	};
};

union vec4_u64 {
	struct {
		u64 x;
		u64 y;
		u64 z;
		u64 w;
	};
	struct {
		u64 r;
		u64 g;
		u64 b;
		u64 a;
	};
};

#define vec2_int64 vec2_u64
#define vec3_int64 vec3_u64
#define vec4_int64 vec4_u64

//S64
union vec2_s64 {
	struct {
		s64 x;
		s64 y;
	};
	struct {
		s64 w;
		s64 h;
	};
	struct {
		s64 width;
		s64 height;
	};
};

union vec3_s64 {
	struct {
		s64 x;
		s64 y;
		s64 z;
	};
	struct {
		s64 r;
		s64 g;
		s64 b;
	};
	struct {
		s64 w;
		s64 h;
		s64 d;
	};
	struct {
		s64 width;
		s64 height;
		s64 depth;
	};
};

union vec4_s64 {
	struct {
		s64 x;
		s64 y;
		s64 z;
		s64 w;
	};
	struct {
		s64 r;
		s64 g;
		s64 b;
		s64 a;
	};
};


//U32
union vec2_u32 {
	struct {
		u32 x;
		u32 y;
	};
	struct {
		u32 w;
		u32 h;
	};
	struct {
		u32 width;
		u32 height;
	};
};

union vec3_u32 {
	struct {
		u32 x;
		u32 y;
		u32 z;
	};
	struct {
		u32 r;
		u32 g;
		u32 b;
	};
	struct {
		u32 width;
		u32 height;
		u32 depth;
	};
};

union vec4_u32 {
	struct {
		u32 x;
		u32 y;
		u32 z;
		u32 w;
	};
	struct {
		u32 r;
		u32 g;
		u32 b;
		u32 a;
	};
};

#define vec2_int vec2_s32
#define vec3_int vec3_s32
#define vec4_int vec4_s32

//S32
union vec2_s32 {
	struct {
		s32 x;
		s32 y;
	};
	struct {
		s32 w;
		s32 h;
	};
	struct {
		s32 width;
		s32 height;
	};
};

union vec3_s32 {
	struct {
		s32 x;
		s32 y;
		s32 z;
	};
	struct {
		s32 r;
		s32 g;
		s32 b;
	};
	struct color {
		s32 r;
		s32 g;
		s32 b;
	};
	struct colour {
		s32 r;
		s32 g;
		s32 b;
	};
	struct {
		s32 w;
		s32 h;
		s32 d;
	};
	struct {
		s32 width;
		s32 height;
		s32 depth;
	};
};

union vec4_s32 {
	struct {
		s32 x;
		s32 y;
		s32 z;
		s32 w;
	};
	struct {
		s32 r;
		s32 g;
		s32 b;
		s32 a;
	};
};

//F32
#define vec2_f vec2_f32
#define vec3_f vec3_f32
#define vec4_f vec4_f32

#define vec2 vec2_f
#define vec3 vec3_f
#define vec4 vec4_f

union vec2_f32 {
	struct {
		f32 x;
		f32 y;
	};
	struct {
		f32 w;
		f32 h;
	};
	struct {
		f32 width;
		f32 height;
	};
};

union vec3_f32 {
	struct {
		f32 x;
		f32 y;
		f32 z;
	};
	struct {
		f32 r;
		f32 g;
		f32 b;
	};
	struct {
		f32 w;
		f32 h;
		f32 d;
	};
	struct {
		f32 width;
		f32 height;
		f32 depth;
	};
};

union vec4_f32 {
	struct {
		f32 x;
		f32 y;
		f32 z;
		f32 w;
	};
	struct {
		f32 r;
		f32 g;
		f32 b;
		f32 a;
	};
};

//F64
union vec2_f64 {
	struct {
		f64 x;
		f64 y;
	};
	struct {
		f64 w;
		f64 h;
	};
	struct {
		f64 width;
		f64 height;
	};
};

union vec3_f64 {
	struct {
		f64 x;
		f64 y;
		f64 z;
	};
	struct {
		f64 r;
		f64 g;
		f64 b;
	};
	struct {
		f64 w;
		f64 h;
		f64 d;
	};
	struct {
		f64 width;
		f64 height;
		f64 depth;
	};
};

union vec4_f64 {
	struct {
		f64 x;
		f64 y;
		f64 z;
		f64 w;
	};
	struct {
		f64 r;
		f64 g;
		f64 b;
		f64 a;
	};
};

union vec4_u8 {
	struct {
		u8 x;
		u8 y;
		u8 z;
		u8 w;
	};
	struct {
		u8 r;
		u8 g;
		u8 b;
		u8 a;
	};
	u32 colorRGBA;
};

//typedef color vec4_u8;

//TODO(joshua): Move this elsewhere!
struct vertex {
	vec3 position;
	vec4 color;
};

#endif //ABYSS_MATH_H