#include "SMB_PlayerPlatformer.h"



SMB_PlayerPlatformer::SMB_PlayerPlatformer()
{
	set_locksTop(false);

	for (int i = 0; i < 10; i++)
		m_iSimulateKeys[i] = -1;
}


SMB_PlayerPlatformer::~SMB_PlayerPlatformer()
{

}

bool SMB_PlayerPlatformer::Update(float f_deltaTime)
{
	//if(!m_bLinkedScreenUI && m_screenUI != nullptr)
	if (s_iPauseControl != nullptr && s_iFinishedLevel != nullptr)
		if (*s_iFinishedLevel <= 0)
			if (*s_iPauseControl == (int)m_playerControl)
			{
				//We are paused! This only happens if we are made bigger or smaller or powerup
				if (m_bEatingMushroom)
					update_mushroomAnimation(f_deltaTime);
				else if (m_bEatingPowerup)
					update_powerupAnimation(f_deltaTime);
				else if (m_bReduceSize)
					update_reduceSize(f_deltaTime);

			}
			else
			{
				if (m_bDeathAnimation || m_deathState == dsDead)
				{
					//cout << "DEATH ANIMATION" << endl;
					update_animateDeath(f_deltaTime);
					return true;
				}
				else if (m_deathState == dsBubble)
				{
					//cout << "BUBBLE" << endl;
					update_bubble(f_deltaTime);
					return true;
				}
				if (p_audio != nullptr && m_iAudioJump == -1)
					load_audio();

				Platformer_Character::Update(f_deltaTime);
				update_input(f_deltaTime);

				update_sprite(f_deltaTime);

				if(m_deathState != dsDead)
					update_handleFeedback();//Feedback from enemies or anything which can kill me
			}
		else
		{
			Platformer_Character::Update(f_deltaTime);
			update_endOfLevel(f_deltaTime);
		}
	/*
	Enable if you want to check collisions
	if (m_controls != nullptr)
	{
		if (m_controls->isKeyDown(Controls::Key_L))
			move_character(100, 0);
		else if (m_controls->isKeyDown(Controls::Key_J))
			move_character(-100, 0);
		else if (m_controls->isKeyDown(Controls::Key_K))
			move_character(0, 100);
		else if (m_controls->isKeyDown(Controls::Key_I))
			move_character(0, -100);
	}
	*/
	return true;
}

bool SMB_PlayerPlatformer::update_endOfLevel(float f_deltaTime)
{
	if (get_isSimulatingKey(Controls::GameKeys::DPAD_RIGHT))
	{
		p_direction = Right;
		p_animation = Moving;
		move_location((m_fMovementSpeed / 2) * f_deltaTime, 0);
		update_sprite(f_deltaTime);
		set_locksRight(false);
	}
	return true;
}

bool SMB_PlayerPlatformer::load_audio()
{
	if (p_audio == nullptr) return false;

	if (m_iAudioJump == -1)
		m_iAudioJump = p_audio->find_soundEffectNeverFail("Audio/SoundEffect/Mario3/smb3_jump.wav");

	if (m_iAudioPowerup == -1)
		m_iAudioPowerup = p_audio->find_soundEffectNeverFail("Audio/SoundEffect/Mario3/smb3_power-up.wav");

	if (m_iAudio1up == -1)
		m_iAudio1up = p_audio->find_soundEffectNeverFail("Audio/SoundEffect/Mario3/smb3_1-up.wav");

	if (m_iAudioLoseLife == -1)
		m_iAudioLoseLife = p_audio->find_soundEffectNeverFail("Audio/SoundEffect/Mario3/smb3_player_down.wav");

	if (m_iAudioLosePower == -1)
		m_iAudioLosePower = p_audio->find_soundEffectNeverFail("Audio/SoundEffect/Mario3/smb3_pipe.wav");

	if (m_iAudioCoin == -1)
		m_iAudioCoin = p_audio->find_soundEffectNeverFail("Audio/SoundEffect/Mario3/smb3_coin.wav");

	return true;
}

void SMB_PlayerPlatformer::set_screenUI(SMB_ScreenUI *f_screenUI)
{
	m_screenUI = f_screenUI;
	if (m_screenUI == nullptr)
		m_bLinkedScreenUI = false;
	else
	{
		m_screenUI->set_playerCounter(&m_cPlayerLetter);//Player
		m_screenUI->set_pSpeed(&m_iPSpeedUI);//P Speed
	}
}
void SMB_PlayerPlatformer::set_player(PlayerControl f_pc)
{
	m_playerControl = f_pc;
	if (m_playerControl == pcPlayerOne)
	{
		set_singleLocation(0, 0);
		m_cPlayerLetter = 'm';
	}
	else if (m_playerControl == pcPlayerTwo)
	{
		set_singleLocation(1, 0);
		m_cPlayerLetter = 'l';
	}
		
}

bool SMB_PlayerPlatformer::update_input(float f_deltaTime)
{
	update_isProneToHoldShell();
	/*
		Every movement change based on button presses are in here:
	*/
	if (m_controls != nullptr)
	{
		update_pSpeed(f_deltaTime);
		
		update_moving(f_deltaTime);

		if ((m_controls->isGameKeyDown(Controls::BTN_A, (int)m_playerControl) || get_isSimulatingKey(Controls::BTN_A))|| m_iJumpAutoTime > 0)
		{
			if (!m_bPressJump)
			{
				if (!m_bJumping)
				{
					update_jumping(f_deltaTime, true); //The Setup of a jump
				}

				//if (m_playerPowerup == ppFeather && m_iPSpeed == 7 && m_iTimesJump > 0 && (m_iTimesJump < 4 || get_gravityPull() > get_vMovementSpeed() / 4))
				if (m_playerPowerup == ppFeather && m_iPSpeed == 7 && m_iTimesJump > 0 && (m_iTimesJump < 4 || get_vMovementSpeed() > 0))
				{
					//Feather Jump, it's complicated but it works
					set_gravityInversionToMax();
					//set_gravityPull((get_vMovementSpeed() * 2) * -1);
					set_disableGravity(false);
					m_bJumping = false;
					m_bCanJump = false;
					m_iTimesJump = 4;
				}
			}
			m_bPressJump = true;
		}
		else
			m_bPressJump = false;

		update_jumping(f_deltaTime, false);
		
		m_iJumpAutoTime -= f_deltaTime;
		if (m_iJumpAutoTime < 0)
			m_iJumpAutoTime = 0;
	}
	return true;
}

bool SMB_PlayerPlatformer::update_moving(float f_deltaTime)
{
	/*
			LEFT AND RIGHT MOVEMENT
		*/
	m_bArePressingMoveButtons = false;
	if (m_controls->isGameKeyDown(Controls::DPAD_RIGHT, (int)m_playerControl) || get_isSimulatingKey(Controls::DPAD_RIGHT))
	{
		if (p_direction == Direction::Left)
			m_fActualSpeed *= 0.5f;

		if (m_fActualSpeed < 0 && m_iPSpeed > 0 && p_animation != Animation::Jumping)
			m_animationModification = animSkid;
		else
			m_animationModification = animNone;

		if (m_fActualSpeed < m_fMovementSpeed)
			m_fActualSpeed += m_fIncrease * f_deltaTime;

		if (m_fActualSpeed > m_fMovementSpeed)
			m_fActualSpeed = m_fMovementSpeed;

		p_direction = Direction::Right;
		if (p_animation == Animation::None)
			p_animation = Animation::Moving;

		m_bArePressingMoveButtons = true;
	}
	else if (m_controls->isGameKeyDown(Controls::DPAD_LEFT, (int)m_playerControl) || get_isSimulatingKey(Controls::DPAD_LEFT))
	{
		if (p_direction == Direction::Right)
			m_fActualSpeed *= 0.5f;

		if (m_fActualSpeed > 0 && m_iPSpeed > 0 && p_animation != Animation::Jumping)
			m_animationModification = animSkid;
		else
			m_animationModification = animNone;

		if (m_fActualSpeed > -m_fMovementSpeed)
			m_fActualSpeed -= m_fIncrease * f_deltaTime;

		if (m_fActualSpeed < -m_fMovementSpeed)
			m_fActualSpeed = -m_fMovementSpeed;

		p_direction = Direction::Left;
		if (p_animation == Animation::None)
			p_animation = Animation::Moving;

		m_bArePressingMoveButtons = true;
	}
	else if (p_animation == Animation::Moving)
	{
		if (m_fActualSpeed > m_fMovementSpeedDefault / 2 || m_fActualSpeed < (m_fMovementSpeedDefault / 2) * -1)
			m_animationModification = animSlowDown;

		p_animation = Animation::None;
	}

	if (p_animation == Animation::None)
	{
		if (m_fActualSpeed > 0)
		{
			m_fActualSpeed -= (m_fIncrease * 2) * f_deltaTime;
			if (m_fActualSpeed > m_fMovementSpeedDefault)
				m_fActualSpeed -= (m_fIncrease * 2) * f_deltaTime;

			if (m_fActualSpeed < 0)
				m_fActualSpeed = 0;
		}
		else if (m_fActualSpeed < 0)
		{
			m_fActualSpeed += (m_fIncrease * 2) * f_deltaTime;
			if (m_fActualSpeed < -m_fMovementSpeedDefault)
				m_fActualSpeed += (m_fIncrease * 2) * f_deltaTime;

			if (m_fActualSpeed > 0)
				m_fActualSpeed = 0;

		}
		if (m_fActualSpeed == 0 && m_animationModification == animSlowDown)
			m_animationModification = animNone;
	}
	else if (p_animation == Animation::Moving)
	{
		if (m_animationModification == animSlowDown)
			m_animationModification = animNone;
	}

	if (m_fActualSpeed != 0)
	{
		float l_fMovement = f_deltaTime * m_fActualSpeed;
		//move_location((float)l_fMovement, 0);
		//move_character((float)l_fMovement, 0);
		if (!move_character((float)l_fMovement, 0))
		{
			if (m_fActualSpeed > 0)
			{
				m_fActualSpeed -= m_fIncrease * f_deltaTime;

				if (m_iPSpeed > 5)
				{
					m_fActualSpeed /= 2;
				}

				if (m_fActualSpeed < 0)
					m_fActualSpeed = 0;
			}
			else if (m_fActualSpeed < 0)
			{
				m_fActualSpeed += m_fIncrease * f_deltaTime;

				if (m_iPSpeed > 5)
				{
					m_fActualSpeed /= 2;
				}

				if (m_fActualSpeed > 0)
					m_fActualSpeed = 0;
			}
			
			if(p_animation == Animation::Moving)
				p_animation = Animation::None;
		}
	}

	if (get_locationY() >= p_camera->get_worldHeight())
	{
		switch_marioSizeKnown(false);
		m_bDeathAnimation = true;
		set_gravityPull(-1);
		m_fActualSpeed = 0;
	}
	return true;
}

bool SMB_PlayerPlatformer::set_simulateButtonPress(Controls::GameKeys f_gk, bool f_bNewState)
{
	int l_iGameKeyAsNumber = (int)f_gk;
	int l_iFoundElementInArray = -1;
	for (int i = 0; i < 10; i++)
		if (m_iSimulateKeys[i] == l_iGameKeyAsNumber)
		{
			if (f_bNewState) return true;
			l_iFoundElementInArray = i;
			break;
		}
	if (l_iFoundElementInArray == -1)//Didn't find it in the array
	{
		for (int i = 0; i < 10; i++)
			if (m_iSimulateKeys[i] == -1)
			{
				l_iFoundElementInArray = i;
				break;
			}
	}
	if (f_bNewState)
		m_iSimulateKeys[l_iFoundElementInArray] = l_iGameKeyAsNumber;
	else
		m_iSimulateKeys[l_iFoundElementInArray] = -1;
	return true;
}

bool SMB_PlayerPlatformer::get_isSimulatingKey(Controls::GameKeys f_gk)
{
	int l_iGameKeyAsNumber = (int)f_gk;
	for (int i = 0; i < 10; i++)
		if (m_iSimulateKeys[i] == l_iGameKeyAsNumber)
			return true;
	return false;
}

bool SMB_PlayerPlatformer::update_handleFeedback()
{
	if (p_iFeedback == 0)//We stomped on something
	{
		m_bPressJump = false;
		m_bJumping = false;
		m_iJumpAutoTime = 200;
		//p_animation = Animation::None;
		p_iFeedback = -1;
	}
	else if(p_iFeedback == 1)//We've taken Damage
	{
		if (p_iInvincibilityFrames <= 0)
		{
			m_bTakeDamage = true;
			update_takeDamage(0);
		}
		p_iFeedback = -1;


	}
	p_iFeedback = -1;
	return true;
}

bool SMB_PlayerPlatformer::update_takeDamage(float f_deltaTime)
{
	if (m_bTakeDamage)
	{
		p_iInvincibilityFrames = m_iInvincibilityFramesTick;
		//LOAD:
		if (m_bIsBig)
		{
			if (p_audio != nullptr && m_iAudioLosePower > -1)
				p_audio->play_soundEffect(m_iAudioLosePower);
			if (p_audio != nullptr)
				p_audio->pause_music();

			if (m_playerPowerup != ppNone)
			{
				m_bEatingPowerup = true;
				if (s_iPauseControl != nullptr)
				{
					*s_iPauseControl = (int)m_playerControl;
				}

				m_iAnimationMushroom = 0;
				m_iAnimationMushroomGoal = 500;
				m_iAnimationSet = 0;
				m_ppAfterState = ppNone;
			}
			else
			{
				m_bReduceSize = true;
				if (s_iPauseControl != nullptr)
				{
					*s_iPauseControl = (int)m_playerControl;
				}

				m_iAnimationMushroom = 0;
				m_iAnimationMushroomGoal = 500;
				m_iAnimationSet = 0;
			}
		}
		else
		{
			m_bDeathAnimation = true;
		}
		m_bTakeDamage = false;
		return true;
	}
	return true;
}

bool SMB_PlayerPlatformer::update_reduceSize(float f_deltaTime)
{
	if (!m_bReduceSize) return false;
	m_iAnimationMushroom += f_deltaTime;
	int m_iFrameRate = 24; //The animation is based on 24 frames and want to to last a second
	if (m_iAnimationMushroom <= m_iAnimationMushroomGoal)
	{
		for (int i = 0; i < 24; i++)
		{
			if (m_iAnimationMushroom < (1000 / m_iFrameRate) * i)//So this means run a change every 24th of a second
			{
				m_iAnimationSet = i / 2;//And this then is the frame
				switch (m_iAnimationSet)
				{
				case 1:
				case 3:
				case 5:
				case 7:
				case 9:
				case 11:
					set_singleLocation(get_sourceRow(), 25);
					break;
				case 6:
				case 8:
				case 10:
					set_singleLocation(get_sourceRow(), 26);
					break;
				case 0:
				case 2:
				case 4:
					set_singleLocation(get_sourceRow(), 3);
					break;
				}
				break;
			}
		}

	}
	else if (m_iAnimationMushroom >= m_iAnimationMushroomGoal)
	{
		convert_animationPosition(false, true);
		//switch_marioSizeKnown(false);
		m_bReduceSize = false;
		set_singleLocation(get_sourceRow(), 2);
		if (p_audio != nullptr)
			p_audio->resume_music();

		*s_iPauseControl = -1;
		//m_scoreOnScreen->new_scoreKnown(get_locationX(), get_locationY(), 5);
	}
	return false;
}

bool SMB_PlayerPlatformer::update_animateDeath(float f_deltaTime)
{
	m_deathState = dsDead;
	if (!get_disableCollisions())//Just in case this wasn't set.
	{
		//Firstly ensure you weren't holding something:
		set_isHoldingShell(false);
		m_bProneToHoldShell = false;

		set_disableCollisions(true);
		set_gravityPull(-1);
		m_fActualSpeed = 0;
		set_disableGravity(false);//If you jump and die on the same frame you'll be stuck in the air. This fixes that issue by ensuring gravity is enabled.

		if (p_audio != nullptr && m_iAudioLoseLife > -1)
		{
			p_audio->play_soundEffect(m_iAudioLoseLife);
		}
	}

	if (p_audio != nullptr)
		p_audio->pause_music();
	set_singleLocation(get_sourceRow(), 11);
	Platformer_Character::Update(f_deltaTime);
	if (get_locationY() >= p_camera->get_worldHeight())
	{
		//cout << "get_locationY(): " << get_locationY() << "p_camera->get_worldHeight() : " << p_camera->get_worldHeight() << endl;
		m_bDeathAnimation = false;

		bool l_bFoundCompletePerson = false;
		if (s_iStatLives != nullptr)
		{
			if (s_iStatLives > 0)
			{
				for (int i = 0; i < p_playerOthersLength; i++)
				{
					if (p_playerOthers[i] != nullptr)
						if (p_playerOthers[i]->get_deathState() == dsAlive)
						{
							m_deathState = dsBubble;
							if (p_audio != nullptr)
								p_audio->resume_music();
							return true;
						}
						else if (p_playerOthers[i]->get_deathState() == dsComplete)
						{
							l_bFoundCompletePerson = true;
						}

				}
			}
		}
		if (m_deathState != dsBubble)
		{
			m_deathState = dsDead;
			if (s_iEndOfLevel != nullptr)
			{
				if(s_iFinishedLevel != nullptr)
					if(!l_bFoundCompletePerson)//No player completed the level
						*s_iEndOfLevel = 0;
					else
						*s_iFinishedLevel = -1;//Indicate to the level we're dead so finish the level with the player who completed the level
				else
					*s_iEndOfLevel = 0;//If we weren't sent a end of level int just kill the player
			}
				
		}
	}
	return true;
}

bool SMB_PlayerPlatformer::update_jumping(float f_deltaTime, bool f_bSetup)
{
	
	if (f_bSetup)
	{
		//Happens once
		if (p_animation != Animation::Jumping || m_iJumpAutoTime > 0)
		{
			m_fPeakJumpHeight = (get_scaleSize() * m_fPeakJumpOffset) + 8;
			m_fJumpPeakMomentumY = get_locationY() - m_fPeakJumpHeight;

			m_fJumpMomentum = m_bJumpStrength * 1;
			float l_fMovement = 0.05f * 0.15f;
			m_fJumpMomentum += l_fMovement * (get_gravityStrength() * 5);

			m_iTimesJump = 0;
			set_disableGravity(true);
			m_bPressJump = true;
			m_bJumping = false;
			p_animation = Animation::Jumping;
			m_bCanJump = true;
			
			if (p_audio != nullptr && m_iAudioJump > -1)
				p_audio->play_soundEffect(m_iAudioJump);
			//m_iTicks = SDL_GetTicks();//Was used for debugging now not needed
		}
	}
	else if(m_bCanJump)
	{
		p_animation = Animation::Jumping;
		if (m_bPressJump && m_iTimesJump < 4 || m_iTimesJump == 0)
		{
			if (get_locationY() <= m_fJumpPeakMomentumY)
				if (m_bJumping)
				{
					//cout << "PEAK " << m_iTimesJump + 1 << endl;
					//We were jumping before
					m_fJumpPeakMomentumY = get_locationY() - m_fPeakJumpHeight;//Set the target Y
					m_iTimesJump++;
				}

			if (get_locationY() > m_fJumpPeakMomentumY)
			{
				//We've not reached the peak:
				//float l_fMovement = f_deltaTime * 0.15f;
				//m_fJumpMomentum += f_deltaTime * (get_gravityStrength() * (m_bJumpStrength / 2));
				//m_fJumpMomentum += f_deltaTime * m_bJumpStrength;
				m_fJumpMomentum = f_deltaTime * m_bJumpStrength;

				//move_location(0, -m_fJumpMomentum);
				if (!move_character(0, -m_fJumpMomentum))//False means collision
				{
					//cout << "NO JUMP" << endl;
					set_gravityPull(0);
					set_disableGravity(false);
					m_bJumping = false;
					m_bCanJump = false;
				}
				else
				{
					update_momentumDirectionCheck();
					m_bJumping = true;
				}
			}
		}
		else if(m_bJumping)
		{
			//A button stopped being pressed and we were jumping
			//cout << m_iTicks - SDL_GetTicks() << endl;
			
			set_gravityPull(-0.2f);
			set_disableGravity(false);
			m_bJumping = false;
			m_bCanJump = false;
		}
	}
	return true;
}

bool SMB_PlayerPlatformer::update_sprite(float f_deltaTime)
{
	if (p_iInvincibilityFrames > 0)
		update_invincibility(f_deltaTime);//Makes the flashing work
	switch (p_direction)
	{
	case Right:
		set_singleLocation(get_sourceRow(), update_animation(f_deltaTime));
		if (m_animationModification != animSkid)
			set_flip(SpriteFlip::None);
		else
			set_flip(SpriteFlip::Horizontal); //Skid is flipped the other way
		break;
	case Left:
		set_singleLocation(get_sourceRow(), update_animation(f_deltaTime));
		if (m_animationModification != animSkid)
			set_flip(SpriteFlip::Horizontal);
		else
			set_flip(SpriteFlip::None); //Skid is flipped the other way
		break;
	}
	return true;
}

bool SMB_PlayerPlatformer::update_invincibility(float f_deltaTime)
{
	p_iInvincibilityFrames -= f_deltaTime;
	if (p_iInvincibilityFrames > 0)
	{
		for (int i = 0; i <= m_iInvincibilityFlicker; i++)
		{
			if (p_iInvincibilityFrames < (m_iInvincibilityFramesTick / m_iInvincibilityFlicker) * i)
			{
				if (i % 2 == 0)
					set_visable(true);
				else
					set_visable(false);
				break;
			}
		}
	}
	else if (p_iInvincibilityFrames <= 0)
	{
		set_visable(true);
		p_iInvincibilityFrames = 0;
	}
	return true;
}

int SMB_PlayerPlatformer::update_animation(float f_deltaTime)
{
	switch (p_animation)
	{
	case None:
		if(m_animationModification != animSlowDown)
			return get_animation(agNone);
		break;
	case Jumping:
		if (m_iPSpeed == 7)
			return get_animation(agPJump);
		else
			return get_animation(agRegularJump);
	}

	if (p_animation == Moving || m_animationModification == animSlowDown)
	{
		if (m_animationModification == animSkid) return get_animation(agSkid);
		m_iAnimationFrame += f_deltaTime;
		if (m_iAnimationFrame > m_iMovingAnimationTick)
			m_iAnimationFrame -= m_iMovingAnimationTick;

		if (m_iPSpeed < 7 || m_bIsHoldingShell)
		{
			if (m_iAnimationFrame < m_iMovingAnimationTick / 4)
				return get_animation(agMovement1);
			else if (m_iAnimationFrame < m_iMovingAnimationTick / 2)
				return get_animation(agMovement2);
			else if (m_iAnimationFrame < (m_iMovingAnimationTick / 4) * 3)
				return get_animation(agMovement3);
			else
				return get_animation(agMovement4);
		}
		else
		{
			if (m_iAnimationFrame < m_iMovingAnimationTick / 4)
				return get_animation(agPMovement1);
			else if (m_iAnimationFrame < m_iMovingAnimationTick / 2)
				return get_animation(agPMovement2);
			else if (m_iAnimationFrame < (m_iMovingAnimationTick / 4) * 3)
				return get_animation(agPMovement3);
			else
				return get_animation(agPMovement4);
		}
	}
	return 2;
}

int SMB_PlayerPlatformer::get_animationRow()
{
	if (!m_bIsBig)
	{
		switch (m_playerControl)
		{
		case pcPlayerOne:
			return 0;
		case pcPlayerTwo:
			return 1;
		case pcPlayerThree:
			return 3;
		case pcPlayerFour:
			return 4;
		}
	}
	else
	{
		switch (m_playerPowerup)
		{
		case ppNone:
			switch (m_playerControl)
			{
			case pcPlayerOne:
				return 2;
			case pcPlayerTwo:
				return 3;
			case pcPlayerThree:
				return 4;
			case pcPlayerFour:
				return 5;
			}
		case ppFeather:
			switch (m_playerControl)
			{
			case pcPlayerOne:
				return 6;
			case pcPlayerTwo:
				return 7;
			case pcPlayerThree:
				return 8;
			case pcPlayerFour:
				return 9;
			}
		}
	}
	return 0;
}

int SMB_PlayerPlatformer::get_animation(AnimationGenerics f_animation)
{
	switch (f_animation)
	{
	case agNone:
	case agMovement1:
		if(!m_bIsHoldingShell)
			return 2;//Same in both
		else
		{
			if (!m_bIsBig)
				return 12;//Small is 13
			else
				return 14;//Big is 14
		}
	case agMovement2:
	case agMovement4:
		if (!m_bIsHoldingShell)
			return 3;//Same in both
		else
		{
			if (!m_bIsBig)
				return 13;//Small is 13
			else
				return 15;//Big is 14
		}
	case agMovement3:
		if (!m_bIsHoldingShell)
			if (!m_bIsBig)
				return 2;//Small is 13
			else
				return 4;//Big is 14
		else
		{
			if (!m_bIsBig)
				return 13;//Small is 13
			else
				return 16;//Big is 14
		}
	case agRegularJump:
		if (!m_bIsHoldingShell)
			if (!m_bIsBig)
				return 5;	
			else
				return 6;//Big
		else
			if (!m_bIsBig)
				return 13;
			else
				return 16;//Big
	case agPJump:
		if (!m_bIsHoldingShell)
			if (!m_bIsBig)
				return 8;
			else
				return 10;//Big
		else
			return get_animation(agRegularJump);
	case agPMovement1:
		if (!m_bIsHoldingShell)
			if (!m_bIsBig)
				return 6;//Small
			else
				return 7;//Big
		else
			return get_animation(agMovement1);
	case agPMovement2:
		if (!m_bIsHoldingShell)
			if (!m_bIsBig)
				return 7; //Small
			else
				return 8;//Big
		else
			return get_animation(agMovement2);
	case agPMovement3:
		if (!m_bIsHoldingShell)
			if (!m_bIsBig)
				return 6;//Small
			else
				return 9;//Big
		else
			return get_animation(agMovement3);
	case agPMovement4:
		if (!m_bIsHoldingShell)
			if (!m_bIsBig)
				return 7;//Small
			else
				return 8;//Big
		else
			return get_animation(agMovement4);
	case agSkid:
		if (!m_bIsHoldingShell)
			if (!m_bIsBig)
				return 4;
			else
				return 5;
		else
			return get_animation(agNone);
	}
	return 0;
}

bool SMB_PlayerPlatformer::reset_upwardMomentum()
{
	//cout << "YEAH" << endl;
	m_fJumpMomentum = 0;
	m_fJumpPeakMomentum = 0;
	return true;

}

bool SMB_PlayerPlatformer::update_isProneToHoldShell()
{
	if (m_controls->isGameKeyDown(Controls::BTN_B, (int)m_playerControl) || get_isSimulatingKey(Controls::BTN_B))
		m_bProneToHoldShell = true;
	else
		m_bProneToHoldShell = false;
	return true;
}

bool SMB_PlayerPlatformer::update_pSpeed(float f_deltaTime)
{
	
	//B is the run button
	m_iPSpeedTicker += f_deltaTime;
	if (m_iPSpeedTicker > 15)
	{
		m_iPSpeedTicker -= 15;
		if ((m_controls->isGameKeyDown(Controls::BTN_B, (int)m_playerControl) || get_isSimulatingKey(Controls::BTN_B)) && (p_animation == Animation::Moving || p_animation == Animation::Jumping) && m_animationModification == animNone)
		{
			bool l_bWeChangedDirectionOrHitSomething = false;

			if (m_fSpeedWhenAtFullSpeed != -9001)
			{
				if (m_fSpeedWhenAtFullSpeed > 0)
					if (m_fSpeedWhenAtFullSpeed > m_fActualSpeed)
						l_bWeChangedDirectionOrHitSomething = true;

				if (m_fSpeedWhenAtFullSpeed < 0)
					if (m_fSpeedWhenAtFullSpeed < m_fActualSpeed)
						l_bWeChangedDirectionOrHitSomething = true;

				if (m_iPSpeed < 4)
					m_fSpeedWhenAtFullSpeed = -9001;
			}

			if(m_bArePressingMoveButtons && !l_bWeChangedDirectionOrHitSomething)
			{
				m_fPSpeedFull++;
				if (m_fPSpeedFull >= 100)
					m_fPSpeedFull = 100;
				
				if (m_iPSpeed == 7)
					if (m_fSpeedWhenAtFullSpeed == -9001)
						m_fSpeedWhenAtFullSpeed = m_fActualSpeed;
			}
			else
			{
				if(!m_bArePressingMoveButtons)
					m_fSpeedWhenAtFullSpeed = m_fActualSpeed;
				m_fPSpeedFull -= 0.5f;
			}
				
		}
		else if (m_fPSpeedFull > 0)
			m_fPSpeedFull -= 0.5f;

		if (m_iPSpeed == 0)
			m_fSpeedWhenAtFullSpeed = -9001;
	}
	if (m_iPSpeed >= 7)
	{
		m_iPSpeedFlashTicker += f_deltaTime;
		if (m_iPSpeedFlashTicker > 120)
		{
			m_iPSpeedFlashTicker -= 120;
			if (m_bPSpeedFlash)
				m_bPSpeedFlash = false;
			else
				m_bPSpeedFlash = true;

			if (m_bPSpeedFlash)
				m_iPSpeedUI = m_iPSpeed;
			else
				m_iPSpeedUI = m_iPSpeed - 1;
		}
	}
	else if (m_iPSpeedFlashTicker > 0)
		m_iPSpeedFlashTicker = 0;

	if (m_fPSpeedFull >= 100)
	{
		m_iPSpeed = 7;
		m_fPSpeedFull = 100;
	}
	else if (m_fPSpeedFull < 81)
	{
		m_iPSpeed = (m_fPSpeedFull - 20) / 10;
		if (m_iPSpeed < 0) m_iPSpeed = 0;
	}
	if (m_iPSpeed < 7)
		m_iPSpeedUI = m_iPSpeed;
	
	//Then This is the footer:
	if (m_iPSpeed > 1)
	{
		m_fMovementSpeed = m_fMovementSpeedDefault * (1 + m_fSpeedMultipler);
		m_fPeakJumpOffset = m_fPeakJumpOffsetDefault + 6;
		m_iMovingAnimationTick = m_iMovingAnimationTickDefault / (m_iPSpeed / 3.0f);
	}
	else
	{
		m_fMovementSpeed = m_fMovementSpeedDefault;
		m_fPeakJumpOffset = m_fPeakJumpOffsetDefault;
		m_iMovingAnimationTick = m_iMovingAnimationTickDefault;
	}
		

	//cout << "m_bMovementSpeed:" << m_fMovementSpeed << endl;
	return true;
}

bool SMB_PlayerPlatformer::player_eatsItem(string f_sItem)
{
	if (get_deathState() != dsAlive) return false;
	f_sItem = Utility::StringToUpperCase(f_sItem);//Just incase
	if (f_sItem == "FEATHER")
	{
		if (!m_bIsBig)
			f_sItem = "MUSHROOM";//If you're small you can only grow in size
		else if(m_playerPowerup == ppFeather)
		{
			//If you're already in feather mode
			m_scoreOnScreen->new_scoreKnown(get_locationX(), get_locationY(), 5);
		}
		else
		{
			m_bEatingPowerup = true;
			if (s_iPauseControl != nullptr)
			{
				*s_iPauseControl = (int)m_playerControl;
			}

			m_iAnimationMushroom = 0;
			m_iAnimationMushroomGoal = 500;
			m_iAnimationSet = 0;
			m_ppAfterState = ppFeather;

			if (p_audio != nullptr && m_iAudioPowerup > -1)
				p_audio->play_soundEffect(m_iAudioPowerup);
		}
	}

	if (f_sItem == "COIN")
	{
		if (s_iCoinCount != nullptr)
			*s_iCoinCount++;
		if (*s_iCoinCount == 100)
		{
			f_sItem = "ONEUPMUSHROOM";
			*s_iCoinCount -= 100;
		}
		if (p_audio != nullptr && m_iAudioCoin > -1)
			p_audio->play_soundEffect(m_iAudioCoin);
	}

	if (f_sItem == "MUSHROOM")
	{
		if (!m_bIsBig)
		{
			m_bEatingMushroom = true;
			if (s_iPauseControl != nullptr)
			{
				*s_iPauseControl = (int)m_playerControl;
			}

			m_iAnimationMushroom = 0;
			m_iAnimationMushroomGoal = 1000;
			m_iAnimationSet = 0;

			convert_animationPosition(true, false);
		}
		else
		{
			m_scoreOnScreen->new_scoreKnown(get_locationX(), get_locationY(), 5);

		}
		if (p_audio != nullptr && m_iAudioPowerup > -1)
			p_audio->play_soundEffect(m_iAudioPowerup);
	}
	else if (f_sItem == "ONEUPMUSHROOM")
	{
		*s_iStatLives += 1;
		m_scoreOnScreen->new_scoreKnown(get_locationX(), get_locationY(), 9);

		if (p_audio != nullptr && m_iAudio1up > -1)
			p_audio->play_soundEffect(m_iAudio1up);
	}

	return true;
}

bool SMB_PlayerPlatformer::update_mushroomAnimation(float f_deltaTime)
{
	m_iAnimationMushroom += f_deltaTime;
	int m_iFrameRate = 24; //The animation is based on 24 frames and want to to last a second
	if (m_iAnimationMushroom <= m_iAnimationMushroomGoal)
	{
		for (int i = 0; i < 24; i++)
		{
			if (m_iAnimationMushroom < (1000 / m_iFrameRate) * i)//So this means run a change every 24th of a second
			{
				m_iAnimationSet = i / 2;//And this then is the frame
				switch (m_iAnimationSet)
				{
				case 0:
				case 2:
				case 4:
				case 6:
				case 8:
				case 10:
					set_singleLocation(get_sourceRow(), 25);
					break;
				case 1:
				case 3:
				case 5:
					set_singleLocation(get_sourceRow(), 26);
					break;
				case 7:
				case 9:
				case 11:
					set_singleLocation(get_sourceRow(), 3);
					break;
				}
				break;
			}
		}
		
	}
	else if (m_iAnimationMushroom >= m_iAnimationMushroomGoal)
	{
		m_bIsBig = true;
		m_bEatingMushroom = false;
		set_singleLocation(get_sourceRow(), 2);
		convert_animationPosition(true);
		*s_iPauseControl = -1;
		m_scoreOnScreen->new_scoreKnown(get_locationX(), get_locationY(), 5);
	}
	return false;
}

bool SMB_PlayerPlatformer::update_powerupAnimation(float f_deltaTime)
{
	m_iAnimationMushroom += f_deltaTime;
	int m_iFrameRate = 24; //The animation is based on 24 frames and want to to last a second
	if (m_iAnimationMushroom <= m_iAnimationMushroomGoal)
	{
		for (int i = 0; i < 24; i++)
		{
			if (m_iAnimationMushroom < (500 / m_iFrameRate) * i)//So this means run a change every 24th of a second
			{
				m_iAnimationSet = i / 2;//And this then is the frame
				switch (m_iAnimationSet)
				{
				case 0:
				case 4:
					set_singleLocation(0, 23);
					break;
				case 2:
				case 10:
					set_singleLocation(0, 24);
					break;
				case 6:
					set_singleLocation(0, 25);
					break;
				case 8:
					set_singleLocation(0, 26);
					break;
				}
				break;
			}
		}

	}
	else if (m_iAnimationMushroom >= m_iAnimationMushroomGoal)
	{
		m_bEatingPowerup = false;
		m_playerPowerup = m_ppAfterState;
		set_singleLocation(get_animationRow(), 2);
		convert_animationPosition(true);
		if(m_ppAfterState != ppNone)
			m_scoreOnScreen->new_scoreKnown(get_locationX(), get_locationY(), 5);
		if (p_audio != nullptr)
			p_audio->resume_music();

		*s_iPauseControl = -1;
	}
	return false;
}

bool SMB_PlayerPlatformer::switch_marioSizeToggle()
{
	if(m_bIsBig)
		convert_animationPosition(false);
	else
		convert_animationPosition(true);
	return true;
}

bool SMB_PlayerPlatformer::switch_marioSizeKnown(bool f_bNewSize)
{
	if (f_bNewSize != m_bIsBig)
	{
		convert_animationPosition(f_bNewSize);
		return true;
	}
	else
		return false;
}

bool SMB_PlayerPlatformer::convert_animationPosition(bool f_bSize, bool f_bDoNotMove)
{
	if (m_bIsBig == f_bSize) return false;
	int l_iNewSourceCol = get_sourceCol();
	if (m_bIsBig)
	{
		//Old is Big new is Small
		set_singleSize(18, 18);
		if(!f_bDoNotMove)
			move_location(0, 11 * get_scaleSize());
		switch (get_sourceCol())
		{
		case 0:
		case 1:
		case 2:
		case 3:
			break;//These are same big as small
		case 4:
		case 5:
		case 6:
		case 7:
		case 8:
			l_iNewSourceCol--;
			break;
		case 9:
		case 10:
			l_iNewSourceCol -= 2;
			break;
		case 11:
			l_iNewSourceCol = 3;
			break;//There is not equilent
		case 12:
		case 13:
			l_iNewSourceCol -= 3;
			break;
		case 14:
		case 15:
			l_iNewSourceCol -= 12;
			break;
		case 16:
		case 17:
			l_iNewSourceCol = 3;
			break;
		case 18:
		case 19:
		case 20:
			l_iNewSourceCol = 16;
			break;
		case 21:
			l_iNewSourceCol = 17;
			break;
		case 22:
			l_iNewSourceCol = 18;
			break;
		case 23:
		case 24:
			l_iNewSourceCol -= 4;
			break;
		default:
			l_iNewSourceCol = 3;//just stood still
			break;
		}

		if (m_playerControl == pcPlayerOne)
			set_singleLocation(0, l_iNewSourceCol);
		else if (m_playerControl == pcPlayerTwo)
			set_singleLocation(1, l_iNewSourceCol);

		set_collision(2, 2, 14, 14);

		m_iMovingAnimationTickDefault = m_iMovingAnimationTickDefaultSingle * 4;

		m_bIsBig = false;
	}
	else
	{
		//Old is Small new is Big
		set_singleSize(30, 36);
		if (!f_bDoNotMove)
			move_location(0, -18 * get_scaleSize());
		switch (get_sourceCol())
		{
		case 0:
		case 1:
		case 2:
		case 3:
			break;//Same Col
		case 4:
		case 5:
		case 6:
		case 7:
			l_iNewSourceCol++;
			break;
		case 8:
			l_iNewSourceCol += 2;
			break;
		case 9:
		case 10:
			l_iNewSourceCol += 3;
			break;
		case 11:
			l_iNewSourceCol = 3;
			break;//Can't die as big mario
		case 12:
		case 13:
		case 14:
		case 15:
			l_iNewSourceCol += 3;
			break;
		case 16:
		case 17:
		case 18:
		case 19:
		case 20:
			l_iNewSourceCol += 4;
			break;
		default:
			l_iNewSourceCol = 3;//just stood still
			break;
		}

		//Keep in mind the height is double
		if (m_playerControl == pcPlayerOne)
			set_singleLocation(2, l_iNewSourceCol);
		else if (m_playerControl == pcPlayerTwo)
			set_singleLocation(3, l_iNewSourceCol);

		//set_collision(4, 9, 16, 25);
		set_collision(6, 9, 16, 25);

		m_iMovingAnimationTickDefault = m_iMovingAnimationTickDefaultSingle * 2;

		m_bIsBig = true;
	}
	return true;
}

bool SMB_PlayerPlatformer::adjust_ifStuckInTopLip(SDL_Rect f_rectTileUnder)
{
	if(!m_bIsBig)
		set_location(get_locationX(), f_rectTileUnder.y - ((get_singleHeight() - 1) * get_scaleSize()));
	else
	{
		//set_location(get_locationX(), f_rectTileUnder.y - ((get_singleHeight() - 1) * get_scaleSize()));
		//cout << " f_rectTileUnder.y - (get_actualSizeHeight() + get_scaleSize()):" << f_rectTileUnder.y - (get_actualSizeHeight() + get_scaleSize()) << endl;
		set_location(get_locationX(), f_rectTileUnder.y - (get_actualSizeHeight() + 1));// +get_scaleSize()));
		//set_location(get_locationX(), f_rectTileUnder.y - ((get_singleHeight() - 1) * get_scaleSize() * 2));
	}
	return true;
}

bool SMB_PlayerPlatformer::update_bubble(float f_deltaTime)
{
	bool l_bFoundPlayer = false;
	for (int p = 0; p < p_playerOthersLength; p++)
		if (p_playerOthers[p] != nullptr)
			if (p_playerOthers[p]->get_deathState() != dsBubble)
			{
				l_bFoundPlayer = true;
				break;
			}
	
	if (!l_bFoundPlayer || p_camera == nullptr)
	{
		m_deathState = dsDead;
		if (s_iEndOfLevel != nullptr)
			*s_iEndOfLevel = 0;
		return false;
	}
	set_singleLocation(get_sourceRow(), 21);

	//Now let's find the player and head to them:
	int l_iAimTowardPlayer = -1;
	int l_iSmallestDistance = p_camera->get_worldWidth() + p_camera->get_worldHeight();
	CircleCollision *l_ccMyCollision = get_circleCollision();
	Vector2D l_v2dMyLocation = l_ccMyCollision->get_location();
	bool l_bAPlayerHasCompleted = false;
	for (int p = 0; p < p_playerOthersLength; p++)
		if (p_playerOthers[p] != nullptr)
		{
			if (p_playerOthers[p]->get_deathState() == dsComplete)
				l_bAPlayerHasCompleted = true;
			if (p_playerOthers[p]->get_deathState() != dsBubble && p_playerOthers[p]->get_deathState() != dsComplete)
			{
				CircleCollision *l_ccOtherPlayer = p_playerOthers[p]->get_circleCollision();
				int l_iOtherPlayerDistance = Utility::circleCollisionDistance(l_ccMyCollision, l_ccOtherPlayer);
				if (l_iOtherPlayerDistance < l_iSmallestDistance)
				{
					l_iSmallestDistance = l_iOtherPlayerDistance;
					l_iAimTowardPlayer = p;
				}
			}
		}
	if (l_iAimTowardPlayer == -1)
	{
		m_deathState = dsDead;
		if (!l_bAPlayerHasCompleted)
		{
			if (s_iEndOfLevel != nullptr)
				*s_iEndOfLevel = 0;
		}
		else
			if (s_iEndOfLevel != nullptr)
				*s_iEndOfLevel = -1;

		return false;
	}

	CircleCollision *l_ccAimingToward = p_playerOthers[l_iAimTowardPlayer]->get_circleCollision();
	Vector2D l_v2dOtherPlayerPosition = l_ccAimingToward->get_location();

	if (Utility::circleCollisionCheck(l_ccMyCollision, l_ccAimingToward))
	{
		set_disableCollisions(false);
		set_gravityPull(-1);
		m_fActualSpeed = 0;
		m_deathState = dsAlive;
		return true;
	}

	int l_iOtherPlayerDistance = Utility::circleCollisionDistance(l_ccMyCollision, l_ccAimingToward);
	if (m_bubbleMode == bmDefault)
		if (l_iOtherPlayerDistance < get_actualSizeWidth() * 3)
		{
			m_bubbleMode = bmHover;
			m_fActualSpeed = 0;
		}
	
	if (m_fActualSpeed < (m_fMovementSpeed / 2))
	{
		m_fActualSpeed += (m_fIncrease / 2) * f_deltaTime;
		if (m_fActualSpeed > (m_fMovementSpeed / 2))
			m_fActualSpeed = (m_fMovementSpeed / 2);
	}
	if (m_fBubbleVSpeed < (m_fMovementSpeed / 2))
	{
		m_fBubbleVSpeed += (m_fIncrease / 2) * f_deltaTime;
		if (m_fBubbleVSpeed > (m_fMovementSpeed / 2))
			m_fBubbleVSpeed = (m_fMovementSpeed / 2);
	}

	if (m_bubbleMode == bmDefault)
	{
		if (l_v2dOtherPlayerPosition.m_iX > l_v2dMyLocation.m_iX)
		{
			move_location(m_fActualSpeed * f_deltaTime, 0);
			if (l_v2dOtherPlayerPosition.m_iX < l_v2dMyLocation.m_iX)
				m_fActualSpeed /= 2;
		}
		else if (l_v2dOtherPlayerPosition.m_iX < l_v2dMyLocation.m_iX)
		{
			move_location(-(m_fActualSpeed * f_deltaTime), 0);
			if (l_v2dOtherPlayerPosition.m_iX > l_v2dMyLocation.m_iX)
				m_fActualSpeed /= 2;
		}

		if (l_v2dOtherPlayerPosition.m_iY > l_v2dMyLocation.m_iY)
		{
			move_location(0, m_fBubbleVSpeed * f_deltaTime);
			if (l_v2dOtherPlayerPosition.m_iY < l_v2dMyLocation.m_iY)
				m_fBubbleVSpeed /= 2;
		}
		else if (l_v2dOtherPlayerPosition.m_iY < l_v2dMyLocation.m_iY)
		{
			move_location(0, -(m_fBubbleVSpeed * f_deltaTime));
			if (l_v2dOtherPlayerPosition.m_iY > l_v2dMyLocation.m_iY)
				m_fBubbleVSpeed /= 2;
		}
	}
	else if (m_bubbleMode == bmHover)
	{
		m_iBubbleModeHoverTick -= f_deltaTime;
		if (m_iBubbleModeHoverTick <= 0)
			if (l_iOtherPlayerDistance > get_actualSizeWidth() * 5)
			{
				m_bubbleMode = bmDefault;
				m_fActualSpeed = 0;
				m_iBubbleModeHoverTick = 0;
			}
			else
			{
				m_fBubbleVSpeed /= 2;
				m_iBubbleModeHoverTick = 500;

				if (m_bBubbleHoverDirection) m_bBubbleHoverDirection = false; else m_bBubbleHoverDirection = true;
			}

		if(m_bBubbleHoverDirection)
			move_location(0, m_fBubbleVSpeed * f_deltaTime);
		else
			move_location(0, -(m_fBubbleVSpeed * f_deltaTime));
	}
	update_cameraLock();//We've got to do this now as we're not using the rest of platform character
	return true;
}

bool SMB_PlayerPlatformer::update_entiesReactions(int f_iDirectionY, int f_iDirectionX)
{
	switch (f_iDirectionY)
	{
	case Up:
		p_tilemap->update_entiesReactions(get_collisionRect(), 2, get_actorNumber(), get_playerNumberFromBases(), &p_iFeedback, false, (int)m_playerControl - 1);
		break;
	case Down:
		p_tilemap->update_entiesReactions(get_collisionRect(), 8, get_actorNumber(), get_playerNumberFromBases(), &p_iFeedback, false, (int)m_playerControl - 1);
		break;
	}
	switch (f_iDirectionX)
	{
	case Left:
		p_tilemap->update_entiesReactions(get_collisionRect(), 6, get_actorNumber(), get_playerNumberFromBases(), &p_iFeedback, false, (int)m_playerControl - 1);
		break;
	case Right:
		p_tilemap->update_entiesReactions(get_collisionRect(), 4, get_actorNumber(), get_playerNumberFromBases(), &p_iFeedback, false, (int)m_playerControl - 1);
		break;
	}
	return true;
}

Platformer_Character::Direction SMB_PlayerPlatformer::get_direction()
{
	SpriteFlip l_flip = get_flip();
	switch (l_flip)
	{
	case Horizontal:
		if(m_animationModification != animSkid)
			return Platformer_Character::Direction::Right;
		else
			return Platformer_Character::Direction::Left;
	default:
	case None:
		if (m_animationModification != animSkid)
			return Platformer_Character::Direction::Left;
		else
			return Platformer_Character::Direction::Right;
	}
	return Platformer_Character::Direction::Left;
}