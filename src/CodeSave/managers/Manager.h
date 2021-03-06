// This is an independent project of an individual developer. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
/* ======================================================================== 
/* ========================================================================
   $File: $
   $Modified: 06-02-2017 07h50m20s $
   $Revision: $
   $Creator:  $
   $Notice:  $
   ======================================================================== */

#define MANAGER_H

#include <string>
#include <map>
#include <glew/glew.h>
#include "../shader/Shader.h"
#include "../mesh/Model.h"

//Struct to differentiate GLuint referenced type for several singleton
struct Tex
{
    GLuint id;
};
struct VAO
{
    GLuint id;
};
struct FBO
{
    GLuint id;
};


template <class T>
class Manager
{
public:
    static T& instance()
    {
        static T instance;
        return instance;
    }

private:    
    //Manager(){}
    //Manager(Manager const&) = delete;
    T& operator=(T const&) {};
    //std::map<std::string, T > managed;    
};

class ModelManager : public Manager<ModelManager>
{
	friend class Manager<ModelManager>;

public:
	std::map<std::string, Model*>& getManaged(){ return(managed); }
	Model* operator[](std::string s){ return(managed[s]); }
private:
	ModelManager() : managed(std::map<std::string, Model*>()){}
	std::map<std::string, Model*> managed;
};

class TextureManager : public Manager<TextureManager>
{
    friend class Manager<TextureManager>;

public :
    std::map<std::string, Tex>& getManaged(){ return( managed ); }
    GLuint operator[](std::string s){ return( managed[s].id ); }
private :
    TextureManager() : managed( std::map<std::string, Tex>()){}
    std::map<std::string, Tex> managed;
};

class VaoManager : public Manager<VaoManager>
{
    friend class Manager<VaoManager>;

public :
    std::map<std::string, VAO>& getManaged(){ return( managed ); }
    GLuint operator[](std::string s){ return( managed[s].id ); }    
private :
    VaoManager() : managed( std::map<std::string, VAO>()){}
    std::map<std::string, VAO> managed;
};

class FboManager : public Manager<FboManager>
{
    friend class Manager<FboManager>;

public :
    std::map<std::string, FBO>& getManaged(){ return( managed ); }
    GLuint operator[](std::string s){ return( managed[s].id ); }    
private :
    FboManager() : managed( std::map<std::string, FBO>()){}
    std::map<std::string, FBO> managed;
};

class ShaderManager : public Manager<ShaderManager>
{
    friend class Manager<ShaderManager>;

public :
    std::map<std::string, Shader*>& getManaged(){ return( managed ); }
    Shader* operator[](std::string s){ return( managed[s] ); }
private :
    ShaderManager() : managed( std::map<std::string, Shader*>()){}
    std::map<std::string, Shader*> managed;
};

#endif


