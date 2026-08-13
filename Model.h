#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cmath>
#include "Math.h"
#include "Geometry.h"
#include "Math.h"
#include "Geometry.h"
struct Model{//obj導入
    std::vector <triangle> triangles;//宣告一個heap動態記憶體區域裝triangle
    bool loadobj(std::string filename){//文件讀取
        std::ifstream file(filename);
        if(!file.is_open()){
            std::cout << "error";
            return false;
        }
        std::string line;
        std::vector <Vec3> vertices;//宣告一個heap動態記憶體區域裝Vec3
        while(std::getline(file,line)){
            if (line[0]=='v'){
                std::istringstream iss(line);//讀取,例:v 1 2 3(istringstream字串流)
                std::string prefix;//字串
                float x, y, z;
                iss >> prefix >> x >> y >> z;
                vertices.push_back(Vec3(x, y, z));
            }
            if(line[0] == 'f'){
                std::istringstream iss(line);//讀取,例:f 1 2 3
                std::string prefix;
                triangle t;
                std::string s0, s1, s2;
                iss >> prefix >> s0 >> s1 >> s2;
                int p0 = std::stoi(s0);  // 自動取 / 前面的數字
                int p1 = std::stoi(s1);
                int p2 = std::stoi(s2);
                //std::cout << "f: " << p0 << " " << p1 << " " << p2 << std::endl;//測試
                //std::cout << "vertices size: " << vertices.size() << std::endl;//測試
                t.v0 = vertices[p0 - 1];//obj檔頂點從1開始
                t.v1 = vertices[p1 - 1];
                t.v2 = vertices[p2 - 1];
                triangles.push_back(t); 
            }
        }
            return true;
    }
};