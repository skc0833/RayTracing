#include <iostream>
#include <fstream>
#include <assert.h>
#include "sphere.h"
#include "hitable_list.h"
#include "camera.h"
#include "material.h"

static vec3 color(const ray& r, hitable* world, int depth) {
    hit_record rec;
    const bool DO_SHADOW_ACNE = true; // 자체 그림자 없애기???
    float min = 0.0;
    if (DO_SHADOW_ACNE)
        min = 0.001;
    if (world->hit(r, min, MAXFLOAT, rec)) {
        ray scattered;
        vec3 attenuation;
        if (depth < 50 && rec.mat_ptr->scatter(r, rec, attenuation, scattered)) {
            return attenuation * color(scattered, world, depth + 1);
        }
        else {
            return vec3(0, 0, 0);
        }
    }
    else {
        vec3 unit_direction = unit_vector(r.direction());
        float t = 0.5 * (unit_direction.y() + 1.0);
        return (1.0 - t) * vec3(1.0, 1.0, 1.0) + t * vec3(0.5, 0.7, 1.0); // sky color
    }
}

int main()
{
    std::string filePath = "ch8_metal.ppm";
    std::ofstream f(filePath);
    if (!f.is_open()) {
        std::cerr << filePath << " open failed!\n";
        return -1;
    }

    int nx = 200;
    int ny = 100;
    int ns = 100;
    f << "P3\n" << nx << " " << ny << "\n255\n";
    hitable* list[4];
    list[0] = new sphere(vec3(0, 0, -1), 0.5, new lambertian(vec3(0.8, 0.3, 0.3)));
    list[1] = new sphere(vec3(0, -100.5, -1), 100, new lambertian(vec3(0.8, 0.8, 0.0)));
    list[2] = new sphere(vec3(1, 0, -1), 0.5, new metal(vec3(0.8, 0.6, 0.2)));
    list[3] = new sphere(vec3(-1, 0, -1), 0.5, new metal(vec3(0.8, 0.8, 0.8)));
    hitable* world = new hitable_list(list, 4);
    camera cam;

    for (int j = ny - 1; j >= 0; j--) {
        for (int i = 0; i < nx; i++) {
            vec3 col(0, 0, 0);
            for (int s = 0; s < ns; s++) {
                float u = float(i + drand48()) / float(nx);
                float v = float(j + drand48()) / float(ny);
                ray r = cam.get_ray(u, v);
                vec3 p = r.point_at_paramter(2.0);
                col += color(r, world, 0);
            }
            col /= float(ns);
            // 1 이하 sqrt() 의 경우, 값이 커진다(e.g, sqrt(0.1) = 0.32, 0.5 -> 0.70, 0.9 -> 0.95
            col = vec3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2])); // yields light gray
            int ir = 255.99 * col[0];
            int ig = 255.99 * col[1];
            int ib = 255.99 * col[2];
            f << ir << " " << ig << " " << ib << "\n";
        }
    }
    f.close();
    std::cout << filePath << " write finished!\n";
}
