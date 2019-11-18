#include "Platformer_Character.h"
#include "Vector2D.h"
#include <cmath>


Platformer_Character::Platformer_Character()
{

}


Platformer_Character::~Platformer_Character()
{
	delete m_rectCollision;
	delete m_circleCollision;
}

bool Platformer_Character::Update(float f_deltaTime)
{
	//set_disableGravity(true);
	update_momentumDirectionCheck();
	update_gravityPull(f_deltaTime);
	update_cameraLock();
	return true;
}

bool Platformer_Character::update_gravityPull(float f_deltaTime)
{
	if (p_tilemap != nullptr && !p_disableCollisions)
	{
		SDL_Rect l_rectPlayerCollision = get_collisionRect();
		SDL_Rect l_rectPlayerCollisionBottom = { l_rectPlayerCollision.x,l_rectPlayerCollision.y + l_rectPlayerCollision.h - (get_scaleSize()), l_rectPlayerCollision.w,get_scaleSize() * 2 };
		Tilemap::Tilemap_Tile l_ttYourTile = p_tilemap->get_tileFromLocation(l_rectPlayerCollision.x + l_rectPlayerCollision.w / 2, l_rectPlayerCollision.y + l_rectPlayerCollision.h);

		additionalBlockCheck(l_rectPlayerCollision);

		Tilemap::Tilemap_Tile l_ttTileUnder; SDL_Rect l_rectTileUnder;
		bool l_bCollision = false;
		for (int i = 0; i < 3; i++)
		{

			l_ttTileUnder = { l_ttYourTile.x, l_ttYourTile.y + 1 }; //Default

			float l_fYTest = 0;
			if (i == 0)
			{
				//THIS WORKS!!!!! YAY, YOU CAN'T JUMP ON THEM FROM THE RIGHT!
				float l_fXTest = (get_scaleSize() * 2) * -1;
				if (move_character_innerCheck(Tilemap::Right, 4, l_fXTest, l_fYTest))//If we're not up against a wall to the left
					l_ttTileUnder = { l_ttYourTile.x - 1, l_ttYourTile.y + 1 }; //Check basically 1 on numpad
				else
					continue;//If we can't move that way then don't check it
			}
			else if (i == 1)
			{
				//THIS WORKS!!!!! YAY, YOU CAN'T JUMP ON THEM FROM THE LEFT!
				//14 works for some reason for small Mario. 18 seems to work for both. Makes sense as this is close to the tilesize
				float l_fXTest = (get_scaleSize() * 18);
				if (move_character_innerCheck(Tilemap::Left, 4, l_fXTest, l_fYTest))//If we're not up against a wall to the right
					l_ttTileUnder = { l_ttYourTile.x + 1, l_ttYourTile.y + 1 }; //Check basically 3 on numpad
				else
					continue;//If we can't move that way then don't check it
			}


			int l_iTileNumber = p_tilemap->get_tileInfo(l_ttTileUnder.x, l_ttTileUnder.y);
			if (l_iTileNumber == -1)
				continue;

			//The following detects whether there is a collision or not
			Tilemap::Tilemap_Tile l_tttLocationOnTileset = p_tilemap->get_tileInTilesetLocation(l_iTileNumber);
			if (!p_tilemap->get_collision(l_tttLocationOnTileset, Tilemap::Top))
				continue;

			l_rectTileUnder = p_tilemap->get_tileCollisionFromReference(l_ttTileUnder);//We're sticking to the whole block
			m_fTileUnder = l_rectTileUnder.y;
			

			if (m_controls != nullptr && i == 0)
				if (m_controls->isGameKeyDown(Controls::BTN_START))
					cout << "PLAYER: " << l_rectPlayerCollisionBottom.y << " TILE: " << l_rectTileUnder.y << endl;

			if (Utility::ReturnCollision(l_rectPlayerCollisionBottom, l_rectTileUnder))
			{
				if (p_momentumV == mvDown)
					m_fGravityPull = 0;

				//if (m_fGravityPull < 0)
				//	m_fGravityPull = 0;

				l_bCollision = true;
				if (p_animation == Jumping || p_animation == Falling)
				{
					//cout << "=============================== I RUN" << endl;
					//The following code is to ensure you can never fall through a block.
					//However!
					//This works on Mario but may not work on other sprites.
					/*if (p_animation == Jumping)//Only Mario Jumps //No Longer needed since making the draw better
						adjust_ifStuckInTopLip(l_rectTileUnder);*/
						//set_location(get_locationX(), l_rectTileUnder.y - ((get_singleHeight() - 1) * get_scaleSize()));// +l_rectPlayerCollision.y);
					//else if(p_animation == Falling)//Only Items or non-mario Fall
					//	set_location(get_locationX(), l_rectTileUnder.y - (get_singleHeight() * get_scaleSize()));// +l_rectPlayerCollision.y);

					
					//m_fVActualSpeed = 0;
					//cout << "MAIN: " << m_iDebugNumber++ << endl;
					p_animation = None;
				}
				m_fVActualSpeed = 0;
				m_bSuccessfulFall = false;

				if (get_locationY() <= p_camera->get_yLowest())
					m_bScrollUp = true;
				else
					m_bScrollUp = false;
				m_bHaveEverLanded = true;
			}
		}

		if (!l_bCollision && !m_bDisableGravity)
			PullCharacterToGravity(f_deltaTime);

	}
	else if (!m_bDisableGravity)
	{
		PullCharacterToGravity(f_deltaTime);
	}
	return true;
}

bool Platformer_Character::adjust_ifStuckInTopLip(SDL_Rect f_rectTileUnder)
{
	return false;
}



bool Platformer_Character::additionalBlockCheck(SDL_Rect f_rectPlayerRect)
{
	if (p_tilemap != nullptr) return false;
	
	return true;
}

bool Platformer_Character::PullCharacterToGravity(float f_deltaTime)
{

	if (m_fVActualSpeed < m_fVMovementSpeed)
	{
		m_fVActualSpeed += m_fVIncrease * f_deltaTime;
		//cout << "m_fVActualSpeed: " << m_fVActualSpeed << endl;
	}
	if (m_fVMovementInverter > 0)
	{
		m_fVActualSpeed -= (m_fVMovementInverterMax - m_fVMovementInverter) * f_deltaTime;
		m_fVMovementInverter -= m_fVIncrease * f_deltaTime;
		if (m_fVMovementInverter < 0)
			m_fVMovementInverter = 0;
	}
		

	if (m_fVActualSpeed > m_fVMovementSpeed)
		m_fVActualSpeed = m_fVMovementSpeed;


	if (!p_disableCollisions)
	{
		if (move_character(0, f_deltaTime * m_fVActualSpeed))
		{
			m_bSuccessfulFall = true;

			if (m_fVActualSpeed > 0.25f)
				p_animation = Jumping;
		}
		else
		{
			if (m_fVMovementInverter > 0)
				m_fVMovementInverter = 0;
			else
				m_bSuccessfulFall = false;
		}
	}
	else
	{
		move_location(0, f_deltaTime * m_fVActualSpeed);
	}
	return true;
}

void Platformer_Character::set_collision()
{
	if (m_rectCollision == nullptr)
		m_rectCollision = new SDL_Rect();
	//set_collision(0, 0, get_actualSizeWidth(), get_actualSizeHeight());
	set_collision(0, 0, get_singleWidth(), get_singleHeight());
}
void Platformer_Character::set_collision(int f_iX, int f_iY, int f_iWidth, int f_iHeight)
{
	if (m_rectCollision == nullptr)
		m_rectCollision = new SDL_Rect();

	m_rectCollision->x = f_iX * get_scaleSize();
	m_rectCollision->y = f_iY * get_scaleSize();
	m_rectCollision->w = f_iWidth * get_scaleSize();
	m_rectCollision->h = f_iHeight * get_scaleSize();
}

void Platformer_Character::shrink_collision(int f_iValue)
{
	if (m_rectCollision == nullptr)
	{
		m_rectCollision = new SDL_Rect();
		set_collision();
	}

	m_rectCollision->x += f_iValue * get_scaleSize();
	m_rectCollision->y += f_iValue * get_scaleSize();
	m_rectCollision->w -= f_iValue * get_scaleSize();
	m_rectCollision->h -= f_iValue * get_scaleSize();
}

Tilemap::Tilemap_Tile Platformer_Character::get_tileReferenceAtFeet()
{
	Tilemap::Tilemap_Tile l_ttReturn;
	SDL_Rect l_rectCollision = get_collisionRect();
	l_ttReturn = p_tilemap->get_tileFromLocation(l_rectCollision.x + (l_rectCollision.w / 2), l_rectCollision.y + l_rectCollision.h);
	return l_ttReturn;
}
Tilemap::Tilemap_Tile Platformer_Character::get_tileReferenceBottomLeft()
{
	Tilemap::Tilemap_Tile l_ttReturn;
	SDL_Rect l_rectCollision = get_collisionRect();
	l_ttReturn = p_tilemap->get_tileFromLocation(l_rectCollision.x, l_rectCollision.y + l_rectCollision.h);
	return l_ttReturn;
}
Tilemap::Tilemap_Tile Platformer_Character::get_tileReferenceBottomRight()
{
	Tilemap::Tilemap_Tile l_ttReturn;
	SDL_Rect l_rectCollision = get_collisionRect();
	l_ttReturn = p_tilemap->get_tileFromLocation(l_rectCollision.x + l_rectCollision.w, l_rectCollision.y + l_rectCollision.h);
	return l_ttReturn;
}

SDL_Rect Platformer_Character::get_collisionRect()
{
	SDL_Rect l_rectReturn;
	if (m_rectCollision == nullptr)
		l_rectReturn = {get_locationX(),get_locationY(),get_actualSizeWidth(),get_actualSizeHeight()};
	else
		l_rectReturn = { get_locationX() + m_rectCollision->x,get_locationY() + m_rectCollision->y,m_rectCollision->w,m_rectCollision->h};
	return l_rectReturn;
}

CircleCollision* Platformer_Character::get_circleCollision()
{
	if (m_circleCollision == nullptr)m_circleCollision = new CircleCollision();
	m_circleCollision->set_location(get_locationX() + (get_actualSizeWidth() / 2), get_locationY() + (get_actualSizeHeight() / 2));
	//cout << "SET: " << get_locationX() + (get_actualSizeWidth() / 2) << ", " << get_locationY() + (get_actualSizeHeight() / 2) << endl;
	//cout << "VALUE: " << m_circleCollision->get_location().m_iX << ", " << m_circleCollision->get_location().m_iY << endl;
	m_circleCollision->set_radius((get_actualSizeWidth() / 2));
	return m_circleCollision;
}

int Platformer_Character::get_tileNumberAtFeet()
{
	if (p_tilemap == nullptr) return -1;
	Tilemap::Tilemap_Tile l_ttTile = get_tileReferenceAtFeet();
	return get_tileNumberAtFeet(l_ttTile);
}

int Platformer_Character::get_tileNumberAtFeet(Tilemap::Tilemap_Tile f_ttRef)
{
	if (p_tilemap == nullptr) return -1;
	int l_iReturn = p_tilemap->get_tileInfo(f_ttRef.x, f_ttRef.y);
	return l_iReturn;
}

float Platformer_Character::get_singleMovementModifier(float f_delta)
{
	//return (f_delta * 1000) * get_scaleSize();
	return (get_scaleSize()) * (f_delta * 1000);
}

bool Platformer_Character::can_snapCamera()
{
	if ((get_locationX() + (get_actualSizeWidth() / 2) > (SCREEN_WIDTH / 2))
		&& (get_locationX() + (get_actualSizeWidth() / 2) < p_camera->get_worldWidth() - (SCREEN_WIDTH / 2)))
		return true;
	return false;
}

bool Platformer_Character::reset_upwardMomentum()
{
	return true;
}

bool Platformer_Character::update_momentumDirectionCheck()
{
	if (m_fMomentumDirectionCheckY == -9001) m_fMomentumDirectionCheckY = get_locationY();
	if (m_fMomentumDirectionCheckY > get_locationY())
		p_momentumV = Momentum_V::mvUp;
	else if (m_fMomentumDirectionCheckY < get_locationY())
		p_momentumV = Momentum_V::mvDown;
	else
		p_momentumV = Momentum_V::mvNone;

	m_fMomentumDirectionCheckY = get_locationY();

	return true;
}

bool Platformer_Character::update_cameraLock()
{
	int l_iCameraLowestY = -1;
	if (p_tilemap != nullptr)
		l_iCameraLowestY = p_tilemap->get_cameraYLowest();
	if (l_iCameraLowestY == -1)
		l_iCameraLowestY = 0;
	Camera *l_camera = nullptr;
	for (int i = -1; i < 10; i++)
	{
		if (i == -1)
			l_camera = p_camera;
		else
			l_camera = p_alternativeCameras[i];

		if (l_camera == nullptr) continue;
		if (l_camera->get_lockId() != p_iCameraLockId) continue;

		float l_fLocationX = get_locationX();
		float l_fSizeWidth = get_actualSizeWidth();

		float l_fCamWidth = l_camera->get_screenWidth();
		float l_fWorldWidth = l_camera->get_worldWidth();

		float l_fCamX = l_camera->get_x();
		float l_fScreenX = l_camera->get_screenX();

		if (l_fLocationX + (l_fSizeWidth / 2) > l_fCamWidth / 2 && l_fLocationX + (l_fSizeWidth / 2) < l_fWorldWidth - (l_fCamWidth / 2))
			l_camera->set_x((l_fLocationX + (l_fSizeWidth / 2)) - (l_fCamWidth / 2) - 20);//The  -20 on the end seems to work perfectly.

		if (m_bHaveEverLanded)
		{
			l_camera->set_scrolledUp(m_bScrollUp);

			if (get_locationY() < l_iCameraLowestY && !m_bScrollUp)
			{
				l_camera->set_y(get_locationY());
			}
			else if (get_locationY() + get_actualSizeHeight() > l_iCameraLowestY && m_bScrollUp)
			{
				m_bScrollUp = false;
			}
		}
		else
		{
			if (get_locationY() > l_iCameraLowestY)
				l_camera->set_y(l_iCameraLowestY);
			else
				l_camera->set_y(get_locationY());
		}
	}


	return true;
}

bool Platformer_Character::move_character(float f_fX, float f_fY, Direction *f_directionReturn)
{
	if (p_tilemap == nullptr)
	{
		//No tilemap? No collisions
		move_location(f_fX, f_fY);
		return true;
	}
	//FALSE MEANS COLLISION
	int l_iNumberOfCollisions = 0;
	SDL_Rect l_rectCollisions[6];
	Tilemap::Tilemap_Tile l_ttReferenceTilemap, l_ttTileset;
	int l_iTileNumber = 0;
	//Firstly let's work out which direction they're going in.
	//X:
	Direction l_directionX = Up;
	if (f_fX < 0)
		l_directionX = Left;
	else if(f_fX > 0)
		l_directionX = Right;

	if (l_directionX == Left)
	{
		float f_fDifforence = (0 + f_fX);
		int l_iNumberPad[3] = {1,4,7};
		for (int i = 0; i < 3; i++)
		{
			float f_newX = f_fX;
			int l_iCount = 0;
			for (int x = 0; x > f_fDifforence; x--)
			{
				/*if(m_controls != nullptr)
					if(m_controls->isKeyDown(Controls::Key_Q))
						cout << ++l_iCount << " f_fX: " << f_fX << "f_newX: " << f_newX << endl;*/

				if (!move_character_innerCheck(Tilemap::Right, l_iNumberPad[i], f_newX, f_fY))
				{
					if (f_directionReturn != nullptr)
						*f_directionReturn = Left;//The thing that stopped us
					return false;
				}
				f_newX++;
			}
		}
	}
	else if (l_directionX == Right)
	{
		float f_fDifforence = f_fX;
		float f_fFurthestDistance = 0;
		bool f_bDidMove = false;
		int l_iNumberPad[3] = { 3,6,9 };
		for (int i = 0; i < 3; i++)
		{
			float f_newX = f_fX;
			for (int x = 0; x < f_fDifforence; x++)
			{
				if (!move_character_innerCheck(Tilemap::Left, l_iNumberPad[i], f_newX, f_fY))
				{
					if (f_directionReturn != nullptr)
						*f_directionReturn = Right;//The thing that stopped us
					return false;
				}
				f_newX--;
			}
		}
	}

	//Y:
	Direction l_directionY = Left;
	if (f_fY < 0)
		l_directionY = Up;
	else if (f_fY > 0)
		l_directionY = Down;

	if (l_directionY == Up)
	{
		float f_fDifforence = (0 + f_fY);
		int l_iNumberPad[3] = { 7,8,9 };
		bool l_bCollisionFound = false;
		bool l_bSpecialCollisionSent = false;
		for (int i = 0; i < 3; i++)
		{
			float f_newY = f_fY;
			for (int y = 0; y > f_fDifforence; y--)
			{
				if (!move_character_innerCheck(Tilemap::Bottom, l_iNumberPad[i], f_fX, f_newY))
				{
					l_ttReferenceTilemap = get_tileReferenceFromNumberPad(l_iNumberPad[i], f_fX, f_newY);
					if (!l_bSpecialCollisionSent)
					{
						p_tilemap->update_specialCollisions(l_ttReferenceTilemap.x, l_ttReferenceTilemap.y, 2, m_iActor, get_playerNumberFromBases());
					}
					l_bSpecialCollisionSent = true;
					l_bCollisionFound = true;
				}
				f_newY++;
			}
				

		}
		if (l_bCollisionFound)
		{
			if (f_directionReturn != nullptr)
				*f_directionReturn = Up;//The thing that stopped us
			return false;
		}
			
	}
	else if (l_directionY == Down)
	{
		float f_fDifforence = (0 + f_fY);
		int l_iNumberPad[3] = { 1,2,3 };
		for (int i = 0; i < 3; i++)
		{
			int l_iNumberPad = 1;
			float l_fXTest = f_fX - get_scaleSize();

			float f_newY = f_fY;
			for (int y = 0; y < f_fDifforence; y++)
			{
				if (move_character_innerCheck(Tilemap::Right, 4, l_fXTest, f_newY))//If we're not up against a wall to the left
					if (!move_character_innerCheck(Tilemap::Top, l_iNumberPad, f_fX, f_newY)) //Test bottom left
					{
						if (f_directionReturn != nullptr)
							*f_directionReturn = Down;//The thing that stopped us
						
						//When I redid my collisions to check not only the destination but everywhere travelled in increments of one
						//(system you're seeing here, this hot fix for mushrooms:
						//Stopped being needed.
						if (p_animation == Falling)// || p_animation == Jumping)
						{
							l_ttReferenceTilemap = get_tileReferenceFromNumberPad(l_iNumberPad, f_fX, f_newY);
							SDL_Rect l_rectCollisionBox = p_tilemap->get_tileCollisionFromReference(l_ttReferenceTilemap);

							if (p_animation == Falling)
								set_location(get_locationX(), l_rectCollisionBox.y - ((get_singleHeight() + 1) * get_scaleSize()));
							else if (p_animation == Jumping)
								adjust_ifStuckInTopLip(l_rectCollisionBox);

							p_animation = None;
							//move_location(0, -get_scaleSize());
							//cout << "YH: " << l_iNumberPad << endl;
						}

						return false;
					}
				f_newY--;
			}

			l_iNumberPad = 3;
			l_fXTest = f_fX + get_scaleSize() * 2;

			f_newY = f_fY;
			for (int y = 0; y < f_fDifforence; y++)
			{
				if (move_character_innerCheck(Tilemap::Left, 6, l_fXTest, f_newY))//If we're not up against a wall to the right
					if (!move_character_innerCheck(Tilemap::Top, l_iNumberPad, f_fX, f_newY)) //Test bottom right
					{
						if (f_directionReturn != nullptr)
							*f_directionReturn = Down;//The thing that stopped us

						return false;
					}
				f_newY--;
			}


			l_iNumberPad = 2;

			f_newY = f_fY;
			for (int y = 0; y < f_fDifforence; y++)
			{
				if (!move_character_innerCheck(Tilemap::Top, l_iNumberPad, f_fX, f_newY))//Always check below
				{
					if (f_directionReturn != nullptr)
						*f_directionReturn = Down;//The thing that stopped us
					/*if (p_animation == Falling)
					{
						l_ttReferenceTilemap = get_tileReferenceFromNumberPad(l_iNumberPad, f_fX, f_newY);
						SDL_Rect l_rectCollisionBox = m_tilemap->get_tileCollisionFromReference(l_ttReferenceTilemap);

						set_location(get_locationX(), l_rectCollisionBox.y - (get_singleHeight() * get_scaleSize()));
						p_animation = None;
						//move_location(0, -get_scaleSize());
						//cout << "YH: " << l_iNumberPad << endl;
					}*/
					return false;
				}
				f_newY--;
			}
				
		}
	}

	SDL_Rect l_rectPlayerCollision = get_collisionRect();
	l_rectPlayerCollision.x += f_fX;
	l_rectPlayerCollision.y += f_fY;
	for (int i = 0; i < l_iNumberOfCollisions; i++)
		if (Utility::ReturnCollision(l_rectPlayerCollision, l_rectCollisions[i]))
		{
			//cout << "YH: " << m_iDebugNumber++ << endl;
			return false;
		}
			

	move_location(f_fX, f_fY);

	if (p_iInvincibilityFrames > 0) return true;
	//p_iFeedback = -1;
	update_entiesReactions(l_directionY, l_directionX);
	return true;
}

bool Platformer_Character::update_entiesReactions(int f_iDirectionY, int f_iDirectionX)
{
	bool l_bShouldCheckSpecialBlocks = false;
	if (m_iActor > 0) l_bShouldCheckSpecialBlocks = true; //If I'm not a player I should interact with the world
	switch (f_iDirectionY)
	{
	case Up:
		p_tilemap->update_entiesReactions(get_collisionRect(), 2, m_iActor, get_playerNumberFromBases(), &p_iFeedback, l_bShouldCheckSpecialBlocks);
		break;
	case Down:
		p_tilemap->update_entiesReactions(get_collisionRect(), 8, m_iActor, get_playerNumberFromBases(), &p_iFeedback , l_bShouldCheckSpecialBlocks);
		break;
	}
	switch (f_iDirectionX)
	{
	case Left:
		p_tilemap->update_entiesReactions(get_collisionRect(), 6, m_iActor, get_playerNumberFromBases(), &p_iFeedback, l_bShouldCheckSpecialBlocks);
		break;
	case Right:
		p_tilemap->update_entiesReactions(get_collisionRect(), 4, m_iActor, get_playerNumberFromBases(), &p_iFeedback, l_bShouldCheckSpecialBlocks);
		break;
	}
	return true;
}

bool Platformer_Character::move_character_innerCheck(Tilemap::tmCollisionSide f_tmcDirection, int f_iNumberpadDirection, float &f_fX, float &f_fY, bool f_bRecurrsion)
{
	Tilemap::Tilemap_Tile l_ttReferenceTilemap, l_ttTileset;
	int l_iTileNumber = 0;
	SDL_Rect l_rectPlayerCollision = get_collisionRect();
	l_rectPlayerCollision.x += f_fX;
	l_rectPlayerCollision.y += f_fY;

	l_ttReferenceTilemap = get_tileReferenceFromNumberPad(f_iNumberpadDirection, f_fX, f_fY);
	SDL_Rect l_rectCollisionBox = p_tilemap->get_tileCollisionFromReference(l_ttReferenceTilemap);
	l_iTileNumber = p_tilemap->get_tileInfo(l_ttReferenceTilemap.x, l_ttReferenceTilemap.y);
	l_ttTileset = p_tilemap->get_tileInTilesetLocation(l_iTileNumber);
	if (l_iTileNumber == -1) return true;
	if (p_tilemap->get_collision(l_ttTileset, f_tmcDirection))
	{
		if (f_tmcDirection == Tilemap::Left || f_tmcDirection == Tilemap::Right)
		{
			if (Utility::ReturnCollision(l_rectCollisionBox, l_rectPlayerCollision))
			{
				//Firstly attempt to nudge the player Up
				if (!f_bRecurrsion)
				{
					float l_fYNew = f_fY - get_scaleSize();
					if (move_character_innerCheck(f_tmcDirection, f_iNumberpadDirection, f_fX, l_fYNew, true))
					{
						f_fY = l_fYNew;
						return true;
					}
					l_fYNew = f_fY + (get_scaleSize());
					if (move_character_innerCheck(f_tmcDirection, f_iNumberpadDirection, f_fX, l_fYNew, true))
					{
						f_fY = l_fYNew;
						return true;
					}
				}
				return false;
			}
			else
				return true;
		}

		if (f_tmcDirection == Tilemap::Bottom)
		{
			SDL_Rect l_rectPlayerCollisionTop = { l_rectPlayerCollision.x,l_rectPlayerCollision.y, l_rectPlayerCollision.w,get_scaleSize() };

			//SDL_Rect l_rectCollisionBoxBottom = { l_rectCollisionBox.x, l_rectCollisionBox.y + l_rectCollisionBox.h - get_scaleSize() * 2, l_rectCollisionBox.w, get_scaleSize() * 2 };
			SDL_Rect l_rectCollisionBoxBottom = { l_rectCollisionBox.x, l_rectCollisionBox.y + l_rectCollisionBox.h - get_scaleSize(), l_rectCollisionBox.w, get_scaleSize() };
			if (Utility::ReturnCollision(l_rectPlayerCollisionTop, l_rectCollisionBoxBottom))
			{
				//Firstly attempt to nudge the player back
				if (!f_bRecurrsion)
				{
					float l_fXNew = f_fX - get_scaleSize();
					if (move_character_innerCheck(f_tmcDirection, f_iNumberpadDirection, l_fXNew, f_fY, true))
					{
						f_fX = l_fXNew;
						return true;
					}
					l_fXNew = f_fX + get_scaleSize();
					if (move_character_innerCheck(f_tmcDirection, f_iNumberpadDirection, l_fXNew, f_fY, true))
					{
						f_fX = l_fXNew;
						return true;
					}
				}
				return false;
			}
		}

		if (f_tmcDirection == Tilemap::Top)
		{
			SDL_Rect l_rectPlayerCollisionBottom = { l_rectPlayerCollision.x + get_scaleSize(),l_rectPlayerCollision.y + l_rectPlayerCollision.h - get_scaleSize(), l_rectPlayerCollision.w - get_scaleSize(),get_scaleSize() * 2 };
			//SDL_Rect l_rectPlayerCollisionBottom = { l_rectPlayerCollision.x,l_rectPlayerCollision.y + l_rectPlayerCollision.h - get_scaleSize(), l_rectPlayerCollision.w,get_scaleSize() };

			SDL_Rect l_rectCollisionBoxTop = { l_rectCollisionBox.x, l_rectCollisionBox.y, l_rectCollisionBox.w, get_scaleSize() * 2 };
			if (Utility::ReturnCollision(l_rectPlayerCollisionBottom, l_rectCollisionBoxTop))
			{
				if (!f_bRecurrsion)
				{
					float l_fXNew = f_fX - get_scaleSize();
					if (move_character_innerCheck(f_tmcDirection, f_iNumberpadDirection, l_fXNew, f_fY, true))
					{
						f_fX = l_fXNew;
						return true;
					}
					l_fXNew = f_fX + get_scaleSize();
					if (move_character_innerCheck(f_tmcDirection, f_iNumberpadDirection, l_fXNew, f_fY, true))
					{
						f_fX = l_fXNew;
						return true;
					}
				}

				//There is a collision so we're going to return false (no movement) however the character may have clipped into the block.
				if (Utility::ReturnCollision(l_rectPlayerCollisionBottom, l_rectCollisionBox))
				{
					//They're trapped within the box
					adjust_ifStuckInTopLip(l_rectCollisionBox);

				}
				return false;
			}
			//l_rectCollisions[l_iNumberOfCollisions++] = m_tilemap->get_tileCollisionFromReference(l_ttReferenceTilemap);
		}
	}
	return true;
}

Tilemap::Tilemap_Tile Platformer_Character::get_tileReferenceFromNumberPad(int f_iLocation)
{
	SDL_Rect l_rectCollision = get_collisionRect();
	return get_tileReferenceFromNumberPad(f_iLocation, 0, 0);
}

Tilemap::Tilemap_Tile Platformer_Character::get_tileReferenceFromNumberPad(int f_iLocation, int f_iX, int f_iY)
{
	Tilemap::Tilemap_Tile l_ttReturn;
	SDL_Rect l_rectCollision = get_collisionRect();
	l_rectCollision.x += f_iX;
	l_rectCollision.y += f_iY;

	int l_iXModify = 0, l_iYModify = 0;
	switch (f_iLocation)
	{
	case 8:
	case 2:
		l_iXModify = (l_rectCollision.w / 2);
		break;
	case 9:
	case 6:
	case 3:
		l_iXModify = l_rectCollision.w - 1;
		break;
	case 1:
	case 4:
	case 7:
		l_iXModify += 1;
		break;
	}
	switch (f_iLocation)
	{
	case 4:
	case 6:
		l_iYModify = (l_rectCollision.h / 2);
		break;
	case 1:
	case 2:
	case 3:
		l_iYModify = l_rectCollision.h - 1;
		break;
	case 7:
	case 8:
	case 9:
		//So if you test 7,8,9 and are a small sprite it doesn't test the tile above even when you're clearly able to jump.
		//This is a hot fix for that
		//*4 of scalesize works on Mario/Luigi
		//l_iYModify -= get_scaleSize() * 4;
		l_iYModify += 1;
		break;
	}
	l_ttReturn = p_tilemap->get_tileFromLocation(l_rectCollision.x + l_iXModify, l_rectCollision.y + l_iYModify);
	return l_ttReturn;
}