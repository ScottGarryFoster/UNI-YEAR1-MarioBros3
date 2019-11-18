#include "Enemy.h"



Enemy::Enemy()
{
	p_animation = Falling;

}


Enemy::~Enemy()
{
}

bool Enemy::Update(float f_deltaTime)
{
	if (m_enemyHealth == ehDead)
	{
		update_death(f_deltaTime);
		return false;
	}

	//Used for us activating / updating
	int l_iCurrentLocationX = get_locationX();
	int l_iCurrentLocationY = get_locationY();

	//cout << "p_animation: " << p_animation << endl;
	//Do things:
	if (!m_bSBeingHeld)
	{
		if (m_bHaveStoodOnAPlatform)//Just makes sure we're on the ground first
		{
			if(m_bHasBeenOnScreen)
				update_movement(f_deltaTime);
		}
		else
		{
			if (p_tilemap != nullptr)
				Platformer_Character::Update(f_deltaTime);

			m_fTimePassed += f_deltaTime;
			if(m_fTimePassed > 750)
				m_bHaveStoodOnAPlatform = true;
		}
	}
	else
		update_beingHeldOnUpdate(f_deltaTime);

	update_additionalBehaviour(f_deltaTime);

	//Used for us activating / updating
	int l_iHDirection = 0;
	if(l_iCurrentLocationX > get_locationX())
		l_iHDirection = 6;
	else if (l_iCurrentLocationX < get_locationX())
		l_iHDirection = 4;

	int l_iVDirection = 0;
	if (l_iCurrentLocationY > get_locationY())
		l_iVDirection = 8;
	else if (l_iCurrentLocationY < get_locationY())
		l_iVDirection = 2;

	for (int i = 0; i < 2; i++)
	{
		if (p_players[i] == nullptr) continue;
		if (p_players[i]->get_deathState() != SMB_PlayerPlatformer::DeathState::dsAlive) continue;
		//cout << "IN" << endl;
		if(l_iHDirection != 0)
			update_actionActivation(p_players[i]->get_collisionRect(), l_iHDirection, 0, *p_players[i]->get_isBigPtr(), p_players[i]->get_feedbackInt());
		if (l_iVDirection != 0)
			update_actionActivation(p_players[i]->get_collisionRect(), l_iVDirection, 0, *p_players[i]->get_isBigPtr(), p_players[i]->get_feedbackInt());
		//cout << "OUT" << endl;
	}
	
	if (m_iActivationBuffer > 0)
	{
		m_iActivationBuffer -= f_deltaTime;
		if (m_iActivationBuffer < 0)
			m_iActivationBuffer = 0;
	}
	if (m_iOtherEnemyActivationBuffer > 0)
	{
		m_iOtherEnemyActivationBuffer -= f_deltaTime;
		if (m_iOtherEnemyActivationBuffer < 0)
			m_iOtherEnemyActivationBuffer = 0;
	}

	if (get_didDrawThisFrame() && !m_bHasBeenOnScreen)
	{
		m_bHasBeenOnScreen = true;
		m_enemyDirection = edLeft;
	}
	return true;
}

bool Enemy::Draw()
{
	RenderToScreen(p_sb->get_renderer());
	return true;
}

bool Enemy::load_fromFile(string f_sLocation)
{
	if (f_sLocation == "") return false;
	m_sMetaDataFile = f_sLocation;
	ifstream infile(f_sLocation);
	if (!infile.is_open())
	{
		cout << "bool Item::load_fromFile(string " << f_sLocation << "):  Could not open file." << endl;
		return false;
	}

	string f_sGetLine = "", f_sContent = "";
	int f_iLine = 0;
	while (getline(infile, f_sGetLine))
	{
		if (f_sGetLine == "Meta")
		{
			while (f_sGetLine != "---")
			{
				getline(infile, f_sGetLine);
				f_sContent += f_sGetLine + "~";
			}
			if (!setup_meta(f_sContent))
				cout << "bool Item::load_fromFile: Animation not loaded" << endl;
		}
		else if (f_sGetLine == "Effect")
		{
			int f_iNextIndex = -1;
			for (int i = 0; i < m_effectsLength; i++)
				if (m_effects[i] == nullptr)
				{
					f_iNextIndex = i; break;
				}
			if (f_iNextIndex == -1) continue;
			m_effects[f_iNextIndex] = new EnemyEffect();
			m_effects[f_iNextIndex]->set_enemyState(&m_iEnemyState);
			f_sContent = "";
			while (f_sGetLine != "---")
			{
				getline(infile, f_sGetLine);
				f_sContent += f_sGetLine + "~";
			}

			if (!m_effects[f_iNextIndex]->setup_effect(f_sContent))
				cout << "Effect not loaded" << endl;
		}
	}

	/*for (int i = 0; i < m_effectsLength; i++)
	{
		if (m_effects[i] != nullptr)
		{
			cout << "i: " << i << endl;
			m_effects[i]->debug_enemyEffect();
		}
	}*/


	infile.close();

	m_bHaveStoodOnAPlatform = false;
	return true;
}

bool Enemy::setup_meta(string f_sData)
{
	LinkedList *l_llLines = Utility::SplitStringToLL(f_sData, '~');
	if (l_llLines->length() <= 1) return false;
	string l_sWholeLine = "";
	for (int line = 0; line < l_llLines->length(); line++)
	{
		l_sWholeLine = l_llLines->read_data(line, l_sWholeLine);
		//cout << "l_sWholeLine: " << l_sWholeLine << endl;
		if (l_sWholeLine == "") continue;
		if (!Utility::SearchStringForChar(l_sWholeLine, ' ')) continue;
		string f_sParameter = l_sWholeLine, f_sValue = "";
		if (!Utility::SplitStringAtPoint(&f_sParameter, &f_sValue, ' '))continue;
		set_byString(f_sParameter, f_sValue);
	}
	delete l_llLines;

	load_audio();

	return true;
}

bool Enemy::load_audio()
{
	if (p_audio == nullptr) return false;

	if (m_iAudioBump == -1)
		m_iAudioBump = p_audio->find_soundEffectNeverFail("Audio/SoundEffect/Mario3/smb3_bump.wav");

	if(m_iAudioKick == -1)
		m_iAudioKick = p_audio->find_soundEffectNeverFail("Audio/SoundEffect/Mario3/smb3_kick.wav");

	if(m_iAudioStomp == -1)
		m_iAudioStomp = p_audio->find_soundEffectNeverFail("Audio/SoundEffect/Mario3/smb3_stomp.wav");

	return true;
}

bool Enemy::set_byString(string f_sParameter, string f_sValue)
{
	string f_sParaUpper = Utility::StringToUpperCase(f_sParameter);//Shouldn't be case sensative
	string f_sValueUpper = Utility::StringToUpperCase(f_sValue);//Shouldn't be case sensative

	//cout << "para: " << f_sParaUpper << endl;
	//cout << "value: " << f_sValueUpper << endl;

	if (f_sParaUpper == "" || f_sValueUpper == "")
		return false;
	else if (f_sParaUpper == "CHARACTERSHEET:")
	{
		m_sCharacterSheet = f_sValue;
		if (!LoadTextureFromFile(p_sb->get_renderer(), "Images/Mario3/"+ m_sCharacterSheet +".png"))
			cout << "Enemy::main_loadContent: Couldn't load: " << "Images/Mario3/" + m_sCharacterSheet + ".png" << endl;
		return true;
	}
	else if (f_sParaUpper == "SINGLESIZEWIDTH:")
	{
		int l_iNumber = Utility::convert_stringToInt(f_sValueUpper);
		if (l_iNumber == -1) return false;
		m_iSingleSizeWidth = l_iNumber;
		if(m_iSingleSizeHeight != 0 && m_iSingleSizeWidth != 0)
			set_singleSize(m_iSingleSizeWidth, m_iSingleSizeHeight);
		return true;
	}
	else if (f_sParaUpper == "SINGLESIZEHEIGHT:")
	{
		int l_iNumber = Utility::convert_stringToInt(f_sValueUpper);
		if (l_iNumber == -1) return false;
		m_iSingleSizeHeight = l_iNumber;
		if (m_iSingleSizeHeight != 0 && m_iSingleSizeWidth != 0)
			set_singleSize(m_iSingleSizeWidth, m_iSingleSizeHeight);
		return true;
	}
	else if (f_sParaUpper == "SINGLELOCATIONROW:")
	{
		int l_iNumber = Utility::convert_stringToInt(f_sValueUpper);
		if (l_iNumber == -1) return false;
		m_iSingleSizeRow = l_iNumber;
		set_singleLocation(m_iSingleSizeRow, m_iSingleSizeCol);
		return true;
	}
	else if (f_sParaUpper == "SINGLELOCATIONCOL:")
	{
		int l_iNumber = Utility::convert_stringToInt(f_sValueUpper);
		if (l_iNumber == -1) return false;
		m_iSingleSizeCol = l_iNumber;
		set_singleLocation(m_iSingleSizeRow, m_iSingleSizeCol);
		return true;
	}
	else if (f_sParaUpper == "ENEMYMOVEMENT:")
	{
		if (f_sValueUpper == "WALK") m_enemyMovement = emWalk;
		if (f_sValueUpper == "WALKDONTFALL") m_enemyMovement = emWalkDontFall;
		return true;
	}
	else if (f_sParaUpper == "ENEMYDIRECTION:")
	{
		if (f_sValueUpper == "RIGHT") m_enemyDirection = edRight;
		if (f_sValueUpper == "LEFT") m_enemyDirection = edLeft;
		return true;
	}
	else if (f_sParaUpper == "COLLISION:")
	{
		if (f_sValueUpper == "BIG") m_esbSizeOriginal = esbBig;
		if (f_sValueUpper == "SMALL")m_esbSizeOriginal = esbSmall;
		m_esbSize = m_esbSizeOriginal;
		return true;
	}
	else if (f_sParaUpper == "ADDITIONALBEHAVIOUR:")
	{
		if (f_sValueUpper == "KOOPA") m_additionalBehaviours = eabKoopa;
		return true;
	}
	else if (f_sParaUpper == "DEATHSPRITE:")
	{
		m_deathSprite = edsNone;
		if (f_sValueUpper == "ONECOL") m_deathSprite = edsOneCol;
		if (f_sValueUpper == "TWOCOL") m_deathSprite = edsTwoCol;
		if (f_sValueUpper == "FLIP") m_deathSprite = edsFlip;
		return true;
	}

	return false;
}

bool Enemy::update_collision(bool f_bDebug)
{
	switch (m_esbSize)
	{
	case esbBig:
		set_collision(2, 2, 14, 26);
		break;
	case esbSmall:
		set_collision(2, 2, 14, 13);
		break;
	}
	return true;
}

bool Enemy::update_movement(float f_deltaTime)
{
	switch (m_enemyState)
	{
	case esProne:
		switch (m_enemyMovement)
		{
		case emWalk:
			update_movementWalk(f_deltaTime);
			break;
		case emWalkDontFall:
			update_movementWalkDontFall(f_deltaTime);
			break;
		}
		break;
	case esShell:
		update_shell(f_deltaTime);
		break;
	case esMovingShell:
		update_movingShell(f_deltaTime);
		break;
	}
	if(m_enemyState != esProne || m_enemyMovement != emWalkDontFall)
		if (p_tilemap != nullptr)
			Platformer_Character::Update(f_deltaTime);
	return true;
}

bool Enemy::update_movementWalk(float f_deltaTime)
{
	float l_fMovement = 0.1 * f_deltaTime;
	if (m_enemyDirection == edLeft)
		l_fMovement *= -1;
	if (!move_character(l_fMovement, 0) || (m_enemyDirection == edLeft && get_locationX() == 0))
	{
		switch (m_enemyDirection)
		{
		case edLeft:
			m_enemyDirection = edRight;
			break;
		case edRight:
			m_enemyDirection = edLeft;
			break;
		}
	}
	update_walkingAnimation(f_deltaTime);
	return true;
}

bool Enemy::update_movementWalkDontFall(float f_deltaTime, bool f_bStoodOnTile, bool f_bRecur)
{
	float l_fMovement = 0.1 * f_deltaTime;
	if (m_enemyDirection == edLeft)
		l_fMovement *= -1;

	bool l_bShouldChangeDirection = update_walkingDontFallTurn(f_deltaTime);

	/*if (l_bShouldChangeDirection)
		cout << 1 << endl;*/

	if (!move_character(l_fMovement, 0))
	{
		//cout << 2 << endl;
		l_bShouldChangeDirection = true;
	}
	if (m_enemyDirection == edLeft && get_locationX() == 0)
	{
		//cout << 3 << endl;
		l_bShouldChangeDirection = true;
	}

	if(l_bShouldChangeDirection)
	{
		switch (m_enemyDirection)
		{
		case edLeft:
			m_enemyDirection = edRight;
			break;
		case edRight:
			m_enemyDirection = edLeft;
			break;
		}
	}
	update_walkingAnimation(f_deltaTime);

	if (p_tilemap != nullptr)
		Platformer_Character::Update(f_deltaTime);
	return true;
}

bool Enemy::update_walkingDontFallTurn(float f_deltaTime)
{
	bool l_bAmStoodOnTile = false;
	switch (m_enemyDirection)
	{
	case edLeft:
		l_bAmStoodOnTile = get_amStoodOnTile({ -1, 0 });
		break;
	case edRight:
		l_bAmStoodOnTile = get_amStoodOnTile({ 1, 0 });
		break;
	}
	if (l_bAmStoodOnTile)
		return false;
	else
		return true;
}

/*bool Enemy::get_amStoodOnTile()
{
	if (p_tilemap == nullptr) return false;

	float l_fYBefore = get_locationY();
	float l_fXBefore = get_locationX();

	Tilemap::Tilemap_Tile l_ttLocationUnderMe = get_tileStoodOn();
	int l_iTileNumber = p_tilemap->get_tileInfo(l_ttLocationUnderMe.x, l_ttLocationUnderMe.y);
	if (l_iTileNumber == -1)
		return false;

	//The following detects whether there is a collision or not
	Tilemap::Tilemap_Tile l_tttLocationOnTileset = p_tilemap->get_tileInTilesetLocation(l_iTileNumber);
	if (!p_tilemap->get_collision(l_tttLocationOnTileset, Tilemap::Top))
		return false;

	SDL_Rect l_rectTileUnder = p_tilemap->get_tileCollisionFromReference(l_ttLocationUnderMe);//We're sticking to the whole block
	if (Utility::ReturnCollision(get_collisionRect(), l_rectTileUnder))
		return true;
	else
		return false;

}*/

Tilemap::Tilemap_Tile Enemy::get_tileStoodOn(Vector2D f_v2dOffsetPixels)
{
	if (p_tilemap == nullptr) return { -1, -1 };
	Vector2D l_v2dOffsetPixels = f_v2dOffsetPixels;
	if (l_v2dOffsetPixels.m_iX == -9001 || l_v2dOffsetPixels.m_iY == -9001)
		l_v2dOffsetPixels = { 0,0 };

	SDL_Rect l_rectMyRect = get_collisionRect();
	Tilemap::Tilemap_Tile l_ttLocationUnderMe = p_tilemap->get_tileFromLocation(l_rectMyRect.x + (l_rectMyRect.w / 2) + l_v2dOffsetPixels.m_iX, l_rectMyRect.y + l_rectMyRect.h + l_v2dOffsetPixels.m_iY);
	return l_ttLocationUnderMe;
}

bool Enemy::get_amStoodOnTile(Vector2D f_v2dOffsetPixels, bool f_bDebug)
{
	Tilemap::Tilemap_Tile l_ttStoodOnTile = get_tileStoodOn(f_v2dOffsetPixels);
	if (l_ttStoodOnTile.x == -1 || l_ttStoodOnTile.y == -1)
		return false;

	//LinkedList *l_llTile = p_tilemap->get_tileInfo(l_ttStoodOnTile.x, l_ttStoodOnTile.y + 1, nullptr);
	LinkedList *l_llTile = p_tilemap->get_tileInfo(l_ttStoodOnTile.x, l_ttStoodOnTile.y + 1, nullptr);
	if (l_llTile == nullptr) return false;
	
	if (l_llTile->get_data(0) == -1)
		return false;
	
	Tilemap::Tilemap_Tile l_ttTilesetTileLocation = p_tilemap->get_tileInTilesetLocation(l_llTile->get_data(0));
	LinkedList *l_llTilesetTile = p_tilemap->get_tileInformationFromTilesetLocation(l_ttTilesetTileLocation.x, l_ttTilesetTileLocation.y, nullptr);
	if (l_llTilesetTile == nullptr) return false;
	string l_sCollision = l_llTilesetTile->get_data(to_string(0));
	if (f_bDebug) cout << "l_sCollision: " << l_sCollision << endl;
	if (l_sCollision == "Undefined" || l_sCollision == "Unfound")
		return false;
	if (l_sCollision == "X")
		return true;
	else if (l_sCollision[0] == 'X')
		return true;

	return false;
}

bool Enemy::update_walkingAnimation(float f_deltaTime)
{
	set_singleSize(m_iSingleSizeWidth, m_iSingleSizeHeight);
	m_iWalkingAnimationTime += f_deltaTime;
	if (m_iWalkingAnimationTime >= 250)
	{
		m_iWalkingAnimationTime -= 250;
		if (get_sourceCol() == m_iSingleSizeCol)
			set_singleLocation(m_iSingleSizeRow, m_iSingleSizeCol + 1);
		else
			set_singleLocation(m_iSingleSizeRow, m_iSingleSizeCol);
	}
	switch (m_enemyDirection)
	{
	case edLeft:
		set_flip(Sprite::SpriteFlip::None);
		break;
	case edRight:
		set_flip(Sprite::SpriteFlip::Horizontal);
		break;
	}
	return true;
}

bool Enemy::update_shell(float f_deltaTime)
{
	set_singleLocation(m_iSingleSizeRow, m_iSingleSizeCol + 2);
	set_singleSize(16, 16);
	m_esbSize = esbSmall;
	update_collision();
	return true;
}

bool Enemy::update_movingShell(float f_deltaTime)
{
	set_singleSize(16, 16);
	m_esbSize = esbSmall;
	update_collision();

	float l_fMovement = (0.5f + m_fABKoopa_movementModifier) * f_deltaTime;
	if (m_enemyDirection == edLeft)
		l_fMovement *= -1;
	if (!move_character(l_fMovement, 0) || (m_enemyDirection == edLeft && get_locationX() == 0))
	{
		switch (m_enemyDirection)
		{
		case edLeft:
			m_enemyDirection = edRight;
			if(m_fABKoopa_movementModifier > 0) m_fABKoopa_movementModifier /= 2;
			break;
		case edRight:
			m_enemyDirection = edLeft;
			if (m_fABKoopa_movementModifier > 0) m_fABKoopa_movementModifier /= 2;
			break;
		}
		if (p_audio != nullptr && m_iAudioBump > -1)
			p_audio->play_soundEffect(m_iAudioBump);
	}



	m_iWalkingAnimationTime += f_deltaTime;

	if (m_iWalkingAnimationTime < m_iMovingShellAnimationFrame)
		set_singleLocation(m_iSingleSizeRow, m_iSingleSizeCol + 2);
	else if (m_iWalkingAnimationTime < m_iMovingShellAnimationFrame * 2)
		set_singleLocation(m_iSingleSizeRow, m_iSingleSizeCol + 3);
	else if (m_iWalkingAnimationTime < m_iMovingShellAnimationFrame * 3)
		set_singleLocation(m_iSingleSizeRow, m_iSingleSizeCol + 4);
	else if (m_iWalkingAnimationTime < m_iMovingShellAnimationFrame * 4)
		set_singleLocation(m_iSingleSizeRow, m_iSingleSizeCol + 5);
	else if (m_iWalkingAnimationTime < m_iMovingShellAnimationFrame * 5)
		m_iWalkingAnimationTime = 0;

	switch (m_enemyDirection)
	{
	case edLeft:
		set_flip(Sprite::SpriteFlip::None);
		break;
	case edRight:
		set_flip(Sprite::SpriteFlip::Horizontal);
		break;
	}
	return true;
}

bool Enemy::update_actionActivation(SDL_Rect f_rectCollision, int f_iDirection, int f_iActor, bool f_bPlayerSize, int *f_iFeedback, int f_iAdditionalNumber)
{
	if (f_iActor == -1) return false;
	if (f_iActor >= 1 && f_iAdditionalNumber == m_iEnemyID) return false;
	if (m_enemyHealth == ehDead) return false;
	if (m_iActivationBuffer > 0 && f_iActor == 0) return false;
	if (m_iOtherEnemyActivationBuffer > 0 && f_iActor == 1) return false;
	if (m_bSBeingHeld) return false;//If I'm being held don't run the normal code
	update_collision();
	SDL_Rect l_rectMyCollision = get_collisionRect();
	int l_iDirection = update_actionActivationDirectionUpdate(l_rectMyCollision, f_rectCollision);

	if (Utility::ReturnCollision(f_rectCollision, l_rectMyCollision))
	{
		//The hold shell code takes presedent over the effects:
		if (f_iActor == 0 && f_iAdditionalNumber != -1)if (!update_playerHoldCheck(f_iAdditionalNumber))return false;

		for (int i = 0; i < m_effectsLength; i++)
		{
			if (m_effects[i] != nullptr)
				if(m_effects[i]->get_effectConditionState() == (int)m_enemyState)
				{
					m_effects[i]->update_actionActivation(l_iDirection, f_iActor, f_bPlayerSize, f_iFeedback);
					if (f_iFeedback != nullptr)
					{
						int l_iFeedbackRun = *f_iFeedback;
						if (*f_iFeedback == 0 || *f_iFeedback == 2)
						{
							m_enemyHealth = ehDead;
							if(*f_iFeedback == 2)
								m_deathSprite = edsFlip;//You always flip when you just die without stomp
						}
						else if (*f_iFeedback == 3)
						{
							string l_sAddition = m_effects[i]->get_newStateAddition();
							change_state(f_rectCollision, l_iDirection, l_sAddition, m_effects[i]->get_newState(), f_iFeedback);
						}
						else if (*f_iFeedback == 4)
						{
							if (m_iLastTurnedBecauseOfThisEnemy == -1 && f_iAdditionalNumber > -1 && f_iActor == 1)
							{
								if (m_enemyDirection == edLeft)
									m_enemyDirection = edRight;
								else
									m_enemyDirection = edLeft;

								m_iLastTurnedBecauseOfThisEnemy = f_iAdditionalNumber;
							}
						}
						else if (*f_iFeedback == 5)
						{
							if (m_enemyState != esMovingShell)
							{
								m_enemyHealth = ehDead;
								m_deathSprite = edsFlip;//You always flip when hit by a shell
							}
						}
						else if (*f_iFeedback == 6)
						{
							if (m_enemyState == esMovingShell)
							{
								//cout << "m_sMetaDataFile: " << m_sMetaDataFile << endl;
								//cout << "f_iActor: " << f_iActor << " f_iAdditionalNumber: " << f_iAdditionalNumber << " m_iEnemyID:" << m_iEnemyID << endl;
								//m_effects[i]->debug_enemyEffect();
								m_enemyHealth = ehDead;
							}

						}

						if (*f_iFeedback > -1)
						{
							m_fABKoopa_movementModifier = 0;
							if (l_iFeedbackRun == 3 && f_iActor == 0) m_iActivationBuffer = m_iActivationBufferLength;
							if ((l_iFeedbackRun == 4 ||  l_iFeedbackRun == 6) && (f_iActor == 1 || f_iActor == 2)) m_iOtherEnemyActivationBuffer = m_iOtherEnemyActivationBufferLength;
							return false;
						}
						else //No Collision or effect
						{
							if (m_iLastTurnedBecauseOfThisEnemy > -1 && f_iActor == 1 && m_iLastTurnedBecauseOfThisEnemy == f_iAdditionalNumber)
							{
								m_iLastTurnedBecauseOfThisEnemy = -1;
							}
						}
					}
				}
				
		}
	}
	else
	{
		if (m_iLastTurnedBecauseOfThisEnemy > -1 && f_iActor == 1 && m_iLastTurnedBecauseOfThisEnemy == f_iAdditionalNumber)
		{
			m_iLastTurnedBecauseOfThisEnemy = -1;
		}
	}
	return true;
}

bool Enemy::change_state(SDL_Rect f_rectCollision, int f_iDirection, string f_sStateAddition, int f_iNewState, int *f_iFeedback)
{
	SDL_Rect l_rectMyCollision = get_collisionRect();
	if (f_sStateAddition == "AWAYFROMPLAYER")
	{
		int l_iAwayFromPlayer = update_actionActivationDirectionUpdate(l_rectMyCollision, f_rectCollision, true);
		if (l_iAwayFromPlayer == 4)
			f_sStateAddition = "RIGHT";
		else
			f_sStateAddition = "LEFT";
	}
	m_iLastTurnedBecauseOfThisEnemy = -1;
	return update_enemyStateChange(f_iNewState, f_sStateAddition, f_iDirection, f_iFeedback);
}

bool Enemy::update_playerHoldCheck(int f_iPlayerNumber)
{
	if (f_iPlayerNumber < -1 || f_iPlayerNumber > 1) return true;
	if (p_players[f_iPlayerNumber] == nullptr) return true;
	if (m_enemyState != EnemyState::esShell) return true;
	
	if (p_players[f_iPlayerNumber]->get_isProneToHoldShells() && m_iSBPlayerIndex == -1)
	{
		if (!p_players[f_iPlayerNumber]->get_isHoldingShell())
		{
			m_bSBeingHeld = true;
			m_iSBPlayerIndex = f_iPlayerNumber;
			m_iSBLastPlayerIndex = m_iSBPlayerIndex;
			if (p_players[m_iSBPlayerIndex] != nullptr) p_players[m_iSBPlayerIndex]->set_isHoldingShell(true);
		}
		else //The player is technically able to hold you but they are holding their own shell
		{
			m_enemyHealth = ehDead;//Let's just die
			m_deathSprite = edsFlip;//It'll be better that way
		}
		return false;
	}
	else if (!p_players[f_iPlayerNumber]->get_isProneToHoldShells() && m_iSBPlayerIndex == f_iPlayerNumber)
	{
		if (p_players[m_iSBPlayerIndex] != nullptr) p_players[m_iSBPlayerIndex]->set_isHoldingShell(false);
		m_bSBeingHeld = false;
		m_iSBPlayerIndex = -1;
		return false;
	}
	else if (m_iSBPlayerIndex == f_iPlayerNumber)
	{
		//cout << "Enemy::update_playerHoldCheck: FALSE" << endl;
		return false;
	}
	return true;
}

int Enemy::update_actionActivationDirectionUpdate(SDL_Rect f_rectMyRect, SDL_Rect f_rectTheirRect, bool f_bOnlyH, bool f_bDebug)
{
	int l_iTheirBottom = f_rectTheirRect.y + f_rectTheirRect.h;
	int l_iMyCenter = f_rectMyRect.x + (f_rectMyRect.w / 2);
	int l_iTheirCenter = f_rectTheirRect.x + (f_rectTheirRect.w / 2);
	int l_iReturnDirection = 4;

	if (l_iTheirBottom < f_rectMyRect.y + (f_rectMyRect.h * 0.75f) && !f_bOnlyH)
		l_iReturnDirection = 8;//Hit My top
	else if (f_rectTheirRect.y - 4 > f_rectMyRect.y + f_rectMyRect.h && !f_bOnlyH)
		l_iReturnDirection = 2;//Hit my Bottom
	else if (l_iMyCenter <= l_iTheirCenter)
		l_iReturnDirection = 6;//Hit my right
	else if (l_iMyCenter > l_iTheirCenter)
		l_iReturnDirection = 4;//Hit my left

	if (f_bDebug)
		cout << "l_iMyCenter: " << l_iMyCenter << " l_iTheirCenter: " << l_iTheirCenter << endl;

	return l_iReturnDirection;
}

bool Enemy::update_death(float f_deltaTime)
{
	m_iDeathAnimationTime += f_deltaTime;
	if (!m_bAnimatedDeath)
	{
		//If we're dead we need to ensure we weren't being held
		m_bSBeingHeld = false;
		if (m_iSBPlayerIndex > -1)
			if (p_players[m_iSBPlayerIndex] != nullptr)
				p_players[m_iSBPlayerIndex]->set_isHoldingShell(false);
		m_iSBPlayerIndex = -1;//And reset everything

		switch (m_deathSprite)
		{
		case edsOneCol:
			set_singleLocation(m_iSingleSizeRow, m_iSingleSizeCol + 1);
			break;
		case edsTwoCol:
			set_singleLocation(m_iSingleSizeRow, m_iSingleSizeCol + 2);
			break;
		case edsFlip:
			//set_singleLocation(m_iSingleSizeRow, m_iSingleSizeCol);
			set_flip(Sprite::SpriteFlip::Vertical);
			//You're meant to fall down:
			set_disableCollisions(true);
			set_gravityPull(-0.5f);


			break;
		}

		if (m_deathSprite == edsFlip)
		{
			if (p_audio != nullptr && m_iAudioKick > -1)
				p_audio->play_soundEffect(m_iAudioKick);
		}
		else
		{
			if (p_audio != nullptr && m_iAudioStomp > -1)
				p_audio->play_soundEffect(m_iAudioStomp);
		}
	}
	m_bAnimatedDeath = true;
	if(m_deathSprite == edsFlip)
		Platformer_Character::Update(f_deltaTime);

	if (m_iDeathAnimationTime > 500)
		m_bDead = true;
	return true;
}

bool Enemy::update_enemyStateChange(int f_iNewState, string f_sAddition, int f_iDirection, int *f_iFeedback)
{
	if (f_iFeedback != nullptr)
	{
		if (f_iDirection == 8)
		{
			*f_iFeedback = 0;
		}
	}

	m_enemyState = (EnemyState)f_iNewState;
	m_iEnemyState = (int)m_enemyState;
	switch (m_enemyState)
	{
	case esProne:
		set_actorNumber(1);
		//cout << "Enemy::update_enemyStateChange: Change to Prone." << endl;
		break;
	case esShell:
		set_actorNumber(1);
		//cout << "Enemy::update_enemyStateChange: Change to esShell." << endl;
		break;
	case esMovingShell:
		set_actorNumber(2);
		check_movingShellCanMove();
		//cout << "Enemy::update_enemyStateChange: Change to esMovingShell." << endl;
		break;
	}
	if (p_audio != nullptr && m_iAudioKick > -1)
		p_audio->play_soundEffect(m_iAudioKick);

	if (f_sAddition != "")
	{
		if (f_sAddition == "RIGHT")
			m_enemyDirection = edRight;
		else if (f_sAddition == "LEFT")
			m_enemyDirection = edLeft;
	}
	return true;
}

bool Enemy::update_additionalBehaviour(float f_deltaTime)
{
	if (m_additionalBehaviours == eabNone)
	{
		m_enemyState = esProne;
		return true;
	}
	switch (m_additionalBehaviours)
	{
	case eabKoopa:
		update_abKoopa(f_deltaTime);
		break;
	}
	return true;
}

bool Enemy::update_abKoopa(float f_deltaTime)
{
	if (m_additionalBehaviours != eabKoopa)	return true;
	if (m_enemyState != EnemyState::esShell) { m_iABKoopa_shellTick = 0; return true; }
	m_iABKoopa_shellTick += f_deltaTime;
	if (m_iABKoopa_shellTick > m_iABKoopa_shellTickTransform)
	{
		m_iABKoopa_shellTick = 0;
		set_singleLocation(m_iSingleSizeRow, m_iSingleSizeCol);
		m_enemyState = EnemyState::esProne;
		move_location(0, -((m_iSingleSizeHeight / 2) * get_scaleSize()));
		m_esbSize = m_esbSizeOriginal;
		update_collision();
		set_actorNumber(1);//We are just an enemy now

		//We can't be held whilst coming out
		if (m_iSBPlayerIndex > -1)
		{
			if (p_players[m_iSBPlayerIndex] != nullptr) p_players[m_iSBPlayerIndex]->set_isHoldingShell(false);
			m_iActivationBuffer = m_iActivationBufferLength;
		}
		m_bSBeingHeld = false;
		m_iSBPlayerIndex = -1;
		m_iLastTurnedBecauseOfThisEnemy = -1;
	}
	else if (m_iABKoopa_shellTick > m_iABKoopa_shellTickShake)
	{
		for (int i = 1; i <= m_iABKoopa_timesToShake; i++)
		{
			if (m_iABKoopa_shellTick < m_iABKoopa_shellTickShake + (m_iABKoopa_shellTickShake / m_iABKoopa_timesToShake) * i)
			{
				if (m_iABKoopa_shakeModification == 0)
					m_iABKoopa_shakeModification = 2;
				else if (m_iABKoopa_shakeModification > 0)
					m_iABKoopa_shakeModification = -2;
				else
					m_iABKoopa_shakeModification = 2;

				if (i > m_iABKoopa_timesToShake / 2)
				{
					if (i % 2 != 0)
						set_singleLocation(m_iSingleSizeRow + 1, m_iSingleSizeCol + 2);
					else
						set_singleLocation(m_iSingleSizeRow, m_iSingleSizeCol + 2);

					m_iABKoopa_shakeModification *= 2;
				}

				move_location(m_iABKoopa_shakeModification, 0);
				break;
			}
				
		}
	}
	return true;
}

bool Enemy::update_beingHeldOnUpdate(float f_deltaTime)
{
	if (m_iSBPlayerIndex < -1 || m_iSBPlayerIndex > 1) return true;
	if (p_players[m_iSBPlayerIndex] == nullptr) return true;
	if (m_enemyState != EnemyState::esShell) { m_bSBeingHeld = false; return true; }
	Direction l_playerDirection = p_players[m_iSBPlayerIndex]->get_direction();
	if (!p_players[m_iSBPlayerIndex]->get_isProneToHoldShells())
	{
		//Let's ensure the shell moves away:
		int l_iPlayerDirection = 0;
		switch (l_playerDirection)
		{
		case Direction::Right:
			l_iPlayerDirection = 4;
			break;
		default:
		case Direction::Left:
			l_iPlayerDirection = 6;
			break;
		}
		change_state(p_players[m_iSBPlayerIndex]->get_collisionRect(), l_iPlayerDirection, "AWAYFROMPLAYER", (int)EnemyState::esMovingShell, nullptr);

		m_fABKoopa_movementModifier = abs(p_players[m_iSBPlayerIndex]->get_actualSpeed());
		m_iActivationBuffer = m_iActivationBufferLength; //This normally happens and just allows players to react and the shell to get away
		//Then reset the holding vars:
		if (p_players[m_iSBPlayerIndex] != nullptr) p_players[m_iSBPlayerIndex]->set_isHoldingShell(false);
		m_iSBPlayerIndex = -1;
		m_bSBeingHeld = false;
		return true;
	}
	if (p_players[m_iSBPlayerIndex] != nullptr) p_players[m_iSBPlayerIndex]->set_isHoldingShell(true);
	
	SDL_Rect l_playerRect = p_players[m_iSBPlayerIndex]->get_collisionRect();
	switch (l_playerDirection)
	{
	case Direction::Left://Player Is facing left
		set_location(l_playerRect.x + l_playerRect.w, l_playerRect.y + l_playerRect.h - get_actualSizeHeight());
		break;
	case Direction::Right://Player Is facing left
		set_location(l_playerRect.x - get_actualSizeWidth(), l_playerRect.y + l_playerRect.h - get_actualSizeHeight());
		break;
	}

	return true;
}

bool Enemy::check_movingShellCanMove()
{
	int l_iDirectionOverride = 0;
	if (m_iSBLastPlayerIndex > -1)
	{
		if (p_players[m_iSBLastPlayerIndex] != nullptr)
		{
			float l_fPlayerLocationX = p_players[m_iSBLastPlayerIndex]->get_locationX();
			if (l_fPlayerLocationX < get_locationX())
				l_iDirectionOverride = 4;
			else
				l_iDirectionOverride = 6;
		}
	}
	if (!move_character(1, 0))
		if (!move_character(-1, 0))
		{
			bool l_bAddLocation = false;
			bool l_bSwitchDirection = false;
			for (int i = 1; i < 55 * 10; i++)
			{
				if (get_locationX() <= 0) l_bAddLocation = true;
				if(l_bAddLocation || l_bSwitchDirection || l_iDirectionOverride == 6)
					move_location(i, 0);
				else if(l_iDirectionOverride == 4)
					move_location((1 + i) * -1, 0);
				else
					move_location((1 + i) * -1, 0);

				if (move_character(1, 0))
					return true;
				else if(move_character(-1, 0))
					return true;

				if (l_bSwitchDirection)
					l_bSwitchDirection = false;
				else
				{
					l_bSwitchDirection = true;
					i--;
				}
			}
		}
	else
		move_character(-1, 0);
	return true;
}