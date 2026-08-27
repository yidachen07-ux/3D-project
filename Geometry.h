#pragma once
#include<iostream>
#include"Math.h"
struct Color{//顏色
    uint8_t b, g, r, a;
};

struct triangle{
    Vec3 v0;
    Vec3 v1;
    Vec3 v2;

    Vec3 getNormal() const{ 
    Vec3 edge1 = v1 - v0;
    Vec3 edge2 = v2 - v0;
    Vec3 rawNormal = edge1.cross(edge2);
    return rawNormal.normalize();
    }
};

struct Ray{
    Vec3 origin;
    Vec3 direction;
};

inline bool hitSphere(const Ray& ray, const Vec3& center, float radius){
    Vec3 oc = ray.origin-center;
    float a = ray.direction.dot(ray.direction);
    float b = 2.0f * oc.dot(ray.direction);
    float c = oc.dot(oc) - radius * radius;
    float delta = b * b - 4 * a *c;
    if(delta < 0.0f){
        return false;
    }
    float t1 = (-b - sqrt(delta)) / (2 * a);
    float t2 = (-b + sqrt(delta)) / (2 * a);
    if(t1 < 0 && t2 < 0){
        return false;
    }
    return true;

};
inline Color rayColor(const Ray&ray){
    if(hitSphere(ray, Vec3(0.0f, 0.0f, 0.0f), 100.0f)){
        return{0, 0, 255, 255};
    }
    Vec3 dir = ray.direction;
    Vec3 unit_dir = dir / dir.length();
    float t = 0.5f * (unit_dir.y + 1.0f);// 背景漸層 (從白到淡藍)
    Vec3 col = Vec3(1.0f, 1.0f, 1.0f) * (1.0f - t) + Vec3(0.5f, 0.7f, 1.0f) * t;
    uint8_t r = (uint8_t)(std::clamp(col.x, 0.0f, 1.0f) * 255.0f);
    uint8_t g = (uint8_t)(std::clamp(col.y, 0.0f, 1.0f) * 255.0f);
    uint8_t b = (uint8_t)(std::clamp(col.z, 0.0f, 1.0f) * 255.0f);
    return Color{b, g, r, 255};
    
}

