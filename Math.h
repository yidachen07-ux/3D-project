#pragma once
#include<iostream>
#include<cmath>
struct Vec3{//operator定義符號+-*/
    float x, y, z;
    Vec3 () : x(0), y(0), z(0) {}
    Vec3 (float x, float y, float z) : x(x), y(y), z(z) {}
    Vec3 operator+(const Vec3& other) const { return Vec3(x+other.x, y+other.y, z+other.z); }//向量加法
    Vec3 operator-(const Vec3& other) const { return Vec3(x-other.x, y-other.y, z-other.z); }//向量減法
    Vec3 operator*(float s) const { return Vec3(x * s, y * s, z * s); }//向量乘法
    Vec3 operator/(float s) const { return Vec3(x / s, y / s, z / s); }//向量除法
    Vec3& operator+=(const Vec3& other) {x += other.x; y += other.y; z += other.z; return *this;}
    Vec3& operator-=(const Vec3& other) {x -= other.x; y -= other.y; z -= other.z; return *this;}
    float length() const { return sqrt (x * x + y * y + z * z); }//向量長度
    Vec3 normalize() const { return Vec3 (x / length(), y / length(), z / length()); }//標準化1
    float dot(const Vec3& other) const {return (x * other.x+ y * other.y+ z * other.z); }//向量內積
    Vec3 cross(const Vec3& other) const {return Vec3(y * other.z-other.y * z, z * other.x-other.z * x, x * other.y-other.x * y);}//向量外積   
};
struct Vec4{
    float x, y, z, w;
};

struct Mat4{//4*4矩陣
        float m[4][4];
    static Mat4 identity(){//對角矩陣
    Mat4 r;
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++)
        if (i == j){
         r.m[i][j] = 1;}
        else {
         r.m[i][j]= 0 ;}
        }
        return r;
    }
    Mat4 operator* (const Mat4& other)const{//矩陣乘法
    Mat4 result{};
        for (int i = 0;i < 4 ; i++){
            for(int j = 0; j < 4; j++)
                for(int k = 0; k < 4; k++)
                result.m[i][j] += m[i][k] * other.m[k][j];                  
        }
            return result;
    }
   
    static Mat4 translation(float tx, float ty, float tz){//平移矩陣
    Mat4 r;
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++)
        if (i == j){
         r.m[i][j] = 1;}
        else {
         r.m[i][j] = 0;}
        }
        r.m[0][3] = tx;
        r.m[1][3] = ty;
        r.m[2][3] = tz;
        return r;
    }
    static Mat4 scale(float sx, float sy, float sz) {//旋轉矩陣
    Mat4 r;
    for(int i = 0; i < 4; i++){   
        for(int j = 0; j < 4; j++)
        if (i == j){
         r.m[i][j] = 1;}
        else {
         r.m[i][j] = 0;}
        }
        r.m[0][0] = sx;
        r.m[1][1] = sy;
        r.m[2][2] = sz;
        return r;
    }
    static Mat4 rotationZ(float angle) {//z旋轉矩陣
    Mat4 r;
    for(int i = 0; i < 4; i++){   
        for(int j = 0; j < 4; j++)
        if (i == j){
         r.m[i][j] = 1;}
        else {
         r.m[i][j] = 0;}
        }
        r.m[0][0] = cos(angle);
        r.m[0][1] = -sin(angle);
        r.m[1][0] = sin(angle);
        r.m[1][1] = cos(angle);
        return r;
    }
    static Mat4 rotationX(float angle) {//x旋轉矩陣
    Mat4 r;
    for(int i=0;i<4;i++){   
        for(int j=0;j<4;j++)
        if (i==j){
         r.m[i][j]=1;}
        else {
         r.m[i][j]=0;}
        }
        r.m[1][1] = cos(angle);
        r.m[1][2] = -sin(angle);
        r.m[2][1] = sin(angle);
        r.m[2][2] = cos(angle);
        return r;
    }
    static Mat4 rotationY(float angle) {//y旋轉矩陣
    Mat4 r;
    for(int i = 0; i < 4; i++){   
        for(int j = 0; j < 4; j++)
        if (i == j){
         r.m[i][j]= 1;}
        else {
         r.m[i][j]= 0;}
        }
        r.m[0][0] = cos(angle);
        r.m[0][2] = -sin(angle);
        r.m[2][0] = sin(angle);
        r.m[2][2] = cos(angle);
        return r;
    }
    static Mat4 lookAt(Vec3 eye, Vec3 center, Vec3 up){//之後優化,eye鏡頭位置,center鏡頭看的方向,up鏡頭向上方向
        Vec3 forward = (center - eye).normalize();
        Vec3 right = (forward.cross(up)).normalize();
        Vec3 camUp = (right.cross(forward)).normalize();
        Mat4 r{};
        for(int i = 0; i < 4; i++){   
            for(int j = 0; j < 4; j++)
            if (i == j){
            r.m[i][j] = 1;}
            else {
            r.m[i][j]= 0 ;}
            }
            r.m[0][0] = right.x;
            r.m[0][1] = right.y;
            r.m[0][2] = right.z;
            r.m[0][3] = -right.dot(eye);
            r.m[1][0] = camUp.x;
            r.m[1][1] = camUp.y;
            r.m[1][2] = camUp.z;
            r.m[1][3] = -camUp.dot(eye);
            r.m[2][0] = -forward.x;
            r.m[2][1] = -forward.y;
            r.m[2][2] = -forward.z;
            r.m[2][3] = forward.dot(eye);
            return r;
    }
    static Mat4 perspective(float fovY, float aspect, float near, float far) {//視角大小,長寬比,近裁剪面,遠裁剪面
        Mat4 r{};
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                if (i == j) {
                    r.m[i][j] = 1;
                } else {
                    r.m[i][j] = 0;
                }
            }
        }
        float t = tan(fovY / 2);
        r.m[0][0] = 1 / (aspect * t);
        r.m[1][1] = 1 / t;
        r.m[2][2] = -(far + near) / (far - near);
        r.m[2][3] = -(2 * far * near) / (far - near);
        r.m[3][2] = -1;
        r.m[3][3] = 0;
        return r;
    }

    Vec4 transform(const Vec3& v) const { // 透視除法
        float x = m[0][0] * v.x + m[0][1] * v.y + m[0][2] * v.z + m[0][3];
        float y = m[1][0] * v.x + m[1][1] * v.y + m[1][2] * v.z + m[1][3];
        float z = m[2][0] * v.x + m[2][1] * v.y + m[2][2] * v.z + m[2][3];
        float w = m[3][0] * v.x + m[3][1] * v.y + m[3][2] * v.z + m[3][3];
        return Vec4{x, y, z, w};
    }

    Vec3 operator*(const Vec3& v) const { // 透視除法
        float x = m[0][0] * v.x + m[0][1] * v.y + m[0][2] * v.z + m[0][3];
        float y = m[1][0] * v.x + m[1][1] * v.y + m[1][2] * v.z + m[1][3];
        float z = m[2][0] * v.x + m[2][1] * v.y + m[2][2] * v.z + m[2][3];
        float w = m[3][0] * v.x + m[3][1] * v.y + m[3][2] * v.z + m[3][3];
        return Vec3(x / w, y / w, z / w);
    }
};