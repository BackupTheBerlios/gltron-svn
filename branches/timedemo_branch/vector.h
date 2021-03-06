#ifndef VECTOR_H
#define VECTOR_H

typedef struct { float v[2]; } vec2;
typedef struct { float v[3]; } vec3;
typedef struct { float v[4]; } vec4;

vec4* vec4Add(vec4 *pOut, const vec4 *pV1, const vec4 *pV2);
vec4* vec4Sub(vec4 *pOut, const vec4 *pV1, const vec4 *pV2);

vec3* vec3Add(vec3 *pOut, const vec3 *pV1, const vec3 *pV2);
vec3* vec3Sub(vec3 *pOut, const vec3 *pV1, const vec3 *pV2);
vec3* vec3Cross(vec3 *pOut, const vec3 *pV1, const vec3 *pV2);

float vec4Dot(const vec4 *pV1, const vec4 *pV2);

float vec3Dot(const vec3 *pV1, const vec3 *pV2);

float vec3Length(const vec3 *pV);
float vec3LengthSqr(const vec3 *pV);
vec3* vec3Normalize(vec3 *pOut, const vec3 *pV);

vec3* vec3fromVec4(vec3 *pOut, const vec4 *pV);
vec4* vec4fromVec3(vec4 *pOut, const vec3 *pV);

vec3* vec3Copy(vec3 *pOut, const vec3 *pV);
vec3* vec3Scale(vec3 *pV, float f);
void vec4Print(vec4 *pV);
void vec3Print(vec3 *pV);

unsigned int uintFromVec3(vec3 *pV);
// vec4* vec4Transform(vec4* pOut, const vec4* pV, const matrix16 *pM);

#endif
