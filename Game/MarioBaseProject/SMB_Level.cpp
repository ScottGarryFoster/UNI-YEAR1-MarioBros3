#include "SMB_Level.h"



SMB_Level::SMB_Level(SpriteBatch *f_sb, Controls *f_controls)
{
	m_sb = f_sb;
	m_controls = f_controls;
}


SMB_Level::~SMB_Level()
{
	delete 	m_camera;
	delete 	m_camera2;
	delete 	m_cameraWhole;
	delete 	m_tmLevelMap;
	delete 	m_UIBlack;
	delete 	m_suiPlayerOne;
	delete 	m_suiPlayerTwo;
	delete 	m_player1;
	delete 	m_player2;
	delete 	m_sprSplitScreenBar;
	delete 	m_scoreOnScreen;
	delete 	m_itemManager;
	delete 	m_itemSpawnInfo;
	delete m_endOfLevel;
}

bool SMB_Level::LoadContent()
{
	if (m_audio != nullptr)
	{
		//Audio
		if (m_iMusicTrack == -1)
			m_iMusicTrack = m_audio->find_musicNeverFail("Audio/Music/01-GrassLand.mp3");

		m_audio->reset_currentPlaying();
	}

	m_sb->set_color(156, 252, 240);//Blue

	if (m_iNumberOfPlayers > 1)
	{
		//Camera1:
		if (m_camera != nullptr) delete m_camera;
		m_camera = new Camera();
		m_camera->set_worldHeight(1650);
		m_camera->set_worldWidth(1920 * 10);
		m_camera->set_lockToWorld(true);
		m_camera->set_screenX(0);
		m_camera->set_screenWidth(640);
		m_camera->set_screenY(0);
		m_camera->set_screenHeight(665);
		m_camera->set_lockId(1);

		//Camera2:
		if (m_camera2 != nullptr) delete m_camera2;
		m_camera2 = new Camera();
		m_camera2->set_worldHeight(1650);
		m_camera2->set_worldWidth(1920 * 10);
		m_camera2->set_lockToWorld(true);
		m_camera2->set_screenX(643);
		m_camera2->set_screenWidth(640);
		m_camera2->set_screenY(0);
		m_camera2->set_screenHeight(665);
		m_camera2->set_y(180);
		m_camera2->set_lockId(2);
	}

	//CameraWhole:
	if (m_cameraWhole != nullptr) delete m_cameraWhole;
	m_cameraWhole = new Camera();
	m_cameraWhole->set_worldHeight(1650);
	m_cameraWhole->set_worldWidth(1920 * 10);
	m_cameraWhole->set_lockToWorld(true);
	m_cameraWhole->set_screenX(0);
	m_cameraWhole->set_screenWidth(1280);
	m_cameraWhole->set_screenY(0);
	m_cameraWhole->set_screenHeight(665);
	if (m_iNumberOfPlayers == 1)
		m_cameraWhole->set_lockId(1);
	if (m_iNumberOfPlayers > 1)
		m_cameraWhole->set_visable(false);


	//Setup Tilemap:
	if (m_tmLevelMap != nullptr) delete m_tmLevelMap;
	m_tmLevelMap = new Tilemap();
	m_tmLevelMap->set_spritebatch(m_sb);
	m_tmLevelMap->set_controls(m_controls);

	m_tmLevelMap->set_camera(m_cameraWhole, true);//Cameras have to be added before tilemap because the worldsize is set in the tilemap.
	if (m_iNumberOfPlayers > 1)
	{
		m_tmLevelMap->set_altCamera(m_camera, 1);
		m_tmLevelMap->set_altCamera(m_camera2, 0);
	}

	string l_sName = "GameFiles/Mario3/Tilemaps/" + m_sTilemap;
	m_tmLevelMap->load_tilemap(Tilemap::FromFile, l_sName);
	m_tmLevelMap->load_tileset(Tilemap::FromFile, "GameFiles/Mario3/Tilesets/" + m_sTileset);
	
	if (m_audio != nullptr)
		m_tmLevelMap->set_audio(m_audio);

	if (m_UIBlack != nullptr) delete m_UIBlack;
	m_UIBlack = new Sprite();
	m_UIBlack->set_textureSelfManageFromPointer(m_tmLevelMap->get_textureSelfManaged());
	m_UIBlack->set_singleSize(16, 16);
	m_UIBlack->set_singleLocation(2, 0);
	m_UIBlack->set_screenSize(1280, 55);
	m_UIBlack->set_location(0, 720 - 55);

	//UI For Player One:
	if (m_suiPlayerOne != nullptr) delete m_suiPlayerOne;
	m_suiPlayerOne = new SMB_ScreenUI(m_sb);
	m_suiPlayerOne->set_coins(&m_iCoins[0]);
	m_suiPlayerOne->set_lives(&m_iLives[0]);
	m_suiPlayerOne->set_points(m_iPoints);
	m_suiPlayerOne->set_time(&m_iTimeLeft);

	if (m_iNumberOfPlayers > 1)
	{
		//UI For Player Two:
		if (m_suiPlayerTwo != nullptr) delete m_suiPlayerTwo;
		m_suiPlayerTwo = new SMB_ScreenUI(m_sb);
		m_suiPlayerTwo->set_offsetX(920);
		m_suiPlayerTwo->set_coins(&m_iCoins[1]);
		m_suiPlayerTwo->set_lives(&m_iLives[1]);
		m_suiPlayerTwo->set_points(m_iPoints);
		m_suiPlayerTwo->set_time(&m_iTimeLeft);
	}

	float l_fScaleFactor = (SCREEN_HEIGHT / 18) * 0.0769230769230769f; //Scale to screenheight

	//Player1
	if (m_player1 != nullptr) delete m_player1;
	m_player1 = new SMB_PlayerPlatformer();
	m_player1->set_controls(m_controls);
	m_player1->set_cameraLock(true);
	m_player1->LoadTextureFromFile(m_sb->get_renderer(), "Images/Mario3/Char_MarioSmall.png");
	m_player1->set_scaleSize(l_fScaleFactor);
	m_player1->set_singleSize(18, 18);
	m_player1->set_collision();
	
	//m_player1->set_collision(4, 2, 12, 14);
	//m_player1->set_collision(2, 2, 14, 14);
	m_player1->set_collision(2, 2, 14, 14);
	if (m_audio != nullptr)
		m_player1->set_audio(m_audio);

	m_tmLevelMap->update_cameraYLowest();
	int l_iCameraLowestY = m_tmLevelMap->get_cameraYLowest();

	//Player Location:
	if (m_v2dPlayerSpawnLocation.m_iX == -1 || m_v2dPlayerSpawnLocation.m_iY == -1)
	{
		int l_iNumberOfTilesHigh = (m_tmLevelMap->get_cameraYLowest() + SCREEN_HEIGHT) / m_tmLevelMap->get_actualWandH();
		m_player1->set_location(0, (l_iNumberOfTilesHigh - 3) * m_tmLevelMap->get_actualWandH());
		for (int i = l_iNumberOfTilesHigh - 3; i > 0; i--)
		{
			if (m_tmLevelMap->get_tileInfo(0,i) != -1)
				continue;
			m_player1->set_location(0, i * m_tmLevelMap->get_actualWandH() - m_tmLevelMap->get_actualWandH());
			break;
		}
	}
	else
		m_player1->set_location(m_v2dPlayerSpawnLocation.m_iX, m_v2dPlayerSpawnLocation.m_iY);

	

	m_player1->set_debugControls(m_controls);
	m_player1->set_camera(m_cameraWhole);
	if (m_iNumberOfPlayers > 1)
	{
		m_player1->set_altCamera(m_camera, 0);
		m_player1->set_altCamera(m_camera2, 1);
	}
	m_player1->set_lockId(1);
	m_player1->set_player(SMB_PlayerPlatformer::pcPlayerOne);
	m_player1->set_screenUI(m_suiPlayerOne);
	m_player1->update_actualsize();
	
	m_player1->set_tilemap(m_tmLevelMap);
	m_player1->set_actorNumber(0);
	m_player1->set_pauseControl(&m_iPauseEverythingButPlayer);
	m_player1->set_statLivesPtr(&m_iLives[0]);
	m_player1->set_coinPtr(&m_iCoins[0]);
	m_player1->set_endOfLevel(s_iEndOfLevel);

	if (m_iNumberOfPlayers > 1)
	{
		//Player 2:
		if (m_player2 != nullptr) delete m_player2;
		m_player2 = new SMB_PlayerPlatformer();
		m_player2->set_camera(m_camera2);
		m_player2->set_controls(m_controls);
		m_player2->set_cameraLock(true);
		m_player2->set_textureSelfManageFromPointer(m_player1->get_textureSelfManageFromPointer());
		m_player2->set_scaleSize(l_fScaleFactor);
		m_player2->set_singleSize(18, 18);
		m_player2->set_location(0, 720 * 2 - (55 * 4));
		m_player2->set_altCamera(m_camera, 0);
		m_player2->set_altCamera(m_cameraWhole, 1);
		m_player2->set_lockId(2);
		m_player2->set_player(SMB_PlayerPlatformer::pcPlayerTwo);
		m_player2->set_screenUI(m_suiPlayerTwo);
		m_player2->update_actualsize();
		m_player2->set_collision();
		m_player2->set_collision(4, 2, 12, 14);
		m_player2->set_tilemap(m_tmLevelMap);
		m_player2->set_actorNumber(0);
		m_player2->set_pauseControl(&m_iPauseEverythingButPlayer);
		m_player2->set_statLivesPtr(&m_iLives[1]);
		m_player2->set_coinPtr(&m_iCoins[1]);

		//Tell players of one another:
		m_player1->set_otherPlayers(m_player2,0);
		m_player2->set_otherPlayers(m_player1,0);
		m_player2->set_endOfLevel(s_iEndOfLevel);

		if (m_audio != nullptr)
			m_player2->set_audio(m_audio);
	}

	if (m_iNumberOfPlayers > 1)
	{
		//Bar Which splits the screen:
		if (m_sprSplitScreenBar != nullptr) delete m_sprSplitScreenBar;
		m_sprSplitScreenBar = new Sprite();
		m_sprSplitScreenBar->LoadTextureFromFile(m_sb->get_renderer(), "Images/Mario3/SplitScreenLine.jpg");
		m_sprSplitScreenBar->set_location((1280 / 2) - 3, 0);
		m_sprSplitScreenBar->set_screenSize(6, 720);
		m_sprSplitScreenBar->set_scaleSize(1);
		m_sprSplitScreenBar->set_singleSize(5, 720);
	}

	m_playersAsArray[0] = m_player1;
	if (m_iNumberOfPlayers > 1)
		m_playersAsArray[1] = m_player2;

	//Shows the score when you aquire it
	m_scoreOnScreen = new ScoreOnScreen();
	m_scoreOnScreen->set_camera(m_cameraWhole);
	if (m_iNumberOfPlayers > 1)
	{
		m_scoreOnScreen->set_altCamera(m_camera2, 0);
		m_scoreOnScreen->set_altCamera(m_camera, 1);
	}
	m_scoreOnScreen->set_scaleSize(l_fScaleFactor);
	m_scoreOnScreen->set_textureSelfManageFromPointer(m_tmLevelMap->get_textureSelfManaged());
	m_scoreOnScreen->set_singleSize(16, 16);
	m_scoreOnScreen->set_pointsPtr(m_iPoints);

	//Item Manager runs all the items
	m_itemManager = new ItemManager(m_sb, m_playersAsArray, 2);

	//Item Spawn Information is a pass through Class to spawn a new item
	m_itemSpawnInfo = new ItemSpawnInformation();
	m_tmLevelMap->set_itemSpawnInformation(m_itemSpawnInfo);

	m_itemManager->set_itemSpawnInformation(m_itemSpawnInfo);
	m_itemManager->set_defaultTexture(m_tmLevelMap->get_textureSelfManaged());
	m_itemManager->set_mainCamera(m_cameraWhole);
	if (m_iNumberOfPlayers > 1)
	{
		m_itemManager->set_altCamera(m_camera, 0);
		m_itemManager->set_altCamera(m_camera2, 1);
	}

	float l_fScaleFactor2 = (SCREEN_HEIGHT / 16) * 0.0769230769230769f; //Scale to screenheight

	m_itemManager->set_scaleSize(l_fScaleFactor2);
	m_itemManager->set_tilemap(m_tmLevelMap);

	m_itemManager->set_scoreOnScreen(m_scoreOnScreen);
	m_player1->set_scoreOnScreen(m_scoreOnScreen);
	if (m_iNumberOfPlayers > 1)
		m_player2->set_scoreOnScreen(m_scoreOnScreen);

	m_tmLevelMap->set_playerOneSize(m_player1->get_isBigPtr());
	if (m_iNumberOfPlayers > 1)
		m_tmLevelMap->set_playerTwoSize(m_player2->get_isBigPtr());

	m_tmLevelMap->set_playerReference(m_player1, 0);
	m_tmLevelMap->set_playerReference(m_player2, 1);
	
	//End of level
	m_endOfLevel = new SMB_LevelEnd();
	m_endOfLevel->set_spritebatch(m_sb);
	m_endOfLevel->set_audio(m_audio);
	m_iCameraPlayers = m_iNumberOfPlayers;
	m_endOfLevel->set_endOfLevel(s_iEndOfLevel);
	m_endOfLevel->set_playerOne(m_player1);
	if (m_player2 != nullptr)
		m_endOfLevel->set_playerTwo(m_player2);
	m_endOfLevel->set_playerOneEndCard(&m_iPlayerOneEndOfLevel);
	m_endOfLevel->set_playerTwoEndCard(&m_iPlayerTwoEndOfLevel);

	//Sent to endblocks:
	m_tmLevelMap->set_playerOneEndCard(&m_iPlayerOneEndOfLevel);
	m_tmLevelMap->set_playerTwoEndCard(&m_iPlayerTwoEndOfLevel);

	//Sent to stop players moving (by buttons) after finishing the level
	m_player1->set_finishedLevel(&m_iPlayerOneEndOfLevel);
	if (m_player2 != nullptr)
		m_player2->set_finishedLevel(&m_iPlayerTwoEndOfLevel);

	//Particles:
	m_partcleManager = new ParticleManager(m_sb);
	m_partcleManager->set_mainCamera(m_cameraWhole);
	if (m_iNumberOfPlayers > 1)
	{
		m_partcleManager->set_altCamera(m_camera, 0);
		m_partcleManager->set_altCamera(m_camera2, 1);
	}
	m_tmLevelMap->set_particleManager(m_partcleManager);
	m_partcleManager->set_scaleSize(m_player1->get_scaleSize());

	//Deubg FPS:
	m_sprlettersFPS = new SprLetters();
	m_sprlettersFPS->LoadTextureFromFile(m_sb->get_renderer(), "Images/Mario3/Letters.png");
	m_sprlettersFPS->set_singleSize(16, 16);
	m_sprlettersFPS->set_location(SCREEN_WIDTH - 200, 0);
	m_sprlettersFPS->set_lettersRow(4);
	m_sprlettersFPS->set_lettersCol(0);
	m_sprlettersFPS->set_numbersRow(5);
	m_sprlettersFPS->set_numbersCol(0);

	return true;
}

bool SMB_Level::Draw()
{
	if (m_sb != nullptr)
	{
		//cout << "6" << endl;
		if (m_background != nullptr)
			m_background->Draw();

		if (m_UIBlack != nullptr)
			m_UIBlack->RenderToScreen(m_sb->get_renderer());

		//cout << "7" << endl;
		if (m_tmLevelMap != nullptr)
			m_tmLevelMap->Draw();//Does not draw Special Blocks
		//cout << "8" << endl;
		if (m_player1 != nullptr)
			if (m_player1->get_deathState() == SMB_PlayerPlatformer::DeathState::dsAlive)
			m_player1->RenderToScreen(m_sb->get_renderer());
		//cout << "9" << endl;
		if (m_player2 != nullptr)
			if (m_player2->get_deathState() == SMB_PlayerPlatformer::DeathState::dsAlive)
			m_player2->RenderToScreen(m_sb->get_renderer());
		//cout << "10" << endl;

		if (m_itemManager != nullptr)
			m_itemManager->Draw();

		if (m_tmLevelMap != nullptr)
			m_tmLevelMap->Draw_specialBlocks();//Just draws special blocks

		if (m_itemManager != nullptr)
			m_itemManager->Draw(true);

		if (m_fireball != nullptr)
			m_fireball->Draw();

		if (m_player1 != nullptr)//if dead you need to be on top otherwise it looks weird
			if (m_player1->get_deathState() != SMB_PlayerPlatformer::DeathState::dsAlive)
				m_player1->RenderToScreen(m_sb->get_renderer());

		if (m_player2 != nullptr)//if dead you need to be on top otherwise it looks weird
			if (m_player2->get_deathState() != SMB_PlayerPlatformer::DeathState::dsAlive)
				m_player2->RenderToScreen(m_sb->get_renderer());

		if (m_partcleManager != nullptr)
			m_partcleManager->Draw();

		if (m_scoreOnScreen != nullptr)
			m_scoreOnScreen->RenderToScreen(m_sb->get_renderer());

		if (m_sprSplitScreenBar != nullptr)
			m_sprSplitScreenBar->RenderToScreen(m_sb->get_renderer());

		if (m_suiPlayerOne != nullptr)
			m_suiPlayerOne->Draw();

		if (m_suiPlayerTwo != nullptr)
			m_suiPlayerTwo->Draw();

		if (m_bFinishTheLevel && m_endOfLevel != nullptr)
			m_endOfLevel->Draw();
	}
	return true;
}
bool SMB_Level::Update(float f_deltaTime)
{

	if(!m_bFinishTheLevel && m_iMusicTrack > -1)
		m_audio->play_music(m_iMusicTrack);

	update_endOfLevel(f_deltaTime);

	if (!m_bStartEndAnimation && m_iPauseEverythingButPlayer <= -1)
	{
		if (m_tmLevelMap != nullptr)
			m_tmLevelMap->Update(f_deltaTime);

		if (m_partcleManager != nullptr)
			m_partcleManager->Update(f_deltaTime);
	}
	
	if(m_iPauseEverythingButPlayer == 1 || m_iPauseEverythingButPlayer <= -1)
		if (m_player1 != nullptr)
			m_player1->Update(f_deltaTime);
	
	if (m_iPauseEverythingButPlayer == 2 || m_iPauseEverythingButPlayer <= -1)
		if (m_player2 != nullptr)
			m_player2->Update(f_deltaTime);

	if (m_bStartEndAnimation) return true; //We only need the above if the level is complete
	
	if (m_iPauseEverythingButPlayer <= -1)
	{
		update_multiplayerCamera(); //Deals with splits

		if (m_suiPlayerOne != nullptr)
			m_suiPlayerOne->Update();

		if (m_suiPlayerTwo != nullptr)
			m_suiPlayerTwo->Update();

		if (m_itemManager != nullptr)
			m_itemManager->Update(f_deltaTime);

		if (m_scoreOnScreen != nullptr)
			m_scoreOnScreen->Update(f_deltaTime);
	}
	return true;
}

bool SMB_Level::update_multiplayerCamera()
{
	if(m_bFinishTheLevel) return false;
	if (m_iNumberOfPlayers == 1) return false;
	if (m_iCameraPlayers <= 1) return false;
	if (m_player1 != nullptr && m_player2 != nullptr)
	{
		int l_iCameraLowestY = -1;
		if (m_tmLevelMap != nullptr)
			l_iCameraLowestY = m_tmLevelMap->get_cameraYLowest();
		if (l_iCameraLowestY == -1)
			l_iCameraLowestY = 0;

		if (m_camera != nullptr) m_camera->set_yLowest(l_iCameraLowestY);
		if (m_camera2 != nullptr) m_camera2->set_yLowest(l_iCameraLowestY);
		if (m_cameraWhole != nullptr) m_cameraWhole->set_yLowest(l_iCameraLowestY);

		float l_fDistanceBetween = abs(m_player1->get_locationX() - m_player2->get_locationX());
		float l_fDistanceBetween_Test = l_fDistanceBetween;

		if (m_player1->get_locationX() < SCREEN_WIDTH / 4)
			l_fDistanceBetween_Test -= (SCREEN_WIDTH / 4) - m_player1->get_locationX();
		else if (m_player2->get_locationX() < SCREEN_WIDTH / 4)
			l_fDistanceBetween_Test -= (SCREEN_WIDTH / 4) - m_player2->get_locationX();

		float l_fDistanceBetweenY = abs(m_player1->get_locationY() - m_player2->get_locationY());
		float l_fDistanceBetween_TestY = l_fDistanceBetweenY;

		if (m_player1->get_locationY() < SCREEN_HEIGHT / 4)
			l_fDistanceBetween_TestY -= (SCREEN_HEIGHT / 4) - m_player1->get_locationY();
		else if (m_player2->get_locationX() < SCREEN_HEIGHT / 4)
			l_fDistanceBetween_TestY -= (SCREEN_HEIGHT / 4) - m_player2->get_locationY();

		if 
			(l_fDistanceBetween_Test < SCREEN_WIDTH / 2 &&
				(
					l_fDistanceBetweenY < SCREEN_HEIGHT / 2 || 
					(m_player1->get_locationY() > l_iCameraLowestY && m_player2->get_locationY() > l_iCameraLowestY)
				)
			)
		{
			//No Split:
			m_sprSplitScreenBar->set_visable(false);
			m_cameraWhole->set_visable(true);
			m_camera->set_visable(false);
			m_camera2->set_visable(false);
			//Lock:
			if (m_player1->get_locationX() < m_player2->get_locationX())
				m_cameraWhole->set_x(m_player1->get_locationX() - (l_fDistanceBetween / 2));
			else
				m_cameraWhole->set_x(m_player2->get_locationX() - (l_fDistanceBetween / 2));

			if (m_player1->get_locationY() < l_iCameraLowestY && m_player2->get_locationY() < l_iCameraLowestY)
			{
				if (m_player1->get_locationY() < m_player2->get_locationY())
					m_cameraWhole->set_y(m_player1->get_locationY() - (l_fDistanceBetweenY / 2));
				else
					m_cameraWhole->set_y(m_player2->get_locationY() - (l_fDistanceBetweenY / 2));
			}
			else
			{
				m_cameraWhole->set_y(l_iCameraLowestY);
			}

			m_bChosenSplitLocation = false;//This locks the split locations. When no split no lock.
		}
		else
		{
			if (!m_bChosenSplitLocation)
			{
				//Split:
				m_sprSplitScreenBar->set_visable(true);
				m_cameraWhole->set_visable(false);
				//Y First.
				//I'd like higher to be right.
				if (m_player1->get_locationY() < SCREEN_HEIGHT && m_player2->get_locationY() > m_player1->get_locationY())
				{
					m_bPlayerOneIsLeft = false;
					m_camera2->set_screenX(0);
					m_camera->set_screenX(640);
				}
				else if (m_player2->get_locationY() < SCREEN_HEIGHT && m_player1->get_locationY() > m_player2->get_locationY())
				{
					m_bPlayerOneIsLeft = true;
					m_camera->set_screenX(0);
					m_camera2->set_screenX(640);
				}
				else
				{
					
					if (m_player1->get_locationX() >= m_player2->get_locationX())
					{
						m_bPlayerOneIsLeft = false;
						m_camera->set_screenX(640);
					}
					else
						m_camera->set_screenX(0);

					if (m_player2->get_locationX() > m_player1->get_locationX())
					{
						m_bPlayerOneIsLeft = true;
						m_camera2->set_screenX(640);
					}
					else
						m_camera2->set_screenX(0);
				}
				m_camera->set_visable(true);
				m_camera2->set_visable(true);

				m_bChosenSplitLocation = true;//Lock in the locations worked out above
				//This stops Y axis splits and then the X axis moving the splits -it's a little jarring.
			}

			update_screenUILocation();//Moves the UI elements
		}
	}
	else if (m_player1 != nullptr)
	{
		m_sprSplitScreenBar->set_visable(false);
		m_cameraWhole->set_visable(true);
		m_cameraWhole->set_lockId(m_player1->get_lockId());
		m_camera->set_visable(false);
		m_camera2->set_visable(false);
	}
}

bool SMB_Level::update_screenUILocation()
{
	if (m_iNumberOfPlayers == 1) return false;
	if (m_bPlayerOneIsLeft)
	{
		m_suiPlayerOne->set_offsetX(0);
		m_suiPlayerTwo->set_offsetX(920);
	}
	else
	{
		m_suiPlayerOne->set_offsetX(920);
		m_suiPlayerTwo->set_offsetX(0);
	}
}

bool SMB_Level::update_endOfLevel(float f_deltaTime)
{
	if (m_bFinishTheLevel)
	{
		if(m_bStartEndAnimation)
			return update_runFinishedSequence(f_deltaTime);
		else
		{
			m_bStartEndAnimation = true;
			return true;
		}
	}
	//If the level isn't over ensure we update the finish state:

	//Camera switch to single
	if (m_iCameraPlayers > 1 && m_iPlayerOneEndOfLevel > 0)
		switch_cameraFocus(2);
	else if (m_iCameraPlayers > 1 && m_iPlayerTwoEndOfLevel > 0)
		switch_cameraFocus(1);

	if (m_iCollapseSplitScreenToWhole > 0)
		update_smoothCameraTransition(f_deltaTime);

	if ((m_iPlayerOneEndOfLevel > 0 && m_iPlayerTwoEndOfLevel == -1) || (m_iPlayerTwoEndOfLevel > 0 && m_iPlayerOneEndOfLevel == -1))
	{
		m_camera->set_visable(false);
		m_camera2->set_visable(false);
		m_cameraWhole->set_visable(true);
		m_sprSplitScreenBar->set_visable(false);

		m_bFinishTheLevel = true;//Run the finish level sequence
	}
	else if ((m_iPlayerOneEndOfLevel > 0 && m_iPlayerTwoEndOfLevel > 0))
		m_bFinishTheLevel = true;//Run the finish level sequence

	if (m_bIsLevelEditor && m_iPlayerOneEndOfLevel > 0)
		*s_iEndOfLevel = 0;

	return true;
}

bool SMB_Level::update_runFinishedSequence(float f_deltaTime)
{
	if (m_endOfLevel != nullptr)
		m_endOfLevel->Update(f_deltaTime);

	if (m_player1->get_deathState() == SMB_PlayerPlatformer::DeathState::dsComplete)
	{
		m_cameraWhole->set_lockId(m_player1->get_lockId());
		m_cameraWhole->set_x(m_player1->get_locationX() - 10);//I don't like this hotfix but honestly can't figure out why lock doesn't work here
	}
	else
	{
		m_cameraWhole->set_lockId(m_player2->get_lockId());
		m_cameraWhole->set_x(m_player2->get_locationX() - 10);
	}

	return true;
}

bool SMB_Level::switch_cameraFocus(int f_iPlayer)
{
	m_iCameraPlayers--;

	if (m_iCameraPlayers == 1)
	{
		int l_iPlayerNewFocus = 1;
		if(f_iPlayer == 1) l_iPlayerNewFocus = 2;

		m_iWholeCameraPerson = f_iPlayer;
		if (l_iPlayerNewFocus == 1)
			m_cameraWhole->set_lockId(m_player2->get_lockId());
		else
			m_cameraWhole->set_lockId(m_player1->get_lockId());

		if (m_camera->get_visable())
		{
			if(l_iPlayerNewFocus == 1)
				m_iCollapseSplitScreenToWhole = 2;//Smooth Transition to player 2
			else
				m_iCollapseSplitScreenToWhole = 1;//Smooth Transition to player 1
		}
		else
		{
			m_camera->set_visable(false);
			m_camera2->set_visable(false);
			m_cameraWhole->set_visable(true);
			m_sprSplitScreenBar->set_visable(false);
		}
	}
	return true;
}

bool SMB_Level::update_smoothCameraTransition(float f_deltaTime)
{
	/*
		This method is used when you want to collapse the cameras
		into the camera whole but you don't want to do it jarringly
		This means altering the size and x cords of the cameras to
		make it look like the split camera we're focusing on is 
		growing and moving to the full screen, the other shrinking.

		Keep in mind we move the m_camera and m_camera2 around based
		on the players. So m_bPlayerOneIsLeft will tell you if m_camera
		is on the left or right along with the position of the cameras.
	*/
	if (m_iCollapseSplitScreenToWhole == 1)//Focus on m_camera
	{
		m_camera->set_screenWidth(m_camera->get_screenWidth() + f_deltaTime);

		if (m_camera->get_screenX() > 0)
			m_sprSplitScreenBar->move_location(-f_deltaTime, 0);
		else
			m_sprSplitScreenBar->move_location(f_deltaTime, 0);

		if(m_camera->get_screenX() > 0)
			m_camera->set_screenX(m_camera->get_screenX() - f_deltaTime);


		m_camera2->set_screenWidth(m_camera2->get_screenWidth() - f_deltaTime);
		if (m_camera2->get_screenX() > 0)
			if(m_bPlayerOneIsLeft)
				m_camera2->set_screenX(m_camera2->get_screenX() + f_deltaTime);
			else
				m_camera2->set_screenX(m_camera2->get_screenX() - f_deltaTime);

		if (m_camera->get_screenWidth() >= m_cameraWhole->get_screenWidth())
		{
			m_camera->set_screenWidth(m_cameraWhole->get_screenWidth());
			m_camera->set_visable(false);
			m_camera2->set_visable(false);
			m_cameraWhole->set_visable(true);
			m_sprSplitScreenBar->set_visable(false);
			m_iCollapseSplitScreenToWhole = 0;
		}
	}
	else if (m_iCollapseSplitScreenToWhole == 2)//Focus on m_camera2
	{
		m_camera2->set_screenWidth(m_camera2->get_screenWidth() + f_deltaTime);

		if (m_camera2->get_screenX() > 0)
			m_sprSplitScreenBar->move_location(-f_deltaTime, 0);
		else
			m_sprSplitScreenBar->move_location(f_deltaTime, 0);

		if (m_camera2->get_screenX() > 0)
			m_camera2->set_screenX(m_camera2->get_screenX() - f_deltaTime);


		m_camera->set_screenWidth(m_camera->get_screenWidth() - f_deltaTime);
		if (m_camera->get_screenX() > 0)
			if (m_bPlayerOneIsLeft)
				m_camera->set_screenX(m_camera->get_screenX() - f_deltaTime);
			else
				m_camera->set_screenX(m_camera->get_screenX() + f_deltaTime);


		if (m_camera2->get_screenWidth() >= m_cameraWhole->get_screenWidth())
		{
			m_camera2->set_screenWidth(m_cameraWhole->get_screenWidth());
			m_camera->set_visable(false);
			m_camera2->set_visable(false);
			m_cameraWhole->set_visable(true);
			m_sprSplitScreenBar->set_visable(false);
			m_iCollapseSplitScreenToWhole = 0;
		}
	}
	return true;
}