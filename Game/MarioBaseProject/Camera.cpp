#include "Camera.h"



Camera::Camera()
{
}


Camera::~Camera()
{
}

void Camera::MoveCamera(float f_iX, float f_iY)
{
	m_iLocationX += f_iX;
	m_iLocationY += f_iY;

	update_lockToWorld();
}

void Camera::update_lockToWorld()
{
	if (m_bLockToWorld)
	{
		if (m_iLocationX < 0) m_iLocationX = 0;
		if (m_iLocationX + m_fScreenWidth > m_iWorldWidth) m_iLocationX = m_iWorldWidth - m_fScreenWidth;

		if (m_iLocationY < 0)
			m_iLocationY = 0;
		/*else if(m_bLevelEditor)
			if (m_iYLowest != 0)
				if (m_iLocationY > m_iYLowest)
					m_iLocationY = m_iYLowest;*/
	}
	if(!m_bLevelEditor)
		update_cameraScroll();
	if (m_bLevelEditor)
		update_cameraLevelEditor();
}

bool Camera::update_cameraScroll()
{
	if (m_bScrolledUp && get_y() > m_iYHighest)
	{
		m_iLocationY -= 2.0f;
		if (!m_bScrolledUp && get_y() < m_iYLowest)
			m_iLocationY = m_iYHighest;
	}
	else if (!m_bScrolledUp && get_y() < m_iYLowest)
	{
		m_iLocationY += 2.0f;
		if (!m_bScrolledUp && get_y() > m_iYLowest)
			m_iLocationY = m_iYLowest;
	}
		
	return true;
}

bool Camera::update_cameraLevelEditor()
{
	if (m_iLocationY > m_iYLowest)
		m_iLocationY = m_iYLowest;

	
	return false;
}