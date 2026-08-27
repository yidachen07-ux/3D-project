#pragma once
#include <algorithm>
#include <cmath>
#include <vector>
#include <thread>
#include "Math.h"
#include "Geometry.h"
#include "Framebuffer.h"
inline Color rayColor(const Ray&ray, Vec3 lightDir){
    float t;
    Color c{0,0,255,255};
    Sphere sphere{Vec3(0.0f, 0.0f, 0.0f), 100.0f};
    if(sphere.hit(ray, t)){
        Vec3 P = ray.origin + (ray.direction * t);
        Vec3 N = (P - sphere.center).normalize();
        float light = std::max(0.0f, N.dot(lightDir));
        float ambient = 0.2f;
        float intensity = ambient + (1.0f - ambient) * light;
        
        uint8_t r = (uint8_t)std::clamp(c.r * intensity, 0.0f, 255.0f);
        uint8_t g = (uint8_t)std::clamp(c.g * intensity, 0.0f, 255.0f);
        uint8_t b = (uint8_t)std::clamp(c.b * intensity, 0.0f, 255.0f);
        Color bright = {(uint8_t)b, (uint8_t)g, (uint8_t)r,255};
        
        return bright;
    }
        
        
    float skyT = 0.5f * (ray.direction.y + 1.0f);
    uint8_t r = (uint8_t)((1.0f - skyT) * 255.0f + skyT * 130.0f);
    uint8_t g = (uint8_t)((1.0f - skyT) * 255.0f + skyT * 180.0f);
    uint8_t b = (uint8_t)((1.0f - skyT) * 255.0f + skyT * 255.0f);

    return Color{b, g, r, 255};
};
struct Raytracer{
    void render(Framebuffer& fb, Vec3 camPos, Vec3 forward, Vec3 right, Vec3 up, Mat4& P, Vec3 lightDir){
        float invWidth  = 2.0f / (float)fb.width;
        float invHeight = 2.0f / (float)fb.height;

        unsigned int numThreads = std::thread::hardware_concurrency();
        if (numThreads == 0) numThreads = 8;

        int rowsPerThread = fb.height / numThreads;
        std::vector<std::thread> threads;
        for(unsigned int i = 0; i < numThreads; i++){
            int yStart = i * rowsPerThread;
                
            int yEnd = (i == numThreads - 1) ? fb.height : (i + 1) * rowsPerThread;

            threads.emplace_back([=,&fb, &P]() {
                for (int y = yStart; y < yEnd; y++) {
                    for (int x = 0; x < fb.width; x++) {             
                        float u = ((x + 0.5f) * invWidth - 1.0f) / P.m[0][0];//寬標準化（中心點）
                        float v = (1.0f - (y + 0.5f) * invHeight) / P.m[1][1];//長標準化（中心點)

                        Vec3 rayDir = (forward + right * u + up * v).normalize();//算出ray方向
                        Ray ray{camPos, rayDir};

                        fb.pixels[fb.width * y + x] = rayColor(ray, lightDir);//著色
                    }
                }
            });
        }
        for (auto& t : threads) {
            t.join();
        }
    }
};