// This is an independent project of an individual developer. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
/* ======================================================================== 
   $File: main.cpp $
   $Created: 01-01-2017 00h00m00s $
   $Modified: 24-02-2017 01h56m01s $
   $Revision: $
   $Creator : TOFFANIN Marc $
   $Notice: Licensed under GNU GPL $
   ======================================================================== */


#if 0
#include <stdlib.h>
#include <array>
#include <iostream>

using namespace std;
 

#include "engine.h"
using namespace TE;// <= Using namesapce TE to save time

//TODO(Marc)

// ------ Multiple window with multiple contexts ------
// Get GLFW 3.2
// Get GLEW MX to be able to have multiple window with multiple context (thread safe) ??
// ------ Coordinate system ------
//Check rotations in good coordinate system

enum class flags_test {
    flag_1 = 0x1,
    flag_2 = 0x2,
    flag_3 = 0x4,
    flag_4 = 0x8
};

using flag_test_flags = flags<flags_test>;

inline
flag_test_flags operator|( flags_test bit0, flags_test bit1 )
{
    return (flag_test_flags( bit0 ) | bit1);
}

inline
flag_test_flags operator~( flags_test bits )
{
    return ~(flag_test_flags( bits ) );
}

static bool Continue = true;
static bool DebugMode = false;

void CloseApp(void)
{
    Continue = false;
}

// IMPORTANT (MArc) : MAKE THIS HIDDEN TO THE USER
void BindImGuiCallbacks()
{
    glfwSetMouseButtonCallback(TE::Window->GetHandle(), ImGui_ImplGlfwGL3_MouseButtonCallback);
    glfwSetScrollCallback(TE::Window->GetHandle(), ImGui_ImplGlfwGL3_ScrollCallback);
    glfwSetKeyCallback(TE::Window->GetHandle(), ImGui_ImplGlfwGL3_KeyCallback);
    glfwSetCharCallback(TE::Window->GetHandle(), ImGui_ImplGlfwGL3_CharCallback);
    glfwSetCursorPosCallback(TE::Window->GetHandle(), NULL);

}

void ToggleDebugMode()
{
    DebugMode = !DebugMode;
    if (DebugMode)
    {
        TE::Window->ShowCursor();
        BindImGuiCallbacks();
    }
    else
    {
        TE::Window->HideCursor();
        TE::Window->BindInputHandler();
    }
}

void TestFunction()
{
    TIMED_FUNCTION();
    for (int i = 0; i < 10; ++i)
        continue;
}

void Update()
{

}

void RenderFrame(const Physic::bvh_object_list& Scene, 
        const mat4f& View, const mat4f& Projection, 
        /*to delete*/ const Renderer::shader& BlitShader, 
        const Core::vertex_array& UnitQuad, 
        Renderer::skybox* skybox)
{
    TIMED_FUNCTION();

    //Matrices
    mat4f MV = Projection * View;
    mat4f MVP = MV;

    glViewport(0, 0, TE::Window->GetWidth(), TE::Window->GetHeight());
    TE::GBufferFBO->Bind();
    glClearColor(0.f, 1.f, 0.0f, 1.f);
    TE::GBufferFBO->Clear();

    // Render Atmosphere
    mat4f ViewNoTrans = mat4f(mat3f(View));    // Remove any translation component of the view matrix
    skybox->display(ViewNoTrans, Projection);
    TE::GBufferFBO->Clear(Core::frame_buffer::CLEAR_DEPTH);
    //Render scene geometry
    for (auto Actor : Scene)
        Actor->Render(Projection, View);

    //Clean default FBOs
    Core::frame_buffer::BindDefaultFBO();
    glClearColor(0.f, 1.f, 1.f, 1.f);
    Core::frame_buffer::ClearDefaultFBO();
    // Blit from GBuffer to default FBO
    BlitShader.Bind();
    BlitShader.SetInt("Texture", 0);
    glActiveTexture(GL_TEXTURE0);
    TE::GBufferFBO->BindTexture("Colors");
    //TE::GBufferFBO->BindTexture("Normals");
    //TE::GBufferFBO->BindDepthTexture();
    UnitQuad.Render();
}

void Render(const Physic::bvh_object_list& Scene, const mat4f& View, const mat4f& Projection, /*to delete*/ const Renderer::shader& BlitShader, const Core::vertex_array& UnitQuad, Renderer::skybox* skybox)
{
    RenderFrame(Scene, View, Projection, BlitShader, UnitQuad, skybox);

    // TODO (Marc) : Make this a debug_ui object
    if (DebugMode)
    {
        ImGui_ImplGlfwGL3_NewFrame();
        ImGui::SetNextWindowSize(ImVec2(200, 100), ImGuiSetCond_FirstUseEver);
        ImGui::Text("FPS : %.1f (%.3f ms/frame)", ImGui::GetIO().Framerate, 1000.0f / ImGui::GetIO().Framerate);
        auto k = simple_memory_manager::instance().MemoryUsed;
        auto test = simple_memory_manager::instance().MemoryUsed / (double)Megabytes(1);

        // NOTE ( Marc) : should test this on AMD cards
        //if (GLEW_ATI_meminfo)
        //	glGetIntegerv(GL_TEXTURE_FREE_MEMORY_ATI, availableKB);

        GLint total_mem_kb = 0;
        glGetIntegerv(GL_GPU_MEMORY_INFO_TOTAL_AVAILABLE_MEMORY_NVX,
                &total_mem_kb);

        GLint cur_avail_mem_kb = 0;
        glGetIntegerv(GL_GPU_MEMORY_INFO_CURRENT_AVAILABLE_VIDMEM_NVX,
                &cur_avail_mem_kb);
        auto VRAMUsed = total_mem_kb - cur_avail_mem_kb;
        ImGui::Text("PROGRAM RAM : %.2f MB", TE::simple_memory_manager::instance().MemoryUsed / (double)Megabytes(1));
        ImGui::Text("SYSTEM VRAM : %d / %d (%.2f %% used)", VRAMUsed / Kilobytes(1), total_mem_kb / Kilobytes(1), (VRAMUsed / (double)total_mem_kb)*100.f);

        std::sort(GlobalDebugEventList.begin(), GlobalDebugEventList.end(), [](const debug_event& a, const debug_event& b) { return (a.CyclesCount > b.CyclesCount); });
        for (auto Event : GlobalDebugEventList)
        {
            ImGui::Text("%s : %llu cycles", Event.GUID, Event.CyclesCount);
        }

        if (ImGui::IsMouseHoveringAnyWindow())
        {
            BindImGuiCallbacks();
        }
        else
        {
            TE::Window->BindInputHandler();
        }

        ImGui::Render();
    }
    else
    {
        ImGui_ImplGlfwGL3_NewFrame();
        ImGui::Render();
    }


    TE::Window->SwapBuffers();
    TE::Core::CheckOpenGLError("end frame");
}

//Launch game
int main(int argc, char** argv)
{
#if 1
    // TE_Init() : Init every global variables, creating a window, some fbos and shaders
    //
    // Window creation example
    // -------------------------------
    // static unsigned int ScreenWidth = 720;
    // static unsigned int ScreenHeight = 720;
    // static Core::window Window(ScreenWidth, ScreenHeight, "New Window");
    // Window.MakeCurrent();
    //
    // FBO creation example
    // -------------------------------
    // static Core::fbo GBufferFBO(ScreenWidth, ScreenHeight);
    // GBufferFBO.AddDrawBuffer("Color", Core::fbo::attachment::COLOR0,
    //	Core::texture2D::base_internal_format::RGBA,
    //	Core::texture2D::sized_internal_format::RGBA32F,
    //	Core::texture2D::data_type::FLOAT);
    // GBufferFBO.AddDrawBuffer("Normals", Core::fbo::attachment::COLOR1,
    //	Core::texture2D::base_internal_format::RGBA,
    //	Core::texture2D::sized_internal_format::RGBA32F,
    //	Core::texture2D::data_type::FLOAT);
    // GBufferFBO.AddDepthBuffer(Core::texture2D::sized_internal_format::DEPTH_COMPONENT24);
    //
    // Input handler creation example
    // -------------------------------
    // static Core::input_handler DefaultInputHandler;
    //
    // Shader creation example
    // -------------------------------
    // static Renderer::shader GBufferShader("Gbuffer");
    //	GBufferShader.Attach(Renderer::shader::type::VERTEX, "../assets/shaders/gbuffer.vert");
    //  GBufferShader.Attach(Renderer::shader::type::FRAGMENT, "../assets/shaders/gbuffer.frag");
    //  GBufferShader.Link();

    TE_Init();

    //Texture
    Core::texture2D Test("../assets/textures/spnza_bricks_a_diff.tga", "final",
            Core::texture2D::base_internal_format::RGB,
            Core::texture2D::sized_internal_format::SRGB8,
            Core::texture2D::data_type::UNSIGNED_BYTE);
    Core::texture2D Test2("../assets/textures/final.png", "final",
            Core::texture2D::base_internal_format::RGB,
            Core::texture2D::sized_internal_format::SRGB8,
            Core::texture2D::data_type::UNSIGNED_BYTE);
    //Sprite actor
    std::shared_ptr<Core::actor> Sprite = std::make_shared<Core::actor>();
    // Sprite component
    Renderer::sprite SpriteComponent(Test2);
    SpriteComponent.SetSize({ 1.f, 1.f });
    SpriteComponent.SetupAttachement(Sprite->GetRoot());
    // Input component
    std::shared_ptr<Core::input_component> SpriteInputComponent = std::make_shared<Core::input_component>();
    //SpriteInputComponent->Init();
    SpriteInputComponent->AddKeyBind(std::make_pair<int, int>(TE::Core::W, TE::Core::PRESS), std::bind(&Core::actor::AddVector, Sprite, vec3f(0.f, 1.f, 0.f)));
    SpriteInputComponent->AddKeyBind(std::make_pair<int, int>(TE::Core::A, TE::Core::PRESS), std::bind(&Core::actor::AddVector, Sprite, vec3f(-1.f, 0.f, 0.f)));
    SpriteInputComponent->AddKeyBind(std::make_pair<int, int>(TE::Core::S, TE::Core::PRESS), std::bind(&Core::actor::AddVector, Sprite, vec3f(0.f, -1.f, 0.f)));
    SpriteInputComponent->AddKeyBind(std::make_pair<int, int>(TE::Core::D, TE::Core::PRESS), std::bind(&Core::actor::AddVector, Sprite, vec3f(1.f, 0.f, 0.f)));
    //SpriteInputComponent->AddCursorBind(std::bind(&Renderer::camera_actor::HandleCursor, &FreeCam, std::placeholders::_1, std::placeholders::_2));
    SpriteInputComponent->SetupAttachement(Sprite->GetRoot());
    // Camera component centered on the sprite 
    Renderer::camera_component CameraComponent;
    CameraComponent.SetLocalPosition({ 0, -1, 0 });
    Renderer::camera_properties CameraProperties = {};
    CameraProperties.AspectRatio = (float)TE::Window->GetWidth() / TE::Window->GetHeight();
    CameraComponent.SetCameraProperty(CameraProperties);
    CameraComponent.SetupAttachement(Sprite->GetRoot());

    vector<const GLchar*> faces;
    faces.push_back("../assets/skyboxes/Test/xpos.png");
    faces.push_back("../assets/skyboxes/Test/xneg.png");
    faces.push_back("../assets/skyboxes/Test/ypos.png");
    faces.push_back("../assets/skyboxes/Test/yneg.png");
    faces.push_back("../assets/skyboxes/Test/zpos.png");
    faces.push_back("../assets/skyboxes/Test/zneg.png");
    Renderer::skybox* Skybox = new Renderer::skybox(faces);


    Renderer::camera_actor FreeCam({ 0, -2, 0 });
    FreeCam.SetCameraProperty(CameraProperties);
    std::shared_ptr<Core::input_component> FreeCamInput = std::make_shared<Core::input_component>();
    FreeCamInput->Init();
    FreeCamInput->AddKeyBind(std::make_pair<int, int>(TE::Core::W, TE::Core::PRESS), std::bind(&Core::actor::AddLocalVector, &FreeCam, vec3f(0.f, 1.f, 0.f)));
    FreeCamInput->AddKeyBind(std::make_pair<int, int>(TE::Core::A, TE::Core::PRESS), std::bind(&Core::actor::AddLocalVector, &FreeCam, vec3f(-1.f, 0.f, 0.f)));
    FreeCamInput->AddKeyBind(std::make_pair<int, int>(TE::Core::S, TE::Core::PRESS), std::bind(&Core::actor::AddLocalVector, &FreeCam, vec3f(0.f, -1.f, 0.f)));
    FreeCamInput->AddKeyBind(std::make_pair<int, int>(TE::Core::D, TE::Core::PRESS), std::bind(&Core::actor::AddLocalVector, &FreeCam, vec3f(1.f, 0.f, 0.f)));
    FreeCamInput->AddCursorBind(std::bind(&Renderer::camera_actor::HandleCursor, &FreeCam, std::placeholders::_1, std::placeholders::_2));
    FreeCamInput->SetupAttachement(FreeCam.GetRoot());

    //Background Sprite
    std::shared_ptr<Core::actor> SpriteBackground = std::make_shared<Core::actor>();
    // Sprite component
    Renderer::sprite SpriteBackgroundComponent(Test);
    SpriteBackgroundComponent.SetSize({ 20.f, 20.f });
    SpriteBackgroundComponent.SetupAttachement(SpriteBackground->GetRoot());



    //Geometry
    std::vector<Core::vertex> Vertices;
    Vertices.push_back({ { -1.f, -1.f, 0.f },  //Position
            { 0.f, 0.f, 1.f },    // Normal
            { 0.f, 0.f } });      // TexCoorsds
    Vertices.push_back({ { 1.f, -1.f, 0.f },  
            { 0.f, 0.f, 1.f },    
            { 1.f, 0.f } }); 
    Vertices.push_back({ { -1.f, 1.f, 0.f },
            { 0.f, 1.f, 1.f },
            { 0.f, 1.f } });
    Vertices.push_back({ { 1.f, 1.f, 0.f },
            { 0.f, 0.f, 1.f },
            { 1.f, 1.f } });
    std::vector<unsigned int> Indices = { 0, 1, 2, 2, 1, 3 };
    Core::vertex_array UnitQuad(Vertices, Indices);



    //Shaders
    Renderer::shader BlitShader("Blit");
    BlitShader.Attach(Renderer::shader::type::VERTEX, "../assets/shaders/blit.vert");
    BlitShader.Attach(Renderer::shader::type::FRAGMENT, "../assets/shaders/blit.frag");
    BlitShader.Link();

    // TODO (MArc) : Make an object "Renderer" so we can enable/disable option directly on it
    // Maybe even register the renderable objects
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glClearColor(0.f, 1.f, 0.0f, 1.f);

    //Physic
    Physic::bvh_object_list SceneActors;
    SceneActors.push_back(Sprite);
    SceneActors.push_back(SpriteBackground);
    // BVH construction
    Physic::bvh SceneTree(SceneActors);
    // Ray intersection
    Physic::ray Ray;
    Ray.Origin = Math::vec3f(0.f, -2.f, 0.f);
    Ray.Direction = Math::vec3f( 0.f, 1.f, 0.f );
    Ray.InverseDirection = 1.f / Ray.Direction;
    Ray.T = 1000000;
    SceneTree.GetIntersection(Ray);


    std::shared_ptr<Core::input_component> WindowController = std::make_shared<Core::input_component>();
    WindowController->Init();
    WindowController->AddKeyBind(std::make_pair<int, int>(TE::Core::ESCAPE, TE::Core::PRESS), &CloseApp);
    WindowController->AddKeyBind(std::make_pair<int, int>(TE::Core::KP_1, TE::Core::PRESS), std::bind(&Core::window::SetFullScreen, Window));
    WindowController->AddKeyBind(std::make_pair<int, int>(TE::Core::KP_2, TE::Core::PRESS), std::bind(&Core::window::SetWindowed, Window));
    WindowController->AddKeyBind(std::make_pair<int, int>(TE::Core::KP_3, TE::Core::PRESS), &ToggleDebugMode);


    flags<flags_test> Flags;
    Flags = flags_test::flag_1 | flags_test::flag_2;


    //Main loop
    while (Continue && TE::Window->Running())
    {
        BEGIN_FRAME();
        TestFunction();
#if 1
        Update();
        //Matrices
        mat4f View = FreeCam.GetView();
        mat4f Projection = FreeCam.GetProjection();
        Render(SceneActors, View, Projection, BlitShader, UnitQuad, Skybox);
#endif
    }

    delete Skybox;
#endif
    TE_Clean();
    return(0);
}

#endif

//#include "TECore/macros.h"
#include "TECore/window.cpp"

const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";
const char *fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\n\0";

using namespace TE::Core;

int main() {
    Window::window MainWindow;
    Window::Init(MainWindow.Handle, "MainWindow", "SuperDuper title");
    
    Renderer::renderer MainRenderer;
    Renderer::Init(MainRenderer, MainWindow.Handle);

    Window::Show(MainWindow.Handle);

    // build and compile our shader program
    // ------------------------------------
    // vertex shader
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    // check for shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // fragment shader
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    // check for shader compile errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // link shaders
    unsigned int shaderProgram = glCreateProgram();
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

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float vertices[] = {
         0.5f,  0.5f, 0.0f,  // top right
         0.5f, -0.5f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f,  // bottom left
        -0.5f,  0.5f, 0.0f   // top left 
    };
    unsigned int indices[] = {  // note that we start from 0!
        0, 1, 3,  // first Triangle
        1, 2, 3   // second Triangle
    };
    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0); 

    // remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0); 


    // uncomment this call to draw in wireframe polygons.
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);



std::cout << "start message loop" << std::endl;
    // Run the message loop.
    MSG msg = { };
    while (GetMessage(&msg, NULL, 0, 0))
    {
		
		        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // draw our first triangle
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
        //glDrawArrays(GL_TRIANGLES, 0, 6);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        // glBindVertexArray(0); // no need to unbind it every time 
		
        TranslateMessage(&msg);
        DispatchMessage(&msg);
		
		SwapBuffers(wglGetCurrentDC()); 
    }

    // NOTE(toffa): This will kill by shooting the program in the face
    // if we do not do this ( e.g return 0 ) the program is still running somehow but I m not sure this is
    // the best way to do it.
    // TODO(toffa): Is this the best way?
    ExitProcess(0);
    //return 0;
    
}