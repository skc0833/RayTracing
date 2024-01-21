#include <iostream>
#include <fstream>
#include <assert.h>
//#include <stdlib.h> // for drand48()
#include "sphere.h"
#include "hitable_list.h"
#include "camera.h"

#define drand48() double(rand()) / (RAND_MAX + 1.0) // [0, 1)
#define MAXFLOAT std::numeric_limits<float>::max()

static vec3 color(const ray& r, hitable* world) {
    hit_record rec;
    if (world->hit(r, 0.0, MAXFLOAT, rec)) {
        // 법선 벡터를 컬러로 표시함
        return 0.5 * vec3(rec.normal.x() + 1, rec.normal.y() + 1, rec.normal.z() + 1);
    }
    else {
        vec3 unit_direction = unit_vector(r.direction());
        float t = 0.5 * (unit_direction.y() + 1.0);
        return (1.0 - t) * vec3(1.0, 1.0, 1.0) + t * vec3(0.5, 0.7, 1.0);
    }
}

int main_6()
{
    bool do_antialias = true;
    std::string filePath = "ch6_antialiasing.ppm";
    if (!do_antialias) {
        filePath = "ch6_antialiasing_off.ppm";
    }
    std::ofstream f(filePath);
    if (!f.is_open()) {
        std::cerr << filePath << " open failed!\n";
        return -1;
    }

    int nx = 200; // [-2, 2) 구간을 200 픽셀에 표시
    int ny = 100; // [-1, 1) 구간을 100 픽셀에 표시
    int ns = 10; // 100;
    f << "P3\n" << nx << " " << ny << "\n255\n";
    hitable* list[2];
    list[0] = new sphere(vec3(0, 0, -1), 0.5);
    list[1] = new sphere(vec3(0, -100.5, -1), 100);
    hitable* world = new hitable_list(list, 2);
    camera cam; // default origin = vec3(0.0, 0.0, 0.0);

    for (int j = ny - 1; j >= 0; j--) {
        for (int i = 0; i < nx; i++) {
            vec3 col(0, 0, 0);
            for (int s = 0; s < ns; s++) {
                float u_alias = 0.f;
                float v_alias = 0.f;
                if (do_antialias) {
                    u_alias = drand48(); // 0 <= drand48() < 1.0
                    v_alias = drand48();
                    assert(u_alias >= 0 && u_alias < 1.0 && v_alias >= 0 && v_alias < 1.0);
                }
                // i(0) / 200 = 0 ~ (i(0) + 0.99) / 200 = 0.00495
                // i(0) 0 ~ 0.00495, i(1) 0.005 ~ 0.00995, ... i(198) 0.99 ~ 0.99495, i(199) 0.995 ~ 0.99995
                float u = float(i + u_alias) / float(nx); // x 축으로 1/200 + random 만큼 움직인다.
                assert(u >= 0 && u < 1.0);
                float v = float(j + v_alias) / float(ny); // y 축으로 1/100 + random 만큼 움직인다.
                assert(v >= 0 && v < 1.0); // (j(99) + 0.99) / 100 = 0.9999

                //float u = float(i + (double(rand()) / RAND_MAX)) / float(nx);
                //float v = float(j + (double(rand()) / RAND_MAX)) / float(ny);
                //float u0 = float(i) / nx; // u0 = [0, 1)
                //float v0 = float(j) / ny; // v0 = (1, 0]
                //assert(abs(u - u0) < 0.02 && abs(v - v0) < 0.02);
                
                // origin(0.0, 0.0, 0.0) 에서 lower_left_corner 기준 u * horizontal, v * vertical 점으로의 벡터
                ray r = cam.get_ray(u, v);
                //vec3 p = r.point_at_paramter(2.0);
                col += color(r, world);
            }
            col /= float(ns);
            int ir = 255.99 * col[0];
            int ig = 255.99 * col[1];
            int ib = 255.99 * col[2];
            f << ir << " " << ig << " " << ib << "\n";
        }
    }
    f.close();
    std::cout << filePath << " write finished!\n";
}
