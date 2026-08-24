#include <iostream>
#include <fstream>
#include <sstream>
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
#include "window.h"

int main(){
    
    Window app;
    app.myModel.loadobj("cat.obj"); // 載入模型

    while (app.processEvents()) {
        app.present(nullptr);       // 刷新螢幕
    }
    
    return 0;
}
