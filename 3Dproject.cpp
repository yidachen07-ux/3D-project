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
    Model myModel;

    myModel.loadobj("cat.obj");
    
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

    float pitch = 0.0f;//滑鼠上下視角
    float yaw = -3.14159f / 2.0f;;//滑鼠左右視角
    float sensitivity = 0.03f;//滑鼠靈敏度

    float ctrlCamX = 1000.0f;//移動相機位置
    float ctrlCamY = 0.0f;
    float ctrlCamZ = 0.0f;
    Vec3 camXYZ{ctrlCamX, ctrlCamY, ctrlCamZ};
    Vec3 lightDir(0.0f, 0.0f, 1.0f);//光照

    bool isRunning = true;
    SDL_Event event;
    float angle = 0.0f;
    while(isRunning){
        while (SDL_PollEvent(&event)) {//視窗關閉
            if (event.type == SDL_QUIT) {
            isRunning = false;
            }
        }

        int msX,msY;
        uint32_t mouseMove = SDL_GetRelativeMouseState(&msX, &msY);
        
        if(mouseMove & SDL_BUTTON(SDL_BUTTON_LEFT)){
            yaw += msX * sensitivity;
            pitch -= msY * sensitivity;
            if (pitch > 1.55f) pitch = 1.55f;
            if (pitch < -1.55f) pitch = -1.55f;
        }
        float viewX = cos(pitch) * sin(yaw);
        float viewY = sin(pitch);
        float viewZ = cos(pitch) * cos(yaw);
        float speed = 50.0f;

        
        Vec3 forward(viewX, viewY, viewZ);
        Vec3 up(0, 1, 0);
        Vec3 right = (forward.cross(up)).normalize();
        

        const Uint8* state = SDL_GetKeyboardState(NULL);
        if(state [SDL_SCANCODE_W]){//kbw
            camXYZ += forward * speed;
        }
        if(state [SDL_SCANCODE_S]){//kbs
            camXYZ -= forward * speed;
        }
        if(state [SDL_SCANCODE_SPACE]){//kbspeace
            camXYZ += up * speed;
        }
        if(state [SDL_SCANCODE_LSHIFT]){//kblshift
            camXYZ -= up * speed;
        }
        if(state [SDL_SCANCODE_D]){//kbd
            camXYZ += right * speed;
        }
        if(state [SDL_SCANCODE_A]){//kba
            camXYZ -= right * speed;
        }

        //std::cout << "Cam: " << ctrlCamX << " , " << ctrlCamY << " , " << ctrlCamZ << std::endl;

        fb.clear({255,255,255,255});
        
        Mat4 M = Mat4::rotationY(angle += 0.02f);
        Mat4 V = Mat4::lookAt(camXYZ, camXYZ + Vec3{viewX, viewY, viewZ}, {0, 1, 0});
        Mat4 P = Mat4::perspective(3.14159f / 3, 1920.0f / 1080.0f, 0.1f, 10000.0f);
        std::fill(fb.zBuffer.begin(), fb.zBuffer.end(), std::numeric_limits<float>::max());
        Mat4 mvp = P * V * M;

        for(int i = 0; i < myModel.triangles.size(); i++){//畫出模型
            fb.filltriangle(myModel.triangles[i].v0, myModel.triangles[i].v1, myModel.triangles[i].v2, mvp, lightDir, {255,100,100,100});
        }

        SDL_UpdateTexture(texture, NULL, fb.pixels.data(), fb.width* sizeof(Color));
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, texture, NULL, NULL);
        SDL_RenderPresent(renderer);
        SDL_Delay(16);  
    } 
    return 0;
}
