#include <iostream>
#include <fstream>
#include "sphere.h"
#include "hitable_list.h"

static vec3 color(const ray& r, hitable* world) {
    hit_record rec;
    if (world->hit(r, 0.0, std::numeric_limits<float>::max(), rec)) {
        return 0.5 * vec3(rec.normal.x() + 1, rec.normal.y() + 1, rec.normal.z() + 1);
    }
    else {
        vec3 unit_direction = unit_vector(r.direction());
        float t = 0.5 * (unit_direction.y() + 1.0);
        return (1.0 - t) * vec3(1.0, 1.0, 1.0) + t * vec3(0.5, 0.7, 1.0); // sky color
    }
}

int main_5_2()
{
    std::string filePath = "ch5_2_multiple_objects.ppm";
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
    hitable* list[2];
    list[0] = new sphere(vec3(0, 0, -1), 0.5);      // 작은 구(r = 0.5)
    list[1] = new sphere(vec3(0, -100.5, -1), 100); // 큰 구(r = 100)
    hitable* world = new hitable_list(list, 2);

    for (int j = ny - 1; j >= 0; j--) {
        for (int i = 0; i < nx; i++) {
            float u = float(i) / nx; // u = [0, 1)
            float v = float(j) / ny; // v = (1, 0]
            ray r(origin, lower_left_corner + u * horizontal + v * vertical);
            
            vec3 p = r.point_at_paramter(2.0);
            vec3 col = color(r, world);
            int ir = 255.99 * col[0]; // skc why 255.99 instead of just 255???
            int ig = 255.99 * col[1];
            int ib = 255.99 * col[2];

            f << ir << " " << ig << " " << ib << "\n";
        }
    }
    f.close();
    std::cout << filePath << " write finished!\n";
}
