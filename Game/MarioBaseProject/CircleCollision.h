#pragma once
#include "Vector2D.h"

using namespace std;
class CircleCollision
{
private:
	Vector2D *m_v2dLocation = nullptr;
	float m_fRadius = 0;
public:
	CircleCollision();
	virtual ~CircleCollision();

	void set_location(int f_iX, int f_iY) { if (m_v2dLocation != nullptr) { m_v2dLocation->m_iX = f_iX; m_v2dLocation->m_iY = f_iY; } }
	void set_radius(float f_fRadius) { m_fRadius = f_fRadius; }

	Vector2D get_location() { if (m_v2dLocation != nullptr) return *m_v2dLocation; else return { -1,-1 }; }
	float get_radius() { return m_fRadius; }
};

