#include<iostream>//editor.quickSuggestions
#include<vector>
#include<cmath>

struct Vec3
{
    float x, y, z;
    Vec3(float x, float y, float z) : x(x), y(y), z(z) {}
    Vec3  operator+(const Vec3& other) const { return Vec3(x+other.x, y+other.y, z+other.z); }
    Vec3  operator-(const Vec3& other) const { return Vec3(x-other.x, y-other.y, z-other.z); }
    Vec3  operator*(const Vec3& other) const { return Vec3(x*other.x, y*other.y, z*other.z); }
    Vec3  operator/(const Vec3& other) const { return Vec3(x/other.x, y/other.y, z/other.z); }
    
} 
int main(){
    Vec3 a={3,4,5};
    Vec3 b={1,2,3};
    Vec3 c = a + b;
    std::cout << c.x << " " << c.y << " " << c.z << std::endl;
    return 0;
}