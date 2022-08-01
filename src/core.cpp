#include "core.hpp"

namespace RandysEngine{

    struct draw_visitor{
        template< typename T >
        void operator() ( T& value ){
            value.draw();
        }   
    };
    struct interact_visitor{
        template< typename T >
        void operator() ( T& value ){
            value.interact();
        }   
    };

    void Rendering_Engine::runFrame(){

        /*
        
            In summary, this function will draw first and then
            it will check if it interacts with the inputs

        */
        {
            auto iterator = layers.begin();
            auto itEnd = layers.end();

            while(iterator != itEnd){
                std::visit( draw_visitor{}, *iterator);
                iterator++;
            }
        }
        
        {
            auto iteratorR = layers.rbegin();
            auto itEndR = layers.rend();

            while(iteratorR != itEndR){
                std::visit( interact_visitor{}, *iteratorR);
                iteratorR++;
            }
        }

    }

#ifdef __3DS__
    //3ds API wrapper
#else
    Rendering_Engine::OPENGL_API_INITIALIZER::OPENGL_API_INITIALIZER(){
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        window = glfwCreateWindow(width,height,"RandysEngine Example Text",nullptr,nullptr);

        if (window == NULL){
            std::cout << "Failed to create GLFW window" << std::endl;
            glfwTerminate();
            //throw err or something
        }
    
        glfwMakeContextCurrent(window);
        //glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
        //glfwSetCursorPosCallback(window, mouse_callback);
        //glfwSetScrollCallback(window, scroll_callback);

        // tell GLFW to capture our mouse
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

        // glad: load all OpenGL function pointers
        // ---------------------------------------
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
            std::cout << "Failed to initialize GLAD" << std::endl;
        }

        glEnable(GL_DEPTH_TEST);



    }
#endif

}