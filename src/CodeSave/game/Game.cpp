// This is an independent project of an individual developer. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
/* ======================================================================== 
/* ========================================================================
   $File: $
   $Modified: 06-02-2017 07h50m20s $
   $Revision: $
   $Creator:  $
   $Notice: $
   ======================================================================== */
#include "Game.h"
#include <memory>
#include <vector>
#define PI 3.14159265359f

using namespace std;

Game::Game(int width, int height) : screenWidth(width),
                                    screenHeight(height),
                                    endgame(false)
{
    init();
}

//Check opengl version
void Game::startupGLDiagnostics()
{
    // print diagnostic information
    printf( "    GL VENDOR: %s\n", glGetString( GL_VENDOR ) );
    printf( "      VERSION: %s\n", glGetString( GL_VERSION ) );
    printf( "     RENDERER: %s\n", glGetString( GL_RENDERER ) );
    printf( "GLSL VERSION : %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION) );

    // test if we've got GL 3.0
    if( !GLEW_VERSION_3_0 )
    {
        printf( "OpenGL 3.0 not supported.\n" 
                "Please update your drivers and/or buy a better graphics card."
                );
    }
}

//Init opengl window and context
int Game::init()
{
    // SDL Init
    if(!glfwInit())
    {
        std::cout << "Erreur lors de l'initialisation de GLFW"  << std::endl; 
        return(-1);
    }

    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    glfwWindowHint(GLFW_VISIBLE, GL_TRUE);
    glfwWindowHint(GLFW_DECORATED, GL_TRUE);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

#if defined(__APPLE__)
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    int const DPI = 2; // For retina screens only
#else
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_FALSE);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
    int const DPI = 1;
# endif

    // Create SDL Wikndow
    window = glfwCreateWindow(screenWidth, screenHeight, "DEMO", NULL, NULL );
    if(window == 0)
    {
        std::cout << "Erreur lors de la creation de la fenetre"  << std::endl;
        glfwTerminate();
        return(-1);
    }

    glfwMakeContextCurrent(window);
    
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK)
    {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return(-1);
    }

    startupGLDiagnostics();

    //glEnable(GL_FRAMEBUFFER_SRGB);
    glEnable(GL_DEPTH_TEST);    // enable Z-buffering
    //glEnable(GL_MULTISAMPLE);
    //glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glClearColor( 0.0f, 0.0f, 0.0f, 1.0f);

    glfwSetInputMode( window, GLFW_STICKY_KEYS, GL_TRUE );
    //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetInputMode( window, GLFW_STICKY_MOUSE_BUTTONS, 1);

    // Enable vertical sync (on cards that support it)
    glfwSwapInterval( 1 );
    GLenum glerr = GL_NO_ERROR;
    glerr = glGetError();

    ImGui_ImplGlfwGL3_Init(window, true);
    return(0);
}

//close game
void Game::close()
{
    endgame = true;
}

//Draw all the scene
void Game::drawScene( )
{    
}

struct Particle
{
    glm::vec3 position;
    glm::vec3 tmp_position;
    glm::vec3 forces;
    glm::vec3 velocity;
    glm::vec3 d;
    float mass;
    float inv_mass;
    bool enabled;

    Particle(glm::vec3 p, float m) 
            :position(p)
            ,tmp_position(p)
            ,enabled(true)
            ,forces(glm::vec3(0.0))
            ,velocity(glm::vec3(0.0))
            ,d(glm::vec3(0.0))
    {
 
        if(m < 0.001) {
            m = 0.001;
        }
 
        mass = m;
        inv_mass = 1.0 / mass;
    }
 
};

struct Hair
{
    Hair( int num, float d, glm::vec3 root)
    {
        setup(num, d, root);
    }
    
    void setup( int num, float d, glm::vec3 root )
    {
        positions = new GLfloat[ 3*num ];
        float dim = 50;
        length = d;
        glm::vec3 pos = root;
        float mass = (float)( rand() % 1000) / 100.0f;
        for( int i = 0; i < num; ++i)
        {
            Particle* p = new Particle(pos, mass);
            particles.push_back(p);

            positions[3*i] = p->position.x;
            positions[(3*i)+1] = p->position.y;
            positions[(3*i)+2] = p->position.z;

            pos.y -= d;            
        }
        particles[0]->enabled = false;

        
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, 3*num*sizeof(GLfloat), positions, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);

        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER,0);
        glBindVertexArray(0);

        currentTime = glfwGetTime();
    }

    void addForce(glm::vec3 f)
    {
        for(std::vector<Particle*>::iterator it = particles.begin(); it != particles.end(); ++it) {
            Particle* p = *it;
            if(p->enabled) {
                p->forces += f;
            }
        }
    }

    void update()
    {
        float dt = (glfwGetTime() - currentTime)/10;
 
        // update velocities
        for(std::vector<Particle*>::iterator it = particles.begin(); it != particles.end(); ++it) {
            Particle* p = *it;
            if(!p->enabled) {
                p->tmp_position = p->position;
                continue;
            }
            p->velocity = p->velocity + dt * (p->forces * p->inv_mass);
            p->tmp_position += (p->velocity * dt);
            p->forces = glm::vec3(0.0, -0.1, 0.0);
            p->velocity *= 0.99;
        }    
        // solve constraints
        glm::vec3 dir;
        glm::vec3 curr_pos;
        for(size_t i = 1; i < particles.size(); ++i) {
            Particle* pa = particles[i - 1];
            Particle* pb = particles[i];
            curr_pos = pb->tmp_position;
            dir = pb->tmp_position - pa->tmp_position;
            dir = glm::normalize(dir);
            //cout << pa->position.y << " " << pb->tmp_position.y << "   + " << (dir*length).y << endl; 
            pb->tmp_position = pa->tmp_position + dir * length;
            pb->d = curr_pos - pb->tmp_position; //  - curr_pos;
            //cout << pb->d.y << endl;
        }    
 
        for(size_t i = 1; i < particles.size(); ++i) {
            Particle* pa = particles[i-1];
            Particle* pb = particles[i];
            if(!pa->enabled) {
                continue;
            }
            pa->velocity = ((pa->tmp_position - pa->position) / dt) + 0.9f *  (pb->d / dt);
            pa->position = pa->tmp_position;
        }
 
        Particle* last = particles.back();
        last->position = last->tmp_position;

        for(int i = 0; i < particles.size(); ++i)
        {
            positions[3*i] = particles[i]->position.x;
            positions[(3*i)+1] = particles[i]->position.y;
            positions[(3*i)+2] = particles[i]->position.z;
            
        }
    }

    void draw()
    {
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER,  3*particles.size()*sizeof(GLfloat) , positions, GL_STATIC_DRAW);
        glDrawArrays(GL_LINE_STRIP, 0, particles.size());           
    }

    GLfloat* positions;
    GLuint VAO, VBO;
    std::vector<Particle*> particles;
    glm::vec3 color;
    float length;
    float currentTime;
};

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    ControllerManager& cm = ControllerManager::getInstance();    
    cm.keyboardCallback( window, key, scancode, action, mods);
}

static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
    ControllerManager& cm = ControllerManager::getInstance();
    cm.mouseCursorCallback( window, xpos, ypos );
}

static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    ControllerManager& cm = ControllerManager::getInstance();
    cm.mouseCallback( window, button, action, mods );
}


struct SpotLight
{
	glm::vec3 position;
	float angle;
	glm::vec3 direction;
	float penumbraAngle;
	glm::vec3 color;
	float intensity;
	glm::mat4 worldToLightScreen;
};

struct DirectionalLight
{
	glm::vec3 direction;
	int padding;
	glm::vec3 color;
	float intensity;
	glm::mat4 worldToLightScreen;
};

struct PointLight
{
	glm::vec3 position;
	int padding;
	glm::vec3 color;
	float intensity;
};


#if 0
void Game::scene1( Player* p, Skybox* skybox, Times times )
{
	if (times.startTime < 1)
	{
		times.startTime = glfwGetTime();
	}
    //Get needed assets
    ShaderManager& shaderManager = Manager<ShaderManager>::instance();
    TextureManager& textureManager = Manager<TextureManager>::instance();
    VaoManager& vaoManager = Manager<VaoManager>::instance();
    FboManager& fboManager = Manager<FboManager>::instance();
	
	// Scene lights
	vector < DirectionalLight > dirLights;
	vector < SpotLight > spotLights;
	vector < PointLight > pointLights;

	float firstCut = 5.f;
	float secondCut = 8.f;
	float thirdCut = 9.f;
	float forthCut = 21.f;
	float fifthCut = 33.f;
	float sixthCut = 40.f;


    glm::mat4 projection = glm::perspective(p->getCamera()->getZoom(),
                                            (float)screenWidth/(float)screenHeight,
                                            0.1f, 1000.0f);
    glm::mat4 view = glm::mat4();

    int instanceCount = 1;
    int pointLightCount = 0;
    int directionalLightCount = 2;
    int spotLightCount = 4;
	if (times.elapsedTime < firstCut)
		spotLightCount = min( spotLightCount, (int)times.elapsedTime);
    p->move(times.deltaTime);
    float t = times.globalTime; 



    //Clean FBOs
    glEnable(GL_DEPTH_TEST);
    glBindFramebuffer( GL_FRAMEBUFFER, 0 );
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glViewport( 0, 0, screenWidth, screenHeight);
    glBindFramebuffer(GL_FRAMEBUFFER, fboManager["gbuffer"] );
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// Animate object in scene
	int roomSize = 20;
	glm::vec3 movingCubePosition; 
	glm::mat4 worldToView;
#if 1
	if (times.elapsedTime < firstCut)
	{
		movingCubePosition = glm::vec3(0.f, 1.f, 0.f);
		float localTime = times.elapsedTime;
		p->setPosition(glm::vec3(0.f, 2.f, -(roomSize / 2.f)));
		worldToView = p->getCamera()->getViewMatrix(movingCubePosition);
	}
	else if (times.elapsedTime < secondCut)
	{
		float localTime = times.elapsedTime - firstCut;
		float diff = secondCut - firstCut;
		float z = 1.f * (localTime / diff );
		movingCubePosition = glm::vec3(0.f, 1.f + z, 0.f);
		float x = ((roomSize) / 2.f) * sin((localTime / diff) * 2 * PI);
		float y = ((roomSize) / 2.f) * cos((localTime / diff) * 2 * PI);
		p->setPosition(glm::vec3(x, 2.f, y));
		worldToView = p->getCamera()->getViewMatrix(movingCubePosition);
	}
	else if (times.elapsedTime < thirdCut )
	{
		float localTime = times.elapsedTime - secondCut;
		movingCubePosition = glm::vec3(0.f, 2.f , 0.f);
		float diff = thirdCut - secondCut;
		float mul = localTime / diff;
		p->setPosition(glm::vec3(0.f, 2.f + mul * ((roomSize / 2.f) - 2.f), (1.f - mul) * -((roomSize / 2.f))));
		p->setPosition(glm::vec3(0.f, 2.f, (1.f - mul) * -((roomSize / 2.f))));
		worldToView = p->getCamera()->getViewMatrix(movingCubePosition);
	}
	else if (times.elapsedTime < forthCut)
	{
		float localTime = times.elapsedTime - thirdCut;
		float x = min(8.f, (localTime)) * sin(t);
		float y = min(8.f, (localTime)) * cos(t);
		movingCubePosition = glm::vec3(x, 2.f + abs(x * 2), y);
		p->setPosition(glm::vec3(0.f, (roomSize / 2.f), 0.f));
		worldToView = p->getCamera()->getViewMatrix(movingCubePosition);
	}
	else if (times.elapsedTime < sixthCut)
	{
		float localTime = times.elapsedTime - thirdCut;
		movingCubePosition = glm::vec3(8.2f, 18.5, 3.5f);
		p->setPosition(glm::vec3(0.f, (roomSize / 2.f), 0.f));
		worldToView = p->getCamera()->getViewMatrix(movingCubePosition);
	}
#endif
#if 0
	movingCubePosition = glm::vec3(0.f, 1.f, 0.f);
	float localTime = times.elapsedTime;
	p->setPosition(glm::vec3(-50.f, -50.f, 0.f));
	worldToView = p->getCamera()->getViewMatrix(movingCubePosition);
#endif
	glm::mat4 movingCubeModel = glm::translate(glm::mat4(), movingCubePosition);


	glm::mat4 roomModel = glm::translate(glm::mat4(), glm::vec3(0.f, (roomSize / 2.f), 0.f)); 
	roomModel = glm::scale(roomModel, glm::vec3(roomSize));

	//Create spotlights
	SpotLight s1 = {        // Setup light data                
		glm::vec3( roomSize/2.f ),  //Spot position
		10.f,  //Spot angle
		glm::normalize(movingCubePosition - glm::vec3(roomSize / 2.f)),  //Spot direction		
		15.f, //Spoit penumbra angle
		glm::vec3(1.f, 1.f, 1.f), //Spot color
	    100.f}; //Spot intensity
	spotLights.push_back(s1); 

	SpotLight s2 = {        // Setup light data                
		glm::vec3((roomSize / 2.f), (roomSize / 2.f), -(roomSize / 2.f)),  //Spot position
		10.f,  //Spot angle
		glm::normalize(movingCubePosition - glm::vec3((roomSize / 2.f), (roomSize / 2.f), -(roomSize / 2.f))),  //Spot direction		
		15.f, //Spoit penumbra angle
		glm::vec3(1.f, 1.f, 1.f), //Spot color
		100.f }; //Spot intensity
	spotLights.push_back(s2);

	SpotLight s3 = {        // Setup light data                
		glm::vec3(-(roomSize / 2.f), (roomSize / 2.f), -(roomSize / 2.f)),  //Spot position
		10.f,  //Spot angle
		glm::normalize(movingCubePosition - glm::vec3(-(roomSize / 2.f), (roomSize / 2.f), -(roomSize / 2.f))),  //Spot direction		
		15.f, //Spoit penumbra angle
		glm::vec3(1.f, 1.f, 1.f), //Spot color
		100.f }; //Spot intensity
	spotLights.push_back(s3);

	SpotLight s4 = {        // Setup light data                
		glm::vec3(-(roomSize / 2.f), (roomSize / 2.f), (roomSize / 2.f)),  //Spot position
		10.f,  //Spot angle
		glm::normalize(movingCubePosition - glm::vec3(-(roomSize / 2.f), (roomSize / 2.f), (roomSize / 2.f))),  //Spot direction		
		15.f, //Spoit penumbra angle
		glm::vec3(1.f, 1.f, 1.f), //Spot color
		100.f }; //Spot intensity
	spotLights.push_back(s4);
	spotLights.push_back(s4);



    glm::mat4 objectToWorld;
    glm::mat4 mv = worldToView * objectToWorld;
    glm::mat4 mvp = projection * mv;
    glm::mat4 inverseProjection = glm::inverse( projection );

    //Render in GBUFFER
    glBindFramebuffer(GL_FRAMEBUFFER, fboManager["gbuffer"]);

#if 0
    shaderManager["gbuffer"]->use();
    // Select textures
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, textureManager["brick_spec"]);
    // Upload uniforms
    glUniformMatrix4fv(glGetUniformLocation(shaderManager["gbuffer"]->getProgram(), "MVP"), 1, GL_FALSE, glm::value_ptr(mvp));
    glUniformMatrix4fv(glGetUniformLocation(shaderManager["gbuffer"]->getProgram(), "MV"), 1, GL_FALSE, glm::value_ptr(mv));
    glUniform1i(glGetUniformLocation(shaderManager["gbuffer"]->getProgram(), "InstanceCount"), (int) instanceCount);
    glUniform1f(glGetUniformLocation(shaderManager["gbuffer"]->getProgram(), "SpecularPower"), 30.f);
    glUniform1f(glGetUniformLocation(shaderManager["gbuffer"]->getProgram(), "Time"), t);
    glUniform1i(glGetUniformLocation(shaderManager["gbuffer"]->getProgram(), "Diffuse"), 0);
    glUniform1i(glGetUniformLocation(shaderManager["gbuffer"]->getProgram(), "Specular"), 1);
    //Render scene
	    // Render room cube
    glBindVertexArray(vaoManager["cube"]);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureManager["RoomTex"]);
	mv = worldToView * roomModel;
    mvp = projection * mv;
    glUniformMatrix4fv(glGetUniformLocation(shaderManager["gbuffer"]->getProgram(), "MVP"), 1, GL_FALSE, glm::value_ptr(mvp));
    glUniformMatrix4fv(glGetUniformLocation(shaderManager["gbuffer"]->getProgram(), "MV"), 1, GL_FALSE, glm::value_ptr(mv));
	glUniform1i(glGetUniformLocation(shaderManager["gbuffer"]->getProgram(), "reverse_normal"), 1);
    glDrawElements(GL_TRIANGLES, 12*3, GL_UNSIGNED_INT, (void*)0);
	glUniform1i(glGetUniformLocation(shaderManager["gbuffer"]->getProgram(), "reverse_normal"), 0);
	    // Render moving cube
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureManager["movingCubeTex"]);
	mv = worldToView * movingCubeModel;
    mvp = projection * mv;
    glUniformMatrix4fv(glGetUniformLocation(shaderManager["gbuffer"]->getProgram(), "MVP"), 1, GL_FALSE, glm::value_ptr(mvp));
    glUniformMatrix4fv(glGetUniformLocation(shaderManager["gbuffer"]->getProgram(), "MV"), 1, GL_FALSE, glm::value_ptr(mv));
    glDrawElements(GL_TRIANGLES, 12*3, GL_UNSIGNED_INT, (void*)0); 


    glUniform1f(glGetUniformLocation(shaderManager["gbuffer"]->getProgram(), "Time"), 0.f);
    shaderManager["gbuffer"]->unuse();
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindVertexArray(0);

	Utils::checkGlError("rendering gbuffer");
#endif  

#if 1
	glDisable(GL_CULL_FACE);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	shaderManager["explode"]->use();
	glPatchParameteri(GL_PATCH_VERTICES, 3);
	// Select textures
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, textureManager["brick_spec"]);
	// Upload uniforms
	glUniformMatrix4fv(glGetUniformLocation(shaderManager["explode"]->getProgram(), "MVP"), 1, GL_FALSE, glm::value_ptr(mvp));
	glUniformMatrix4fv(glGetUniformLocation(shaderManager["explode"]->getProgram(), "MV"), 1, GL_FALSE, glm::value_ptr(mv));
	glUniformMatrix4fv(glGetUniformLocation(shaderManager["explode"]->getProgram(), "inverse_proj"), 1, GL_FALSE, glm::value_ptr(inverseProjection));
	glUniform1i(glGetUniformLocation(shaderManager["explode"]->getProgram(), "InstanceCount"), (int)instanceCount);
	glUniform1f(glGetUniformLocation(shaderManager["explode"]->getProgram(), "SpecularPower"), 30.f);
	glUniform1f(glGetUniformLocation(shaderManager["explode"]->getProgram(), "Time"), t);
	glUniform1i(glGetUniformLocation(shaderManager["explode"]->getProgram(), "Diffuse"), 0);
	glUniform1i(glGetUniformLocation(shaderManager["explode"]->getProgram(), "Specular"), 1);

	if ( times.elapsedTime < forthCut)
		glUniform1f(glGetUniformLocation(shaderManager["explode"]->getProgram(), "magnitude"), 0.f);
	else if ( times.elapsedTime < fifthCut )
	{
		glUniform1f(glGetUniformLocation(shaderManager["explode"]->getProgram(), "magnitude"), 10.f);
		glUniform1i(glGetUniformLocation(shaderManager["explode"]->getProgram(), "reverse"), 0);
		glUniform1f(glGetUniformLocation(shaderManager["explode"]->getProgram(), "reverse_from_max"), 50.f);
		glUniform1f(glGetUniformLocation(shaderManager["explode"]->getProgram(), "Time"), (times.elapsedTime - forthCut));
	}
	else if (times.elapsedTime < sixthCut)
	{
		float diff = fifthCut - forthCut;
		glUniform1f(glGetUniformLocation(shaderManager["explode"]->getProgram(), "magnitude"), 10.f);
		glUniform1i(glGetUniformLocation(shaderManager["explode"]->getProgram(), "reverse"), 1);
		glUniform1f(glGetUniformLocation(shaderManager["explode"]->getProgram(), "reverse_from_max"), diff * 10.f);
		glUniform1f(glGetUniformLocation(shaderManager["explode"]->getProgram(), "Time"), (times.elapsedTime - fifthCut));
	}
	//Render scene
	glBindVertexArray(vaoManager["cube"]);
	if (times.elapsedTime < fifthCut)
	{
		// Render room cube		
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureManager["RoomTex"]);
		mv = worldToView * roomModel;
		mvp = projection * mv;
		glUniformMatrix4fv(glGetUniformLocation(shaderManager["explode"]->getProgram(), "MVP"), 1, GL_FALSE, glm::value_ptr(mvp));
		glUniformMatrix4fv(glGetUniformLocation(shaderManager["explode"]->getProgram(), "MV"), 1, GL_FALSE, glm::value_ptr(mv));
		glUniform1i(glGetUniformLocation(shaderManager["explode"]->getProgram(), "reverse_normal"), 1);
		glDrawElements(GL_PATCHES, 12 * 3, GL_UNSIGNED_INT, (void*)0);
	}
	glUniform1i(glGetUniformLocation(shaderManager["explode"]->getProgram(), "reverse_normal"), 0);
	// Render moving cube
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureManager["movingCubeTex"]);
	mv = worldToView * movingCubeModel;
	mvp = projection * mv;
	glUniformMatrix4fv(glGetUniformLocation(shaderManager["explode"]->getProgram(), "MVP"), 1, GL_FALSE, glm::value_ptr(mvp));
	glUniformMatrix4fv(glGetUniformLocation(shaderManager["explode"]->getProgram(), "MV"), 1, GL_FALSE, glm::value_ptr(mv));
	glDrawElements(GL_PATCHES, 12 * 3, GL_UNSIGNED_INT, (void*)0);


	glUniform1f(glGetUniformLocation(shaderManager["explode"]->getProgram(), "Time"), 0.f);
	shaderManager["explode"]->unuse();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindVertexArray(0);
	glEnable(GL_CULL_FACE);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
#endif

	Utils::checkGlError("rendering gbuffer");
    // Shadow passes


#if 1
    const int uboSize = 512;
    // Map the spot light data UBO
    glBindBuffer(GL_UNIFORM_BUFFER, fboManager["ubo"]);
    char * spotLightBuffer = (char *)glMapBufferRange(GL_UNIFORM_BUFFER, 0, uboSize, GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);
    // Bind the shadow FBO
    glBindFramebuffer(GL_FRAMEBUFFER, fboManager["shadow"]);
    // Use shadow program
    shaderManager["shadow"]->use();
    glViewport(0, 0, 1024, 1024);

    for (int i = 0; i < spotLightCount; ++i)
    {          
        // Light space matrices
		glm::mat4 lightProjection = glm::perspective(glm::radians(spotLights[i].penumbraAngle*2.f), 1.f, 1.f, 150.f);
		glm::mat4 worldToLight = glm::lookAt(spotLights[i].position, spotLights[i].position + spotLights[i].direction, glm::vec3(0.f, 1.f, 0.f));
        //std::cout << glm::to_string(worldToLight) << std::endl;
        // Attach shadow texture for current light
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, textureManager["shadow"+to_string(i)], 0);
        // Clear only the depth buffer
        glClear(GL_DEPTH_BUFFER_BIT);
		Utils::checkGlError("rendering shadowmap");

        // Render the scene
        glUniform1f(glGetUniformLocation(shaderManager["shadow"]->getProgram(), "Time"),t);
        glBindVertexArray(vaoManager["cube"]);
		// Render moving cube
		glm::mat4 objectToLight = worldToLight * movingCubeModel;
		glm::mat4 objectToLightScreen = lightProjection * objectToLight;
		glUniformMatrix4fv(glGetUniformLocation(shaderManager["shadow"]->getProgram(), "MVP"), 1, GL_FALSE, glm::value_ptr(objectToLightScreen));
		glUniformMatrix4fv(glGetUniformLocation(shaderManager["shadow"]->getProgram(), "MV"), 1, GL_FALSE, glm::value_ptr(objectToLight));
		glUniform1i(glGetUniformLocation(shaderManager["shadow"]->getProgram(), "reverse_normal"), 0);
		glDrawElements(GL_TRIANGLES, 12 * 3, GL_UNSIGNED_INT, (void*)0);
		Utils::checkGlError("rendering shadowmap");
    }        
    glUnmapBuffer(GL_UNIFORM_BUFFER);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
	Utils::checkGlError("rendering shadowmap");
#endif

#if 0
    const int uboSize = 512;
    const int SPOT_LIGHT_COUNT = 1;
    // Map the spot light data UBO
    glBindBuffer(GL_UNIFORM_BUFFER, fboManager["ubo"]);
    char * directionalLightBuffer = (char *)glMapBufferRange(GL_UNIFORM_BUFFER, 0, uboSize, GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);
    // Bind the shadow FBO
    glBindFramebuffer(GL_FRAMEBUFFER, fboManager["shadow"]);
    // Use shadow program
    shaderManager["shadow"]->use();
    glViewport(0, 0, 1024, 1024);

    for (int i = 0; i < directionalLightCount; ++i)
    {
        glm::vec3 lp = glm::vec3( 4.0, 4.0, 0.0);
        // Light space matrices
        glm::mat4 lightProjection = glm::ortho(-50.0f, 50.0f, -50.0f, 50.0f, 0.1f, 100.0f);
        glm::mat4 worldToLight = glm::lookAt(lp, glm::vec3(0.0f), glm::vec3(1.f));
        glm::mat4 objectToWorld;
        glm::mat4 objectToLight = worldToLight * objectToWorld;
        glm::mat4 objectToLightScreen = lightProjection * objectToLight;

        // Attach shadow texture for current light

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, textureManager["shadow"+to_string(i)], 0);
        // Clear only the depth buffer
        glClear(GL_DEPTH_BUFFER_BIT);

        // Update scene uniforms
        glUniformMatrix4fv(glGetUniformLocation(shaderManager["shadow"]->getProgram(), "MVP"), 1, GL_FALSE, glm::value_ptr(objectToLightScreen));
        glUniformMatrix4fv(glGetUniformLocation(shaderManager["shadow"]->getProgram(), "MV"), 1, GL_FALSE, glm::value_ptr(objectToLight));
		Utils::checkGlError("rendering shadowmap");
        // Render the scene
		// Render room cube
		glBindVertexArray(vaoManager["cube"]);
		glm::mat4 m = glm::mat4();
		glUniform1i(glGetUniformLocation(shaderManager["shadow"]->getProgram(), "reverse_normal"), 0);
		Utils::checkGlError("rendering shadowmap");
		// Render moving cube
		m = glm::mat4();
		objectToLight = worldToLight * movingCubeModel;
		objectToLightScreen = lightProjection * objectToLight;
		glUniformMatrix4fv(glGetUniformLocation(shaderManager["shadow"]->getProgram(), "MVP"), 1, GL_FALSE, glm::value_ptr(objectToLightScreen));
		glUniformMatrix4fv(glGetUniformLocation(shaderManager["shadow"]->getProgram(), "MV"), 1, GL_FALSE, glm::value_ptr(objectToLight));
		glDrawElements(GL_TRIANGLES, 12 * 3, GL_UNSIGNED_INT, (void*)0);
		shaderManager["shadow"]->unuse();
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glBindVertexArray(0);
		Utils::checkGlError("rendering shadowmap");
  }        
  glUnmapBuffer(GL_UNIFORM_BUFFER);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

	Utils::checkGlError("rendering shadowmap");
#endif



#if 0
    const int uboSize = 512;
    const int LIGHT_COUNT = 1;
    // Bind the shadow FBO
    glBindFramebuffer(GL_FRAMEBUFFER, fboManager["plShadow"]);
    // Use shadow program
    shaderManager["plShadow"]->use();
    glViewport(0, 0, 1024, 1024);
    glEnable(GL_DEPTH_TEST);
    //glClearDepth(0.5f);

    for (int i = 0; i < pointLightCount; ++i)
    {
        
        glm::vec3 lp = glm::vec3( worldToView * glm::vec4( 4.0, 4.0, 0.0, 1.0) );
        // Light space matrices
        glm::mat4 lightProjection = glm::perspective( glm::radians(90.f), 1.f, 1.f, 100.f);

        std::vector<glm::mat4> worldToLight;
        worldToLight.push_back(lightProjection * glm::lookAt(lp, lp + glm::vec3(1.0,0.0,0.0), glm::vec3(0.0,-1.0,0.0)));
        worldToLight.push_back(lightProjection * glm::lookAt(lp, lp + glm::vec3(-1.0,0.0,0.0), glm::vec3(0.0,-1.0,0.0)));
        worldToLight.push_back(lightProjection * glm::lookAt(lp, lp + glm::vec3(0.0,1.0,0.0), glm::vec3(0.0,0.0,1.0)));
        worldToLight.push_back(lightProjection * glm::lookAt(lp, lp + glm::vec3(0.0,-1.0,0.0), glm::vec3(0.0,0.0,-1.0)));
        worldToLight.push_back(lightProjection * glm::lookAt(lp, lp + glm::vec3(0.0,0.0,1.0), glm::vec3(0.0,-1.0,0.0)));
        worldToLight.push_back(lightProjection * glm::lookAt(lp, lp + glm::vec3(0.0,0.0,-1.0), glm::vec3(0.0,-1.0,0.0)));
                                   
        glm::mat4 objectToWorld = worldToView;

        // Attach shadow texture for current light
        // glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, textureManager["plShadow"], 0);
        // Clear only the depth buffer
        glClear(GL_DEPTH_BUFFER_BIT);
        // Update scene uniforms
        for(GLuint i = 0 ; i < 6 ; ++i)
            glUniformMatrix4fv(glGetUniformLocation(shaderManager["plShadow"]->getProgram(), ("shadowMatrices[" + to_string(i) + "]").c_str()), 1 , GL_FALSE, glm::value_ptr(worldToLight[i]));
        glUniform3fv(glGetUniformLocation(shaderManager["plShadow"]->getProgram(), "lightPos"), 1, &lp[0]);
        glUniformMatrix4fv(glGetUniformLocation(shaderManager["plShadow"]->getProgram(), "model"), 1, GL_FALSE, glm::value_ptr(objectToWorld));
        // Render the scene
        glBindVertexArray(vaoManager["cube"]);
        glDrawElementsInstanced(GL_TRIANGLES, 12 * 3, GL_UNSIGNED_INT, (void*)0, (int) instanceCount);
        glBindVertexArray(vaoManager["plane"]);
        glDrawElements(GL_TRIANGLES, 2 * 3, GL_UNSIGNED_INT, (void*)0);
      
    }
    //glClear(GL_DEPTH_BUFFER_BIT);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindVertexArray(0);

glClearDepth(1.f);
#endif

    
    //Render lighting in postfx fbo
    glBindFramebuffer(GL_FRAMEBUFFER, fboManager["fx"]);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 , GL_TEXTURE_2D, textureManager["fx0"], 0);

    glViewport(0,0,screenWidth, screenHeight);
    glClear(GL_COLOR_BUFFER_BIT);
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE, GL_ONE);
    Utils::checkGlError("before lights");
    // Select textures
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureManager["gBufferColor"]);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, textureManager["gBufferNormals"]);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, textureManager["gBufferDepth"]);
    // Bind the same VAO for all lights
    glBindVertexArray(vaoManager["quad"]);

#if 0
    // Render point lights
    shaderManager["pointLight"]->use();
    glUniformMatrix4fv(glGetUniformLocation(shaderManager["pointLight"]->getProgram(),"InverseProjection") , 1, 0, glm::value_ptr(inverseProjection));
    glUniform1i(glGetUniformLocation(shaderManager["pointLight"]->getProgram(), "ColorBuffer"), 0);
    glUniform1i(glGetUniformLocation(shaderManager["pointLight"]->getProgram(), "NormalBuffer"), 1);
    glUniform1i(glGetUniformLocation(shaderManager["pointLight"]->getProgram(), "DepthBuffer"), 2);
    glUniform1i(glGetUniformLocation(shaderManager["pointLight"]->getProgram(), "Shadow"), 3);

    glActiveTexture(GL_TEXTURE3);
    for (int i = 0; i < pointLightCount; ++i)
    {
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureManager["plShadow"]);
    glBindBuffer(GL_UNIFORM_BUFFER, fboManager["ubo"]);
    PointLight p = { 
    glm::vec3( worldToView * glm::vec4(4.f,4.f,0.f, 1.0)),
        0,
        glm::vec3(1.0,1.0,1.0),
        3.0
        };
    PointLight * pointLightBuffer = (PointLight *)glMapBufferRange(GL_UNIFORM_BUFFER, 0, 512, GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);
    *pointLightBuffer = p;
    glUnmapBuffer(GL_UNIFORM_BUFFER);
    glBindBufferBase(GL_UNIFORM_BUFFER, glGetUniformBlockIndex(shaderManager["pointLight"]->getProgram(), "light"), fboManager["ubo"]);
    glDrawElements(GL_TRIANGLES, 2 * 3, GL_UNSIGNED_INT, (void*)0);
}
    shaderManager["pointLight"]->unuse();
#endif
    
#if 0
// Render directional lights
    shaderManager["dirLight"]->use();
    glUniformMatrix4fv(glGetUniformLocation(shaderManager["dirLight"]->getProgram(),"InverseProjection") , 1, 0, glm::value_ptr(inverseProjection));
    glUniform1i(glGetUniformLocation(shaderManager["dirLight"]->getProgram(), "ColorBuffer"), 0);
    glUniform1i(glGetUniformLocation(shaderManager["dirLight"]->getProgram(), "NormalBuffer"), 1);
    glUniform1i(glGetUniformLocation(shaderManager["dirLight"]->getProgram(), "DepthBuffer"), 2);
    glUniform1i(glGetUniformLocation(shaderManager["dirLight"]->getProgram(), "Shadow"), 3);

    glActiveTexture(GL_TEXTURE3);
    for (int i = 0; i < directionalLightCount; ++i)
    {
    glBindTexture(GL_TEXTURE_2D, textureManager["shadow" + to_string(i)]);
    glBindBuffer(GL_UNIFORM_BUFFER, fboManager["ubo"]);
    glm::vec3 lp = glm::vec3(4.f, 4.0f, 0.0f);
    // Light space matrices
    glm::mat4 lightProjection = glm::ortho(-50.0f, 50.0f, -50.0f, 50.0f, 0.1f, 100.0f);
    glm::mat4 worldToLight = glm::lookAt(lp, glm::vec3(0.0f), glm::vec3(1.0f));
    glm::mat4 objectToWorld;
    glm::mat4 objectToLight = worldToLight * objectToWorld;
    glm::mat4 objectToLightScreen = lightProjection * objectToLight;
        
    DirectionalLight d = { 
    glm::vec3( worldToView * glm::vec4(-1.0, -1.0, 0.0, 0.0)), 0,
        glm::vec3(1.0, 1.0, 1.0),
        0.8f,
        objectToLightScreen * glm::inverse(worldToView)
        };
    DirectionalLight * directionalLightBuffer = (DirectionalLight *)glMapBufferRange(GL_UNIFORM_BUFFER, 0, 512, GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);
    *directionalLightBuffer = d;
    glUnmapBuffer(GL_UNIFORM_BUFFER);
    glBindBufferBase(GL_UNIFORM_BUFFER, glGetUniformBlockIndex(shaderManager["dirLight"]->getProgram(), "light"), fboManager["ubo"]);
    glDrawElements(GL_TRIANGLES, 2 * 3, GL_UNSIGNED_INT, (void*)0);
}
    shaderManager["dirLight"]->unuse();
#endif

#if 1
// Render spot lights
    shaderManager["spotLight"]->use();
    glUniformMatrix4fv(glGetUniformLocation(shaderManager["spotLight"]->getProgram(),"InverseProjection") , 1, 0, glm::value_ptr(inverseProjection));
    glUniform1i(glGetUniformLocation(shaderManager["spotLight"]->getProgram(), "ColorBuffer"), 0);
    glUniform1i(glGetUniformLocation(shaderManager["spotLight"]->getProgram(), "NormalBuffer"), 1);
    glUniform1i(glGetUniformLocation(shaderManager["spotLight"]->getProgram(), "DepthBuffer"), 2);
    glUniform1i(glGetUniformLocation(shaderManager["spotLight"]->getProgram(), "Shadow"), 3);

    glActiveTexture(GL_TEXTURE3);
    for (int i = 0; i < spotLightCount; ++i)
    {
    glBindTexture(GL_TEXTURE_2D, textureManager["shadow"+to_string(i)]);
    glBindBuffer(GL_UNIFORM_BUFFER, fboManager["ubo"]);
    // Light space matrices
    glm::mat4 projection = glm::perspective(glm::radians(spotLights[i].penumbraAngle*2.f), 1.f, 1.f, 150.f); 
	glm::mat4 worldToLight = glm::lookAt(spotLights[i].position, spotLights[i].position + spotLights[i].direction, glm::vec3(0.f, 1.f, 0.f));
    glm::mat4 objectToLight = worldToLight * objectToWorld;
    glm::mat4 objectToLightScreen = projection * objectToLight;
	spotLights[i].position = glm::vec3(worldToView * glm::vec4(spotLights[i].position, 1.f));
	spotLights[i].direction = glm::vec3(worldToView * glm::vec4(spotLights[i].direction, 0.f));
	spotLights[i].worldToLightScreen = projection * worldToLight * glm::inverse(worldToView);
    SpotLight * spotLightBuffer = (SpotLight *)glMapBufferRange(GL_UNIFORM_BUFFER, 0, 512, GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);
	*spotLightBuffer = spotLights[i];
    glUnmapBuffer(GL_UNIFORM_BUFFER);
    glBindBufferBase(GL_UNIFORM_BUFFER, glGetUniformBlockIndex(shaderManager["spotLight"]->getProgram(), "light"), fboManager["ubo"]);
    glDrawElements(GL_TRIANGLES, 2 * 3, GL_UNSIGNED_INT, (void*)0);
    }  
    //glDisable(GL_BLEND);
    shaderManager["spotLight"]->unuse();
#endif
 
    Utils::checkGlError("poiintlight");

    // Draw skybox as last
    glm::vec4 sunNDC = mvp * ((-10000.f)*glm::vec4(-1.f, -1.f, 0.f, 0.f));  // Sun equal infinite point
    sunNDC = glm::vec4( glm::vec3(sunNDC)/sunNDC.w, 1.0);
    sunNDC += 1.0;
    sunNDC *= 0.5;
    view = glm::mat4(glm::mat3(p->getCamera()->getViewMatrix()));    // Remove any translation component of the view matrix
    skybox->display(view, projection, textureManager["gBufferDepth"], screenWidth, screenHeight);
#if 0
    shaderManager["sun"]->use();
    //glUniform1i(glGetUniformLocation(shaderManager["sun"]->getProgram(), "Depth"), 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureManager["gBufferDepth"]);
    glUniform3fv(glGetUniformLocation(shaderManager["sun"]->getProgram(), "sun"), 1, &sunNDC[0]);
    glBindVertexArray(vaoManager["quad"]);
    glDrawElements(GL_TRIANGLES, 2 * 3, GL_UNSIGNED_INT, (void*)0);
#endif
    //Render sun with occludee in black for light shaft postFX
    shaderManager["sun"]->use();
    glBindFramebuffer( GL_FRAMEBUFFER, fboManager["fx"] );
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 , GL_TEXTURE_2D, textureManager["fx4"], 0);
    glClear(GL_COLOR_BUFFER_BIT);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureManager["gBufferDepth"]);
    glUniform3fv(glGetUniformLocation(shaderManager["sun"]->getProgram(), "sun"), 1, &sunNDC[0]);
    glBindVertexArray(vaoManager["quad"]);
    glDrawElements(GL_TRIANGLES, 2 * 3, GL_UNSIGNED_INT, (void*)0);
    shaderManager["sun"]->unuse();

    Utils::checkGlError("Skybox");
    glDisable(GL_BLEND);

#if 1
		glBindFramebuffer(GL_FRAMEBUFFER, fboManager["fx"]);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureManager["fx5"], 0);
		glClear(GL_COLOR_BUFFER_BIT);
		glViewport(0, 0, screenWidth, screenHeight);
		shaderManager["bright"]->use();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureManager["gBufferColor"]);
		glBindVertexArray(vaoManager["quad"]);
		glDrawElements(GL_TRIANGLES, 2 * 3, GL_UNSIGNED_INT, (void*)0);

		if (times.elapsedTime < secondCut)
			glClear(GL_COLOR_BUFFER_BIT);  //Clear texture to black so we dont add any bloom areas

		shaderManager["bright"]->unuse();

		int amount = max(0.f, (times.elapsedTime - secondCut)*20.f);
		amount = min(6, amount);
		GLboolean horizontal = true, first_iteration = true;
		amount = amount + (amount % 2);
		shaderManager["blur"]->use();
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureManager["fx" + to_string(2 + horizontal)], 0);
		glClear(GL_COLOR_BUFFER_BIT);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureManager["fx" + to_string(2 + !horizontal)], 0);
		glClear(GL_COLOR_BUFFER_BIT);
		for (GLuint i = 0; i < amount; i++)
		{
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureManager["fx" + to_string(2 + horizontal)], 0);
			glUniform1i(glGetUniformLocation(shaderManager["blur"]->getProgram(), "horizontal"), horizontal);
			glBindTexture(
				GL_TEXTURE_2D, first_iteration ? textureManager["fx5"] : textureManager["fx" + to_string(2 + !horizontal)]
				);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0);
			horizontal = !horizontal;
			if (first_iteration)
				first_iteration = false;
		}
		shaderManager["blur"]->unuse();

		shaderManager["bloom"]->use();
		glBindFramebuffer(GL_FRAMEBUFFER, fboManager["fx"]);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureManager["fx1"], 0);
		glClear(GL_COLOR_BUFFER_BIT);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureManager["fx0"]);
		glUniform1i(glGetUniformLocation(shaderManager["bloom"]->getProgram(), "scene"), 0);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, textureManager["fx3"]);
		glUniform1i(glGetUniformLocation(shaderManager["bloom"]->getProgram(), "bloomBlur"), 1);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0);
		shaderManager["bloom"]->unuse();
		Utils::checkGlError("bloom");
#endif

#if 0
    shaderManager["ssr"]->use();
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 , GL_TEXTURE_2D, textureManager["fx3"], 0);
    glClear(GL_COLOR_BUFFER_BIT);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureManager["gBufferColor"]);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, textureManager["gBufferNormals"]);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, textureManager["gBufferDepth"]);
    glUniform1i(glGetUniformLocation(shaderManager["ssr"]->getProgram(), "ColorBuffer"), 0);
    glUniform1i(glGetUniformLocation(shaderManager["ssr"]->getProgram(), "NormalBuffer"), 1);
    glUniform1i(glGetUniformLocation(shaderManager["ssr"]->getProgram(), "DepthBuffer"), 2);
    
    glUniformMatrix4fv(glGetUniformLocation(shaderManager["ssr"]->getProgram(), "proj"), 1, GL_FALSE, glm::value_ptr(projection));
    glUniformMatrix4fv(glGetUniformLocation(shaderManager["ssr"]->getProgram(), "InverseProjection"), 1, GL_FALSE, glm::value_ptr(inverseProjection));
    glBindVertexArray(vaoManager["quad"]);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0);
    shaderManager["ssr"]->unuse();
#endif

    
#if 1
        shaderManager["lightShaft"]->use();
        glBindFramebuffer( GL_FRAMEBUFFER, fboManager["fx"]);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 , GL_TEXTURE_2D, textureManager["fx2"], 0);
        glClear(GL_COLOR_BUFFER_BIT);
        //NOTE(marc) : only one texture, so next lines are not necessary
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureManager["fx4"]);
        glUniform1i(glGetUniformLocation(shaderManager["lightShaft"]->getProgram(), "Texture"), 0);
        glUniform2fv(glGetUniformLocation(shaderManager["lightShaft"]->getProgram(), "ScreenLightPos"), 1, &sunNDC[0]); 
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0);
        shaderManager["lightShaft"]->unuse();

        glEnable(GL_BLEND);
        glBlendFunc(GL_ONE, GL_ONE);
        shaderManager["blit"]->use();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureManager["fx1"]);
        glBindVertexArray(vaoManager["quad"]);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0);
        shaderManager["blit"]->unuse();
        glDisable(GL_BLEND);
#endif
      
    //Draw final frame on screen
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClear(GL_COLOR_BUFFER_BIT);
    glBindVertexArray(vaoManager["quad"]);
    glViewport(0,0, screenWidth, screenHeight);
    shaderManager["blitHDR"]->use();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture( GL_TEXTURE_2D, textureManager["fx2"]);
    glDrawElements(GL_TRIANGLES, 2 * 3, GL_UNSIGNED_INT, (void*)0);
    shaderManager["blitHDR"]->unuse();
    Utils::checkGlError("blit");

#if 0        
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    shaderManager["blit"]->use();
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(vaoManager["quad"]);
    // Viewport 
    glViewport( 0, 0, screenWidth/4, screenHeight/4  );
    // Bind texture
    glBindTexture(GL_TEXTURE_2D, textureManager["gBufferColor"]);
    // Draw quad
    glDrawElements(GL_TRIANGLES, 2 * 3, GL_UNSIGNED_INT, (void*)0);
    // Viewport 
    glViewport( screenWidth/4, 0, screenWidth/4, screenHeight/4  );
    // Bind texture
    glBindTexture(GL_TEXTURE_2D, textureManager["gBufferNormals"]);
    // Draw quad
    glDrawElements(GL_TRIANGLES, 2 * 3, GL_UNSIGNED_INT, (void*)0);
    shaderManager["depth"]->use();
    // Viewport 
    glViewport( screenWidth/4 * 2, 0, screenWidth/4, screenHeight/4  );
    // Bind texture
    glBindTexture(GL_TEXTURE_2D, textureManager["gBufferDepth"]);
    // Draw quad
    glDrawElements(GL_TRIANGLES, 2 * 3, GL_UNSIGNED_INT, (void*)0);
    shaderManager["depth"]->unuse();
    shaderManager["blit"]->use();
    // Viewport 
    glViewport( screenWidth/4 * 3, 0, screenWidth/4, screenHeight/4  );
    // Bind texture
    glBindTexture(GL_TEXTURE_2D, textureManager["fx3"]);
    // Draw quad
    glDrawElements(GL_TRIANGLES, 2 * 3, GL_UNSIGNED_INT, (void*)0);
    shaderManager["blit"]->unuse();
    #endif
}
#endif

void Game::loadShaders()
{
    ShaderManager& shaderManager = Manager<ShaderManager>::instance();
    //DEBUG SHADERS
    Shader* blitShader = new Shader("Blit shader");
    blitShader->attach(GL_VERTEX_SHADER, "assets/shaders/blit.vert");
    blitShader->attach(GL_FRAGMENT_SHADER, "assets/shaders/blit.frag");
    blitShader->link();

	Shader* InvertBlitShader = new Shader("Invert Blit shader");
	InvertBlitShader->attach(GL_VERTEX_SHADER, "assets/shaders/PBR/Invert.vert");
	InvertBlitShader->attach(GL_FRAGMENT_SHADER, "assets/shaders/PBR/Invert.frag");
	InvertBlitShader->link();

    GLuint blitVertShader = (*blitShader)[0];
    Shader* blitHDRShader = new Shader("HDR blit shader");
    blitHDRShader->attach(blitVertShader);
    blitHDRShader->attach(GL_FRAGMENT_SHADER, "assets/shaders/blitHDR.frag");
    blitHDRShader->link();
    Shader* blitDepth = new Shader("Depth blit");
    blitDepth->attach(blitVertShader);
    blitDepth->attach(GL_FRAGMENT_SHADER, "assets/shaders/blitDepth.frag");
    blitDepth->link();
    //RENDERING SHADERS
    Shader* gbuffer = new Shader("G-buffer");
    gbuffer->attach(GL_VERTEX_SHADER, "assets/shaders/gbuffer.vert");
    gbuffer->attach(GL_FRAGMENT_SHADER, "assets/shaders/gbuffer.frag");
    gbuffer->link();
	Shader* explode = new Shader("Explode");
	explode->attach(GL_VERTEX_SHADER, "assets/shaders/explode.vert");
	explode->attach(GL_TESS_CONTROL_SHADER, "assets/shaders/explode.tcs");
	explode->attach(GL_TESS_EVALUATION_SHADER, "assets/shaders/explode.tes");
	explode->attach(GL_GEOMETRY_SHADER, "assets/shaders/explode.geom");
	explode->attach(GL_FRAGMENT_SHADER, "assets/shaders/explode.frag");
	explode->link();
	Shader* terrain = new Shader("terrain");
	terrain->attach(GL_VERTEX_SHADER, "assets/shaders/terrain.vert");
	terrain->attach(GL_TESS_CONTROL_SHADER, "assets/shaders/terrain.tcs");
	terrain->attach(GL_TESS_EVALUATION_SHADER, "assets/shaders/terrain.tes");
	terrain->attach(GL_FRAGMENT_SHADER, "assets/shaders/terrain.frag");
	terrain->link();
    Shader* shadowShader = new Shader( "Shadows" );
    shadowShader->attach( (*gbuffer)[0] );
    shadowShader->attach(GL_FRAGMENT_SHADER, "assets/shaders/shadow.frag");
    shadowShader->link();
    Shader* plShader = new Shader("Point light");
    plShader->attach(blitVertShader);
    plShader->attach(GL_FRAGMENT_SHADER, "assets/shaders/pointlight.frag");
    plShader->link();
	Shader* newPLShader = new Shader("Point light");
	newPLShader->attach(blitVertShader);
	newPLShader->attach(GL_FRAGMENT_SHADER, "assets/shaders/PBR/pointlight.frag");
	newPLShader->link();
    Shader* slShader = new Shader("Spot light");
    slShader->attach(blitVertShader);
    slShader->attach(GL_FRAGMENT_SHADER, "assets/shaders/spotlight.frag");
    slShader->link();
    Shader* dlShader = new Shader("Directional light");
    dlShader->attach(blitVertShader);
    dlShader->attach(GL_FRAGMENT_SHADER, "assets/shaders/directionallight.frag");
    dlShader->link();
    Shader* plShadowShader = new Shader("piint light shadow");
    plShadowShader->attach(GL_VERTEX_SHADER, "assets/shaders/cubemap.vert");
    plShadowShader->attach(GL_GEOMETRY_SHADER, "assets/shaders/cubemap.geom");
    plShadowShader->attach(GL_FRAGMENT_SHADER, "assets/shaders/cubemap.frag");
    plShadowShader->link();
    //POSTFX SHADERS
    Shader* blurShader = new Shader("Blur shader");
    blurShader->attach(GL_VERTEX_SHADER, "assets/shaders/blur.vert");
    blurShader->attach(GL_FRAGMENT_SHADER, "assets/shaders/blur.frag");
    blurShader->link();
    Shader* brightShader = new Shader("Bright shader");
    brightShader->attach(blitVertShader);
    brightShader->attach(GL_FRAGMENT_SHADER, "assets/shaders/extractBright.frag");
    brightShader->link();
    Shader* bloomShader = new Shader("Bloom shader");
    bloomShader->attach(blitVertShader);
    bloomShader->attach(GL_FRAGMENT_SHADER, "assets/shaders/bloom.frag");
    bloomShader->link();
	Shader* terrainNormalShader = new Shader("Terrain normal");
	terrainNormalShader->attach(blitVertShader);
	terrainNormalShader->attach(GL_FRAGMENT_SHADER, "assets/shaders/terrainNormal.frag");
	terrainNormalShader->link();
    Shader* lightShaftShader = new Shader("Light shaft");
    lightShaftShader->attach(blitVertShader);
    lightShaftShader->attach(GL_FRAGMENT_SHADER, "assets/shaders/lightShaft.frag");
    lightShaftShader->link();
    Shader* sunShader = new Shader("Sun shader");
    sunShader->attach(blitVertShader);
    sunShader->attach(GL_FRAGMENT_SHADER, "assets/shaders/sun.frag");
    sunShader->link();
    Shader* ssrShader = new Shader("SSR shader");
    ssrShader->attach(blitVertShader);
    ssrShader->attach(GL_FRAGMENT_SHADER, "assets/shaders/SSRR.frag");
    ssrShader->link();

	Shader* FXAA = new Shader("FXAA");
	FXAA->attach(blitVertShader);
	FXAA->attach(GL_FRAGMENT_SHADER, "assets/shaders/PBR/FXAA.frag");
	FXAA->link();

	Shader* IntegrateBRDFShader = new Shader("IntegrateBRDF");
	IntegrateBRDFShader->attach(blitVertShader);
	IntegrateBRDFShader->attach(GL_FRAGMENT_SHADER, "assets/shaders/PBR/IntegrateBRDF.frag");
	IntegrateBRDFShader->link();

	Shader* ShadingFinal = new Shader("ShadingFinal");
	ShadingFinal->attach(blitVertShader);
	ShadingFinal->attach(GL_FRAGMENT_SHADER, "assets/shaders/PBR/ShadingFinal.frag");
	ShadingFinal->link();

	Shader* PreEnvFilterShader = new Shader("PreEnvFilkter");
	PreEnvFilterShader->attach(GL_VERTEX_SHADER, "assets/shaders/PBR/PreEnvFilter.vert");
	PreEnvFilterShader->attach(GL_GEOMETRY_SHADER, "assets/shaders/PBR/PreEnvFilter.geom");
	PreEnvFilterShader->attach(GL_FRAGMENT_SHADER, "assets/shaders/PBR/PreEnvFilter.frag");
	PreEnvFilterShader->link();

	Shader* CubeMapBlit = new Shader("CubeMapBlit");
	CubeMapBlit->attach(GL_VERTEX_SHADER, "assets/shaders/PBR/CubeMapBlit.vert");
	CubeMapBlit->attach(GL_FRAGMENT_SHADER, "assets/shaders/PBR/CubeMapBlit.frag");
	CubeMapBlit->link();


	shaderManager.getManaged().insert(pair<string, Shader*>("FXAA", FXAA));
	shaderManager.getManaged().insert(pair<string, Shader*>("InvertBlitShader", InvertBlitShader));
	shaderManager.getManaged().insert(pair<string, Shader*>("ShadingFinal", ShadingFinal));
	shaderManager.getManaged().insert(pair<string, Shader*>("CubeMapBlit", CubeMapBlit));
	shaderManager.getManaged().insert(pair<string, Shader*>("PreEnvFilter", PreEnvFilterShader));
	shaderManager.getManaged().insert(pair<string, Shader*>("IntegrateBRDF", IntegrateBRDFShader));
    shaderManager.getManaged().insert( pair<string, Shader*>( "dirLight", dlShader));
    shaderManager.getManaged().insert( pair<string, Shader*>( "ssr", ssrShader));
    shaderManager.getManaged().insert( pair<string, Shader*>( "sun", sunShader));
    shaderManager.getManaged().insert( pair<string, Shader*>( "lightShaft", lightShaftShader));
    shaderManager.getManaged().insert( pair<string, Shader*>( "plShadow", plShadowShader));
    shaderManager.getManaged().insert( pair<string, Shader*>( "depth", blitDepth));
    shaderManager.getManaged().insert( pair<string, Shader*>( "shadow", shadowShader));
    shaderManager.getManaged().insert( pair<string, Shader*>( "pointLight", plShader));
	shaderManager.getManaged().insert(pair<string, Shader*>("PBRpointLight", newPLShader));
    shaderManager.getManaged().insert( pair<string, Shader*>( "spotLight", slShader));
    shaderManager.getManaged().insert( pair<string, Shader*>( "blur", blurShader));
    shaderManager.getManaged().insert( pair<string, Shader*>( "bloom", bloomShader));
    shaderManager.getManaged().insert( pair<string, Shader*>( "bright", brightShader));
    shaderManager.getManaged().insert( pair<string, Shader*>( "gbuffer", gbuffer));
	shaderManager.getManaged().insert(pair<string, Shader*>("explode", explode));
    shaderManager.getManaged().insert( pair<string, Shader*>( "blit", blitShader));
    shaderManager.getManaged().insert( pair<string, Shader*>( "blitHDR", blitHDRShader));
	shaderManager.getManaged().insert(pair<string, Shader*>("terrain", terrain));
	shaderManager.getManaged().insert(pair<string, Shader*>("terrainNormal", terrainNormalShader));

    bloomShader->unuse();
}

void Game::loadGeometry()
{
    VaoManager& vaoManager = Manager<VaoManager>::instance();
	ModelManager& modelManager = Manager<ModelManager>::instance();
    // Load geometry
    int cube_triangleCount = 12;
    int cube_triangleList[] = {0, 1, 2, 2, 1, 3, 4, 5, 6, 6, 5, 7, 8, 9, 10, 10, 9, 11, 12, 13, 14, 14, 13, 15, 16, 17, 18, 19, 17, 20, 21, 22, 23, 24, 25, 26, };
    float cube_uvs[] = {0.f, 0.f, 0.f, 1.f, 1.f, 0.f, 1.f, 1.f, 0.f, 0.f, 0.f, 1.f, 1.f, 0.f, 1.f, 1.f, 0.f, 0.f, 0.f, 1.f, 1.f, 0.f, 1.f, 1.f, 0.f, 0.f, 0.f, 1.f, 1.f, 0.f, 1.f, 1.f, 0.f, 0.f, 0.f, 1.f, 1.f, 0.f,  1.f, 0.f,  1.f, 1.f,  0.f, 1.f,  1.f, 1.f,  0.f, 0.f, 0.f, 0.f, 1.f, 1.f,  1.f, 0.f,  };
    float cube_vertices[] = {-0.5, -0.5, 0.5, 0.5, -0.5, 0.5, -0.5, 0.5, 0.5, 0.5, 0.5, 0.5, -0.5, 0.5, 0.5, 0.5, 0.5, 0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, -0.5, -0.5, 0.5, -0.5, -0.5, -0.5, -0.5, -0.5, 0.5, -0.5, -0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, -0.5, -0.5, -0.5, -0.5, -0.5, -0.5, 0.5, -0.5, 0.5, -0.5, -0.5, 0.5, -0.5, -0.5, -0.5, 0.5, -0.5, 0.5, 0.5 };
    float cube_normals[] = {0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 0, -1, 0, 0, -1, 0, 0, -1, 0, 0, -1, 0, -1, 0, 0, -1, 0, 0, -1, 0, 0, -1, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, -1, 0, 0, -1, 0, 0, -1, 0, 0, -1, 0, 0, -1, 0, 0, -1, 0, 0, };
    int plane_triangleCount = 2;
    int plane_triangleList[] = {0, 1, 2, 2, 1, 3}; 
    float plane_uvs[] = {0.f, 0.f, 0.f, 50.f, 50.f, 0.f, 50.f, 50.f};
    float plane_vertices[] = {-50.0, -1.0, 50.0, 50.0, -1.0, 50.0, -50.0, -1.0, -50.0, 50.0, -1.0, -50.0};
    float plane_normals[] = {0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0};
    int   quad_triangleCount = 2;
    int   quad_triangleList[] = {0, 1, 2, 2, 1, 3}; 
    float quad_vertices[] =  {-1.0, -1.0, 1.0, -1.0, -1.0, 1.0, 1.0, 1.0};
	float inverted_quad_vertices[] = { -1.0, 1.0, 1.0, 1.0, -1.0, -1.0, 1.0, -1.0 };
    // Vertex Array Object
    GLuint vao[4];
    glGenVertexArrays(4, vao);
    // Vertex Buffer Objects
    GLuint vbo[12];
    glGenBuffers(12, vbo);
    // Cube
    glBindVertexArray(vao[0]);
    // Bind indices and upload data
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[0]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_triangleList), cube_triangleList, GL_STATIC_DRAW);
    // Bind vertices and upload data
    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT)*3, (void*)0);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);
    // Bind normals and upload data
    glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT)*3, (void*)0);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_normals), cube_normals, GL_STATIC_DRAW);
    // Bind uv coords and upload data
    glBindBuffer(GL_ARRAY_BUFFER, vbo[3]);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT)*2, (void*)0);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_uvs), cube_uvs, GL_STATIC_DRAW);
    // Plane
    glBindVertexArray(vao[1]);
    // Bind indices and upload data
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[4]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(plane_triangleList), plane_triangleList, GL_STATIC_DRAW);
    // Bind vertices and upload data
    glBindBuffer(GL_ARRAY_BUFFER, vbo[5]);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT)*3, (void*)0);
    glBufferData(GL_ARRAY_BUFFER, sizeof(plane_vertices), plane_vertices, GL_STATIC_DRAW);
    // Bind normals and upload data
    glBindBuffer(GL_ARRAY_BUFFER, vbo[6]);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT)*3, (void*)0);
    glBufferData(GL_ARRAY_BUFFER, sizeof(plane_normals), plane_normals, GL_STATIC_DRAW);
    // Bind uv coords and upload data
    glBindBuffer(GL_ARRAY_BUFFER, vbo[7]);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT)*2, (void*)0);
    glBufferData(GL_ARRAY_BUFFER, sizeof(plane_uvs), plane_uvs, GL_STATIC_DRAW);
    // Quad
    glBindVertexArray(vao[2]);
    // Bind indices and upload data
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[8]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(quad_triangleList), quad_triangleList, GL_STATIC_DRAW);
    // Bind vertices and upload data
    glBindBuffer(GL_ARRAY_BUFFER, vbo[9]);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT)*2, (void*)0);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quad_vertices), quad_vertices, GL_STATIC_DRAW);
	// Inverted Quad to render upside down textures
	glBindVertexArray(vao[3]);
	// Bind indices and upload data
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[10]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(quad_triangleList), quad_triangleList, GL_STATIC_DRAW);
	// Bind vertices and upload data
	glBindBuffer(GL_ARRAY_BUFFER, vbo[11]);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT) * 2, (void*)0);
	glBufferData(GL_ARRAY_BUFFER, sizeof(inverted_quad_vertices), inverted_quad_vertices, GL_STATIC_DRAW);

	GLuint terrainVAO;
	glGenVertexArrays(1, &terrainVAO);
	glBindVertexArray(terrainVAO);

    // Unbind everything. Potentially illegal on some implementations
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    vaoManager.getManaged().insert( pair<string, VAO>("cube", {vao[0]}));
    vaoManager.getManaged().insert( pair<string, VAO>("plane", {vao[1]}));
    vaoManager.getManaged().insert( pair<string, VAO>("quad", {vao[2]}));  
	vaoManager.getManaged().insert(pair<string, VAO>("InvertedQuad", { vao[3] }));
	vaoManager.getManaged().insert(pair<string, VAO>("terrain", { terrainVAO }));
	Utils::checkGlError("terrainNormal0");
	Model* sphere = new Model("assets/sphere.nff");
	modelManager.getManaged().insert(pair<string, Model*>("sphere", sphere));

#if 1
	Model* tree = new Model("assets/models/palm/Tree.FBX");
	modelManager.getManaged().insert(pair<string, Model*>("tree", tree));
#endif
	Utils::checkGlError("terrainNormal0");
}

void CreateTexture(GLuint Texture, int SizedInternalType, int BaseInternalFormat, int ComponentType, int X, int Y)
{
	glBindTexture(GL_TEXTURE_2D, Texture);
	glTexImage2D(GL_TEXTURE_2D, 0, SizedInternalType, X, Y, 0, BaseInternalFormat, ComponentType, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}

void Game::initGbuffer()
{
    TextureManager& textureManager = Manager<TextureManager>::instance();    
    FboManager& fboManager = Manager<FboManager>::instance();
    // Init frame buffers
	const int NumberOfOutputTextures = 3;
	const int NumberOfDrawBuffers = 2;
    GLuint gbufferFbo;
	GLuint gbufferTextures[NumberOfOutputTextures];
	GLuint gbufferDrawBuffers[NumberOfDrawBuffers];
	glGenTextures(NumberOfOutputTextures, gbufferTextures);

	for (int i = 0; i < NumberOfOutputTextures-1; ++i)
		CreateTexture(gbufferTextures[i], GL_RGBA32F, GL_RGBA, GL_FLOAT, screenWidth, screenHeight);
    // Create depth texture
	CreateTexture(gbufferTextures[NumberOfOutputTextures-1], GL_DEPTH_COMPONENT24, GL_DEPTH_COMPONENT, GL_FLOAT, screenWidth, screenHeight);
    // Create Framebuffer Object
    glGenFramebuffers(1, &gbufferFbo);
    glBindFramebuffer(GL_FRAMEBUFFER, gbufferFbo);
	for (int i = 0; i < NumberOfDrawBuffers ; ++i)
        gbufferDrawBuffers[i] = GL_COLOR_ATTACHMENT0 + i;
	glDrawBuffers(NumberOfDrawBuffers, gbufferDrawBuffers);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, gbufferTextures[NumberOfOutputTextures-1], 0);
    // Attach textures to framebuffer
	for (int i = 0; i < NumberOfDrawBuffers; ++i)
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i , GL_TEXTURE_2D, gbufferTextures[i], 0);

    if ( glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE )
        std::cout << "Error Gbuffer fbo" << std::endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0 );

	for (int i = 0; i < NumberOfDrawBuffers; ++i)
        textureManager.getManaged().insert( pair<string, Tex>( "GBufferOutput" + std::to_string(i), {gbufferTextures[i]}));

    textureManager.getManaged().insert( pair<string, Tex>( "GBufferDepth", {gbufferTextures[2]}));
    fboManager.getManaged().insert( pair<string, FBO>( "GBuffer", {gbufferFbo}));
}

void Game::initFX()
{
    TextureManager& textureManager = Manager<TextureManager>::instance();    
    FboManager& fboManager = Manager<FboManager>::instance();

    GLuint fxFbo;
    GLuint fxDrawBuffers[1];
    glGenFramebuffers(1, &fxFbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fxFbo);
    fxDrawBuffers[0] = GL_COLOR_ATTACHMENT0;
    glDrawBuffers(1, fxDrawBuffers);
    // Create Fx textures
    const int FX_TEXTURE_COUNT = 6;
    GLuint fxTextures[FX_TEXTURE_COUNT];
    glGenTextures(FX_TEXTURE_COUNT, fxTextures);
    for (int i = 0; i < FX_TEXTURE_COUNT; ++i)
    {
		CreateTexture(fxTextures[i], GL_RGBA16F, GL_RGBA, GL_FLOAT, screenWidth, screenHeight);
		textureManager.getManaged().insert(pair<string, Tex>("fx" + to_string(i), { fxTextures[i] }));
    }
    // Attach first fx texture to framebuffer
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 , GL_TEXTURE_2D, fxTextures[0], 0);
    if ( glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE )
        std::cout << "Error FX fbo" << std::endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);

    fboManager.getManaged().insert( pair<string, FBO>( "fx", {fxFbo}));
}

void Game::initShadows()
{
    TextureManager& textureManager = Manager<TextureManager>::instance();    
    FboManager& fboManager = Manager<FboManager>::instance();

    const int TEX_SIZE = 1024;
	const int BIG_TEX_SIZE = 4096;
    const int LIGHT_COUNT = 10;
    // Create shadow FBO
    GLuint shadowFbo;
    glGenFramebuffers(1, &shadowFbo);
    glBindFramebuffer(GL_FRAMEBUFFER, shadowFbo);
    // Create shadow textures
    GLuint shadowTextures[LIGHT_COUNT];
    glGenTextures(LIGHT_COUNT, shadowTextures);
    for (int i = 0; i < LIGHT_COUNT; ++i) 
    {

        glBindTexture(GL_TEXTURE_2D, shadowTextures[i]);
		if (i > LIGHT_COUNT / 2)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, BIG_TEX_SIZE, BIG_TEX_SIZE, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
		}
		else
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, TEX_SIZE, TEX_SIZE, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
		}
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE,  GL_COMPARE_REF_TO_TEXTURE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC,  GL_LEQUAL);
        textureManager.getManaged().insert( pair<string, Tex>( "shadow"+to_string(i), {shadowTextures[i]}));
    }
    fboManager.getManaged().insert(pair<string, FBO>( "shadow", {shadowFbo}));
    // Create a render buffer since we don't need to read shadow color 
    // in a texture
    GLuint shadowRenderBuffer[2];
    glGenRenderbuffers(2, shadowRenderBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, shadowRenderBuffer[1]);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_RGB, BIG_TEX_SIZE, BIG_TEX_SIZE);
	glBindRenderbuffer(GL_RENDERBUFFER, shadowRenderBuffer[0]);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_RGB, TEX_SIZE, TEX_SIZE);
    // Attach the first texture to the depth attachment
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadowTextures[0], 0);
    // Attach the renderbuffer
    glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, shadowRenderBuffer[0]);
    if ( glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE )
        std::cout << "Error shadow fbo" << std::endl;
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
	fboManager.getManaged().insert(pair<string, FBO>("shadowRBO", { shadowRenderBuffer[0] }));
	fboManager.getManaged().insert(pair<string, FBO>("shadowBigRBO", { shadowRenderBuffer[1] }));

    //Omnidirectional Shadow
	GLuint plShadowFBO;
	glGenFramebuffers(1, &plShadowFBO);
	for (int i = 0; i < 10; ++i)
	{
		GLuint plShadowTex;
		glGenTextures(1, &plShadowTex);
		glBindTexture(GL_TEXTURE_CUBE_MAP, plShadowTex);
		for (GLuint i = 0; i < 6; ++i)
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, TEX_SIZE, TEX_SIZE, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_BASE_LEVEL, 0);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAX_LEVEL, 0);
		textureManager.getManaged().insert(pair<string, Tex>("plShadow" + to_string(i), { plShadowTex }));
		glBindFramebuffer(GL_FRAMEBUFFER, plShadowFBO);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, plShadowTex, 0);
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			std::cout << "Error shadow fbo" << std::endl;
		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
	fboManager.getManaged().insert(pair<string, FBO>("plShadow" , { plShadowFBO }));


}

//Load models into the singleton modelmanager
void Game::loadAssets()
{

    std::cout << std::endl << "---- LOAD ASSETS AND CREATE FBOs ---" << std::endl << std::endl;
    //Manager<Model*>& modelManager = Manager<Model*>::getInstance();
    TextureManager& textureManager = Manager<TextureManager>::instance();    
    FboManager& fboManager = Manager<FboManager>::instance();
	ShaderManager& shaderManager = Manager<ShaderManager>::instance();
	VaoManager& vaoManager = Manager<VaoManager>::instance();

    loadShaders();
    loadGeometry();
    initGbuffer();
    initFX();
    initShadows();

    // Load images and upload textures
	const int nb_tex = 6;
    GLuint textures[nb_tex];
    glGenTextures(nb_tex, textures);
    int x;
    int y;
    int comp;

    unsigned char * diffuse = stbi_load("assets/textures/spnza_bricks_a_diff.tga", &x, &y, &comp, 3);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textures[0]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB8, x, y, 0, GL_RGB, GL_UNSIGNED_BYTE, diffuse);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glGenerateMipmap(GL_TEXTURE_2D);
    fprintf(stderr, "Diffuse %dx%d:%d\n", x, y, comp);
    glBindTexture(GL_TEXTURE_2D, 0);

    unsigned char * spec = stbi_load("assets/textures/spnza_bricks_a_spec.tga", &x, &y, &comp, 1);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, textures[1]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, x, y, 0, GL_RED, GL_UNSIGNED_BYTE, spec);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glGenerateMipmap(GL_TEXTURE_2D);
    fprintf(stderr, "Spec %dx%d:%d\n", x, y, comp);
    glBindTexture(GL_TEXTURE_2D, 0);

	unsigned char * movingCubeTex = stbi_load("assets/textures/movingCubeTex.bmp", &x, &y, &comp, 3);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textures[2]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB8, x, y, 0, GL_RGB, GL_UNSIGNED_BYTE, movingCubeTex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);
	fprintf(stderr, "Diffuse %dx%d:%d\n", x, y, comp);
	glBindTexture(GL_TEXTURE_2D, 0);

	unsigned char * RoomTex = stbi_load("assets/textures/RoomTex.bmp", &x, &y, &comp, 3);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textures[3]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB8, x, y, 0, GL_RGB, GL_UNSIGNED_BYTE, RoomTex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);
	fprintf(stderr, "Diffuse %dx%d:%d\n", x, y, comp);
	glBindTexture(GL_TEXTURE_2D, 0);

	unsigned char * FinalTex = stbi_load("assets/textures/final.png", &x, &y, &comp, 3);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textures[5]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB8, x, y, 0, GL_RGB, GL_UNSIGNED_BYTE, FinalTex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);
	fprintf(stderr, "Diffuse %dx%d:%d\n", x, y, comp);
	glBindTexture(GL_TEXTURE_2D, 0);
	
	unsigned char * heightMap = stbi_load("assets/textures/HM_test.jpg", &x, &y, &comp, 3);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textures[4]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB8, x, y, 0, GL_RGB, GL_UNSIGNED_BYTE, heightMap);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);
	fprintf(stderr, "Diffuse %dx%d:%d\n", x, y, comp);
	glBindTexture(GL_TEXTURE_2D, 0);
	textureManager.getManaged().insert(pair<string, Tex>("heightMap", { textures[4] }));

	
	//Compute normal maps from heightMap
	GLuint normalTex;
	glGenTextures(1, &normalTex);
	glBindTexture(GL_TEXTURE_2D, normalTex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, screenWidth, screenHeight, 0, GL_RGBA, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	textureManager.getManaged().insert(pair<string, Tex>("terrainNormal", { normalTex }));

	shaderManager["terrainNormal"]->use();
	glBindVertexArray(vaoManager["quad"]);
	glBindFramebuffer(GL_FRAMEBUFFER, fboManager["fx"]);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureManager["terrainNormal"], 0);
	glClear(GL_COLOR_BUFFER_BIT);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureManager["heightMap"]);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0);
	shaderManager["terrainNormal"]->unuse();
	Utils::checkGlError("terrainNormal");

	int amount = 20.f;
	GLboolean horizontal = true, first_iteration = true;
	shaderManager["blur"]->use();
	for (GLuint i = 0; i < amount; i++)
	{
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureManager["fx" + to_string(2 + horizontal)], 0);
		glUniform1i(glGetUniformLocation(shaderManager["blur"]->getProgram(), "horizontal"), horizontal);
		glBindTexture(
			GL_TEXTURE_2D, first_iteration ? textureManager["terrainNormal"] : textureManager["fx" + to_string(2 + !horizontal)]
			);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0);
		horizontal = !horizontal;
		if (first_iteration)
			first_iteration = false;
	}
	shaderManager["blur"]->unuse();
	shaderManager["blit"]->use();
	glBindFramebuffer(GL_FRAMEBUFFER, fboManager["fx"]);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureManager["terrainNormal"], 0);
	glClear(GL_COLOR_BUFFER_BIT);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureManager["fx3"]);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0);
	shaderManager["blit"]->unuse();



    textureManager.getManaged().insert( pair<string, Tex>( "brick_diff", {textures[0]}));
    textureManager.getManaged().insert( pair<string, Tex>( "brick_spec", {textures[1]}));
	textureManager.getManaged().insert(pair<string, Tex>("movingCubeTex", { textures[2] }));
	textureManager.getManaged().insert(pair<string, Tex>("RoomTex", { textures[3] }));
	textureManager.getManaged().insert(pair<string, Tex>("FinalTex", { textures[5] }));
	

    //create UBO
    // Update and bind uniform buffer object
    GLuint ubo[1];
    glGenBuffers(1, ubo);
    glBindBuffer(GL_UNIFORM_BUFFER, ubo[0]);
    // Ignore ubo size, allocate it sufficiently big for all light data structures
    GLint uboSize = 512;
    glBufferData(GL_UNIFORM_BUFFER, uboSize, 0, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    fboManager.getManaged().insert( pair<string, FBO>("ubo", {ubo[0]}));

    Utils::checkGlError("LOAD ASSETS");
    //manager.getModels().insert( pair<string, Model*>( "scalp", new Model( "../Assets/Models/Hair/scalp_mesh.obj" )));
}

enum Faces { XPos = 0, XNeg, YPos, YNeg, ZPos, ZNeg };


struct dock
{
	ImVec2 Size;
	ImVec2 Position;

};


glm::vec4 EDITORViewBoudingBox;

bool Game::Is2DPointInRectangle( glm::vec2 Point, glm::vec4 Rectangle )
{
	return (Point.x >= Rectangle.x && Point.x <= Rectangle.z && Point.y >= Rectangle.y && Point.y <= Rectangle.w);
}

bool Game::EditorViewToScreenCoordinates(glm::vec4 ViewBoundingBox, glm::vec2 MousePosition, glm::vec2& Result)
{
	if (Is2DPointInRectangle(MousePosition, ViewBoundingBox))
	{
		auto RatioX = screenWidth / (ViewBoundingBox.z - ViewBoundingBox.x);
		auto RatioY = screenHeight / (ViewBoundingBox.w - ViewBoundingBox.y);
		glm::vec2 UpperLeftCorner = glm::vec2(ViewBoundingBox.x, ViewBoundingBox.y);
		glm::vec2 V = MousePosition - UpperLeftCorner;
		Result = glm::vec2(RatioX * V.x, RatioY*V.y);
		return true;
	}
	return false;
}

Player* p;
bvh* SceneTree;
int CurrentSelectedObjectID;


void Game::EDITORMouseOnClick()
{
	double mouse_x, mouse_y;
	glfwGetCursorPos(window, &mouse_x, &mouse_y);
	glm::vec2 MousePos = glm::vec2(mouse_x, mouse_y);
	glm::vec2 Result;
	if (EditorViewToScreenCoordinates( EDITORViewBoudingBox, MousePos, Result ))
	{
		glm::mat4 projection = glm::perspective(p->getCamera()->getZoom(),
			(float)screenWidth / (float)screenHeight,
			0.1f, 1000.0f);
		ray R;
		glm::mat4 mat = glm::inverse(projection * p->getCamera()->getViewMatrix());
		glm::vec4 screenPos = glm::vec4(Result.x, -Result.y, 1.0f, 1.0f);
		glm::vec4 worldPos = mat * screenPos;

		R.Origin = glm::unProject(glm::vec3(Result, 0.f), p->getCamera()->getViewMatrix(), projection, glm::vec4(0, 0, screenWidth, screenHeight));

		auto Test = glm::unProject(glm::vec3(Result, 1.f), p->getCamera()->getViewMatrix(), projection, glm::vec4(0, 0, screenWidth, screenHeight));
		auto Vec = glm::normalize(Test - R.Origin);
		//R.Direction = glm::normalize(glm::vec3(worldPos));
		R.Direction = Vec;
		
		
		//R.Origin = glm::vec3(0,0,-200);
		//R.Direction = glm::vec3(1, 0, 0);
		R.T = 1000000;
		R.InverseDirection = 1.f / R.Direction;

		if( SceneTree->GetIntersection(R) )
		    std::cout << "HIT OBJECT " << R.ObjectID << std::endl;
		CurrentSelectedObjectID = R.ObjectID;
	}
	else
	{
		CurrentSelectedObjectID = -1;
	}
}

//Main rendering loop
int Game::mainLoop()
{

	//Get needed assets
	ShaderManager& shaderManager = Manager<ShaderManager>::instance();
	TextureManager& textureManager = Manager<TextureManager>::instance();
	VaoManager& vaoManager = Manager<VaoManager>::instance();
	FboManager& fboManager = Manager<FboManager>::instance();
	ControllerManager& cm = ControllerManager::getInstance();

	//editor controller
	Controller c;
	c.addMouseListener(std::make_pair<int, int>(GLFW_MOUSE_BUTTON_1, GLFW_PRESS), bind(&Game::EDITORMouseOnClick, this));

	cm.add(&c);
    /*
      Create all the vars that we may need for rendering such as shader, VBO, FBO, etc
      .     */
    p = new Player();
    p->setPosition(glm::vec3(0.f, 0.f, 2.f));
    
    glfwSetKeyCallback( window, key_callback);
    glfwSetCursorPosCallback(window, cursor_position_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);

    loadAssets();
    //create skybox
    vector<const GLchar*> faces;
    faces.push_back("assets/skyboxes/Test/xpos.png");
    faces.push_back("assets/skyboxes/Test/xneg.png");
    faces.push_back("assets/skyboxes/Test/ypos.png");
    faces.push_back("assets/skyboxes/Test/yneg.png");
    faces.push_back("assets/skyboxes/Test/zpos.png");
    faces.push_back("assets/skyboxes/Test/zneg.png");
    Skybox* skybox = new Skybox(faces); 
    Utils::checkGlError("skybox error");


	GLuint PreFilteredEnvTex;
	glGenTextures(1, &PreFilteredEnvTex);
	glBindTexture(GL_TEXTURE_CUBE_MAP, PreFilteredEnvTex);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_BASE_LEVEL, 0);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAX_LEVEL, 5);	
	for (int i = 0; i < 6; ++i)
	{
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 512, 512, 0, GL_RGB, GL_FLOAT, 0);
	}	
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

	Utils::checkGlError("Generate PreFilteredEnvMap");

	material Gold = {};
	Gold.BaseColor = { 1.f, 0.78f, 0.34f };
	Gold.Metalness = 1.f;
	Gold.Roughness = 0.f;

	Model Sphere("assets/models/Sphere.FBX");
	Sphere.SetMaterial(Gold);
	auto ratio = 1 / 50.f;
	glm::mat4 M = glm::scale(glm::mat4(), glm::vec3(ratio));
	glm::mat4 projection = glm::perspective(p->getCamera()->getZoom(),
		(float)screenWidth / (float)screenHeight,
		0.1f, 1000.0f);
	glm::mat4 inverseProjection = glm::inverse(projection);
    Times t;



	std::vector<std::shared_ptr<Actor> > SceneComponents;

	auto Actor0 = std::make_shared<Actor>(glm::vec3(0,0,0));
	Actor0->SetModel(Sphere);
	auto Actor1 = std::make_shared<Actor>(glm::vec3(60, 0, 0));
	Actor1->SetModel(Sphere);
	auto Actor2 = std::make_shared<Actor>(glm::vec3(-120, 0, 0));
	Actor2->SetModel(Sphere);
	auto Actor3 = std::make_shared<Actor>(glm::vec3(-180, 0, 0));
	Actor3->SetModel(Sphere);
	auto Actor4 = std::make_shared<Actor>(glm::vec3(0, 200, 0));
	Actor4->SetModel(Sphere);
	SceneComponents.push_back(Actor0);
	SceneComponents.push_back(Actor1);
	SceneComponents.push_back(Actor2);
	SceneComponents.push_back(Actor3);
	SceneComponents.push_back(Actor4);

	SceneTree = new bvh(SceneComponents);
	ray Ray;
	Ray.Origin = glm::vec3(0, 0, -10);
	Ray.Direction = glm::vec3(0, 0, 1);
	Ray.InverseDirection = 1.f / Ray.Direction; 
	Ray.T = 1000000;
	SceneTree->GetIntersection(Ray);

	//Compute pre filtered environnement maps
	glDisable(GL_CULL_FACE);
	glDisable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);
	glBindFramebuffer(GL_FRAMEBUFFER, fboManager["fx"]);
	glViewport(0, 0, screenWidth, screenHeight);
	glClear(GL_COLOR_BUFFER_BIT);

	const int MIPMAPLEVELS = 6;
	const int MIPMAPBASELEVELSIZE = 512;
	int MipMapTextureSize;
	glBindVertexArray(vaoManager["cube"]);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, skybox->texture_id);
	Shader* PreEnvFilter = shaderManager["PreEnvFilter"];
	PreEnvFilter->use();

	glm::vec3 EyePosition = glm::vec3(0.f);
	// Light space matrices
	glm::mat4 CubeMapProjection = glm::perspective(glm::radians(90.f), 1.f, 1.f, 100.f);
	std::vector<glm::mat4> worldToLight;
	worldToLight.push_back(CubeMapProjection * glm::lookAt(EyePosition, EyePosition + glm::vec3(1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)));
	worldToLight.push_back(CubeMapProjection * glm::lookAt(EyePosition, EyePosition + glm::vec3(-1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)));
	worldToLight.push_back(CubeMapProjection * glm::lookAt(EyePosition, EyePosition + glm::vec3(0.0, 1.0, 0.0), glm::vec3(0.0, 0.0, 1.0)));
	worldToLight.push_back(CubeMapProjection * glm::lookAt(EyePosition, EyePosition + glm::vec3(0.0, -1.0, 0.0), glm::vec3(0.0, 0.0, -1.0)));
	worldToLight.push_back(CubeMapProjection * glm::lookAt(EyePosition, EyePosition + glm::vec3(0.0, 0.0, 1.0), glm::vec3(0.0, -1.0, 0.0)));
	worldToLight.push_back(CubeMapProjection * glm::lookAt(EyePosition, EyePosition + glm::vec3(0.0, 0.0, -1.0), glm::vec3(0.0, -1.0, 0.0)));
	for (GLuint i = 0; i < 6; ++i)
		PreEnvFilter->SetMatrix4(("ViewMatrix[" + to_string(i) + "]").c_str(), worldToLight[i]);
	PreEnvFilter->SetInt("EnvMapSampler", 0);
	PreEnvFilter->SetMatrix4("Model", glm::mat4());
	//For each faces compute all mipmaps levels
	for (unsigned int j = 0; j < MIPMAPLEVELS; ++j)
	{
		//For each mipmap level, render the filtered environnement in it
		MipMapTextureSize = std::max(1, MIPMAPBASELEVELSIZE / (1 << j));
		glViewport(0, 0, MipMapTextureSize, MipMapTextureSize);
		float roughness = (j + 0.5f) / MIPMAPLEVELS;
		PreEnvFilter->SetFloat("Roughness", roughness);
		//Bind to the current buffer
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, PreFilteredEnvTex, j);
		glDrawElements(GL_TRIANGLES, 12 * 3, GL_UNSIGNED_INT, (void*)0);
	}

	PreEnvFilter->unuse();
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	Utils::checkGlError("REndering preenvmap");

	//Compute pre-integrated BRDF
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureManager["fx5"], 0);
	glViewport(0, 0, screenWidth, screenHeight);
	glClear(GL_COLOR_BUFFER_BIT);
	Shader* IntegrateBRDF = shaderManager["IntegrateBRDF"];
	IntegrateBRDF->use();
	glBindVertexArray(vaoManager["quad"]);
	glDrawElements(GL_TRIANGLES, 2 * 3, GL_UNSIGNED_INT, (void*)0);

    while(glfwGetKey( window, GLFW_KEY_ESCAPE ) != GLFW_PRESS )
    {
        //ImGui_ImplGlfwGL3_NewFrame();
        t.globalTime = glfwGetTime();
        t.elapsedTime = t.globalTime - t.startTime;
        t.deltaTime = t.globalTime - t.previousTime;
		t.previousTime = t.globalTime;

		p->move(t.deltaTime);

		glm::mat4 WorldToView = p->getCamera()->getViewMatrix();
		glm::mat4 mv = WorldToView * M;
		glm::mat4 mvp = projection * mv;		

		//Clean FBOs
		glEnable(GL_DEPTH_TEST);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glViewport(0, 0, screenWidth, screenHeight);

		glBindFramebuffer(GL_FRAMEBUFFER, fboManager["GBuffer"]);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		Utils::checkGlError("rendering gbuffer");
		Shader* GBuffer = shaderManager["gbuffer"];
		GBuffer->use();
		// Upload uniforms
		GBuffer->SetMatrix4("MVP", mvp);
		GBuffer->SetMatrix4("MV", mv);
		GBuffer->SetInt("ReverseNormals", 0);
		Utils::checkGlError("rendering gbuffer");
		//Render scene
		GBuffer->SetInt("Material.HasTexture", 0);
		for (auto Component : SceneComponents)
		{
			Component->Draw(GBuffer, WorldToView, projection);
		}
		//glBindVertexArray(vaoManager["cube"]);
		//glDrawElements(GL_TRIANGLES, 12 * 3, GL_UNSIGNED_INT, (void*)0);
		Utils::checkGlError("rendering gbuffer");
		shaderManager["gbuffer"]->unuse();
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glBindVertexArray(0);

		Utils::checkGlError("rendering gbuffer");

		//Render lighting in postfx fbo
		glBindFramebuffer(GL_FRAMEBUFFER, fboManager["fx"]);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureManager["fx1"], 0);

		glViewport(0, 0, screenWidth, screenHeight);
		glClear(GL_COLOR_BUFFER_BIT);
		glDisable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glBlendFunc(GL_ONE, GL_ONE);
		Utils::checkGlError("before lights");
		// Select textures
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureManager["GBufferOutput0"]);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, textureManager["GBufferOutput1"]);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, textureManager["GBufferDepth"]);
		// Bind the same VAO for all lights
		glBindVertexArray(vaoManager["quad"]);

#if 0
		// Render point lights
		const int pointLightCount = 1;
		auto PointLightShader = shaderManager["PBRpointLight"];
		PointLightShader->use();
		PointLightShader->SetMatrix4("InverseProjection", inverseProjection);
		PointLightShader->SetInt("ColorBuffer", 0);
		PointLightShader->SetInt("NormalAndMaterialsBuffer", 1);
		PointLightShader->SetInt("DepthBuffer", 2);
		PointLightShader->SetInt("Shadow", 3);

		glActiveTexture(GL_TEXTURE3);
		for (int i = 0; i < pointLightCount; ++i)
		{
			glBindTexture(GL_TEXTURE_CUBE_MAP, textureManager["plShadow"]);
			glBindBuffer(GL_UNIFORM_BUFFER, fboManager["ubo"]);
			PointLight p = {
				glm::vec3(WorldToView * glm::vec4(0.f, 0.f, -5.f, 1.0)),
				0,
				glm::vec3(1.0, 1.0, 1.0),
				10.0
			};
			PointLight * pointLightBuffer = (PointLight *)glMapBufferRange(GL_UNIFORM_BUFFER, 0, 512, GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);
			*pointLightBuffer = p;
			glUnmapBuffer(GL_UNIFORM_BUFFER);
			glBindBufferBase(GL_UNIFORM_BUFFER, glGetUniformBlockIndex(PointLightShader->getProgram(), "light"), fboManager["ubo"]);
			glDrawElements(GL_TRIANGLES, 2 * 3, GL_UNSIGNED_INT, (void*)0);
		}
		PointLightShader->unuse();
#endif


		auto ShadingShader = shaderManager["ShadingFinal"];
		ShadingShader->use();
		ShadingShader->SetInt("ColorBuffer", 0);
		ShadingShader->SetInt("NormalAndMaterialsBuffer", 1);
		ShadingShader->SetInt("DepthBuffer", 2);
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, textureManager["fx5"]);
		ShadingShader->SetInt("PreIntegratedBRDF", 3);
		glActiveTexture(GL_TEXTURE4);
		glBindTexture(GL_TEXTURE_CUBE_MAP, PreFilteredEnvTex);
		ShadingShader->SetInt("PreFilteredEnvironnementMap", 4);
		ShadingShader->SetMatrix4("InverseProjection", inverseProjection);
		glDrawElements(GL_TRIANGLES, 2 * 3, GL_UNSIGNED_INT, (void*)0);
		ShadingShader->unuse();


		// Draw skybox as last
		glm::vec4 sunNDC = mvp * ((-10000.f)*glm::vec4(-1.f, -1.f, 0.f, 0.f));  // Sun equal infinite point
		sunNDC = glm::vec4(glm::vec3(sunNDC) / sunNDC.w, 1.0);
		sunNDC += 1.0;
		sunNDC *= 0.5;
		glm::mat4 view = glm::mat4(glm::mat3(p->getCamera()->getViewMatrix()));    // Remove any translation component of the view matrix
		skybox->display(view, projection, textureManager["GBufferDepth"], screenWidth, screenHeight);
		glDisable(GL_BLEND);

		//FXAA
#ifdef FXAA
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureManager["fx1"], 0);
		glViewport(0, 0, screenWidth, screenHeight);
		glClear(GL_COLOR_BUFFER_BIT);
		shaderManager["FXAA"]->use();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureManager["fx0"]);
		glBindVertexArray(vaoManager["quad"]);
		glDrawElements(GL_TRIANGLES, 2 * 3, GL_UNSIGNED_INT, (void*)0);
#endif

		//Invert image to use in editor
#if 1
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureManager["fx0"], 0);
		glViewport(0, 0, screenWidth, screenHeight);
		glBindVertexArray(vaoManager["quad"]);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		shaderManager["InvertBlitShader"]->use();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureManager["fx1"]);
		glDrawElements(GL_TRIANGLES, 2 * 3, GL_UNSIGNED_INT, (void*)0);
		shaderManager["blit"]->unuse();
		glClearColor( 0.0f, 0.0f, 0.0f, 1.0f);
#endif

#if 0        
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		shaderManager["blit"]->use();
		ShadingShader->SetInt("Texture", 0);
		glActiveTexture(GL_TEXTURE0);
		glBindVertexArray(vaoManager["quad"]);
		// Viewport 
		glViewport( 0, 0, screenWidth/4, screenHeight/4  );
		// Bind texture
		glBindTexture(GL_TEXTURE_2D, textureManager["GBufferOutput0"]);
		// Draw quad
		glDrawElements(GL_TRIANGLES, 2 * 3, GL_UNSIGNED_INT, (void*)0);
		// Viewport 
		glViewport(screenWidth / 4, 0, screenWidth / 4, screenHeight / 4);
		// Bind texture
		glBindTexture(GL_TEXTURE_2D, textureManager["GBufferOutput1"]);
		// Draw quad
		glDrawElements(GL_TRIANGLES, 2 * 3, GL_UNSIGNED_INT, (void*)0);
		shaderManager["depth"]->use();
		// Viewport 
		glViewport(screenWidth / 4 * 2, 0, screenWidth / 4, screenHeight / 4);
		// Bind texture
		glBindTexture(GL_TEXTURE_2D, textureManager["GBufferDepth"]);
		// Draw quad
		glDrawElements(GL_TRIANGLES, 2 * 3, GL_UNSIGNED_INT, (void*)0);
		shaderManager["blit"]->use();
		// Viewport 
		glViewport(screenWidth / 4 * 3, 0 , screenWidth / 4, screenHeight / 4);
		// Bind texture
		glBindTexture(GL_TEXTURE_2D, textureManager["fx1"]);
		// Draw quad
		glDrawElements(GL_TRIANGLES, 2 * 3, GL_UNSIGNED_INT, (void*)0);
		shaderManager["depth"]->unuse();


		//Draw cubemap debug
		GLuint CubeMapID = skybox->texture_id;
		CubeMapID = PreFilteredEnvTex;
		for (int i = 0; i < 6; ++i)
		{
			glViewport(i* (screenWidth / 6), 200, screenWidth / 6, screenHeight / 4);
			shaderManager["CubeMapBlit"]->use();
			shaderManager["CubeMapBlit"]->SetInt("Face", Faces::XPos+i);
			shaderManager["CubeMapBlit"]->SetInt("MipMapLevel", 0);
			glBindTexture(GL_TEXTURE_CUBE_MAP, CubeMapID);
			glDrawElements(GL_TRIANGLES, 2 * 3, GL_UNSIGNED_INT, (void*)0);
		}
		for (int i = 0; i < 6; ++i)
		{
			glViewport(i* (screenWidth / 6), 400, screenWidth / 6, screenHeight / 4);
			shaderManager["CubeMapBlit"]->use();
			shaderManager["CubeMapBlit"]->SetInt("Face", Faces::XPos + i);
			shaderManager["CubeMapBlit"]->SetInt("MipMapLevel", 0);
			glBindTexture(GL_TEXTURE_CUBE_MAP, CubeMapID);
			glDrawElements(GL_TRIANGLES, 2 * 3, GL_UNSIGNED_INT, (void*)0);
		}
#endif

#if 1
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glViewport(0, 0, screenWidth, screenHeight);

		unsigned int flags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_ShowBorders;

		ImGui_ImplGlfwGL3_NewFrame();

		ImGui::BeginMainMenuBar();
		ImGui::MenuItem("Menu1");
		ImGui::EndMainMenuBar();

		int SelectedIndex = 0;
		const char * NameTags[5];
		NameTags[0] = "Actor 0";
		NameTags[1] = "Actor 1";
		NameTags[2] = "Actor 2";
		NameTags[3] = "Actor 3";
		NameTags[4] = "Actor 4";
		ImGui::SetNextWindowSize(ImVec2(200, 300));
		ImGui::Begin("Entity", 0, flags);
		ImGui::ListBox("Scene", &SelectedIndex, NameTags, 5);
		ImGui::End();
		
		//Display frame
		ImGui::Begin("View", 0, flags);
		ImGui::Image( (ImTextureID)textureManager["fx0"], ImGui::GetContentRegionAvail() );

		auto test3 = ImGui::GetItemBoxMax();
		auto test4 = ImGui::GetItemBoxMin();

		auto BBMin = glm::vec2(test4.x , test4.y );
		auto BBMax = glm::vec2(test3.x , test3.y );
		EDITORViewBoudingBox = glm::vec4(BBMin, BBMax);
		ImGui::End();
#if 0
		ImGui::SetNextWindowPos(ImVec2(0, 0));
		ImGui::SetNextWindowSize(ImVec2(200, 300));
		ImGui::Begin("Entity", 0, flags);
		ImGui::Text("This is the entity widget");
		ImGui::End();

		ImGui::InvisibleButton("vsplitter", ImVec2(8.0f, 300));

		ImGui::SetNextWindowPos(ImVec2(200, 0));
		ImGui::SetNextWindowSize(ImVec2(200, 300));
		ImGui::Begin("View", 0, flags);
		ImGui::Image((void *)textureManager["brick_diff"], ImVec2(200, 300));
		ImGui::End();

		ImGui::SetNextWindowPos(ImVec2(0, 300));
		ImGui::SetNextWindowSize(ImVec2(400, 100));
		ImGui::Begin("Assets", 0, flags);
		ImGui::Text("This is the Assets widget");
		ImGui::End();

		ImGui::SetNextWindowPos(ImVec2(400, 0));
		ImGui::SetNextWindowSize(ImVec2(200, 400));
		ImGui::Begin("Properties", 0, flags);
		ImGui::Text("This is the Properties widget");
		ImGui::End();
#endif

#if 0
		ImGui::SetNextWindowPos(ImVec2(0, 20));
		auto window_size = ImVec2(screenWidth, screenHeight);
		ImGui::SetNextWindowSize(window_size);
		ImGui::Begin("Splitter test", 0, flags);

		auto b = ImGui::GetCursorScreenPos();
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));


		ImGui::BeginChild("Entity", ImVec2(200, 500), true, flags);
		ImGui::Text("Entity");
		ImGui::EndChild();
		ImGui::SameLine();
		ImGui::InvisibleButton("vsplitter", ImVec2(3.0f, 500));
		if (ImGui::IsItemActive())
			w += ImGui::GetIO().MouseDelta.x;
		ImGui::SameLine();
		ImGui::BeginChild("View", ImVec2(w, h), true, flags);
		ImGui::Image(textureManager["brick_diff"], ImGui::GetContentRegionAvail());
		ImGui::EndChild();
		ImGui::InvisibleButton("hsplitter", ImVec2(-1, 8.0f));
		if (ImGui::IsItemActive())
			h += ImGui::GetIO().MouseDelta.y;
		ImGui::BeginChild("child3", ImVec2(2 * w + 8, 0), true, flags);
		ImGui::Text("Chilkd3");
		ImGui::EndChild();
		ImGui::SameLine();
		ImGui::SetCursorScreenPos(ImVec2(400, b.y));
		ImGui::InvisibleButton("vsplitter", ImVec2(8.0f, h));
		if (ImGui::IsItemActive())
			w += ImGui::GetIO().MouseDelta.x;
		ImGui::SameLine();
		ImGui::BeginChild("child4", ImVec2(0, 0), true, flags);
		ImGui::Text("Chilkd4");
		ImGui::EndChild();
		ImGui::PopStyleVar();

		ImGui::End();
#endif

		ImGui::Render();
		
#endif
        Utils::checkGlError("end frame");
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    //NOTE(marc) : We should properly clean the app, but since this will be the last
    //thing the program will do, it will clean them for us
    // Close OpenGL window and terminate GLFW
    ImGui_ImplGlfwGL3_Shutdown();
    glfwTerminate();
    return(0);
}
