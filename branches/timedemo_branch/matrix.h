#ifndef MATRIX_H
#define MATRIX_H

#include "vector.h"

typedef struct {
  float m[16]; // column order
} matrix;

vec4* vec4Transform(vec4* pOut, const vec4* pV, const matrix *pM);
float matrixCofactor(const matrix *pM, int cf_row, int cf_col);
float matrixDeterminant(const matrix *pM);
matrix* matrixIdentity(matrix *pOut);
matrix* matrixInverse(matrix *pOut, float* pDet, const matrix *pM);
matrix* matrixMultiply(matrix *pOut, const matrix *pM1, const matrix *pM2);
matrix* matrixTranspose(matrix *pOut, const matrix *pM);
matrix* matrixAdjoint(matrix *pOut, float* pDet, const matrix *pM);

vec4* vec4Transform(vec4 *pOut, const vec4 *pV, const matrix *pM);

matrix* matrixRotationAxis(matrix *pOut, float fAngle, const vec3 *vAxis);
void matrixPrint(matrix *m);

#endif
