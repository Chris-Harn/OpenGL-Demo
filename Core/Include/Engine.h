#ifndef __ENGINE_H__
#define __ENGINE_H__

class Window;
class Quad;
class Shader;
class TextRenderer;

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
private:
    Engine();
    ~Engine() { }

    static Engine *s_pInstance;

    // Controls
    bool m_bRunning;

    // OpenGL Objects
    Window *m_pWindow;
    Window *m_pSecondaryWindow;
    Quad *m_pQuad;
    Quad *m_pSecondQuad;
    TextRenderer *m_pTextRenderer;

    // Shaders
    Shader *m_pColorGradient;
};

typedef Engine TheEngine;

#endif