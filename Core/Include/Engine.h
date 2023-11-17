#ifndef __ENGINE_H__
#define __ENGINE_H__

#include "OpenGL/Window.h"
#include "OpenGL/TextRenderer.h"
#include "Timer.h"
class Quad;
class Shader;

class Engine {
public:
    static Engine *Instance() {
        if( s_pInstance == 0 ) {
            s_pInstance = new Engine();
        }
        return s_pInstance;
    }

    bool Init( const char *title, int windowWidth, int windowHeight, int majorVersionOGL, int minorVersionOGL );
    void HandleEvents();
    void Render();
    void CleanUp();

    bool Running();

    // Non-OpenGL Objects
    Timer *m_pTimer;

    // OpenGL Objects
    Window *m_pWindow;
    TextRenderer *m_pTextRenderer;
private:
    Engine();
    ~Engine() { }

    static Engine *s_pInstance;

    // Controls
    bool m_bRunning;

    // OpenGL Objects
    Window *m_pSecondaryWindow;
    Quad *m_pQuad;
    Quad *m_pSecondQuad;

    // Shaders
    Shader *m_pColorGradient;
};

typedef Engine TheEngine;

#endif