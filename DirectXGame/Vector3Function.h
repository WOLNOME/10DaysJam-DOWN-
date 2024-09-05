#pragma once
#include<Vector3.h>
#include<math.h>
#include "myFunction.h"

Vector3 Add(const Vector3& v1, const Vector3& v2);

Vector3 Subtract(const Vector3& v1, const Vector3& v2);

Vector3 Multiply(const Vector3& v, float scaler);

float Dot(const Vector3& v1, const Vector3& v2);

double Length(const Vector3& v);

Vector3 Normalize(const Vector3& v);

Vector3 Slerp(const Vector3& v1, const Vector3& v2, float t);