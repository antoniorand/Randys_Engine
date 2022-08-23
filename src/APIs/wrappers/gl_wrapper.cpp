//If the __3DS__ identifier macro is not defined, it will compile this
#ifndef __3DS__

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

#include "gl_wrapper.hpp"

namespace RandysEngine{

    gl_main::gl_main() noexcept{
        // glad: load all OpenGL function pointers
        // ---------------------------------------
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
            std::cout << "Failed to initialize GLAD" << std::endl;
        }
        glEnable(GL_DEPTH_TEST);
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
        window = glfwCreateWindow(width*4, height*4, "Randys Engine Example", NULL, NULL);
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


    gl_shader::~gl_shader() noexcept{
        
        glDeleteProgram(shaderProgram);
    }

    void gl_shader::useShader() const noexcept{
        glUseProgram(shaderProgram);
    }

    gl_mesh_resource::gl_mesh_resource(std::string file) noexcept{
        
        tinyobj::ObjReaderConfig reader_config;
        tinyobj::ObjReader reader;
        reader_config.mtl_search_path = "./resources/";

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

        std::vector<Vertex> vertices;

        for(size_t s = 0; s < 1;s++){
            // Loop over faces(polygon)
            size_t index_offset = 0;
            size_t numberFaces = 0;
            for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
                numberFaces+= size_t(shapes[s].mesh.num_face_vertices[f]);
            }
            vertices.reserve(numberFaces);
            for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
                size_t fv = size_t(shapes[s].mesh.num_face_vertices[f]);
                
                // Loop over vertices in the face.
                for (size_t v = 0; v < fv; v++) {

                    RandysEngine::Vertex newVertex;
                    
                    // access to vertex
                    tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
                    newVertex.x = attrib.vertices[3*size_t(idx.vertex_index)+0];
                    newVertex.y = attrib.vertices[3*size_t(idx.vertex_index)+1];
                    newVertex.z = attrib.vertices[3*size_t(idx.vertex_index)+2];

                    // Check if `normal_index` is zero or positive. negative = no normal data
                    if (idx.normal_index >= 0) {
                        newVertex.i = attrib.normals[3*size_t(idx.normal_index)+0];
                        newVertex.j = attrib.normals[3*size_t(idx.normal_index)+1];
                        newVertex.w = attrib.normals[3*size_t(idx.normal_index)+2];
                    }

                    // Check if `texcoord_index` is zero or positive. negative = no texcoord data
                    if (idx.texcoord_index >= 0) {
                        newVertex.coordX = attrib.texcoords[2*size_t(idx.texcoord_index)+0];
                        newVertex.coordY = attrib.texcoords[2*size_t(idx.texcoord_index)+1];
                    }
                    vertices.push_back(newVertex);
                    index_offset += fv;
                }
            }
        }

        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(RandysEngine::Vertex)*vertices.size(),
            &vertices[0], GL_STATIC_DRAW);

        //vertex position
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
        //normals
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 
            (void*)(3*sizeof(GL_FLOAT)));
        //texcoords
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), 
            (void*)(5*sizeof(GL_FLOAT)));

        // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
        glBindBuffer(GL_ARRAY_BUFFER, 0); 

        // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
        // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
        glBindVertexArray(0);
    }

    gl_mesh_resource::~gl_mesh_resource() noexcept{
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
    }


    void gl_mesh_resource::draw() const noexcept{
        unsigned int numberVertices = 6;
        glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
        glDrawArrays(GL_TRIANGLES,0, numberVertices);
        //glDrawElements(GL_TRIANGLES, numberVertices, GL_UNSIGNED_INT,0);
        glBindVertexArray(0); // no need to unbind it every time 
    }

}
#endif