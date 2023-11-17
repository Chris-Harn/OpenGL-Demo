#ifndef __ENGINE__
#define __ENGINE__

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

    
    bool m_bRunning;
};

typedef Engine TheEngine;

#endif