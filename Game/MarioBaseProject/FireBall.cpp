#include "FireBall.h"



FireBall::FireBall()
{
}


FireBall::~FireBall()
{
}

bool FireBall::Update(float f_deltaTime)
{
	//double l_dAngle = m_dAngle;
	//double l_dMovementX = get_locationX() + cos(l_dAngle) * (0.1 * f_deltaTime);
	//double l_dMovementY = get_locationY() + sin(l_dAngle) * (0.1 * f_deltaTime);
	//set_location(l_dMovementX, l_dMovementY);

	double l_dAngle = m_dAngle;
	double l_dMovementX = get_locationX() + l_dAngle * (f_deltaTime);
	double l_dMovementY = get_locationY() + l_dAngle * (f_deltaTime);
	//cout << "l_dMovementX: " << l_dMovementX << endl;
	//cout << "l_dMovementY: " << l_dMovementY << endl;
	set_location(l_dMovementX, l_dMovementY);
	
	/*double l_dMovementX = get_locationX() + (0.1 * f_deltaTime);
	double l_dMovementY = m_dGradient * l_dMovementX + m_dIntercept;
	set_location(l_dMovementX, l_dMovementY);*/

	/*float l_fMovementX = get_locationX() + m_v2dMovement.m_iX * (0.001f * f_deltaTime);
	float l_fMovementY = get_locationY() + m_v2dMovement.m_iY * (0.001f * f_deltaTime);

	set_location(l_fMovementX, l_fMovementY);*/
	return true;
}

bool FireBall::Draw()
{
	RenderToScreen(p_sb->get_renderer());
	return true;
}