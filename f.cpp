/*
#include <SDL3/SDL.h>
#include <GL/glew.h>
#include <iostream>


int main()
{
    SDL_Init(SDL_INIT_VIDEO);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    SDL_Window* w1 = SDL_CreateWindow("Window1", 800, 600, SDL_WINDOW_OPENGL);
    SDL_Window* w2 = SDL_CreateWindow("Window2", 800, 600, SDL_WINDOW_OPENGL);

    SDL_GLContext ctx1 = SDL_GL_CreateContext(w1);
    SDL_GLContext ctx2 = SDL_GL_CreateContext(w2);

    float verts[] =
    {
        -0.5f,-0.5f,
         0.5f,-0.5f,
         0.0f, 0.5f
    };

    const char* vs =
        "#version 330 core\n"
        "layout (location=0) in vec2 pos;\n"
        "void main(){gl_Position=vec4(pos,0,1);}";

    const char* fs =
        "#version 330 core\n"
        "out vec4 color;\n"
        "void main(){color=vec4(0.2,0.7,1.0,1);}";


    // -------- CONTEXT 1 SETUP --------
    SDL_GL_MakeCurrent(w1, ctx1);
    glewInit();

    GLuint vao1, vbo1, vs1, fs1, prog1;

    glGenVertexArrays(1, &vao1);
    glGenBuffers(1, &vbo1);

    glBindVertexArray(vao1);
    glBindBuffer(GL_ARRAY_BUFFER, vbo1);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    vs1 = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs1, 1, &vs, nullptr);
    glCompileShader(vs1);

    fs1 = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs1, 1, &fs, nullptr);
    glCompileShader(fs1);

    prog1 = glCreateProgram();
    glAttachShader(prog1, vs1);
    glAttachShader(prog1, fs1);
    glLinkProgram(prog1);

    glDeleteShader(vs1);
    glDeleteShader(fs1);


    // -------- CONTEXT 2 SETUP --------
    SDL_GL_MakeCurrent(w2, ctx2);
    glewInit();

    GLuint vao2, vbo2, vs2, fs2, prog2;

    glGenVertexArrays(1, &vao2);
    glGenBuffers(1, &vbo2);

    glBindVertexArray(vao2);
    glBindBuffer(GL_ARRAY_BUFFER, vbo2);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    vs2 = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs2, 1, &vs, nullptr);
    glCompileShader(vs2);

    fs2 = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs2, 1, &fs, nullptr);
    glCompileShader(fs2);

    prog2 = glCreateProgram();
    glAttachShader(prog2, vs2);
    glAttachShader(prog2, fs2);
    glLinkProgram(prog2);

    glDeleteShader(vs2);
    glDeleteShader(fs2);


    bool running = true;

    while (running)
    {
        SDL_Event event;

        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED)
            {
                if (w1 && event.window.windowID == SDL_GetWindowID(w1))
                {
                    SDL_DestroyWindow(w1);
                    w1 = nullptr;
                }

                if (w2 && event.window.windowID == SDL_GetWindowID(w2))
                {
                    SDL_DestroyWindow(w2);
                    w2 = nullptr;
                }
            }

            if (event.type == SDL_EVENT_QUIT)
                running = false;
        }

        if (!w1 && !w2)
            running = false;


        if (w1)
        {
            SDL_GL_MakeCurrent(w1, ctx1);

            glViewport(0, 0, 800, 600);
            glClear(GL_COLOR_BUFFER_BIT);

            glUseProgram(prog1);
            glBindVertexArray(vao1);
            glDrawArrays(GL_TRIANGLES, 0, 3);

            SDL_GL_SwapWindow(w1);
        }


        if (w2)
        {
            SDL_GL_MakeCurrent(w2, ctx2);

            glViewport(0, 0, 800, 600);
            glClear(GL_COLOR_BUFFER_BIT);

            glUseProgram(prog2);
            glBindVertexArray(vao2);
            glDrawArrays(GL_TRIANGLES, 0, 3);

            SDL_GL_SwapWindow(w2);
        }
    }

    SDL_Quit();
}



#include <SDL3/SDL.h>
#include <GL/glew.h>
#include <iostream>
#include "Engine.hpp"


int main()
{
  

        SDL_Init(SDL_INIT_VIDEO);

        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

        SDL_Window* w1 = SDL_CreateWindow("Window1", 800, 600, SDL_WINDOW_OPENGL);


        const char* vs =
            "#version 330 core\n"
            "layout (location=0) in vec2 pos;\n"
            "void main(){gl_Position=vec4(pos,0,1);}";

        const char* fs =
            "#version 330 core\n"
            "out vec4 color;\n"
            "void main(){color=vec4(0.2,0.7,1.0,1);}";

        SDL_GLContext ctx1 = SDL_GL_CreateContext(w1);
        SDL_GL_MakeCurrent(w1, ctx1);
        glewInit();



        bool running = true;

        while (running)
        {
            SDL_Event event;

            while (SDL_PollEvent(&event))
            {
                if (event.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED)
                {
                    if (w1 && event.window.windowID == SDL_GetWindowID(w1))
                    {
                        SDL_DestroyWindow(w1);
                        w1 = nullptr;
                    }
                }

                if (event.type == SDL_EVENT_QUIT)
                    running = false;




                if (w1)
                {
                    SDL_GL_MakeCurrent(w1, ctx1);

                    glViewport(0, 0, 800, 600);
                    glClear(GL_COLOR_BUFFER_BIT);


                    SDL_GL_SwapWindow(w1);
                }



            }

            SDL_Quit();
        }




        */