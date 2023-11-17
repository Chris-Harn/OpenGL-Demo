#include "Engine.h"

#include <iostream>

Engine *Engine::s_pInstance = 0;

Engine::Engine() {
    m_bRunning = false;
}

bool Engine::Init( const char *title, int windowWidth, int windowHeight, int majorVersionOGL, int minorVersionOGL ) {
    std::cout << "Inside Init" << std::endl;
    m_bRunning = true;

    return true;
}

void Engine::HandleEvents() {
    std::cout << "Inside Handle Events" << std::endl;
}

void Engine::Render() {
    std::cout << "Inside Render" << std::endl;

    m_bRunning = false;
}

void Engine::CleanUp() {
    std::cout << "Inside CleanUp" << std::endl;
}

bool Engine::Running() {
    return m_bRunning;
}