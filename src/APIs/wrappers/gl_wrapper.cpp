//If the __3DS__ identifier macro is not defined, it will compile this
#ifndef __3DS__

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>


//Source: https://github.com/tinyobjloader/tinyobjloader
#define TINYOBJLOADER_IMPLEMENTATION // define this in only *one* .cc
#include "../../dependencies/tiny_obj_loader.h"

#include "gl_wrapper.hpp"

namespace RandysEngine{

    gl_main::gl_main() noexcept{
        // glad: load all OpenGL function pointers
        // ---------------------------------------
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
            std::cout << "Failed to initialize GLAD" << std::endl;
        }
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_TEXTURE_2D);
    }

    void window_close_callback(GLFWwindow* window){
        glfwSetWindowShouldClose(window, GL_TRUE);
    }

    gl_screen::gl_screen() noexcept{
        // glfw: initialize and configure
        // ------------------------------
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        // glfw window creation
        // --------------------
        window = glfwCreateWindow(width*2, height*2, "Randys Engine Example", NULL, NULL);
        if (window == NULL){
            std::cout << "Failed to create GLFW window" << std::endl;
            glfwTerminate();
        }
        glfwMakeContextCurrent(window);

        glfwSetWindowCloseCallback(window, window_close_callback);
    }
    
    gl_screen::~gl_screen() noexcept{
                    
        // glfw: terminate, clearing all previously allocated GLFW resources.
        // ------------------------------------------------------------------
        glfwTerminate();
    }

    void gl_screen::changeWindowSize(int e_width, int e_height) noexcept{
        width = e_width;
        height = e_height;
        glfwSetWindowSize(window,width,height);
    }

    void gl_screen::swapBuffers() const noexcept{
        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    bool gl_screen::getInputPressed(KeyInput input) const noexcept{
        bool devolver = false;
        switch (input){
            case KeyInput::exit :
                if(glfwGetKey(window,GLFW_KEY_ESCAPE) == GLFW_PRESS)
                    devolver = true;
            break;
            case KeyInput::left :
                if(glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
                    devolver = true;
            break;
            case KeyInput::right :
                if(glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
                    devolver = true;
            break;
            case KeyInput::up :
                if(glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
                    devolver = true;
            break;
            case KeyInput::down :
                if(glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
                    devolver = true;
            break;
            default: 
            break;
        }
        return devolver;
    }

    bool gl_screen::isAppRunning() const noexcept{
        return !glfwWindowShouldClose(window);
    }

    void gl_screen::closeApp() noexcept{
        glfwSetWindowShouldClose(window,GL_TRUE);
    }

    void gl_screen::prepareDraw() const noexcept{
        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);       
    }

    gl_shader::gl_shader() noexcept{
        std::string vertexCode;
        std::string fragmentCode;
        std::ifstream vShaderFile;
        std::ifstream fShaderFile;
        // ensure ifstream objects can throw exceptions:
        vShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
        fShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);

        try{
            // open files
            vShaderFile.open("shaders/shader.vs");
            fShaderFile.open("shaders/shader.fs");
            std::stringstream vShaderStream, fShaderStream;
            // read file's buffer contents into streams
            vShaderStream << vShaderFile.rdbuf();
            fShaderStream << fShaderFile.rdbuf();		
            // close file handlers
            vShaderFile.close();
            fShaderFile.close();
            // convert stream into string
            vertexCode   = vShaderStream.str();
            fragmentCode = fShaderStream.str();
        }catch(std::ifstream::failure& e){
            std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
        }

        // vertex shader
        const char *vShaderCode = vertexCode.c_str();
        const char *fShaderCode = fragmentCode.c_str();
        unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1, &vShaderCode, NULL);
        glCompileShader(vertexShader);
        // check for shader compile errors
        int success;
        char infoLog[512];
        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
        if (!success){
            glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
        }

        /////////////

        // fragment shader
        unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &fShaderCode, NULL);
        glCompileShader(fragmentShader);
        // check for shader compile errors
        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
        if (!success){
            glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
        }

        // link shaders
        shaderProgram = glCreateProgram();
        glAttachShader(shaderProgram, vertexShader);
        glAttachShader(shaderProgram, fragmentShader);
        glLinkProgram(shaderProgram);
        // check for linking errors
        glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
        }
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

        
            
    }

    void gl_shader::setBool(const std::string &name, bool value) const{
        glUniform1i(glGetUniformLocation(shaderProgram, name.c_str()), (int)value); 
    }
    void gl_shader::setInt(const std::string &name, int value) const{
        glUniform1i(glGetUniformLocation(shaderProgram, name.c_str()), value); 
    }
    void gl_shader::setFloat(const std::string &name, float value) const{
        glUniform1f(glGetUniformLocation(shaderProgram, name.c_str()), value); 
    }

    void gl_shader::setMat4(const std::string &name, gl_matrix& mat) const{
        unsigned int location = glGetUniformLocation(shaderProgram, name.c_str());
        auto value = glm::value_ptr(mat.getTransformationMatrix());
        glUniformMatrix4fv(location, 1, GL_FALSE, value);
    }


    gl_shader::~gl_shader() noexcept{
        
        glDeleteProgram(shaderProgram);
    }

    void gl_shader::useShader() noexcept{
        glUseProgram(shaderProgram);
    }

    gl_texture_resource::gl_texture_resource(std::string file) noexcept{

        int width, height, channels;
        stbi_set_flip_vertically_on_load(true);
        unsigned char *data = stbi_load(file.c_str(),
            &width,&height,&channels,0);
        if(data != 0){
            glGenTextures(1,&texture);
            glBindTexture(GL_TEXTURE_2D, texture);
            
            glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
                GL_LINEAR);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 
                width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
            stbi_image_free(data);
            glBindTexture(GL_TEXTURE_2D,0);
        }
    }

    void gl_texture_resource::use() const noexcept{

        //TO DO
        glBindTexture(GL_TEXTURE_2D, texture); 

    }

    void gl_texture_resource::unlink() const noexcept{

        //TO DO
        glBindTexture(GL_TEXTURE_2D, 0); 

    }

    std::pair<std::vector<Vertex>,std::vector<unsigned short>>
        gl_mesh_resource::loadModel(std::string file) noexcept{

        std::pair<std::vector<Vertex>,std::vector<unsigned short>> devolver;

        tinyobj::ObjReaderConfig reader_config;
        reader_config.mtl_search_path = "./"; // Path to material files

        tinyobj::ObjReader reader;

        if (!reader.ParseFromFile(file, reader_config)) {
            if (!reader.Error().empty()) {
                std::cerr << "TinyObjReader: " << reader.Error();
            }
        }

        if (!reader.Warning().empty()) {
            std::cout << "TinyObjReader: " << reader.Warning();
        }

        auto& attrib = reader.GetAttrib();
        auto& shapes = reader.GetShapes();

        devolver.first.reserve(attrib.vertices.size());
        devolver.second.reserve(attrib.vertices.size());

        for(unsigned int i = 0; i < attrib.vertices.size();i++){
            
        }

        // Loop over shapes
        for (size_t s = 0; s < shapes.size(); s++) {
            // Loop over faces(polygon)
            size_t index_offset = 0;
            for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
                size_t fv = size_t(shapes[s].mesh.num_face_vertices[f]);

                // Loop over vertices in the face.
                for (size_t v = 0; v < fv; v++) {
                    // access to vertex
                    tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
                    tinyobj::real_t vx = attrib.vertices[3*size_t(idx.vertex_index)+0];
                    tinyobj::real_t vy = attrib.vertices[3*size_t(idx.vertex_index)+1];
                    tinyobj::real_t vz = attrib.vertices[3*size_t(idx.vertex_index)+2];

                    // Check if `normal_index` is zero or positive. negative = no normal data
                    /*if (idx.normal_index >= 0) {
                        tinyobj::real_t nx = attrib.normals[3*size_t(idx.normal_index)+0];
                        tinyobj::real_t ny = attrib.normals[3*size_t(idx.normal_index)+1];
                        tinyobj::real_t nz = attrib.normals[3*size_t(idx.normal_index)+2];
                    }*/

                    tinyobj::real_t tx = 0;
                    tinyobj::real_t ty = 0;
                    // Check if `texcoord_index` is zero or positive. negative = no texcoord data

                    if (idx.texcoord_index >= 0) {
                        tx = attrib.texcoords[2*size_t(idx.texcoord_index)+0];
                        ty = attrib.texcoords[2*size_t(idx.texcoord_index)+1];
                    }

                    devolver.first.emplace_back(vx,vy,vz,tx,ty);

                    // Optional: vertex colors
                    // tinyobj::real_t red   = attrib.colors[3*size_t(idx.vertex_index)+0];
                    // tinyobj::real_t green = attrib.colors[3*size_t(idx.vertex_index)+1];
                    // tinyobj::real_t blue  = attrib.colors[3*size_t(idx.vertex_index)+2];
                }
                index_offset += fv;

                // per-face material
                shapes[s].mesh.material_ids[f];
            }

        }

        count_loadedVertices = devolver.first.size();
        size_loadedVertices = count_loadedVertices*sizeof(Vertex);

        return devolver;

    }

    gl_mesh_resource::gl_mesh_resource(std::string file) noexcept{
        
        auto vertexData = loadModel(file);

        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        //glGenBuffers(1, &EBO);
        // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, size_loadedVertices,&vertexData.first[0], GL_STATIC_DRAW);

        //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        //glBufferData(GL_ELEMENT_ARRAY_BUFFER, size_loadedIndices, &vertexData.second[0], GL_STATIC_DRAW);

        //vertex position
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 
            (void*)0);
        glEnableVertexAttribArray(0);
        //texture coordinates
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), 
            (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
        glBindBuffer(GL_ARRAY_BUFFER, 0); 

        // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
        // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
        glBindVertexArray(0);
    }

    gl_mesh_resource::~gl_mesh_resource() noexcept{
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1,&EBO);
    }


    void gl_mesh_resource::draw() const noexcept{
        glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
        glDrawArrays(GL_TRIANGLES, 0, count_loadedVertices);
        //glDrawElements(GL_TRIANGLES, count_loadedIndices, GL_UNSIGNED_SHORT, 0);

        glBindVertexArray(0); // no need to unbind it every time 
    }

    const glm::mat4& gl_matrix::getTransformationMatrix() noexcept{
        if(changed){
            glm::vec3 
                trans{glm::vec3(translation[0],translation[1],translation[2])},
                rot{glm::vec3(rotation[0],rotation[1],rotation[2])},
                scale{glm::vec3(scalation[0],scalation[1],scalation[2])};

            if(perspective){
                transform = glm::perspective(glm::radians(fov), aspect, near,far);
            }
            else{
                transform = glm::mat4(1.0f);
                transform =  transform * glm::translate(transform,trans)  
                    * (
                        glm::rotate(transform,rot.z,{0.0f,0.0f,1.0f})*
                        glm::rotate(transform,rot.y,{0.0f,1.0f,0.0f})*
                        glm::rotate(transform,rot.x,{1.0f,0.0f,0.0f}))
                    * glm::scale(transform,scale);
            }

            changed = false;
        }
        return transform;
    }

    void gl_matrix::multiply(gl_matrix& other){

        this->changed = true;

        auto& thisMatrix = this->getTransformationMatrix();
        auto& otherMatrix = other.getTransformationMatrix();

        transform = otherMatrix*thisMatrix;
    }

    gl_matrix gl_matrix::reverse() noexcept{

        gl_matrix devolver;
        devolver.changed = false;
        devolver.transform = glm::inverse(this->getTransformationMatrix());

        return(devolver);
    }

}
#endif