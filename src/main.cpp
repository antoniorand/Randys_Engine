

#include "core.hpp"


constexpr double averageTick = 1/60.0;
int main(){

	RandysEngine::Rendering_Engine renderer;

    renderer.addLayerBack<RandysEngine::layer_skybox>();

    auto skybox = renderer.getLayer<RandysEngine::layer_skybox>(0);
    skybox->changeTexture(
        "resources/right.jpg",
        "resources/left.jpg",
        "resources/top.jpg",
        "resources/bottom.jpg",
        "resources/front.jpg",
        "resources/back.jpg"
    );

    renderer.runFrame();

    char i;

    std::cin >> i; 

    return 0;

}