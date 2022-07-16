//#include <GL/glew.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stb/stb_image.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <streambuf>
#include <string>

#include "graphics/shader.h"
#include "graphics/texture.h"

#include "io/keyboard.h"
#include "io/mouse.h"
#include "io/joystick.h"
#include "io/camera.h"
#include "io/screen.h"

void processInput(double dt);

float mixVal = 0.5;

glm::mat4 transform = glm::mat4(1.0f);
glm::mat4 mouseTransform = glm::mat4(1.0f);
Joystick mainJ(0);

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float deltaTime = 0.0f;
float lastFrame = 0.0f;

Screen screen;
float x, y, z;

int main( void )
{
    int success;
    char infoLog[512];
	// Initialise GLFW
	if( !glfwInit() )
	{
		fprintf( stderr, "Failed to initialize GLFW\n" );
		getchar();
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);

    // OpenGL version 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

# ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
#endif

    if(!screen.init()){
        std::cout<<"Could not create window."<<std::endl;
        glfwTerminate();
        return -1;
    }

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        std::cout<<"Fail to initialize GLAD"<<std::endl;
        return -1;
    }

    screen.setParameters();

    /*
        Shaders
    */

    Shader shader("assets/object.vs", "assets/object.fs");
    Shader shader2("assets/object.vs", "assets/object.fs");

    // Continue figures:
    // Vertex Array
/*
    float vertices [] = {
        // positions               colors
       -0.25f, -0.5f, 0.0f,   1.0f, 1.0f, 0.5f, // {x1,y1,z1} {RGB Color}
        0.15f,  0.0f, 0.0f,   0.5f, 1.0f, 0.75f,  // {x2,y2,z2} {RGB Color}
        0.0f,   0.5f, 0.0f,   0.6f, 1.0f, 0.2f,
        0.5f,  -0.4f, 0.0f,   1.0f, 0.2f, 1.0f
    };

    unsigned int indices [] = {
        0,1,2,
        3,1,2
    };
*/

/*
    // Rectangle
    float vertices [] = {
        // positions               colors         texture coordinates
       -0.5f, -0.5f, 0.0f,   1.0f, 1.0f, 0.5f,      0.0f,0.0f,          // bottom left {x1,y1,z1} {RGB Color}
       -0.5f,  0.5f, 0.0f,   0.5f, 1.0f, 0.75f,     0.0f,1.0f,          // top left  {x2,y2,z2} {RGB Color}
        0.5f, -0.5f, 0.0f,   0.6f, 1.0f, 0.2f,      1.0f,0.0f,          // bottom right
        0.5f,  0.5f, 0.0f,   1.0f, 0.2f, 1.0f,      1.0f,1.0f           // top right
    };

    unsigned int indices [] = {
        0,1,2, // first Triangle
        3,1,2 // Second Triangle
    };


   float vertices [] = {
        // positions
        -0.5f,-0.5f,0.0f,
        -0.25f,0.5f,0.0f,
        -0.1f,-0.5f,0.0f,

        0.5f,-0.5f,0.0f,
        0.25f,0.5f,0.0f,
        0.1f,-0.5f,0.0f
    };

    unsigned int indices [] = {
        0,1,2,
        3,4,5
    };
*/


   // Cube
   float vertices[] = {
    // position             texcoords
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
};

    // VAO, VBO
    unsigned int VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1,&VBO);
    //glGenBuffers(1,&EBO);

    // bind VAO
    glBindVertexArray(VAO);

    // bind VBO
    glBindBuffer(GL_ARRAY_BUFFER,VBO);
    glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);

    // set EBO
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices),indices,GL_STATIC_DRAW);

    // set attribute pointer

    // positions
    //glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE, 8 * sizeof(float), (void * )0);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE, 5 * sizeof(float), (void * )0); // Cube
    glEnableVertexAttribArray(0);

    // color
    //glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE, 8 * sizeof(float), (void * )(3 * sizeof(float)));
    //glEnableVertexAttribArray(1);

    // texture coordinates
    //glVertexAttribPointer(2,2,GL_FLOAT,GL_FALSE, 8 * sizeof(float), (void * )(6 * sizeof(float)));
    //glEnableVertexAttribArray(2);
    glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE, 5 * sizeof(float), (void * )(3 * sizeof(float))); // Cube
    glEnableVertexAttribArray(1); // Cube

    // Textures
    Texture texture1("assets/image1.png","texture1");
    texture1.load();
    Texture texture2("assets/boric2.jpg","texture2");
    texture2.load();

    shader.activate();
    shader.setInt("texture1",texture1.id);
    shader.setInt("texture2",texture2.id);

    std::cout<<"i can fucking load everything except the while"<<std::endl;

    // Initialize the identity matrix
/*
    glm::mat4 trans = glm::mat4(1.0f);
    trans = glm::rotate(trans, glm::radians(45.0f),glm::vec3(0.0f,0.0f,1.0f));
    shader.activate();
    shader.setMat4("transform", trans);


    glm::mat4 trans2 = glm::mat4(1.0f);
    trans2 = glm::scale(trans2, glm::vec3(1.5f));
    trans2 = glm::rotate(trans2, glm::radians(15.0f),glm::vec3(0.0f,0.0f,1.0f));
    shader2.activate();
    shader2.setMat4("transform", trans2);

    // Cube
    glm::mat4 trans = glm::mat4(1.0f);
    trans = glm::rotate(trans, glm::radians(45.0f),glm::vec3(1.0f,0.0f,0.0f));
    trans = glm::rotate(trans, glm::radians(45.0f),glm::vec3(0.0f,1.0f,0.0f));
    shader.activate();
    shader.setMat4("transform", trans);
*/
    //x = 0.0f;
    //y = 0.0f;
    //z = 3.0f;

    mainJ.update();
    if(mainJ.isPresent()){
        std::cout<< mainJ.getName() << " is present "<<std::endl;
    }else{
        std::cout<<"Not present for joystick"<<std::endl;
    }

	do{
        double currentTime = glfwGetTime();
        deltaTime = currentTime - lastFrame;
        lastFrame = currentTime;

        processInput(deltaTime);

        // render
        screen.update();

        glActiveTexture(GL_TEXTURE0);
        texture1.bind();
        glActiveTexture(GL_TEXTURE1);
        texture2.bind();
        //trans = glm::rotate(trans, glm::radians((float)glfwGetTime()/100.0f),glm::vec3(0.0f,0.0f,1.0f));
        //shader.activate();
        //shader.setMat4("transform", trans);
        // draw shapes
        glBindVertexArray(VAO);

        // Create transformation for screen
        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = glm::mat4(1.0f);
        glm::mat4 projection = glm::mat4(1.0f);

        model = glm::rotate(model, (float)glfwGetTime() * glm::radians(-55.f),glm::vec3(0.5f));
        //view = glm::translate(view, glm::vec3(-x, -y, -z));
        view = camera.getViewMatrix();
        //projection = glm::perspective(glm::radians(45.f),(float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f,100.0f);
        projection = glm::perspective(glm::radians(camera.getZoom()),(float)screen.getWidth() / (float)screen.getHeight(), 0.1f,100.0f);

        shader.activate();

        shader.setMat4("model",model);
        shader.setMat4("view",view);
        shader.setMat4("projection",projection);

        //shader.setMat4("mouseTransform",mouseTransform);
        shader.setFloat("mixVal",mixVal);
        //shader.setMat4("transform",transform);
        //shader.setMat4
        //glDrawElements(GL_TRIANGLES,3,GL_UNSIGNED_INT,0);
        //glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,0);

        glDrawArrays(GL_TRIANGLES, 0, 36); // Cube

        //trans2 = glm::rotate(trans2, glm::radians((float)glfwGetTime()/-100.0f),glm::vec3(0.0f,0.0f,1.0f));
        //shader2.activate();
        //shader2.setMat4("transform", trans2);

        //shader2.activate();
        //glDrawElements(GL_TRIANGLES,3,GL_UNSIGNED_INT,(void *)(3 * sizeof(GLuint)));

        glBindVertexArray(0);
        // Swap buffers
		screen.newFrame();

	} // Check if the ESC key was pressed or the window was closed
	while(!screen.shouldClose());

    glDeleteVertexArrays(1,&VAO);
    glDeleteBuffers(1,&VBO);
    //glDeleteBuffers(1,&EBO);

    // Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}

void processInput(double dt){
    if(Keyboard::key(GLFW_KEY_ESCAPE)) screen.setShouldClose(true);

    // change mix value
    if(Keyboard::key(GLFW_KEY_UP)){
        mixVal += .05f;
        if(mixVal > 1) mixVal = 1.0f;
    }

    if(Keyboard::key(GLFW_KEY_DOWN)){
        mixVal -= .05f;
        if(mixVal < 0) mixVal = 0.0f;
    }

/*
    if(Keyboard::key(GLFW_KEY_W)){
        transform = glm::translate(transform, glm::vec3(0.0f,0.01f,0.0f));
    }
    if(Keyboard::key(GLFW_KEY_S)){
        transform = glm::translate(transform, glm::vec3(0.0f,-0.01f,0.0f));
    }
    if(Keyboard::key(GLFW_KEY_A)){
        transform = glm::translate(transform, glm::vec3(-0.01f,0.0f,0.0f));
    }
    if(Keyboard::key(GLFW_KEY_D)){
        transform = glm::translate(transform, glm::vec3(0.01f,0.0f,0.0f));
    }

    float scrollDX = Mouse::getScrollDX();
    float scrollDY = Mouse::getScrollDY();

    if(scrollDY > 0){
        transform = glm::scale(transform, glm::vec3(1 + scrollDY / 10,1 + scrollDY / 10,0.0f));
    }else if(scrollDY < 0){
        transform = glm::scale(transform, glm::vec3(1 + scrollDY / 10,1 + scrollDY / 10,0.0f));
    }

    //std::cout<<"scrollDX,scrollDY ="<<scrollDX<<","<<scrollDY<<std::endl;

    mainJ.update();
*/

    //move camera
    if(Keyboard::key(GLFW_KEY_W)){
        camera.updateCameraPos(CameraDirection::FORWARD, dt);
    }
    if(Keyboard::key(GLFW_KEY_S)){
        camera.updateCameraPos(CameraDirection::BACKWARD, dt);
    }
    if(Keyboard::key(GLFW_KEY_D)){
        camera.updateCameraPos(CameraDirection::RIGHT, dt);
    }
    if(Keyboard::key(GLFW_KEY_A)){
        camera.updateCameraPos(CameraDirection::LEFT, dt);
    }
    if(Keyboard::key(GLFW_KEY_SPACE)){
        camera.updateCameraPos(CameraDirection::UP, dt);
    }
    if(Keyboard::key(GLFW_KEY_LEFT_SHIFT)){
        camera.updateCameraPos(CameraDirection::DOWN, dt);
    }

    double dx = Mouse::getDX(), dy = Mouse::getDY();
    if(dx != 0 || dy != 0) {
        //std::cout<<"(dx, dy)=("<<dx<<","<<dy<<")"<<std::endl;
        camera.updateCameraDirection(dx,dy);
    }

    double scrollDy = Mouse::getScrollDY();
    if(scrollDy != 0) camera.updateCameraZoom(scrollDy);

}

