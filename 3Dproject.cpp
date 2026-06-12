#include<iostream>//editor.quickSuggestions
#include <fstream>
#include <cstdint>
#include<vector>
#include<cmath>
struct Vec3;
struct Mat4;
struct Color;
struct Framebutte;
struct drawline;
struct drawtriangle;


struct Vec3{//operator定義符號+-*/
    float x, y, z;
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

struct Framebutte{
    Color pixels [800][600];//畫布大小
    int width = 800;
    int height = 600;
    void setpixels (int x, int y, Color c){
        pixels [x][y] = c;
    }
    void clear (Color c){//畫出畫布
        for (int i=0;i<800;i++){
            for(int j=0;j<600;j++)
            setpixels(i, j, c);
        }
    }
    void savePPM(const std::string& filename){//畫布匯出.ppm
        std::ofstream outfile(filename);
        outfile << "P3\n";
        outfile << width << " " << height << "\n";
        outfile << "255\n";
        for (int i=599;i>=0;i--){//反轉畫布
            for (int j=0;j<800;j++){
                Color c = pixels [j][i];
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
        int ystep = (y1<y0) ? -1 : 1;
        int P = 2 * std::abs(dy) - dx;
        for(int x=x0; x<=x1; x++){//Bresenham演算法斜率0~1
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
        int x0 = (int)( (v0.x / v0.z) * (width / 2) + (width / 2) );
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
         int x0 = (int)( (v0.x / v0.z) * (width / 2) + (width / 2) );
        int y0 = (int)( (v0.y / v0.z) * (height/ 2) + (height/ 2) );

        int x1 = (int)( (v1.x / v1.z) * (width / 2) + (width / 2) );
        int y1 = (int)( (v1.y / v1.z) * (height/ 2) + (height/ 2) );

        int x2 = (int)( (v2.x / v2.z) * (width / 2) + (width / 2) );
        int y2 = (int)( (v2.y / v2.z) * (height/ 2) + (height/ 2) );

        int maxX = std::max(x0, std::max(x1, x2));
        int minX = std::min(x0, std::min(x1, x2));
        int maxY = std::max(y0, std::max(y1, y2));
        int minY = std::min(y0, std::min(y1, y2));
    }
};

int main(){
    Framebutte fb;
    fb.clear({255, 255, 0, 0});
    
    
    fb.drawLine(100, 100, 400, 200, {255, 255, 255, 255});
    fb.drawLine(400, 300, 0, 600, {255, 255, 255, 255});
    fb.drawLine(400, 200, 100, 100, {255, 255, 255, 255});
    
    Vec3 p0(-0.5f, -0.5f, 2.0f); // 左下
    Vec3 p1( 0.5f, -0.5f, 2.0f); // 右下
    Vec3 p2( 0.0f,  0.5f, 2.0f); // 正上頂點

    // 呼叫你親手刻完的 3D 三角形外框函式，顏色一樣用純白
    fb.drawtriangle(p0, p1, p2, {255, 255, 255, 255});
    

    fb.savePPM("output.ppm");
    return 0;

}