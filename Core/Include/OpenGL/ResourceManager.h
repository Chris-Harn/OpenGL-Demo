#ifndef __RESOURCEMANAGER_H__
#define __RESOURCEMANAGER_H__

#include <map>
#include <string>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "OpenGL/Shader.h"

class ResourceManager {
public:
    // Resource Storage
    static std::map<std::string, std::string>   m_ShaderFilePaths;
    static std::map<std::string, Shader *>      m_Shaders;

    static Shader *LoadShader( const char *vShaderFile, const char *fShaderFile, const char *gShaderFile, std::string name );
    static Shader *LoadShader( const char *ShaderFile, std::string name );
    static Shader *GetShader( std::string name );
    static void Clean(); // properly de-allocates all loaded resources

    static void ReloadShaders();
private:
    ResourceManager() {}  // Singleton

    static Shader *LoadShaderFromFiles( const char *vShaderFile, const char *fShaderFile, const char *gShaderFile = nullptr );
    static Shader *LoadShaderFromFile( const char *shaderFile );
};

#endif