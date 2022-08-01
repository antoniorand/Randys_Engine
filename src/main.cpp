

#include "core.hpp"


constexpr double averageTick = 1/60.0;
int main(){

	RandysEngine::Rendering_Engine renderer;

    renderer.addLayer<RandysEngine::layer_GUI>(1);
    renderer.addLayer<RandysEngine::layer_GUI>(1);
    renderer.addLayerBack<RandysEngine::layer_GUI>();
    renderer.addLayerBack<RandysEngine::layer_GUI>();


    renderer.runFrame();

    std::cout << "//////\n";

    renderer.deleteLayer<RandysEngine::layer_skybox>(0);

    renderer.runFrame();

    return 0;

}