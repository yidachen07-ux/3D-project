#include<iostream>
#include<fstream>
#include<sstream>
#include <fstream>
#include <cstdint>
#include <algorithm>
#include<vector>
#include<cmath>
struct Vec3;
struct Mat4;
struct Color;
struct Framebutte;
struct drawline;

struct Vec3{//operator定義符號+-*/
    float x, y, z;
    Vec3() : x(0), y(0), z(0) {}
    Vec3(float x, float y, float z) : x(x), y(y), z(z) {}
    Vec3  operator+(const Vec3& other) const { return Vec3(x+other.x, y+other.y, z+other.z); }//向量加法
    Vec3  operator-(const Vec3& other) const { return Vec3(x-other.x, y-other.y, z-other.z); }//向量減法
    Vec3  operator*(float s) const { return Vec3(x*s, y*s, z*s); }//向量乘法
    Vec3  operator/(float s) const { return Vec3(x/s, y/s, z/s); }//向量除法
    float length() const { return sqrt ( x*x + y*y + z*z ); }//向量長度
    Vec3  normalize() const { return Vec3 ( x/length(), y/length(), z/length()); }//標準化1
    float dot(const Vec3& other) const {return ( x*other.x+ y*other.y+ z*other.z); }//向量內積
    Vec3  cross(const Vec3& other) const {return Vec3( y*other.z-other.y*z, z*other.x-other.z*x, x*other.y-other.x*y);}//向量外積 
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
    static Mat4 perspective(float fovY, float aspect, float near, float far) {
    Mat4 r{};
    for(int i = 0;i < 4;i++){   
        for(int j = 0;j < 4;j++)
        if (i == j){
        r.m[i][j] = 1;}
        else {
        r.m[i][j] = 0;}
        }
        float t = tan(fovY / 2);
        r.m[0][0] = 1 / (aspect*t);
        r.m[1][1] = 1 / t;
        r.m[2][2] = -(far+near)/(far-near);
        r.m[2][3] = -(2*far*near)/(far-near);
        r.m[3][2] = -1;
        r.m[3][3] = 0;
        return r;
    }
    Vec3 operator*(const Vec3& v)const{//透視除法
        float x = m[0][0] * v.x + m[0][1] * v.y + m[0][2] * v.z + m[0][3];
        float y = m[1][0] * v.x + m[1][1] * v.y + m[1][2] * v.z + m[1][3];
        float z = m[2][0] * v.x + m[2][1] * v.y + m[2][2] * v.z + m[2][3];
        float w = m[3][0] * v.x + m[3][1] * v.y + m[3][2] * v.z + m[3][3];
        return Vec3(x/w, y/w, z/w);
    }

};

struct Color{//顏色
    uint8_t a ,r, g, b;
};

struct Framebuffer{//畫布,斜線,解析度,三角形,光柵化
    int width = 1920;//解析度寬度
    int height = 1080;//解析度高度
    std::vector< Color > pixels;
    std::vector< float >zBuffer;
    Framebuffer() : pixels(width * height), zBuffer(width * height, std::numeric_limits<float>::max()){};
    void setpixels (int x, int y, Color c){
        if(x >= 0 && x < width && y >= 0 && y < height){//判斷是否出界
            pixels [width * y + x] = c;
        }
    }
    void clear (Color c){//畫出畫布
        for (int i = 0; i < width; i++){
            for(int j = 0; j < height; j++)
            setpixels(i, j, c);
        }
    }
    void savePPM(const std::string& filename){//畫布匯出.ppm
        std::ofstream outfile(filename);
        outfile << "P3\n";
        outfile << width << " " << height << "\n";
        outfile << "255\n";
        for (int y = height -1 ;y >= 0; y--){//反轉畫布
            for (int x = 0;x < width; x++){
                Color c = pixels [width * y + x];
                outfile << (int) c.r << " " << (int) c.g << " " << (int) c.b <<"\n";
            }
            outfile << "\n";
        }
        outfile.close();
    }
    void drawLine(int x0, int y0, int x1 ,int y1, Color c){
        bool steep = std::abs(x0 - x1) < std::abs(y0 - y1);//if steep 1=ture
        if (steep){//如果斜率大於一把x軸y軸調換(以x=y調換)
            std::swap(x0, y0);
            std::swap(x1, y1);
        }
        if(x0 > x1){//如果起點>終點調換(從終點開始)
            std::swap(x0, x1);
            std::swap(y0, y1);
        }
        int dx = x1 - x0;
        int dy = y1 - y0;
        int y = y0;
        int ystep = (y1 < y0) ? -1 : 1;
        int P = 2 * std::abs(dy) - dx;
        for(int x = x0; x <= x1; x++){//Bresenham演算法
            if(steep){
            setpixels(y, x, c);
            }
            else{
            setpixels(x, y, c);
            }
            if(P>=0){
            y += ystep;
            P = P + 2 * std::abs(dy) - 2 * dx;
            }
            else {
            P = P + 2 * std::abs(dy);
            }   
        }
    }
    void drawtriangle(Vec3 v0, Vec3 v1, Vec3 v2, Mat4 mvp, Color c){
        Vec3 p0 = mvp * v0;
        Vec3 p1 = mvp * v1;
        Vec3 p2 = mvp * v2;
        int x0 = (int)((p0.x + 1) * width / 2);//Vec3轉換螢幕座標
        int y0 = (int)((1 + p0.y) * height / 2);
        int x1 = (int)((p1.x + 1) * width / 2);
        int y1 = (int)((1 + p1.y) * height / 2);
        int x2 = (int)((p2.x + 1) * width / 2);
        int y2 = (int)((1 + p2.y) * height / 2);
        int z0 = v0.z;
        int z1 = v1.z;
        int z2 = v2.z;

        drawLine(x0, y0, x1, y1, c);
        drawLine(x0, y0, x2, y2, c);
        drawLine(x1, y1, x2, y2, c);
    } 
    void filltriangle(Vec3 v0, Vec3 v1, Vec3 v2, Mat4 mvp, Color c){
        Vec3 p0 = mvp * v0;
        Vec3 p1 = mvp * v1;
        Vec3 p2 = mvp * v2;
        int x0 = (int)((p0.x + 1) * width / 2);//Vec3轉換螢幕座標
        int y0 = (int)((1 + p0.y) * height / 2);
        int x1 = (int)((p1.x + 1) * width / 2);
        int y1 = (int)((1 + p1.y) * height / 2);
        int x2 = (int)((p2.x + 1) * width / 2);
        int y2 = (int)((1 + p2.y) * height / 2);
        float z0 = p0.z;
        float z1 = p1.z;
        float z2 = p2.z;

        int maxX = std::max(x0, std::max (x1, x2) );//畫出四邊形
        int minX = std::min(x0, std::min (x1, x2) );
        int maxY = std::max(y0, std::max (y1, y2) );
        int minY = std::min(y0, std::min (y1, y2) );

        for(int x = minX; x <= maxX; x++){//用外積決定在三角形線左邊還右邊
            for(int y = minY; y <= maxY; y++){
                int cross0 = (x1 - x0) * (y - y0) - (y1 - y0) * (x - x0);
                int cross1 = (x2 - x1) * (y - y1) - (y2 - y1) * (x - x1);
                int cross2 = (x0 - x2) * (y - y2) - (y0 - y2) * (x - x2);
                int z = (cross0 * z2 + cross1 * z0 + cross2 * z1) / (cross0 + cross1 + cross2);                
                if(cross0 >= 0 && cross1 >= 0 && cross2 >= 0 || cross0 <= 0 && cross1 <= 0 && cross2 <= 0){
                    if(z < zBuffer[width * y + x]){
                    zBuffer[width * y + x] = z;
                    setpixels(x, y, c);
                    }
                    
                }
            }
        }
    }
};

struct triangle{
    Vec3 v0;
    Vec3 v1;
    Vec3 v2;
};

struct Model{//obj導入
    std::vector <triangle> triangles;

    bool loadobj(std::string filename){
        std::ifstream file(filename);
        if(!file.is_open()){
            std::cout << "error";
            return false;
        }
        std::string line;
        std::vector <Vec3> vertices;
        while(std::getline(file,line)){
            if (line[0]=='v'){
                std::istringstream iss(line);
                std::string prefix;
                float x, y, z;
                iss >> prefix >> x >> y >> z;
                vertices.push_back(Vec3(x, y, z));
            }
            if(line[0] == 'f'){
                
            }
            }
            return true;
        }
};


int main(){
    Framebuffer fb;
    fb.clear({255, 255, 255, 255});
    Model myModel;
    Mat4 M = Mat4::identity();
    Mat4 V = Mat4::lookAt({6, 10, 5}, {0, 0, 0}, {0, 1, 0});
    Mat4 P = Mat4::perspective(3.14159f / 3, 1920.0f / 1080.0f, 0.1f, 100.0f);
    Mat4 mvp = P * V * M;

    // t.v0 = {0.0, 0.5, 0.0};
    // t.v1 = {0.5, -0.5, 0.0};
    // t.v2 = {-0.5, -0.5, 0.0};
    std::vector <Vec3> cubeVerts = {
    {-0.5, -0.5, -0.5},//後左下0
    { 0.5, -0.5, -0.5},//後右下1
    { 0.5,  0.5, -0.5},//後右上2
    {-0.5,  0.5, -0.5},//後左上3
    {-0.5, -0.5,  0.5},//前左下4
    { 0.5, -0.5,  0.5},//前右下5
    { 0.5,  0.5,  0.5},//前右上6
    {-0.5,  0.5,  0.5},//前左下7
    };
    int indices[] = {
    // 後面 (面向 Z = -0.5)
    1, 0, 3,   1, 3, 2,
    // 前面 (面向 Z = 0.5)
    4, 5, 6,   4, 6, 7,
    // 右面 (面向 X = 0.5)
    5, 1, 2,   5, 2, 6,
    // 左面 (面向 X = -0.5)
    0, 4, 7,   0, 7, 3,
    // 上面 (面向 Y = 0.5)
    7, 6, 2,   7, 2, 3,
    // 下面 (面向 Y = -0.5)
    4, 5, 1,   4, 1, 0
};
    
    int indicesCount = sizeof(indices) / sizeof(indices[0]);
    for(int i = 0; i < indicesCount; i += 3){
        triangle t;
        t.v0 = cubeVerts[indices[i]];
        t.v1 = cubeVerts[indices[1+i]];
        t.v2 = cubeVerts[indices[2+i]];
        myModel.triangles.push_back(t);
    }
    for(int i = 0; i < myModel.triangles.size(); i++){
        fb.filltriangle(myModel.triangles[i].v0, myModel.triangles[i].v1, myModel.triangles[i].v2, mvp, {255,0,0,0});
    }
    // fb.drawtriangle(myModel.triangles[0].v0, myModel.triangles[0].v1, myModel.triangles[0].v2, mvp, {255, 0, 0, 0});
    // fb.filltriangle(myModel.triangles[0].v0, myModel.triangles[0].v1, myModel.triangles[0].v2, mvp, {255, 0, 0, 0});
    
    fb.savePPM("output.ppm");
    return 0;

}