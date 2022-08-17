#pragma once

namespace RandysEngine{


    template<typename api>
    struct matrix_wrapper{

    };

    template<typename api>
    struct mesh_resource_wrapper{

        virtual ~mesh_resource_wrapper(){};

        void draw() const noexcept {
            static_cast<api*>(this)->draw();
        }
    };

    template<typename api>
    struct shader_wrapper{

        virtual ~shader_wrapper(){};

        void useShader() const noexcept{
            static_cast<api*>(this)->useShader();
        }
    };

    template<typename api>
    struct initializer_wrapper {
        
        void prepareDraw() const noexcept{
            static_cast<api*>(this)->prepareDraw();
        }

    };

    
    enum class KeyInput : unsigned int{
        exit = 0
    };

    template<typename api>
    struct screen_wrapper{

        int width {400},height{240};

        virtual ~screen_wrapper(){};

        void changeWindowSize(int e_width, int e_height){
            static_cast<api*>(this)->changeWindowSize(e_width,e_height);
        }

        void swapBuffers(){
            static_cast<api*>(this)->swapBuffers();
        }

        bool getInputPressed(KeyInput input) const{
            return static_cast<api*>(this)->getInput();
        }

        bool isAppRunning() const{
            return static_cast<api*>(this)->isAppRunning();
        }

        void closeApp(){
            return static_cast<api*>(this)->closeApp();
        }

    };

}
