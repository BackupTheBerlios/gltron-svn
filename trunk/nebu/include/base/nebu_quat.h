#ifndef NEBU_QUAT_H
#define NEBU_QUAT_H

#include "base/nebu_matrix.h"

typedef struct {
  float x,y,z,w;
} quat;

typedef quat nebu_Quat;

nebu_Matrix4D quat_RotationMatrix(const nebu_Quat *quat);
quat quat_FromLookAt(const vec3 *vLookAt, const vec3 *vUp);

#endif