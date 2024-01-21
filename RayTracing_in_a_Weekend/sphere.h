#pragma once
#include "hitable.h"

class sphere : public hitable {
public:
    sphere() {}
    //sphere(vec3 cen, float r) : center(cen), radius(r) {}
    sphere(vec3 cen, float r, material* mat = nullptr) : center(cen), radius(r), mat(mat) {}
    virtual inline bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const;
    vec3 center;
    float radius;
    material* mat = nullptr;
};

bool sphere::hit(const ray& r, float t_min, float t_max, hit_record& rec) const {
    vec3 oc = r.origin() - center;
    float a = dot(r.direction(), r.direction());
    float b = dot(oc, r.direction());
    float c = dot(oc, oc) - radius * radius;
    // 근의 공식 - 짝수 공식(https://mathbang.net/30#gsc.tab=0)
    // 일차항의 계수가 짝수(b')일때 ax^2 + 2b'x + c = 0
    // eliminated a bunch of redundant 2’s that cancel each other
    // x = (-2b' +- sqrt(4b'^2 - 4ac)) / 2a
    // x = (-b' +- sqrt(b'^2 - ac)) / a
    // 원래는 float discriminant = b * b - 4 * a * c; 였음!(여기서는 x 대신 t 가 사용됨)
    float discriminant = b * b - a * c;
    if (discriminant > 0) {
        rec.mat_ptr = mat;
        float temp = (-b - sqrt(b * b - a * c)) / a;
        if (temp < t_max && temp > t_min) { // 구 앞쪽에서 교차점
            rec.t = temp; // ray 와 구가 만날 때의 t
            rec.p = r.point_at_paramter(rec.t); // origin(0, 0, 0) 에서 ray 와 구의 접점으로의 벡터?!
            rec.normal = (rec.p - center) / radius; // center(0, 0, -1) 에서 접점 p 로의 단위 벡터?!
            return true;
        }
        temp = (-b + sqrt(b * b - a * c)) / a;
        if (temp < t_max && temp > t_min) { // 구 뒤쪽에서 교차점(이하 계산식은 위와 동일함)
            rec.t = temp;
            rec.p = r.point_at_paramter(rec.t);
            rec.normal = (rec.p - center) / radius;
            return true;
        }
    }
    return false;
}
