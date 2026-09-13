#pragma once
#include <iostream>
#include <algorithm>
#include <cmath>
#include <vector>
#include <thread>
#include "Math.h"
#include "Geometry.h"
#include "Framebuffer.h"
// inline Color rayColor(const Ray&ray, const std::vector<triangle>& triangles, Vec3 lightDir){//光追著色
//     float light = std::max(0.0f, rec.N.dot(lightDir));
//             float ambient = 0.2f;//環境光
//             float intensity = ambient + (1.0f - ambient) * light;//著色強度

            
//             uint8_t r = (uint8_t)std::clamp(c.r * intensity, 0.0f, 255.0f);//顏色邊界測試
//             uint8_t g = (uint8_t)std::clamp(c.g * intensity, 0.0f, 255.0f);//顏色邊界測試
//             uint8_t b = (uint8_t)std::clamp(c.b * intensity, 0.0f, 255.0f);//顏色邊界測試
//             Color bright = {(uint8_t)b, (uint8_t)g, (uint8_t)r,255};
    
         
//     float skyT = 0.5f * (ray.direction.y + 1.0f);//天空著色
//     uint8_t r = (uint8_t)((1.0f - skyT) * 255.0f + skyT * 130.0f);
//     uint8_t g = (uint8_t)((1.0f - skyT) * 255.0f + skyT * 180.0f);
//     uint8_t b = (uint8_t)((1.0f - skyT) * 255.0f + skyT * 255.0f);

//     return Color{b, g, r, 255};
// };
// inline bool hitTriangle(Ray& ray, const std::vector<triangle>& mesh, hitRecord& rec){
//     float closest_t = std::numeric_limits<float>::max();
//     float dist;
//     const triangle* hitTri = nullptr;
    
//     for(int i = 0; i < mesh.size(); i++){
//         const triangle& onceT = mesh[i];
//         if(onceT.hit(ray, dist)){
//             if(dist > 0.0001f && dist < closest_t){
//             closest_t = dist;
//             hitTri = &onceT;
//             }
//         }
//     }
    
//     if(hitTri != nullptr){
//         Vec3 N = hitTri -> getNormal();
//         rec.t = closest_t;
//     }
//     else{

//     }
// };
struct TracerColor{
    Sphere sphere;
    const std::vector<triangle>& triangles;
    Vec3 lightDir;
    
    bool hitSphere(Ray& ray, Sphere& sp, hitRecord& rec){
        float t;
        Color c{0,0,255,255};//顏色
        Sphere sphere{Vec3(0.0f, 0.0f, 0.0f), 100.0f,{0,0,255,255}};//球(位子,半徑)
        if(sphere.hit(ray, t)){//球,三角形光追著色
            rec.t = t;
            rec.P = ray.origin + (ray.direction * t);
            rec.N = (rec.P - sphere.center).normalize();
            rec.hit = true;            
        }
        else{
            rec.hit = false;
            return false;
        }
    };
    bool hitTriangle(Ray& ray, const std::vector<triangle>& mesh, hitRecord& rec){
        float closest_t = std::numeric_limits<float>::max();
        const triangle* hitTri = nullptr;
        
        for(int i = 0; i < mesh.size(); i++){
            const triangle& onceT = mesh[i];
            float dist;
            bool found;
            if(onceT.hit(ray, dist)){
                if(dist > 0.0001f && dist < closest_t){
                closest_t = dist;
                rec.t = dist;
                rec.P = ray.origin + (ray.direction * dist);
                rec.N = (rec.P - sphere.center).normalize();
                found = true;
                }            
            }
            rec.hit = found;
        }
        
        
    };




};

struct Raytracer{//光追和多執行緒
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

                        // fb.pixels[fb.width * y + x] = rayColor(ray, triangles, lightDir);//著色
                    }
                }
            });
        }
        for (auto& t : threads) {
            t.join();
        }
    };


};
