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
    float length   () const { return sqrt ( x*x + y*y + z*z ); }//向量長度
    Vec3  normalize() const { return Vec3 ( x/length(), y/length(), z/length()); }//標準化1
    float dot  (const Vec3& other) const {return ( x*other.x+ y*other.y+ z*other.z); }//向量內積
    Vec3  cross(const Vec3& other) const {return Vec3( y*other.z-other.y*z, z*other.x-other.z*x, x*other.y-other.x*y);}//向量外積 
};

struct Mat4{//4*4矩陣
        float m[4][4];
    static Mat4 identity(){
    Mat4 r;
    for(int i=0;i<4;i++){   //對角矩陣
        for(int j=0;j<4;j++)
        if (i==j){
         r.m[i][j]=1;}
        else {
         r.m[i][j]=0;}
        }
        return r;
    }
    Mat4 operator* (const Mat4& other)const{//矩陣乘法
    Mat4 result{};
        for (int i=0;i<4;i++){
            for(int j=0;j<4;j++)
                for(int k=0;k<4;k++)
                result.m[i][j] += m[i][k] * other.m[k][j];                  
        }
            return result;
    }
   
    static Mat4 translation(float tx, float ty, float tz) {
    Mat4 r;
    for(int i=0;i<4;i++){   //平移矩陣
        for(int j=0;j<4;j++)
        if (i==j){
         r.m[i][j]=1;}
        else {
         r.m[i][j]=0;}
        }
        r.m[0][3]= tx;
        r.m[1][3]= ty;
        r.m[2][3]= tz;
        return r;
    }
    static Mat4 scale(float sx, float sy, float sz) {//旋轉矩陣
    Mat4 r;
    for(int i=0;i<4;i++){   
        for(int j=0;j<4;j++)
        if (i==j){
         r.m[i][j]=1;}
        else {
         r.m[i][j]=0;}
        }
        r.m[0][0]=sx;
        r.m[1][1]=sy;
        r.m[2][2]=sz;
        return r;
    }
    static Mat4 rotationZ(float angle) {//z旋轉矩陣
    Mat4 r;
    for(int i=0;i<4;i++){   
        for(int j=0;j<4;j++)
        if (i==j){
         r.m[i][j]=1;}
        else {
         r.m[i][j]=0;}
        }
        r.m[0][0]=cos(angle);
        r.m[0][1]=-sin(angle);
        r.m[1][0]=sin(angle);
        r.m[1][1]=cos(angle);
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
        r.m[1][1]=cos(angle);
        r.m[1][2]=-sin(angle);
        r.m[2][1]=sin(angle);
        r.m[2][2]=cos(angle);
        return r;
    }
    static Mat4 rotationY(float angle) {//y旋轉矩陣
    Mat4 r;
    for(int i=0;i<4;i++){   
        for(int j=0;j<4;j++)
        if (i==j){
         r.m[i][j]=1;}
        else {
         r.m[i][j]=0;}
        }
        r.m[0][0]=cos(angle);
        r.m[0][2]=-sin(angle);
        r.m[2][0]=sin(angle);
        r.m[2][2]=cos(angle);
        return r;
    }
};

struct Color{//顏色
    uint8_t a ,r, g, b;
};

struct Framebutte{//畫布,斜線,解析度,三角形,光柵化
    int width = 1920;//解析度寬度
    int height = 1080;//解析度高度
    std::vector< Color > pixels;
    Framebutte() : pixels(width * height){};
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
    void drawtriangle(Vec3 v0, Vec3 v1, Vec3 v2, Color c){
        int x0 = (int)( (v0.x / v0.z) * (width / 2) + (width / 2) );//Vec3轉換螢幕座標
        int y0 = (int)( (v0.y / v0.z) * (height/ 2) + (height/ 2) );

        int x1 = (int)( (v1.x / v1.z) * (width / 2) + (width / 2) );
        int y1 = (int)( (v1.y / v1.z) * (height/ 2) + (height/ 2) );

        int x2 = (int)( (v2.x / v2.z) * (width / 2) + (width / 2) );
        int y2 = (int)( (v2.y / v2.z) * (height/ 2) + (height/ 2) );

        drawLine(x0, y0, x1, y1, c);
        drawLine(x0, y0, x2, y2, c);
        drawLine(x1, y1, x2, y2, c);
    } 
    void filltriangle(Vec3 v0, Vec3 v1, Vec3 v2, Color c){
        int x0 = (int)( (v0.x / v0.z) * (width / 2) + (width / 2) );//Vec3轉換螢幕座標
        int y0 = (int)( (v0.y / v0.z) * (height/ 2) + (height/ 2) );

        int x1 = (int)( (v1.x / v1.z) * (width / 2) + (width / 2) );
        int y1 = (int)( (v1.y / v1.z) * (height/ 2) + (height/ 2) );

        int x2 = (int)( (v2.x / v2.z) * (width / 2) + (width / 2) );
        int y2 = (int)( (v2.y / v2.z) * (height/ 2) + (height/ 2) );

        int maxX = std::max(x0, std::max (x1, x2) );//畫出四邊形
        int minX = std::min(x0, std::min (x1, x2) );
        int maxY = std::max(y0, std::max (y1, y2) );
        int minY = std::min(y0, std::min (y1, y2) );

        for(int x = minX; x <= maxX; x++){//
            for(int y = minY; y <= maxY; y++){
                int cross0 = (x1 - x0) * (y - y0) - (y1 - y0) * (x - x0);
                int cross1 = (x2 - x1) * (y - y1) - (y2 - y1) * (x - x1);
                int cross2 = (x0 - x2) * (y - y2) - (y0 - y2) * (x - x2);
                if(cross0 >= 0 && cross1 >= 0 && cross2 >= 0 || cross0 <= 0 && cross1 <= 0 && cross2 <= 0){
                    setpixels(x, y, c);
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

struct Model{
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
            if(line[0]=='f'){
                
            }
            }
            return true;
        }
   
   
    
};

int main(){
    Framebutte fb;
    fb.clear({255, 255, 0, 0});
    
    Model myModel;       // 這是你的模型貨櫃
    triangle t;          // 這是你剛做好的手工三角形
    t.v0.x = 0; t.v0.y = 0; t.v0.z = 1.0;
    t.v1.x = 1; t.v1.y = 0; t.v1.z = 1.0;
    t.v2.x = 0; t.v2.y = 1; t.v2.z = 1.0;
    

    myModel.triangles.push_back(t);

    // 💡 呼叫畫三角形的函式，把畫布 fb 和模型裡的三角形丟進去
    // fb.drawtriangle(
    //     myModel.triangles[0].v0,
    //     myModel.triangles[0].v1,
    //     myModel.triangles[0].v2,
    //     {255, 255, 255, 255}
    // );

    fb.drawtriangle(
    myModel.triangles[0].v0, 
    myModel.triangles[0].v1, 
    myModel.triangles[0].v2, 
    Color{255, 255, 255, 255}
    );
    myModel.triangles.push_back(t);
    
    fb.savePPM("output.ppm");
    return 0;

}