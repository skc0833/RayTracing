#pragma once
#include "vec3.h"

class ray {
public:
    ray() {}
    //ray() = delete;
    ray(const vec3& a, const vec3& b) { A = a; B = b; }
    vec3 origin() const { return A; }       // ray 중심점
    vec3 direction() const { return B; }    // ray 방향
    vec3 point_at_paramter(float t) const { return A + t*B; }

    vec3 A, B;
};
