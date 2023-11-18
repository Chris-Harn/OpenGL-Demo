#include "Engine.h"

#include "Logger.h"
#include "Timer.h"
#include "OpenGL/Window.h"
#include "OpenGL/Shader.h"
#include "OpenGL/Quad.h"
#include "OpenGL/ResourceManager.h"
#include "OpenGL/TextRenderer.h"
#include "OpenGL/Helper.h"

#include <iostream> // std::bad_alloc

Engine *Engine::s_pInstance = 0;

Engine::Engine() {
	// Non-OpenGL objects
	m_pTimer = nullptr;

	// OpenGL objects
	m_pWindow = nullptr;
	m_pSecondaryWindow = nullptr;
	m_pQuad = nullptr;
	m_pSecondQuad = nullptr;
	m_pTextRenderer = nullptr;

	// Shaders
	m_pColorGradient = nullptr;

	// Engine globals
	m_bRunning = false;
}

bool Engine::Init( const char *title, int windowWidth, int windowHeight, int targetFps, int majorVersionOGL, int minorVersionOGL ) {
	/**************** START WINDOW CREATION ***************/
	try { m_pWindow = new Window(); }
	catch( const std::bad_alloc &e ) {
		(void)e;
		TheLogger::Instance()->LogError( (const char *)"*** Window failed to have memory allocated. ***" );
		return false;
	}
	if( m_pWindow->Initialization( windowWidth, 
		                           windowHeight, 
		                           title, 
		                           majorVersionOGL, 
		                           minorVersionOGL, 
		                           0, 
		                           nullptr ) == false ) {
		TheLogger::Instance()->LogError( (const char *)"*** Exited Program ***" );
		return false;
	}
	m_pWindow->GetVersion();

	//try { m_pSecondaryWindow = new Window(); }
	//catch( const std::bad_alloc &e ) {
	//	(void)e;
	//	TheLogger::Instance()->LogError( (const char *)"*** Secondary Window failed to have memory allocated. ***" );
	//	return false;
	//}
	//if( m_pSecondaryWindow->Initialization( windowWidth, 
	//										windowHeight, 
	//	                                    "Secondary Window", 
	//										majorVersionOGL, 
	//	                                    minorVersionOGL, 
	//										1, 
	//										m_pWindow->GetWindow() ) == false ) {
	//	TheLogger::Instance()->LogError( (const char *)"*** Exited Program ***" );
	//	return false;
	//}

	//m_pSecondaryWindow->GetVersion();
	/******************** END WINDOW CREATION *****************/

	// Switch to main context and initalize the program's assets. All of
	// these objects and opengl assets will be tied to this context, and
	// will work with secondary context if shared during window creation
	m_pWindow->MakeCurrentContext();
	try { m_pQuad = new Quad(); }
	catch( const std::bad_alloc &e ) {
		(void)e;
		TheLogger::Instance()->LogError( (const char *)"*** Quad failed to have memory allocated. ***" );
		return false;
	}

	// Initialize shaders
	ResourceManager::LoadShader( "Resource/Shaders/CautionStrips.shader.glsl", "CautionImage" );
	ResourceManager::LoadShader( "Resource/Shaders/BlitColorGradient.shader.glsl", "ColorGradient" );
	ResourceManager::LoadShader( "Resource/Shaders/FastBlitTextToScreen.shader.glsl", "FastBlitText" );
	ResourceManager::GetShader( "FastBlitText" )->SetInteger( "text", 0, true );

	try { m_pTextRenderer = new TextRenderer(); }
	catch( const std::bad_alloc &e ) {
		(void)e;
		TheLogger::Instance()->LogError( (const char *)"*** Text Renderer failed to have memory allocated. ***" );
		return false;
	}
	m_pTextRenderer->Initialize( m_pWindow );
	try { m_pTimer = new Timer( targetFps ); }
	catch( const std::bad_alloc &e ) {
		(void)e;
		TheLogger::Instance()->LogError( (const char *)"*** Timer failed to have memory allocated. ***" );
		return false;
	}
	m_pTimer->Tick();
	m_pTimer->RegulateFPS( true );


	//m_pSecondaryWindow->MakeCurrentContext();
	//try { m_pSecondQuad = new Quad(); }
	//catch( const std::bad_alloc &e ) {
	//	(void)e;
	//	TheLogger::Instance()->LogError( (const char *)"*** Second Quad failed to have memory allocated. ***" );
	//	return false;
	//}

	#ifdef _DEBUG
		TheLogger::Instance()->Printf_ntstamp( (const char *)"*** Compeleted setup. ***\n" );
	#endif

	// Haven't returned false yet, so tell sim to run
	m_bRunning = true;

	return true;
}

void Engine::HandleEvents() {
	m_pTimer->Tick();
	m_pWindow->PollEvents();

	if( ( m_pWindow->GetsKeys()[256] == true ) ||
		( m_pWindow->GetShouldClose() == true ) ) {
		m_bRunning = false; // GLFW_KEYS_ESCAPE
	}
}

void Engine::Render() {
	// Do whatever on the first window...
	m_pWindow->MakeCurrentContext();
	m_pWindow->ClearColorBuffer();

	// Draw to main window
	ResourceManager::GetShader( "ColorGradient" )->Use();
	//ResourceManager::GetShader( "CautionImage" )->Use();
	m_pQuad->RenderQuad();

	DisplayPerformanceInformation();

	m_pWindow->SwapBuffers();

	// Do whatever on the second window...
	//m_pSecondaryWindow->MakeCurrentContext();
	//m_pSecondaryWindow->ClearColorBuffer( 0.0f, 1.0f, 0.0f );

	// Draw to secondary window
	//m_pColorGradient->BindShader();
	//m_pSecondQuad->RenderQuad();
	//m_pColorGradient->UnbindShader();

	//m_pSecondaryWindow->SwapBuffers();
}

void Engine::CleanUp() {
	// Clean up main window first
	m_pWindow->MakeCurrentContext();
	m_pWindow->CleanUp();

	// Crashes if attempt to clean up safety critical window first
	//m_pSecondaryWindow->MakeCurrentContext();
	//m_pSecondaryWindow->CleanUp();
}

bool Engine::Running() {
    return m_bRunning;
}