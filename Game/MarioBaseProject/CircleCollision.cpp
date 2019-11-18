#include "CircleCollision.h"



CircleCollision::CircleCollision()
{
	m_v2dLocation = new Vector2D();
}


CircleCollision::~CircleCollision()
{
	if (m_v2dLocation != nullptr)
		delete m_v2dLocation;
}
