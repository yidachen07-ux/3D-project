#pragma once
#include<iostream>
#include"Math.h"
struct Color{//顏色
    uint8_t a ,r, g, b;
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