#pragma once
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

struct Sphere {
    Vec3 center;
    float radius;

    bool hit(const Ray& ray, float& t) const {
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
    if(t1 > 0.0f){
        t = t1;
    }
    else if(t2 > 0.0f) {
    t = t2;
    }
    else{
        return false;
    }
    return true;
    }
};

// inline bool hitSphere(const Ray& ray, const Vec3& center, float radius, float& t){
//     Vec3 oc = ray.origin-center;
//     float a = ray.direction.dot(ray.direction);
//     float b = 2.0f * oc.dot(ray.direction);
//     float c = oc.dot(oc) - radius * radius;
//     float delta = b * b - 4 * a *c;
//     if(delta < 0.0f){
//         return false;
//     }
//     float t1 = (-b - sqrt(delta)) / (2 * a);
//     float t2 = (-b + sqrt(delta)) / (2 * a);
//     if(t1 < 0 && t2 < 0){
//         return false;
//     }
//     if(t1 > 0.0f){
//         t = t1;
//     }
//     return true;
    
// };