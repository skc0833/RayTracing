#include <iostream>
#include <fstream>
#include "ray.h"

static float hit_sphere(const vec3& center, float radius, const ray& r) {
    // 근의 공식 -> ax^2 + bx + c = 0
    // x = (-b +- sqrt(b^2 - 4ac)) / 2a
    vec3 oc = r.origin() - center; // center(0, 0, -1) 에서 origin(0.0, 0.0, 0.0) 방향 벡터
    float a = dot(r.direction(), r.direction());
    // r.direction() 은 origin(0.0, 0.0, 0.0) 에서 r 방향 벡터 => 항상 b == -2
    float b = 2.0 * dot(oc, r.direction());
    float c = dot(oc, oc) - radius * radius;
    float discriminant = b * b - 4 * a * c;
    if (discriminant < 0) {
        return -1.0;
    } else {
        // right handed coordinate system, into the screen is the negative z-axis!
        // 두 점중 -sqrt() 이므로, origin 에 가까운 점임(closest hit point(smallest t))
        float r = (-b - sqrt(discriminant)) / (2.0 * a);
        return r;
    }
}

static vec3 color(const ray& r) {
    float t = hit_sphere(vec3(0, 0, -1), 0.5, r);
    if (t > 0.0) {
        vec3 N = unit_vector(r.point_at_paramter(t) - vec3(0, 0, -1));
        // r 은 왼쪽(0) -> 오른쪽(1), g 는 아래 -> 위, b 는 뒤 -> 앞
        return 0.5 * vec3(N.x() + 1, N.y() + 1, N.z() + 1); // normal [-1, 1] => [0, 1]
        //return 0.5 * vec3(N.x() + 1, N.y() + 1, 0);
        //return 1.0 * vec3(N.x() + 1, N.y() + 1, N.z() + 1); // 구체가 4등분 돼서 보임!
    }
#if 0
    return vec3(1.0, 1.0, 1.0); // white bg
#else
    vec3 unit_direction = unit_vector(r.direction());
    t = 0.5 * (unit_direction.y()/*[-1, 1]*/ + 1.0); //t = [0.0, 1.0]
    return (1.0 - t) * vec3(1.0, 1.0, 1.0) + t * vec3(0.5, 0.7, 1.0); // sky color
#endif
}

int main_5_1()
{
    std::string filePath = "ch5_1_surface_normals.ppm";
    std::ofstream f(filePath);
    if (!f.is_open()) {
        std::cerr << filePath << " open failed!\n";
        return -1;
    }

    int nx = 200;
    int ny = 100;
    f << "P3\n" << nx << " " << ny << "\n255\n";
    vec3 lower_left_corner(-2.0, -1.0, -1.0);
    vec3 horizontal(4.0, 0.0, 0.0);
    vec3 vertical(0.0, 2.0, 0.0);
    vec3 origin(0.0, 0.0, 0.0);
    for (int j = ny - 1; j >= 0; j--) {
        for (int i = 0; i < nx; i++) {
            float u = float(i) / nx; // u = [0, 1)
            float v = float(j) / ny; // v = (1, 0]
            ray r(origin, lower_left_corner + u * horizontal + v * vertical);
            vec3 col = color(r);
            int ir = 255.99 * col[0];
            int ig = 255.99 * col[1];
            int ib = 255.99 * col[2];

            f << ir << " " << ig << " " << ib << "\n";
        }
    }
    f.close();
    std::cout << filePath << " write finished!\n";
}
