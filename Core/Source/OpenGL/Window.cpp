#include "OpenGL/Window.h"
#include "Logger.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#ifdef _DEBUG
#include <iostream>
#endif

Window::Window() {
	m_pWindow = nullptr;

	// Clear all keys
	for( int i = 0; i < TOTAL_KEYS; i++ ) {
		m_bKeys[i] = false;
	}

	m_BufferWidth = 0.0f;
	m_BufferHeight = 0.0f;
}

bool Window::Initialization( unsigned int width, unsigned int height, const char *title, unsigned int major, unsigned int minor, int monitor, GLFWwindow *shareContents ) {
	if( glfwInit() == false ) {
		TheLogger::Instance()->LogError( (const char *)"*** GLFW failed to start up. ***" );
		glfwTerminate();
		return false;
	}

	glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, major );
	glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, minor );
	glfwWindowHint( GLFW_CLIENT_API, GLFW_OPENGL_API );
	// Core profile is preferable for versions above 3.3
	if( ( major > 3 ) || ( ( major == 3 ) && ( minor > 2 ) ) ) {
		TheLogger::Instance()->LogError( (const char *)"*** Enabling Core Profile Mode. ***" );
		glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );
		glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE );
	}

	// Grab list of monitors
	int count;
	GLFWmonitor **monitors = glfwGetMonitors( &count );
	if( count < monitor ) {
		TheLogger::Instance()->LogError( (const char *)"*** Tried to create monitor that didn't exist %u. Max monitors found was %u. ***", monitor, count );
		return false;
	}

	const GLFWvidmode *mode = nullptr;

	if( monitor == 0 ) {
		// Primary Window setup
		mode = glfwGetVideoMode( monitors[0] );

		// **********************************
		// Add Settings here...
		//glfwWindowHint( GLFW_DECORATED, GLFW_TRUE );
		//glfwWindowHint( GLFW_FLOATING, GLFW_FALSE );
		//glfwWindowHint( GLFW_AUTO_ICONIFY, GLFW_FALSE );
		glfwWindowHint( GLFW_AUTO_ICONIFY, GLFW_TRUE );

		glfwWindowHint( GLFW_RESIZABLE, GLFW_FALSE );
		//glfwWindowHint( GLFW_MAXIMIZED, GLFW_TRUE );
		glfwWindowHint( GLFW_RED_BITS, mode->redBits );
		glfwWindowHint( GLFW_GREEN_BITS, mode->greenBits );
		glfwWindowHint( GLFW_BLUE_BITS, mode->blueBits );
		glfwWindowHint( GLFW_REFRESH_RATE, mode->refreshRate );

		m_pWindow = glfwCreateWindow( mode->width, mode->height, title, nullptr, nullptr );
	}
	else if( monitor == 1 ) {
		// Secondary Window setup
		mode = glfwGetVideoMode( monitors[1] );

		// **********************************
		// Add Settings here for secondary monitor...
		// Duplicate anything that changes
		glfwWindowHint( GLFW_DECORATED, GLFW_FALSE );
		//glfwWindowHint( GLFW_FLOATING, GLFW_TRUE );
		glfwWindowHint( GLFW_AUTO_ICONIFY, GLFW_TRUE );
		glfwWindowHint( GLFW_RESIZABLE, GLFW_FALSE );
		//glfwWindowHint( GLFW_MAXIMIZED, GLFW_TRUE );
		glfwWindowHint( GLFW_RED_BITS, mode->redBits );
		glfwWindowHint( GLFW_GREEN_BITS, mode->greenBits );
		glfwWindowHint( GLFW_BLUE_BITS, mode->blueBits );
		glfwWindowHint( GLFW_REFRESH_RATE, mode->refreshRate );

		// Must use pointer of primary window to be able to share opengl objects between windows: objects, shaders, FBOs, etc
		m_pWindow = glfwCreateWindow( mode->width, mode->height, title, monitors[1], shareContents );
	}
	else {
		// Exit because trying to create window that shouldn't exist
		TheLogger::Instance()->LogError( (const char *)"*** Tried to create monitor outside intended scope. ***" );
		return false;
	}

	if( !m_pWindow ) {
		TheLogger::Instance()->LogError( (const char *)"*** Window failed to be created. ***" );
		glfwTerminate();
		return false;
	}

	// Need to make current context so glViewport and Glew will work on the correct window
	this->MakeCurrentContext();

	// Set the viewport and lock it
	glViewport( 0, 0, mode->width, mode->height );
	glfwSetWindowSize( m_pWindow, mode->width, mode->height );

	// Get buffer size information
	glfwGetFramebufferSize( m_pWindow, &m_BufferWidth, &m_BufferHeight );

	//if( monitor == 1 ) {
	//	// Make the window always be on top
	//	glfwSetWindowAttrib( m_pWindow, GLFW_FLOATING, GLFW_TRUE );
	//}

	// Enable OpenGL commands as everything up to this point has been GLFW
	this->CallGlew();

	// Set up callbacks on primary monitor... keyboard polling, mouse polling, etc
	if( monitor == 0 ) {
		CreateCallbacks();
	}

	return true;
}

bool Window::CallGlew() {
	glfwMakeContextCurrent( m_pWindow );

	// Allow modern extension features
	glewExperimental = GL_TRUE; // Tells drivers to expose all addresses
	if( glewInit() != GLEW_OK ) {
		TheLogger::Instance()->LogError( (const char *)"*** GLEW failed to initalize. ***" );
		glfwDestroyWindow( m_pWindow );
		glfwTerminate();
		return false;
	}

	return true;
}

void Window::MakeCurrentContext() {
	glfwMakeContextCurrent( m_pWindow );
}

void Window::HideWindow( bool hide ) {
	if( hide == true ) {
		glfwWindowHint( GLFW_VISIBLE, GLFW_FALSE );
	}
	else {
		glfwWindowHint( GLFW_VISIBLE, GLFW_TRUE );
	}
}

void Window::ClearColorBuffer( float red, float green, float blue ) {
	glClearColor( red, green, blue, 1.0f );
	glClear( GL_COLOR_BUFFER_BIT );
}

void Window::SwapBuffers() {
	glfwSwapBuffers( m_pWindow );
}

void Window::PollEvents() {
	glfwPollEvents();
}

void Window::CleanUp() {
	if( m_pWindow != nullptr ) {
		glfwDestroyWindow( m_pWindow );
		m_pWindow = nullptr;
	}

	glfwTerminate();
}

void Window::GetVersion() {
	// Does not work unless CallGlew GLEW has been called...
#ifdef _DEBUG
	TheLogger::Instance()->Printf_ntstamp( (const char *)glGetString( GL_VENDOR ) );
	TheLogger::Instance()->Printf_ntstamp( (const char *)glGetString( GL_RENDERER ) );
	TheLogger::Instance()->Printf_ntstamp( (const char *)glGetString( GL_VERSION ) );
#endif
}

float Window::GetBufferWidth() {
	return (float)m_BufferWidth;
}

float Window::GetBufferHeight() {
	return (float)m_BufferHeight;
}

bool *Window::GetsKeys() {
	return m_bKeys;
}

bool Window::IsKeyUp( int key ) {
	if( key != 0 ) {
		return m_bKeys[key];
	}

	return false;
}

bool Window::IsKeyDown( int key ) {
	if( key != 0 ) {
		return !m_bKeys[key];
	}

	return false;
}

bool Window::GetShouldClose() {
	return int( glfwWindowShouldClose( m_pWindow ) );
}

GLFWwindow *Window::GetWindow() {
	return m_pWindow;
}

void Window::CreateCallbacks() {
	// Setup GLFW callback that uses this object as owner
	// or else keys won't work
	glfwSetWindowUserPointer( m_pWindow, this );

	// Callback that captures key presses and releases
	glfwSetKeyCallback( m_pWindow, HandleKeys );
	glfwSetInputMode( m_pWindow, GLFW_STICKY_KEYS, 1 );
}

void Window::HandleKeys( GLFWwindow *window,
	int key,
	int code,
	int action,
	int mode ) {
	// Window pointer user function
	Window *theWindow = static_cast<Window *>( glfwGetWindowUserPointer( window ) );

	if( ( key == GLFW_KEY_ESCAPE ) && ( action == GLFW_PRESS ) ) {
		glfwSetWindowShouldClose( window, GL_TRUE );
		theWindow->m_bKeys[GLFW_KEY_ESCAPE] = true;
	}

	if( ( key >= 0 ) && ( key < 1024 ) ) {
		if( action == GLFW_PRESS ) {
			theWindow->m_bKeys[key] = true;
			#ifdef _DEBUG
				std::cout << "Pressed " << key << std::endl; // Trouble Shooting Code
			#endif
		}
		else if( action == GLFW_RELEASE ) {
			theWindow->m_bKeys[key] = false;
			#ifdef _DEBUG
				std::cout << "Released " << key << std::endl; // Trouble Shooting Code
			#endif
		}
	}

}