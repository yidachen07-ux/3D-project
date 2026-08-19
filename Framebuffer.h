#pragma once
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include <limits>
#include "Math.h"
#include "Geometry.h"
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
        for (int y = 0; y < height; y++){
            for(int x = 0; x < width; x++)
            setpixels(x, y, c);
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
        int y0 = (int)((1 - p0.y) * height / 2);
        int x1 = (int)((p1.x + 1) * width / 2);
        int y1 = (int)((1 - p1.y) * height / 2);
        int x2 = (int)((p2.x + 1) * width / 2);
        int y2 = (int)((1 - p2.y) * height / 2);

        int z0 = v0.z;
        int z1 = v1.z;
        int z2 = v2.z;

        drawLine(x0, y0, x1, y1, c);
        drawLine(x0, y0, x2, y2, c);
        drawLine(x1, y1, x2, y2, c);
    } 
    void filltriangle(Vec3 v0, Vec3 v1, Vec3 v2, Mat4 mvp,Vec3 lightDir, Color c){
        Vec4 transform;
        Vec4 c0 = mvp.transform(v0);
        Vec4 c1 = mvp.transform(v1);
        Vec4 c2 = mvp.transform(v2);

        if(c0.w < 0.5f || c1.w < 0.5f || c2.w < 0.5f){
            return;
        }
        
        Vec3 p0 = {c0.x / c0.w, c0.y / c0.w, c0.z / c0.w};
        Vec3 p1 = {c1.x / c1.w, c1.y / c1.w, c1.z / c1.w};
        Vec3 p2 = {c2.x / c2.w, c2.y / c2.w, c2.z / c2.w};
        
        int x0 = (int)((p0.x + 1) * width / 2);//Vec3轉換螢幕座標
        int y0 = (int)((1 - p0.y) * height / 2);
        int x1 = (int)((p1.x + 1) * width / 2);
        int y1 = (int)((1 - p1.y) * height / 2);
        int x2 = (int)((p2.x + 1) * width / 2);
        int y2 = (int)((1 - p2.y) * height / 2);

        float z0 = p0.z;
        float z1 = p1.z;
        float z2 = p2.z;

        int maxX = std::max(x0, std::max (x1, x2) );//畫出四邊形
        int minX = std::min(x0, std::min (x1, x2) );
        int maxY = std::max(y0, std::max (y1, y2) );
        int minY = std::min(y0, std::min (y1, y2) );

        minX = std::max(0,minX);//螢幕,三角形邊界檢查
        maxX = std::min(width-1,maxX);
        minY = std::max(0,minY);
        maxY = std::min(height-1,maxY);

        Vec3 normal = triangle{v0, v1, v2}.getNormal();
        float intensity = std::max(0.0f, lightDir.dot(normal));

        Color litColor = {
            c.a, (uint8_t)(c.r*intensity), (uint8_t)(c.g*intensity), (uint8_t)(c.b*intensity)
        };

        
        

        for(int y = minY; y <= maxY; y++){//用外積決定在三角形線左邊還右邊
            for(int x = minX; x <= maxX; x++){
                int cross0 = (x1 - x0) * (y - y0) - (y1 - y0) * (x - x0);
                int cross1 = (x2 - x1) * (y - y1) - (y2 - y1) * (x - x1);
                int cross2 = (x0 - x2) * (y - y2) - (y0 - y2) * (x - x2);
                float z = (cross0 * z2 + cross1 * z0 + cross2 * z1) / (cross0 + cross1 + cross2);//zbuffer
                if(cross0 >= 0 && cross1 >= 0 && cross2 >= 0 || cross0 <= 0 && cross1 <= 0 && cross2 <= 0){
                    if(z < zBuffer[width * y + x]){//zbuffer比較
                    zBuffer[width * y + x] = z;
                    setpixels(x, y, litColor);
                    }  
                }
            }
        }
    }
};