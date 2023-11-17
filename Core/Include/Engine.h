#ifndef __ENGINE__
#define __ENGINE__

class Engine {
public:
    Engine();
    ~Engine();

    bool Init();
    void HandleEvents();
    void Render();
    void Clean();
private:
    bool Running();

};

#endif