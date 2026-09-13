#pragma once
#include"Math.h"
#include<cstdint>


struct Ray{
    Vec3 origin;
    Vec3 direction;
};

struct hitRecord{
    float t;
    Vec3 P;
    Vec3 N;
    Color c;
    bool hit = false;
};

struct Sphere{//球體
    Vec3 center;
    float radius;
    Color color;

    bool hit(const Ray& ray, float& t) const {//計算是否相交
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

struct triangle{
    Vec3 v0;
    Vec3 v1;
    Vec3 v2;
    Color c;
    Vec3 getNormal()const{ 
    Vec3 edge1 = v1 - v0;
    Vec3 edge2 = v2 - v0;
    Vec3 rawNormal = edge1.cross(edge2);
    return rawNormal.normalize();
    }
    
    bool hit(const Ray&ray, float& t)const{
        Vec3 edge1 = v1 - v0;
        Vec3 edge2 = v2 - v0;
        Vec3 pvec = ray.direction.cross(edge2);
        Vec3 T = ray.origin - v0;
        Vec3 qvec = T.cross(edge1);
        float det = edge1.dot(pvec);
        if(std::abs(det) < 1e-8f){
            return false;
        }
        float invDet = 1 / det;
        float u = T.dot(pvec) * invDet;
        float v = ray.direction.dot(qvec) * invDet;
        if(v < 0){
            return false;
        }
        if(u < 0){
            return false;
        }
        else if(1 <= (u + v) ||(u + v) < 0){
            return false;
        }
        t = edge2.dot(qvec) * invDet;
        if(t < 1e-8f){
            return false;
        }
        return true;
    }

};
