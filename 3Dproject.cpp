#include<iostream>//editor.quickSuggestions
#include <fstream>
#include<vector>
#include<cmath>

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
    for(int i=0
        ;i<4;i++){   //對角矩陣
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

struct Framebutter{
    Color pixels [800][600];
    int width = 800;
    int height = 600;
    void setpixels (int x, int y, Color c){
        pixels [x][y] = c;
    }
    void clear (Color c){
        for (int i=0;i<800;i++){
            for(int j=0;j<600;j++)
            setpixels(i, j, c);
        }
    }
    void savePPM(const std::string& filename){
        std::ofstream outfile(filename);
        outfile << "P3\n";
        outfile << width << " " << height << "\n";
        outfile << "255\n";
        for (int i=599;i>=0;i--){
            for (int j=0;j<800;j++){
            Color c = pixels [j][i];
            outfile << (int) c.r << " " << (int) c.g << " " << (int) c.b <<"\n";
            }
            outfile << "\n";
        }
        outfile.close();
    }
    
};

int main(){
    Vec3 a={3, 4, 0};//向量加減
    Vec3 b={1, 0, 0};

    Vec3 c = a + b;
    std::cout << c.x << " " << c.y << " " << c.z << std::endl;

    float len = a.length();//向量長度
    std::cout << len << std::endl;

    Vec3 n = a.normalize();//標準化1
    std::cout << n.x << " " << n.y << " " << n.z << std::endl;
    Vec3 up = {0, 1, 0};
    Vec3 light = {0, 1, 0};
    std::cout << up.dot(light) << std::endl;

    Vec3 side = {1, 0, 0};//內積
    std::cout << up.dot(side) << std::endl;

    Vec3 x_axis = {1, 0, 0};//外積
    Vec3 y_axis = {0, 1, 0};
    Vec3 z = x_axis.cross(y_axis);
    std::cout << z.x << " " << z.y << " " << z.z << std::endl;

    Mat4 m = Mat4::identity();
    std::cout << m.m[0][0] << " " << m.m[1][1] << std::endl;

    Mat4 ma = Mat4::identity();
    Mat4 mb = Mat4::identity();
    Mat4 mc = ma * mb;
    std::cout << mc.m[0][0] << " " << mc.m[1][1] << std::endl;

    Mat4 t = Mat4::translation(3, 0, 0);
    std::cout << t.m[0][3] << std::endl;

    Mat4 s = Mat4::scale(2, 2, 2);
    std::cout << s.m[0][0] << " " << s.m[1][1] << std::endl;

    Mat4 rz = Mat4::rotationZ(3.14159f / 2);
    std::cout << rz.m[0][0]<< " " << rz.m[1][0]<<std::endl;

    Framebutter fb;
    fb.clear({255, 255, 0, 0});
    fb.savePPM("output.ppm");
    for(int x = 390; x < 410; x++)
        for(int y = 290; y < 310; y++)
         fb.setpixels(x, y, {255, 255, 255, 255});
    return 0;

}