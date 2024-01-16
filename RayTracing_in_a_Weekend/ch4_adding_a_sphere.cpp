#include <iostream>
#include <fstream>
#include "ray.h"

// center = 구의 중심 좌표
static bool hit_sphere(const vec3& center, float radius, const ray& r) {
    // 근의 공식 -> ax^2 + bx + c = 0
    // x^2 + (b/a)x = -(c/a)
    // x^2 + (b/a)x + (b/2a)^2 = -(c/a) + (b/2a)^2
    // (x + (b/2a))^2 = -(c/a) + (b^2/4a^2)
    // (x + (b/2a))^2 = -(4ac/4a^2) + (b^2/4a^2) = (b^2 - 4ac)/4a^2
    // x + (b/2a) = +-sqrt((b^2 - 4ac)/4a^2)
    // x + (b/2a) = +-sqrt(b^2 - 4ac)/2a
    // x = -(b/2a) +-sqrt(b^2 - 4ac)/2a
    // x = (-b +-sqrt(b^2 - 4ac))/2a
    // --> b^2 - 4ac = 0 이면 1점에서 만남, 양수면 2점, 음수면 안맞남(허수?)
    // from https://mathbang.net/30#gsc.tab=0

    // origin 이 중심이고, R 이 반지름인 구의 방정식
    // x*x + y*y + z*z = R*R
    // 중심 C = (cx, cy, cz) 이고, R 이 반지름인 구의 방정식
    // (x-cx)*(x-cx) + (y-cy)*(y-cy) + (z-cz)*(z-cz) = R*R
    // 중심 C로부터 점 p = (x, y, z) 로의 벡터 = p - C
    // dot((p - C), (p - C)) = (x-cx)*(x-cx) + (y-cy)*(y-cy) + (z-cz)*(z-cz) = R*R
    // dot((p - C), (p - C)) = R*R // any point p that satisfies this equation is on the sphere

    // 구에 1점에서 만날 경우(동일 벡터의 곱이므로 각도가 0 이 되고, 내적이 반지름의 제곱과 동일하게됨)
    // p(t) = A + tB 라고 하면 (p(t) 는 점 A 로부터 B 방향으로의 벡터)
    // dot(p(t) - C, p(t) - C) = R^2, C = center    // 이 식을 만족하면, 점 p(t) 는 구 위에 있다!
    // p(t) = A + tB = r.origin() + t * r.direction()
    // dot(p(t) - C, p(t) - C) = dot(r.origin() + t * r.direction() - C, ~)

    // 벡터합에 대한 결합법칙
    // ==> vec_a*(vec_b + vec_c) = vec_a*vec_b + vec_a*vec_c
    // 스칼라곱에 대한 결합법칙
    // ==> (c x vec_a) * vec_b = c x (vec_a * vec_b) = vec_a * (c x vec_b)

    // The rules of vector algebra 에 의해서 아래 공식이 성립된다고 함!!!
    // dot(X + tY, X + tY) = R*R (t 는 실수(only unknown), X, Y 는 벡터)
    // t*t*dot(Y, Y) + 2*t*(Y, X) + dot(X, X) - R*R = 0 // from 벡터합에 대한 결합법칙?!
    // X = A - C, Y = B 를 대입하면
    // dot((A + tB - C), (A + tB - C)) = R*R
    // ==> t*t*dot(B, B) + 2*t*(B, A - C) + dot(A - C, A - C) - R*R = 0

    // A = r.origin(), B = r.direction()
    // t^2 * dot(r.direction(), r.direction()) + 2t * dot(r.direction(), r.origin() - C) + dot(r.origin() - C, r.origin() - C) - R^2 = 0
    // r.origin() - C == oc, 내적은 교환법칙 성립
    // t^2 * dot(r.direction(), r.direction()) + 2t * dot(oc, r.direction()) + dot(oc, oc) - R^2 = 0
    // ==> ax^2 + bx + c = 0 이라 하면

    // a, b, c 값이 아래와 같음!
    vec3 oc = r.origin() - center; // center(구의 중심)에서 r.origin() 까지의 vector
    //assert(oc.x() == 0.f && oc.y() == 0.f && oc.z() == 1.f);
    float a = dot(r.direction(), r.direction());
    float b = 2.0 * dot(oc, r.direction());
    float c = dot(oc, oc) - radius * radius;
    // x = (-b +- sqrt(b^2 - 4ac)) / 2a 에서 b^2 - 4ac == 0 이면, 1점에서 만남
    // b^2 - 4ac > 0 크면 2점에서 만남
    float discriminant = b * b - 4 * a * c;
    return (discriminant > 0);
}

static vec3 color(const ray& r) {
    if (hit_sphere(vec3(0, 0, -1), 0.5, r)) // vec3(0, 0, 1) 여도 동일하게 빨간 구체가 그려지게 된다! (lower_left_corner.z 가 -1 or 1 이든 OK!)
        return vec3(1.0, 0, 0); // 구와 만나면 red color
    vec3 unit_direction = unit_vector(r.direction());
    // top -> bottom 순서로 그리므로 unit_direction.y() = [1, -1] 범위
    float t = 0.5 * (unit_direction.y()/*[1, -1]*/ + 1.0); // t = [1.0, 0.0]

    // t 가 [1.0, 0.0] 순서이므로 top = sky color, bottom = white color 
    // linear blend, or linear interpolation, or lerp for short
    // lerp 식은 blended_value = (1-t)*start_value + t*end_value, with t going from 0 to 1.
    return (1.0 - t) * vec3(1.0, 1.0, 1.0) + t * vec3(0.5, 0.7, 1.0); // sky color
    //return (1.0 - t) * vec3(1.0, 1.0, 1.0) + t * vec3(1.0, 0., 0.); // red color
}

int main_4()
{
    std::string filePath = "ch4_adding_a_sphere.ppm";
    std::ofstream f(filePath);
    if (!f.is_open()) {
        std::cerr << filePath << " open failed!\n";
        return -1;
    }

    int nx = 200;
    int ny = 100;
    f << "P3\n" << nx << " " << ny << "\n255\n";
    vec3 horizontal(4.0, 0.0, 0.0); // 폭이 4
    vec3 vertical(0.0, 2.0, 0.0);   // 높이 2
    vec3 origin(0.0, 0.0, 0.0);     // 정중앙
    vec3 lower_left_corner(-2.0, -1.0, -1.0);   // origin 에서 화면 안쪽(into the screen, z=-1), 왼쪽 아래 위치

    // 아래 color(r) 리턴값이 [0, 1] 이므로, 256 이 아닌 255.99 를 사용함([0, 255])
    const float color_coefficient = 255.99;

    for (int j = ny - 1; j >= 0; j--) {
        for (int i = 0; i < nx; i++) {
            float u = float(i) / nx; // u = [0, 1)
            float v = float(j) / ny; // v = (1, 0]
            ray r(origin, lower_left_corner + u * horizontal + v * vertical);
            // -> (-2.0, -1.0, -1.0) + 0 * (4.0, 0.0, 0.0) + 1 * (0.0, 2.0, 0.0) = (-2.0, 1.0, -1.0)
            
            // 하늘색 배경에 빨간색 구가 표시됨
            vec3 col = color(r);
            int ir = color_coefficient * col[0];
            int ig = color_coefficient * col[1];
            int ib = color_coefficient * col[2];

            f << ir << " " << ig << " " << ib << "\n";
            if (j == 0) {
                //std::cout << "[" << i << "] " << ir << " " << ig << " " << ib << "\n";
            }
        }
    }
    f.close();
    std::cout << filePath << " write finished!\n";
}
