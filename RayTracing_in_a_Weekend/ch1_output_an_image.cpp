#include <iostream>
#include <fstream>
#include "vec3.h"

// from
// http://www.realtimerendering.com/raytracing/Ray%20Tracing%20in%20a%20Weekend.pdf
// http://www.realtimerendering.com/raytracing/Ray%20Tracing_%20The%20Next%20Week.pdf
// http://www.realtimerendering.com/raytracing/Ray%20Tracing_%20the%20Rest%20of%20Your%20Life.pdf
// .ppm 뷰어는 GIMP 사용 가능(https://www.gimp.org/ 에서 gimp-2.10.36-setup.exe 로 테스트함)
int main_1()
{
    // 좌상단 green, 우하단 red 로 사각형을 채움(eye(or camera center) 는 (0, 0, 0))
    std::string filePath = "ch1_output_an_image.ppm";
    // default mode 는 std::ios_base::out 이며, std::ios_base::binary mode 추가시 \r\n 대신 \n 가 기록됨
    std::ofstream f(filePath);
    if (!f.is_open()) {
        std::cerr << filePath << " open failed!\n";
        return -1;
    }

    int nx = 200; // 256;
    int ny = 100;
    f << "P3\n" << nx << " " << ny << "\n255\n";

    // 책에는 원래 255.99 인데, 256 이 더 정확한듯함
    // nx = 256 이고 color_coefficient = 256 일 경우, r = [0, 255] 범위 이지만,
    // color_coefficient = 255.99 의 경우, r = [0, 254] 임(255 값이 누락되고 있음)
    const float color_coefficient = 255.99; // 256;
    for (int j = ny - 1; j >= 0; j--) {
        // top 라인(ny - 1 == 99)부터 bottom 라인(0)까지 매 라인마다
        for (int i = 0; i < nx; i++) {
            // left 컬럼(0)부터 right 컬럼(nx - 1)까지
            // x 축 [0, 199] 구간은 r 값이 0 부터 255 까지 증가
            // --> right-top = red + green + b(0.2) = yellow, right-bottom = red + b(0.2)
            // y 축 [99, 0] 구간은 g 값이 255 부터 0 까지 감소
            // --> left-top = green + b(0.2), left-bottom = b(0.2) = black

            vec3 col(float(i) / nx, float(j) / ny, 0.2);
            int ir = color_coefficient * col[0]; // 원래는 color_coefficient 대신 255.99 였음
            int ig = color_coefficient * col[1];
            int ib = color_coefficient * col[2];

            f << ir << " " << ig << " " << ib << "\n";
            if (j == ny - 1) {
                std::cout << "[" << i << "] " << ir << " " << ig << " " << ib << "\n";
            }
        }
    }
    f.close();
    std::cout << filePath << " write finished!\n";
}
