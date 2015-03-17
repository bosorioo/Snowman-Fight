#ifndef _SPHERICAL_TEXTURE_H
#define _SPHERICAL_TEXTURE_H

#include <vector>
#include "glutInclude.h"

class SphericalTexture
{
public:

	SphericalTexture(int quadPoints = 10, float radius = 1.f);

	void setTexture(std::string& path);
	void draw();

private:

	struct Vertex {
		float x, y, z;
		float u, v;
		void vertex() {glTexCoord2f(u, v); glVertex3f(x, y, z);}
		void setXYZ(float _x, float _y, float _z) {x = _x; y = _y; z = _z;}
		void setUV(float _x, float _y) {u = _x; v = _y;}
	};

	void calculate();
	
	unsigned m_TextureId;
	int m_QuadPoints;
	float m_Radius;
	std::vector<Vertex> m_Vertexes;

};

#endif