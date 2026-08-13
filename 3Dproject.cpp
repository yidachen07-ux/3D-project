#include<iostream>
#include<fstream>
#include<sstream>
#include <fstream>
#include <cstdint>
#include <algorithm>
#include <vector>
#include <cmath>
#include <SDL2/SDL.h>
#include "Math.h"
#include "Geometry.h"
#include "Framebuffer.h"
#include "Model.h"

int main(){
    Framebuffer fb;

    fb.clear({255, 255, 255, 255});
    Model myModel;
    Mat4 M = Mat4::identity();
    Mat4 V = Mat4::lookAt({2000, 000, 000}, {0, 0, 0}, {0, 1, 0});
    Mat4 P = Mat4::perspective(3.14159f / 3, 1920.0f / 1080.0f, 0.1f, 100.0f);
    Mat4 mvp = P * V * M;
    
    myModel.loadobj("cat.obj");

    Vec3 lightDir(0.0f, 0.0f, 1.0f);//光照
    
    if(SDL_Init(SDL_INIT_VIDEO) < 0){
        std::cout<<"初始化失敗";
        return -1;
    }
    SDL_Window* window = SDL_CreateWindow(
    "3D Engine", 
    SDL_WINDOWPOS_CENTERED, 
    SDL_WINDOWPOS_CENTERED, 
    fb.width,
    fb.height,
    SDL_WINDOW_SHOWN
    );
    if(window == nullptr){
        std::cout<<"視窗創造失敗";
        return -1;
    }
    SDL_Renderer* renderer = SDL_CreateRenderer(
        window,
        -1,
        SDL_RENDERER_ACCELERATED
    );
    if(renderer == nullptr){
        std::cout<<"渲染器創造失敗";
        return -1;
    }
    SDL_Texture* texture = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_ARGB8888,
        SDL_TEXTUREACCESS_STREAMING,
        fb.width,
        fb.height
    );
    if(texture == nullptr){
        std::cout<<"紋理創造失敗";
        return -1;
    }
    
    bool isRunning = true;
    SDL_Event event;
    float angle = 0.0f;
    while(isRunning){
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
            isRunning = false;
            }
        }   
        fb.clear({255,255,255,255});
        std::fill(fb.zBuffer.begin(), fb.zBuffer.end(), std::numeric_limits<float>::max());
        Mat4 M = Mat4::rotationY(angle += 0.02f);
        Mat4 mvp = P * V * M;
        for(int i = 0; i < myModel.triangles.size(); i++){
            fb.filltriangle(myModel.triangles[i].v0, myModel.triangles[i].v1, myModel.triangles[i].v2, mvp, lightDir, {255,100,100,100});
        }
        SDL_UpdateTexture(texture, NULL, fb.pixels.data(), fb.width* sizeof(Color));
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, texture, NULL, NULL);
        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }



    


    
    // //for(int i = 0; i < myModel.triangles.size(); i++){
    //     fb.filltriangle(myModel.triangles[i].v0, myModel.triangles[i].v1, myModel.triangles[i].v2, mvp, lightDir, {255,100,100,100});
    // }
    // // fb.drawtriangle(myModel.triangles[0].v0, myModel.triangles[0].v1, myModel.triangles[0].v2, mvp, {255, 0, 0, 0});
    // // fb.filltriangle(myModel.triangles[0].v0, myModel.triangles[0].v1, myModel.triangles[0].v2, mvp, {255, 0, 0, 0});
    // std::cout << "loaded " << myModel.triangles.size() << " triangles" << std::endl;
    
    // fb.savePPM("output.ppm");
    return 0;

}
//     t.v0 = {0.0, 0.5, 0.0};
//     t.v1 = {0.5, -0.5, 0.0};
//     t.v2 = {-0.5, -0.5, 0.0};
//     std::vector <Vec3> cubeVerts = {
//     {-0.5, -0.5, -0.5},//後左下0
//     { 0.5, -0.5, -0.5},//後右下1
//     { 0.5,  0.5, -0.5},//後右上2
//     {-0.5,  0.5, -0.5},//後左上3
//     {-0.5, -0.5,  0.5},//前左下4
//     { 0.5, -0.5,  0.5},//前右下5
//     { 0.5,  0.5,  0.5},//前右上6
//     {-0.5,  0.5,  0.5},//前左下7
//     };
//     int indices[] = {
//     // 後面 (面向 Z = -0.5)
//     1, 0, 3,   1, 3, 2,
//     // 前面 (面向 Z = 0.5)
//     4, 5, 6,   4, 6, 7,
//     // 右面 (面向 X = 0.5)
//     5, 1, 2,   5, 2, 6,
//     // 左面 (面向 X = -0.5)
//     0, 4, 7,   0, 7, 3,
//     // 上面 (面向 Y = 0.5)
//     7, 6, 2,   7, 2, 3,
//     // 下面 (面向 Y = -0.5)
//     4, 5, 1,   4, 1, 0
// };
    
//    int indicesCount = sizeof(indices) / sizeof(indices[0]);
//     for(int i = 0; i < myModel.triangles.size(); i += 3){
//         triangle t;
//         t.v0 = cubeVerts[indices[i]];
//         t.v1 = cubeVerts[indices[1+i]];
//         t.v2 = cubeVerts[indices[2+i]];
//         myModel.triangles.push_back(t);
//     }
