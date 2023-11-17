#ifndef __WINDOW_H__
#define __WINDOW_H__

struct GLFWwindow;

class Window {
public:
	Window();
	~Window() {}
	// Initialization creates a window using GLFW, and sets up the Callbacks (keyboard, mouse support). No GLEW.
	bool Initialization( unsigned int width, unsigned int height, const char *title, unsigned int major, unsigned int minor, int monitor, GLFWwindow *shareContents );
	bool CallGlew();
	void MakeCurrentContext();
	void HideWindow( bool hide );
	void ClearColorBuffer( float red = 0.0f, float green = 0.0f, float blue = 0.0f );
	void SwapBuffers();
	void PollEvents();
	void CleanUp();
	// GetVersion requires CallGlew called before it will work
	void GetVersion();
	float GetBufferWidth();
	float GetBufferHeight();

	bool *GetsKeys();
	bool IsKeyUp( int key );
	bool IsKeyDown( int key );
	bool GetShouldClose();

	GLFWwindow *GetWindow();

	enum PROTO {
		TOTAL_KEYS = 1024 /* Dev choosen number */
	};

private:
	GLFWwindow *m_pWindow;
	bool m_bKeys[TOTAL_KEYS];
	int m_BufferWidth;
	int m_BufferHeight;

	void CreateCallbacks();
	static void HandleKeys( GLFWwindow *window,
		int key,
		int code,
		int action,
		int mode );
};

#endif