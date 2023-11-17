#ifndef __QUAD_H__
#define __QUAD_H__

class Quad {
public:
	Quad();
	~Quad();
	void RenderQuad();

private:
	unsigned int m_VAO, m_VBO, m_IBO;
	int m_indexCount;
	int m_numOfIndices;
	int m_numOfVertices;
};

#endif