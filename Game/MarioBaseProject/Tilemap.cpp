#include "Tilemap.h"
#include "Vector2D.h"
#include "Enemy.h"
#include "SMB_PlayerPlatformer.h"
#include "ParticleManager.h"

Tilemap::Tilemap()
{
	
}


Tilemap::~Tilemap()
{
	delete m_tsmTexture;
	delete m_llTileMap;
	delete m_llTileInformation;

	for (int i = 0; i < 20; i++)
		if (m_tiInformation[i] != nullptr)
			delete m_tiInformation[i];
			
	for (int i = 0; i < 20; i++)
		if (m_tmInformation[i] != nullptr)
			delete m_tmInformation[i];
			

	for (int i = 0; i < m_specialBlocksLength; i++)
		if (m_specialBlocks[i] != nullptr)
		{
			delete m_specialBlocks[i];
			m_specialBlocks[i] = nullptr;
		}

	for (int i = 0; i < m_sSpecialBlockLoadersLength; i++)
		if (m_sSpecialBlockLoaders[i] != nullptr)
			delete m_sSpecialBlockLoaders[i];

	for (int i = 0; i < m_enemiesLength; i++)
		if (m_enemies[i] != nullptr)
			delete m_enemies[i];

	if (m_llItemsToSpawn != nullptr)
	{
		LinkedList *l_llCurrent = m_llItemsToSpawn;
		for (int i = 0; i < m_llItemsToSpawn->length(); i++)
		{
			if (l_llCurrent->get_data(nullptr, false))
			{
				ItemSpawnInformationPacket *l_isip = (ItemSpawnInformationPacket*)l_llCurrent->get_data(nullptr, false);
				delete l_isip;
				l_isip = nullptr;
			}
			l_llCurrent = l_llCurrent->get_ahead();
		}
		delete m_llItemsToSpawn;
	}

}

bool Tilemap::load_tilemap(MapLoadType f_lt, string f_sData)
{
	if (f_sData == "") return false;

	switch (f_lt)
	{
	case FromFile:
		m_bTilemapLoaded = load_tilemapFromFile(f_sData);
		break;
	case FromString:
		m_bTilemapLoaded = load_tilemapFromString(f_sData);
		break;
	default:
		return false;
	}
	return true;
}

bool Tilemap::update_cameraYLowest()
{
	m_fScaleSize = ((SCREEN_HEIGHT / m_iSingleHeight) * 0.0769230769230769f);// *0.25f;
	int l_fNewWidth = m_fScaleSize * m_iSingleWidth;
	m_iScaledWidthAndHeight = l_fNewWidth;
	if (m_iScaledWidthAndHeight == 0) return false;

	int l_iTilesHigh = m_llTileMap->length();//This is all the rows
	int l_iHeightInPixels = l_iTilesHigh * m_iScaledWidthAndHeight;

	//int l_iTilesOnScreenRoundedDown = SCREEN_HEIGHT / m_iScaledWidthAndHeight;//Used only for bug testing

	m_iCameraYLowest = l_iHeightInPixels - SCREEN_HEIGHT;
	return true;
}

bool Tilemap::load_tilemapFromString(string f_sData)
{
	if (f_sData == "") return false;
	string l_sLeft = f_sData, l_sRight = "";
	int l_iLine = 0;
	LinkedList *l_llLineWorkingWith;
	bool l_bFoundResult = false;

	if (m_llTileMap != nullptr)
		delete m_llTileMap;
	m_llTileMap = new LinkedList();

	if (m_bDebug_TilemapLoad) cout << "FROM STRING" << endl;
	while (true)
	{
		if (l_sLeft == "") break;

		if (m_bDebug_TilemapLoad)cout << "SPLIT: " << endl;
		l_bFoundResult = Utility::SplitStringAtPoint(&l_sLeft, &l_sRight, '~');

		if (m_bDebug_TilemapLoad)
		{
			cout << "WORKING ON LINE: " << l_iLine << "LEFT: " << l_sLeft << " FOUND:";
			if (l_bFoundResult) cout << "TRUE" << endl; else cout << "FALSE" << endl;
		}

		if (l_bFoundResult || l_sLeft != "")
		{
			if (m_bDebug_TilemapLoad) cout << "FOUND RESULT" << endl;

			l_llLineWorkingWith = m_llTileMap->ReadFromList(l_iLine);
			if (l_llLineWorkingWith == nullptr)
				l_llLineWorkingWith = m_llTileMap->AddNewToEnd();

			if (m_bDebug_TilemapLoad) cout << "LOAD STRING LINE" << endl;

			load_tilemapFromSingleLine(l_sLeft, l_iLine);
			l_iLine++;

			l_sLeft = l_sRight;
			l_sRight = "";
		}
		else
			break;
	}
	return true;
}
bool Tilemap::load_tilemapFromFile(string f_sData)
{
	if (f_sData == "") return false;

	ifstream infile(f_sData);
	if (!infile.is_open())
	{
		if (m_bDebug_TilemapLoad) cout << "load_tilesetFromFile(string f_sData):  Could not open file." << endl;
		return false;
	}
	LinkedList *l_llLineWorkingWith = nullptr;

	if (m_llTileMap != nullptr)
		delete m_llTileMap;
	m_llTileMap = new LinkedList();

	string f_sGetLine = "";
	int f_iLine = 0;
	while (getline(infile, f_sGetLine))
	{
		if (f_sGetLine == "META")
		{
			while (true)
			{
				getline(infile, f_sGetLine);
				if (f_sGetLine == "---")
					break;
				else
					load_metaData(f_sGetLine);
			}
		}
		else
		{
			l_llLineWorkingWith = m_llTileMap->ReadFromList(f_iLine);
			if (l_llLineWorkingWith == nullptr)
				l_llLineWorkingWith = m_llTileMap->AddNewToEnd();

			load_tilemapFromSingleLine(f_sGetLine, f_iLine++);
		}
	}
	infile.close();

	load_metaDataActions();

	return true;
}

bool Tilemap::load_metaData(string f_sData)
{
	string l_sLeft = f_sData, l_sRight = "";
	if (!Utility::SplitStringAtPoint(&l_sLeft, &l_sRight, ' '))
		return false;
	l_sLeft = Utility::StringToUpperCase(l_sLeft);
	l_sRight = Utility::StringToUpperCase(l_sRight);

	if (l_sLeft == "WORLDSIZE:")
	{
		int l_iWorldSize = Utility::convert_stringToInt(l_sRight);
		if (l_iWorldSize == -1) return false;
		m_iWorldSize = l_iWorldSize;
		return true;
	}
	return true;
}

bool Tilemap::load_metaDataActions()
{
	if (p_camera != nullptr) p_camera->set_worldWidth(m_iWorldSize);
	for(int c = 0; c < 10; c++)
		if(p_alternativeCameras[c] != nullptr)
			p_alternativeCameras[c]->set_worldWidth(m_iWorldSize);

	return true;
}

bool Tilemap::load_tilemapFromSingleLine(string f_sData, int f_iLine)
{
	if (f_sData == "") return false;
	string l_sLeft = f_sData, l_sRight = "", l_sWorkingWith = "", l_sCurrentFlag = "", l_sSpecialBlockInformation = "";
	char l_cWorkingWith, l_cFlip = '`';
	int l_iNumuberOfTiles = -1;
	bool l_bErrorCheck = false;
	LinkedList *l_llFlags = nullptr, *l_llCurrent = nullptr;

	if (m_bDebug_TilemapLoad) cout << "BEFORE POINTERS" << endl;
	//Okay this bit might be confusing:
	LinkedList *l_llLineWorkingWith = m_llTileMap->ReadFromList(f_iLine);//Find the line in the linked list
	if (l_llLineWorkingWith == nullptr) return false;//If we don't don't continue
	LinkedList *l_llLineWorkingWithLine = l_llLineWorkingWith->get_data(nullptr);//Find the data inside the line item for the linked lists
	if (l_llLineWorkingWithLine == nullptr)//If there is no linked list stored
	{
		l_llLineWorkingWith->set_data(new LinkedList());//Make a new Linked list for the coloumn data
		l_llLineWorkingWithLine = l_llLineWorkingWith->get_data(nullptr);//Then set this to the start of this new coloumn list
	}

	if (m_bDebug_TilemapLoad) cout << "AFTER" << endl;
	while (true)
	{
		if (l_sLeft == "") break;

		l_bErrorCheck = Utility::SplitStringAtPoint(&l_sLeft, &l_sRight, '|');
		if (m_bDebug_TilemapLoad) cout << "l_sLeft: " << l_sLeft << endl;
		if (l_bErrorCheck || l_sLeft != "")
		{
			l_cWorkingWith = l_sLeft[0];//The tile

			/*
				FLAG CHECK
				Flags are in the code deliminated by / and can be many.
				Single Char flags:
				H, V and B = Flips
			*/
			if (Utility::SearchStringForChar(l_sLeft, '/') > -1)//Think string search in Javascript
			{
				//Flags code
				l_llFlags = Utility::SplitStringToLL(l_sLeft, '/');//This creates a linked list, ensure you delete it to avoid leaks
				l_sLeft = l_llFlags->read_data(0, l_sLeft);//Just the bit before the flags
				l_sSpecialBlockInformation = "";
				for (int i = 1; i < l_llFlags->length(); i++)
				{
					l_sCurrentFlag = l_llFlags->read_data(i, l_sCurrentFlag);
					//if (l_sCurrentFlag.length() == 1)
					//{
					//CHAR FLAGS:
					switch (l_sCurrentFlag[0])
					{
					case 'H'://Horizontal Flip
					case 'V'://Vert flip
					case 'B'://Both flips
						l_cFlip = l_sCurrentFlag[0];
						break;
					case 'S'://Special Block flag
					case 'E'://Enemy Block flag
					case 'I'://Item Block flag
						if (!Utility::SearchStringForChar(l_sCurrentFlag, '_')) continue;//Double check there is a flie location
						string l_sLeft = l_sCurrentFlag, l_sRight = "";
						
						if (!Utility::SplitStringAtPoint(&l_sLeft, &l_sRight, '_')) continue;//If couldn't don't continue
						if (l_sSpecialBlockInformation == "")
						{
							l_sSpecialBlockInformation = l_sCurrentFlag[0];
						}
						l_sSpecialBlockInformation += "_" + l_sRight;

						//cout << "l_sSpecialBlockInformation:" << l_sSpecialBlockInformation << endl;
					}
					//}
				}
				delete l_llFlags; l_llFlags = nullptr; //Don't need this now
			}

			if (l_cWorkingWith != '@')//The single character
			{
				l_sWorkingWith = l_sLeft.substr(1, l_sLeft.length() - 1);//Shave the first character

				l_iNumuberOfTiles = Utility::convert_stringToInt(l_sWorkingWith, &l_bErrorCheck);//Convert the rest to an int
				if (!l_bErrorCheck) continue;//Couldn't convert to number

				for (int i = 0; i < Utility::convert_charToInt(l_cWorkingWith); i++)
				{
					l_llCurrent = l_llLineWorkingWithLine->AddNewToEnd(l_iNumuberOfTiles);//I've switch this to store an Int for char number so the tilemap can be near infinate
					//l_llLineWorkingWithLine->AddNewToEnd(l_cWorkingWith);//This is just the old code if I want to switch back
					if (l_cFlip != '`')//There are char flags:
						l_llCurrent->set_data(l_cFlip);//Set them
					if (l_sSpecialBlockInformation != "")
						l_llCurrent->set_data(l_sSpecialBlockInformation);//Set them
						
				}
					

				if (m_bDebug_TilemapLoad) cout << "===ADDING:" << l_cWorkingWith << " to " << l_llLineWorkingWithLine->length() << endl;
			}
			else//These are singles
			{
				for (int i = 1; i < l_sLeft.length(); i++)//Go through string
				{
					l_llCurrent = l_llLineWorkingWithLine->AddNewToEnd(Utility::convert_charToInt(l_sLeft[i]));//Copy the exact character
					if (l_cFlip != '`')//There are char flags:
						l_llCurrent->set_data(l_cFlip);//Set them
					if (l_sSpecialBlockInformation != "")
						l_llCurrent->set_data(l_sSpecialBlockInformation);//Set them
				}
			}
			l_sWorkingWith = "";
			l_cFlip = '`';
			l_sSpecialBlockInformation = "";
			l_sLeft = l_sRight;
			l_sRight = "";
		}
		else
			break;
	}
	return true;
}

bool Tilemap::Draw()
{
	update_cameraYLowest();
	if (p_camera == nullptr) return false;
	if (!m_bTilemapLoaded) return false;
	if (p_camera->get_visable())
		Draw_eachCamera(p_camera);
	for (int i = 0; i < 10; i++)
		if (p_alternativeCameras[i] != nullptr)
			if (p_alternativeCameras[i]->get_visable())
				Draw_eachCamera(p_alternativeCameras[i]);

	if(m_bRenderSpecialBlocks)
		for (int i = 0; i < m_enemiesLength; i++)
			if (m_enemies[i] != nullptr)
				m_enemies[i]->Draw();

	if(m_bRenderSpecialBlocks)
		Draw_specialBlocks(false);//Draw any special blocks which have opt for drawing below the player
	return true;
}

bool Tilemap::Update(float f_deltaTime)
{
	update_animations(f_deltaTime);
	if(m_bNeedToLoadSpecialBlocks)
		setup_specialBlocks(f_deltaTime);
	m_bNeedToLoadSpecialBlocks = false;
	update_specialBlocks(f_deltaTime);
	update_enemies(f_deltaTime);
	if(m_llItemsToSpawn != nullptr)
		update_items();
	return false;
}

bool Tilemap::update_animations(float f_deltaTime)
{
	bool l_bHaveAnimations = false;
	for (int i = 0; i < 20; i++)
	{
		if (m_tiInformation[i] == nullptr)	continue;

		l_bHaveAnimations = true;

		m_tiInformation[i]->add_tick(f_deltaTime);
		if (m_tiInformation[i]->get_currentTick() > m_tiInformation[i]->get_tickLimit())
		{
			m_tiInformation[i]->add_tick(-(m_tiInformation[i]->get_tickLimit()));
			m_tiInformation[i]->TickOver_begin();

			if (!m_bDebugMessage)
				m_tiInformation[i]->debug_animation();
			m_bDebugMessage = true;
		}
	}
	if (!l_bHaveAnimations) return false;

	LinkedList *l_llOutterLine = nullptr, *l_llLine = nullptr, *l_llTile = nullptr;
	int l_iTileNumber = 0;
	for (int line = 0; line < m_llTileMap->length(); line++)
	{
		l_llOutterLine = m_llTileMap->ReadFromList(line);
		if (l_llOutterLine == nullptr) continue;
		l_llLine = m_llTileMap->read_data(line, nullptr);
		for (int tile = 0; tile < l_llLine->length(); tile++)
		{
			l_llTile = l_llLine->ReadFromList(tile);
			if (l_llTile == nullptr) continue;

			l_iTileNumber = l_llTile->get_data(0);
			if (l_iTileNumber < 0) continue;

			for (int i = 0; i < 20; i++)
			{
				if (m_tiInformation[i] == nullptr)	continue;
				if (m_tiInformation[i]->get_newTileNumber() != -1 && m_tiInformation[i]->get_newTileNumber() != 20)
					if (l_iTileNumber == m_tiInformation[i]->get_currentTileNumber() && m_tiInformation[i]->get_currentTileNumber() != -1 && m_tiInformation[i]->get_currentTileNumber() != 20)
					{
						//cout << m_tiInformation[i]->get_currentTileNumber() << endl;
						l_llTile->set_data(m_tiInformation[i]->get_newTileNumber());
					}
					
			}
		}
	}

	for (int i = 0; i < 20; i++)
	{
		if (m_tiInformation[i] == nullptr)	continue;
		m_tiInformation[i]->TickOver_end();
	}

	return true;
}

bool Tilemap::update_specialBlocks(float f_deltaTime)
{
	for (int i = 0; i < m_specialBlocksLength; i++)
	{
		if (m_specialBlocks[i] != nullptr)
			if (m_specialBlocks[i]->get_deleteMe())
			{
				Tilemap_Tile l_tt = { m_specialBlocks[i]->get_tilemapLocationX(), m_specialBlocks[i]->get_tilemapLocationY() };
				set_tileInPlace(l_tt, -1, false, true);
				delete m_specialBlocks[i];
				m_specialBlocks[i] = nullptr;
			}
		if (m_specialBlocks[i] != nullptr)
			m_specialBlocks[i]->Update(f_deltaTime);
	}
	return true;
}

bool Tilemap::update_enemies(float f_deltaTime)
{
	for (int i = 0; i < m_enemiesLength; i++)
	{
		if (m_enemies[i] == nullptr) continue;
		if (m_enemies[i] != nullptr)
			if (m_enemies[i]->get_amDead())
			{
				delete m_enemies[i];
				m_enemies[i] = nullptr;
			}
		//Update enemies Based on each other:
		for (int j = 0; j < m_enemiesLength; j++)
		{
			if (i == j) continue;//Don't activate yourself
			if (m_enemies[j] == nullptr) continue;
			if (m_enemies[i] == nullptr) continue;
			if (m_enemies[j]->get_id() == m_enemies[i]->get_id()) continue;
			if (m_enemies[i]->get_amDead()) continue;
			if (m_enemies[i]->get_isBeingHeld()) continue;
			if (m_enemies[i]->get_enemyHealth() == Enemy::EnemyHealth::ehDead) continue;
			if (m_enemies[j]->get_amDead()) continue;
			if (m_enemies[j]->get_enemyHealth() == Enemy::EnemyHealth::ehDead) continue;
			SDL_Rect l_rectCollision = m_enemies[j]->get_collisionRect();
			int l_iDirection = m_enemies[j]->get_direction();
			if (l_iDirection == 0) l_iDirection = 4; else l_iDirection = 6;
			int l_iActor = 1;
			//if (m_enemies[i]->get_enemyState() == 2) l_iActor = 2;//If they're a shell they are 2
			//if (m_enemies[j]->get_isBeingHeld()) l_iActor = 3;//If they're a shell in hand
			switch (m_enemies[j]->get_enemyState(true))
			{
			case Enemy::EnemyState::esProne:
				l_iActor = 1;
				break;
			case Enemy::EnemyState::esMovingShell:
				l_iActor = 2;
				break;
			case Enemy::EnemyState::esShell:
				if (m_enemies[j]->get_isBeingHeld())
					l_iActor = 3;
				break;
			}
			int l_iFeedback = -1;
			m_enemies[i]->update_actionActivation(l_rectCollision, (int)l_iDirection, l_iActor, false, &l_iFeedback, j);

			if(l_iActor == 3 && l_iFeedback == 2)//If you were a shell and you killed something
				m_enemies[j]->set_enemyHealth(Enemy::EnemyHealth::ehDead);//Also kill the shell

			if (m_enemies[i]->get_enemyState() == 2 && l_iActor == 2 && l_iFeedback == 6)//If you were a moving shell and you killed another moving shell
			{
				m_enemies[i]->set_enemyHealth(Enemy::EnemyHealth::ehDead);//Also kill the shell
				m_enemies[j]->set_enemyHealth(Enemy::EnemyHealth::ehDead);//Also kill the shell
			}

			//if (m_enemies[i]->get_enemyState() <= 1 && l_iActor == 3 && l_iFeedback == 6)//If you were a moving shell and you killed another moving shell

			if (m_enemies[i]->get_amDead())
			{
				//cout << 9001 << endl;
				delete m_enemies[i];
				m_enemies[i] = nullptr;
			}
			if (m_enemies[j]->get_amDead())
			{
				//cout << 9002 << endl;
				delete m_enemies[j];
				m_enemies[j] = nullptr;
			}
		}

		if (m_enemies[i] != nullptr)
			m_enemies[i]->Update(f_deltaTime);
	}
			
	return true;
}

bool Tilemap::update_items()
{
	if (m_itemSpawnInfo == nullptr) return false;
	if (m_llItemsToSpawn == nullptr) return false;
	if (m_llItemsToSpawn->get_data(nullptr, false) == nullptr) return false;
	void *l_voidPtr = m_llItemsToSpawn->get_data(nullptr, false);
	ItemSpawnInformationPacket *l_isipCurrent = (ItemSpawnInformationPacket*)l_voidPtr;

	bool l_bHaveSpawnedItem = false;

	if (l_isipCurrent->p_sItemName != "")
	{
		l_bHaveSpawnedItem = m_itemSpawnInfo->setup_all(l_isipCurrent->p_sItemName, l_isipCurrent->p_iLocationX, l_isipCurrent->p_iLocationY, l_isipCurrent->p_ise);
	}
	if (l_bHaveSpawnedItem)
	{
		delete l_isipCurrent;
		m_llItemsToSpawn->set_data(nullptr, false);
		if (m_llItemsToSpawn->length() == 1)
		{
			delete m_llItemsToSpawn;
			m_llItemsToSpawn = nullptr;
		}
		else
		{
			//Delete first element in the array:
			LinkedList *l_llOldHead = m_llItemsToSpawn;
			m_llItemsToSpawn = m_llItemsToSpawn->get_ahead();
			m_llItemsToSpawn->set_behind(nullptr);
			l_llOldHead->set_ahead(nullptr);
			delete l_llOldHead;
			m_llItemsToSpawn->update_identifier(0);
		}
	}
	return true;
}

bool Tilemap::setup_specialBlocks(float f_deltaTime)
{
	//This runs once and produces the Special blocks
	//We don't need to revist making these
	for (int i = 0; i < m_specialBlocksLength; i++)
	{
		if (m_specialBlocks[i] != nullptr)
			delete m_specialBlocks[i];
		m_specialBlocks[i] = nullptr;
	}

	string l_sFlags = "", l_sCurrentFlag = "";
	float l_fSpecialBlockIndex = -1;
	int l_iCount = 0;
	LinkedList *l_llOutterLine = nullptr, *l_llLine = nullptr, *l_llTile = nullptr;
	
	int l_iTileNumber = 0;
	for (int line = 0; line < m_llTileMap->length(); line++)
	{
		l_llOutterLine = m_llTileMap->ReadFromList(line);
		if (l_llOutterLine == nullptr) continue;
		l_llLine = m_llTileMap->read_data(line, nullptr);
		for (int tile = 0; tile < l_llLine->length(); tile++)
		{
			l_llTile = l_llLine->ReadFromList(tile);
			if (l_llTile == nullptr) continue;
			
			l_iTileNumber = l_llTile->get_data(0);
			if (l_iTileNumber < 0) continue;
			
			l_sFlags = l_llTile->get_data(l_sFlags);
			if (l_sFlags == "") continue;
			if (l_sFlags == "Undefined") continue;
			if (l_sFlags == "Unfound") continue;

			l_fSpecialBlockIndex = l_llTile->get_data(0.0f);

			//TEST TO SEE IF THERE IS NO SPECIAL BLOCK:
			if (l_fSpecialBlockIndex == -1)
			{
				
				string l_sFlagsLeft = l_sFlags, l_sFlagsSend = "";
				if (!Utility::SplitStringAtPoint(&l_sFlagsLeft, &l_sFlagsSend, '_'))
					continue;

				switch (l_sFlagsLeft[0])
				{
					case 'S':
						setup_specialBlocksS(l_fSpecialBlockIndex, f_deltaTime, l_sFlagsSend, l_llTile, tile, line);
						//m_cSpecialBlockLoaded[l_iCount++] = 'S';
					break;
					case 'E':
						setup_enemiesS(l_fSpecialBlockIndex, f_deltaTime, l_sFlagsSend, l_llTile, tile, line);
						//m_cSpecialBlockLoaded[l_iCount++] = 'E';
						break;
					case 'I':
						setup_itemsS(l_fSpecialBlockIndex, f_deltaTime, l_sFlagsSend, l_llTile, tile, line);
						//m_cSpecialBlockLoaded[l_iCount++] = 'I';
						break;
				}
					
				l_sFlags = "";
			}
		}
	}
	return true;
}

bool Tilemap::setup_specialBlocksS(float f_fBlockIndex, float f_deltaTime, string f_sFlags, LinkedList *f_llTile, int f_iTile, int f_iLine)//Special Blocks begining with an S
{
	LinkedList *l_llSpecialFileNames = nullptr, *l_llTilesetReference = nullptr;
	float l_fSpecialBlockIndex = -1;
	int l_iSpecialBlockTilesetIndex = -1;

	int l_iTileNumber = f_llTile->get_data(0);

	//If there is no Special block added
				//Find a place for the specialBlock:
	for (int i = 0; i < m_specialBlocksLength; i++)
		if (m_specialBlocks[i] == nullptr)
		{
			f_fBlockIndex = i;
			break;
		}
	if (f_fBlockIndex == -1)
	{
		cout << "Tilemap::update_specialBlocks: Not enough space for more Special blocks. Increase m_specialBlocks array." << endl;
		return false;
	}
	f_fBlockIndex += 0.0f;
	m_specialBlocks[(int)f_fBlockIndex] = new SpecialBlock();
	f_llTile->set_data(f_fBlockIndex);
	int l_iPolymorphSignal = -1;
	m_specialBlocks[(int)f_fBlockIndex]->set_polymorphSignal(&l_iPolymorphSignal);
	
	l_llSpecialFileNames = Utility::SplitStringToLL(f_sFlags, '_');
	string l_sCurrentFlag = "";
	for (int i = 0; i < l_llSpecialFileNames->length(); i++)//Loads the information for the block from files
	{
		l_sCurrentFlag = l_llSpecialFileNames->read_data(i, l_sCurrentFlag);

		if (l_sCurrentFlag == "") continue;
		if (l_sCurrentFlag == "Undefined") continue;
		if (l_sCurrentFlag == "Unfound") continue;

		if (l_sCurrentFlag == "Own") l_sCurrentFlag = to_string(l_iTileNumber);//Own is a short cut to avoid repeats
		l_iSpecialBlockTilesetIndex = Utility::convert_stringToInt(l_sCurrentFlag);
		if (l_iSpecialBlockTilesetIndex == -1) continue;//No Number found
		int l_iTilesetRow = -1, l_iTilesetCol = -1;
		Utility::GetRowAndColFromNumber(m_iNumberOfRows, m_iNumberOfCols, l_iSpecialBlockTilesetIndex, l_iTilesetRow, l_iTilesetCol);
		l_llTilesetReference = get_tileInformationFromTilesetLocation(l_iTilesetRow, l_iTilesetCol, nullptr);

		if (l_llTilesetReference == nullptr) continue;
		//The output is getting the tileset it'd like to import the special attributes from:
		//In the tilesetInformation which this is, there's an int which refers to the string with the locations
		float l_fLoaderIndex = l_llTilesetReference->get_data(0.0f);
		if (l_fLoaderIndex == -1) continue;
		if (m_sSpecialBlockLoaders[(int)l_fLoaderIndex] == nullptr) continue;

		if (m_audio != nullptr)
			m_specialBlocks[(int)f_fBlockIndex]->set_audio(m_audio);

		m_specialBlocks[(int)f_fBlockIndex]->load_specialBlock("GameFiles/Mario3/SpecialBlocks/" + *m_sSpecialBlockLoaders[(int)l_fLoaderIndex] + ".txt");//Actually load the information in the file.
		if (l_iPolymorphSignal > -1)
		{
			//This handles blocks that are vastly difforent from the norm such as end of level blocks
			handle_polymorphOfSpecialBlocks((int)f_fBlockIndex, l_iPolymorphSignal);
			break;
		}
	}
	m_specialBlocks[(int)f_fBlockIndex]->set_itemSpawnInformation(m_itemSpawnInfo);
	if (m_audio != nullptr)
		m_specialBlocks[(int)f_fBlockIndex]->set_audio(m_audio);
	if (p_particleManager != nullptr)
		m_specialBlocks[(int)f_fBlockIndex]->set_particleManager(p_particleManager);
	m_specialBlocks[(int)f_fBlockIndex]->set_tilemapLocation(f_iTile - 1, f_iLine); //Sets the location of the block (X and Y world cords)
	
	m_specialBlocks[(int)f_fBlockIndex]->set_scaleSize(m_fScaleSize);
	m_specialBlocks[(int)f_fBlockIndex]->set_singleSize(16, 16);
	m_specialBlocks[(int)f_fBlockIndex]->set_cameraLock(true);
	if (m_sb != nullptr)
		m_specialBlocks[(int)f_fBlockIndex]->setup_sprite(m_sb, m_tsmTexture);//Setup the sprite for example texture
	if (p_camera != nullptr)
		m_specialBlocks[(int)f_fBlockIndex]->set_camera(p_camera);

	for (int c = 0; c < 10; c++)
		if (p_alternativeCameras[c] != nullptr)
			m_specialBlocks[(int)f_fBlockIndex]->set_altCamera(p_alternativeCameras[c], c);

	delete l_llSpecialFileNames; //SplitStringToLL makes a linkedList

	return true;
}

bool Tilemap::setup_enemiesS(float f_fBlockIndex, float f_deltaTime, string f_sFlags, LinkedList *f_llTile, int f_iTile, int f_iLine)//Special Blocks begining with an S
{
	LinkedList *l_llSpecialFileNames = nullptr, *l_llTilesetReference = nullptr;
	float l_fSpecialBlockIndex = -1;
	int l_iSpecialBlockTilesetIndex = -1;

	int l_iTileNumber = f_llTile->get_data(0);

	//If there is no Special block added
				//Find a place for the specialBlock:
	for (int i = 0; i < m_enemiesLength; i++)
		if (m_enemies[i] == nullptr)
		{
			f_fBlockIndex = i;
			break;
		}
	if (f_fBlockIndex == -1)
	{
		cout << "Tilemap::setup_enemiesS: Not enough space for more Special blocks. Increase m_specialBlocks array." << endl;
		return false;
	}
	f_fBlockIndex += 0.0f;
	m_enemies[(int)f_fBlockIndex] = new Enemy();
	if(m_bRenderSpecialBlocks)
		f_llTile->set_data(-9001.0f);

	m_enemies[(int)f_fBlockIndex]->set_spriteBatch(m_sb);
	m_enemies[(int)f_fBlockIndex]->set_audio(m_audio);
	m_enemies[(int)f_fBlockIndex]->set_playersReference(p_players[0], 0);
	m_enemies[(int)f_fBlockIndex]->set_playersReference(p_players[1], 1);
	m_enemies[(int)f_fBlockIndex]->set_id((int)f_fBlockIndex);

	//ItemSpawnInformation *l_isi = &m_itemSpawnInfo[(int)l_fSpecialBlockIndex];
				//m_specialBlocks[(int)f_fBlockIndex]->set_itemSpawnInformation(m_itemSpawnInfo);
	//m_specialBlocks[(int)l_fSpecialBlockIndex]->set_itemManager(p_itemManager);//Allows the special block to spawn items like mushrooms
				//m_specialBlocks[(int)f_fBlockIndex]->set_tilemapLocation(f_iTile - 1, f_iLine); //Sets the location of the block (X and Y world cords)
	//cout << "tile: " << tile - 1 << "line: " << line<< endl;
	l_llSpecialFileNames = Utility::SplitStringToLL(f_sFlags, '_');
	string l_sCurrentFlag = "";
	for (int i = 0; i < l_llSpecialFileNames->length(); i++)//Loads the information for the block from files
	{
		l_sCurrentFlag = l_llSpecialFileNames->read_data(i, l_sCurrentFlag);

		if (l_sCurrentFlag == "") continue;
		if (l_sCurrentFlag == "Undefined") continue;
		if (l_sCurrentFlag == "Unfound") continue;

		if (l_sCurrentFlag == "Own") l_sCurrentFlag = to_string(l_iTileNumber);//Own is a short cut to avoid repeats
		l_iSpecialBlockTilesetIndex = Utility::convert_stringToInt(l_sCurrentFlag);
		if (l_iSpecialBlockTilesetIndex == -1) continue;//No Number found
		int l_iTilesetRow = -1, l_iTilesetCol = -1;
		Utility::GetRowAndColFromNumber(m_iNumberOfRows, m_iNumberOfCols, l_iSpecialBlockTilesetIndex, l_iTilesetRow, l_iTilesetCol);
		l_llTilesetReference = get_tileInformationFromTilesetLocation(l_iTilesetRow, l_iTilesetCol, nullptr);

		if (l_llTilesetReference == nullptr) continue;
		//The output is getting the tileset it'd like to import the special attributes from:
		//In the tilesetInformation which this is, there's an int which refers to the string with the locations
		float l_fLoaderIndex = l_llTilesetReference->get_data(0.0f);
		if (l_fLoaderIndex == -1) continue;
		if (m_sSpecialBlockLoaders[(int)l_fLoaderIndex] == nullptr) continue;

		m_enemies[(int)f_fBlockIndex]->load_fromFile("GameFiles/Mario3/Enemies/" + *m_sSpecialBlockLoaders[(int)l_fLoaderIndex] + ".txt");//Actually load the information in the file.
	}
	m_enemies[(int)f_fBlockIndex]->set_scaleSize(m_fScaleSize);
	m_enemies[(int)f_fBlockIndex]->set_cameraLock(true);
	m_enemies[(int)f_fBlockIndex]->set_location((f_iTile - 1) * get_actualWandH(), f_iLine * get_actualWandH());
	m_enemies[(int)f_fBlockIndex]->set_tilemap(this);

	if(m_bDebugReturnOnce)
		m_enemies[(int)f_fBlockIndex]->update_collision(true);
	else
		m_enemies[(int)f_fBlockIndex]->update_collision();
	m_bDebugReturnOnce = true;

	if (p_camera != nullptr)
		m_enemies[(int)f_fBlockIndex]->set_camera(p_camera);

	for (int c = 0; c < 10; c++)
		if (p_alternativeCameras[c] != nullptr)
			m_enemies[(int)f_fBlockIndex]->set_altCamera(p_alternativeCameras[c], c);

	delete l_llSpecialFileNames; //SplitStringToLL makes a linkedList
	
	return true;
}

bool Tilemap::setup_itemsS(float f_fBlockIndex, float f_deltaTime, string f_sFlags, LinkedList *f_llTile, int f_iTile, int f_iLine)
{
	//If there is no Special block added
	//Find a place for the specialBlock:
	for (int i = 0; i < m_specialBlocksLength; i++)
		if (m_specialBlocks[i] == nullptr)
		{
			f_fBlockIndex = i;
			break;
		}
	if (f_fBlockIndex == -1)
	{
		cout << "Tilemap::update_specialBlocks: Not enough space for more Special blocks. Increase m_specialBlocks array." << endl;
		return false;
	}

	float l_fSpecialBlockIndex = -1;
	int l_iSpecialBlockTilesetIndex = -1;

	LinkedList *l_llSpecialFileNames = Utility::SplitStringToLL(f_sFlags, '_');
	LinkedList *l_llCurrentItem = nullptr, *l_llTilesetReference = nullptr;;
	string l_sCurrentFlag = "";
	for (int i = 0; i < l_llSpecialFileNames->length(); i++)//Loads the information for the block from files
	{
		l_sCurrentFlag = l_llSpecialFileNames->read_data(i, l_sCurrentFlag);

		if (l_sCurrentFlag == "") continue;
		if (l_sCurrentFlag == "Undefined") continue;
		if (l_sCurrentFlag == "Unfound") continue;
		if (l_sCurrentFlag == "Own") continue;
		if (l_sCurrentFlag == "33") continue;

		//cout << "l_sCurrentFlag: " << l_sCurrentFlag << endl;

		//GRAB THE LOADER:
		l_iSpecialBlockTilesetIndex = Utility::convert_stringToInt(l_sCurrentFlag);
		if (l_iSpecialBlockTilesetIndex == -1) continue;//No Number found
		int l_iTilesetRow = -1, l_iTilesetCol = -1;
		Utility::GetRowAndColFromNumber(m_iNumberOfRows, m_iNumberOfCols, l_iSpecialBlockTilesetIndex, l_iTilesetRow, l_iTilesetCol);
		l_llTilesetReference = get_tileInformationFromTilesetLocation(l_iTilesetRow, l_iTilesetCol, nullptr);

		if (l_llTilesetReference == nullptr) continue;
		//The output is getting the tileset it'd like to import the special attributes from:
		//In the tilesetInformation which this is, there's an int which refers to the string with the locations
		float l_fLoaderIndex = l_llTilesetReference->get_data(0.0f);
		if (l_fLoaderIndex == -1) continue;
		if (m_sSpecialBlockLoaders[(int)l_fLoaderIndex] == nullptr) continue;


		if (m_llItemsToSpawn == nullptr)
		{
			m_llItemsToSpawn = new LinkedList();
			l_llCurrentItem = m_llItemsToSpawn;
		}
		else
			l_llCurrentItem = m_llItemsToSpawn->AddNewToEnd();

		ItemSpawnInformationPacket *l_isipNew = new ItemSpawnInformationPacket();
		l_isipNew->p_sItemName = *m_sSpecialBlockLoaders[(int)l_fLoaderIndex];
		l_isipNew->p_iLocationX = (f_iTile - 1) * get_actualWandH();
		l_isipNew->p_iLocationY = f_iLine * get_actualWandH();
		l_isipNew->p_ise = ItemSpawnInformation::ItemSpawnEvent::iseAsIs;

		l_llCurrentItem->set_data((void*)l_isipNew, true);
	}
	if (m_bRenderSpecialBlocks)
	{
		//Don't draw the item:
		Tilemap_Tile l_ttLocation = { f_iTile - 1, f_iLine };
		set_tileInPlace(l_ttLocation, -1, false, true);
	}
	return true;
}

bool Tilemap::handle_polymorphOfSpecialBlocks(int f_iSpecialBlockIndex, int f_iPolymorph)
{
	if (f_iPolymorph <= -1 && f_iPolymorph > 0) return false;//Adjust this as I create more polymorphs
	if (m_specialBlocks[f_iSpecialBlockIndex] != nullptr)
		delete m_specialBlocks[f_iSpecialBlockIndex];
	switch (f_iPolymorph)
	{
	case 0:
		m_specialBlocks[f_iSpecialBlockIndex] = new EndOfLevelBlock();
		m_specialBlocks[f_iSpecialBlockIndex]->set_playerOneEndCard(m_iPlayerOneFinishedLevel);
		m_specialBlocks[f_iSpecialBlockIndex]->set_playerTwoEndCard(m_iPlayerTwoFinishedLevel);

		break;
	}
	m_specialBlocks[f_iSpecialBlockIndex]->set_polymorphSignal(nullptr);
	m_specialBlocks[f_iSpecialBlockIndex]->set_polymorph(f_iPolymorph);
	return true;
}

bool Tilemap::Draw_eachCamera(Camera *f_camera)
{
	if (!m_bTilemapLoaded) return false;
	if (m_llTileMap == nullptr) return false;
	if (f_camera == nullptr) return false;
	LinkedList *l_llOutterLine = nullptr, *l_llLine = nullptr, *l_llTile = nullptr;
	int l_iTileNumber = 0;
	int l_iLocationX = 0, l_iLocationY = 0;
	int l_iNewScreenSizeX = 0, l_iNewScreenSizeY = 0;
	int m_iSpriteRow = 0, m_iSpriteCol = 0;
	char l_cFlipFlag = '`';
	float l_fSpecialBlock = 0;
	//m_fScaleSize = ((SCREEN_WIDTH / m_iSingleWidth) * 0.1f);// *0.25f;
	m_fScaleSize = ((SCREEN_HEIGHT / m_iSingleHeight) * 0.0769230769230769f);// *0.25f;

	int l_fNewWidth = m_fScaleSize * m_iSingleWidth;
	int l_fNewHeight = m_fScaleSize * m_iSingleHeight;
	m_iScaledWidthAndHeight = l_fNewWidth;
	for (int line = 0; line < m_llTileMap->length(); line++)
	{
		if (l_llOutterLine == nullptr)
			l_llOutterLine = m_llTileMap;
		else
			l_llOutterLine = l_llOutterLine->get_ahead();
		//l_llOutterLine = m_llTileMap->ReadFromList(line);
		if (l_llOutterLine == nullptr) continue;
		//l_llLine = m_llTileMap->read_data(line, nullptr);
		l_llLine = l_llOutterLine->get_data(nullptr);
		l_llTile = l_llLine;

		//This draw is pretty intense so I want to bother with the tiles I can see:
		int l_iDrawTileStart = (int)(f_camera->get_x() / l_fNewWidth) - 3;
		if (l_iDrawTileStart < 0) l_iDrawTileStart = 0;
		int l_iDrawTileEnd = l_iDrawTileStart + (f_camera->get_screenWidth() / l_fNewWidth) + 6;
		if (l_iDrawTileEnd > l_llLine->length()) l_iDrawTileEnd = l_llLine->length();

		for (int tile = l_iDrawTileStart; tile < l_iDrawTileEnd; tile++)
		{
			if (tile != l_iDrawTileStart)
				l_llTile = l_llTile->get_ahead();
			else
				l_llTile = l_llLine->ReadFromList(tile);

			if (l_llTile == nullptr) continue;

			l_iTileNumber = l_llTile->get_data(0);
			if (l_iTileNumber < 0) continue;

			l_iLocationX = m_iLocationX + (l_fNewWidth * (tile - 1));
			if (!m_bDrawFromBottom && f_camera != nullptr)
				l_iLocationY = m_iLocationY + (l_fNewHeight * line);
			else
				l_iLocationY = f_camera->get_worldHeight() - (l_fNewHeight * (m_llTileMap->length() - line));

			//This never worked as a shortcut and now I'm using the float to store the specialblock index
			//float l_fCurrentY = l_iLocationY;
			//l_llTile->set_data(l_fCurrentY);

			l_cFlipFlag = l_llTile->get_data('`');

			//SpecialBlock
			l_fSpecialBlock = l_llTile->get_data(0.0f);
			if (l_fSpecialBlock > -1 && m_bRenderSpecialBlocks)
			{
				if (m_specialBlocks[(int)l_fSpecialBlock] != nullptr)
				{
					/*
						WE DO NOT DRAW SPECIAL BLOCKS AT THE SAME TIME AS THE TILEMAP.
						Why?
						Well Items need to appear from behind the special blocks.
						This means the level needs to decide the draw order and not the
						tilemap.

						We send a location on creation and they're their own sprites,
						so we don't need it to be in the loop.
						All we need to do is not to draw it twice, and this set of if
						statments is where we would draw one and so we need to move on.

						The specialBlock draw method is Draw_specialBlocks();
					*/
					/*
						HOWEVER,
						Special blocks can override this and this function tells us if they
						would like to override it:
					*/
					if (m_specialBlocks[(int)l_fSpecialBlock]->get_drawThisTileUnderMe() > -1)
					{
						l_iTileNumber = m_specialBlocks[(int)l_fSpecialBlock]->get_drawThisTileUnderMe();
						//cout << "l_iTileNumber: " << l_iTileNumber << endl;
					}
					else
						continue;//They also don't want to override
				}
			}
			else if (m_bRenderSpecialBlocks && l_fSpecialBlock == -9001.0f)//Case for enemies
				continue;

			//SDL_Rect renderLocation = { l_iLocationX, l_iLocationY, l_fNewWidth, l_fNewHeight };

			//Getting the location from number is a repeatative task so this makes it easier:
			if (!Utility::GetRowAndColFromNumber(m_iNumberOfRows, m_iNumberOfCols, l_iTileNumber, m_iSpriteRow, m_iSpriteCol))//Last two are pass by ref and are the result
				continue;//Couldn't convert and get the rows/cols

			//SDL_Rect *renderLocation = new SDL_Rect({ l_iLocationX, l_iLocationY, l_fNewWidth, l_fNewHeight });
			//SDL_Rect *sourceLocation = new SDL_Rect({ m_iSpriteCol * m_iSingleWidth, m_iSpriteRow * m_iSingleHeight, m_iSingleWidth, m_iSingleHeight });

			//Utility::ReturnDrawForCulling(renderLocation, sourceLocation, f_camera->get_x(), f_camera->get_screenX(), f_camera->get_screenWidth(), m_fScaleSize);

			float l_fRenderWidth = l_fNewWidth;
			float l_fRenderHeight = l_fNewHeight;
			float l_fScreenLocationX = l_iLocationX - f_camera->get_x() + f_camera->get_screenX();
			float l_fScreenLocationY = l_iLocationY - f_camera->get_y() + f_camera->get_screenY();

			int l_iSourceX = m_iSpriteCol * m_iSingleWidth;
			int l_iSourceY = m_iSpriteRow * m_iSingleHeight;
			int l_iSourceWidth = m_iSingleWidth;
			int l_iSourceHeight = m_iSingleHeight;

			/*
				This code seems repeated on Sprite and Background however it's all slightly difforent due to how I render them all
				I may split this out before the handin.

				Tilemap was the first, then sprite then Background.

				X:
			*/

			if (l_fScreenLocationX + l_fNewWidth < f_camera->get_screenX())
			{
				continue;
			}
			else if (l_fScreenLocationX < f_camera->get_screenX())
			{
				//If they're moving off screen on the left
				//l_fRenderWidth -= f_camera->get_x() - ((tile - 1) * l_fNewWidth);
				l_fRenderWidth -= f_camera->get_x() - (l_iLocationX);
				l_iLocationX -= l_fRenderWidth;
				l_iLocationX += l_fNewWidth;
				l_iSourceWidth = l_fRenderWidth / m_fScaleSize;
				l_iSourceX += m_iSingleWidth - l_iSourceWidth;
			}

			if (l_fScreenLocationX > f_camera->get_screenX() + f_camera->get_screenWidth())
			{
				//If they're off screen on the Right
				continue;
			}
			else if (l_fScreenLocationX + l_fNewWidth > f_camera->get_screenX() + f_camera->get_screenWidth())
			{
				//If they're moving on screen on the right
				//l_fRenderWidth += f_camera->get_x() + f_camera->get_screenWidth() - ((tile) * l_fNewWidth);
				//l_fRenderWidth += f_camera->get_x() + f_camera->get_screenWidth() - ((tile) * l_fNewWidth);
				l_fRenderWidth += f_camera->get_x() + f_camera->get_screenWidth() - (l_iLocationX + m_iScaledWidthAndHeight);
				l_iSourceWidth = l_fRenderWidth / m_fScaleSize;
			}

			//Y:
			if (l_fScreenLocationY + l_fNewHeight < f_camera->get_screenY())
			{
				continue;
			}
			else if (l_fScreenLocationY < f_camera->get_screenY())
			{
				//If they're moving off screen on the left
				//l_fRenderWidth -= f_camera->get_x() - ((tile - 1) * l_fNewWidth);
				l_fRenderHeight -= f_camera->get_y() - (l_iLocationY);
				l_iLocationY -= l_fRenderHeight;
				l_iLocationY += l_fNewHeight;
				l_iSourceHeight = l_fRenderHeight / m_fScaleSize;
				l_iSourceY += m_iSingleHeight - l_iSourceHeight;
			}

			if (l_fScreenLocationY > f_camera->get_screenY() + f_camera->get_screenHeight())
			{
				//If they're off screen on the Right
				continue;
			}
			else if (l_fScreenLocationY + l_fNewHeight > f_camera->get_screenY() + f_camera->get_screenHeight())
			{
			//If they're moving on screen on the right
			//l_fRenderWidth += f_camera->get_x() + f_camera->get_screenWidth() - ((tile) * l_fNewWidth);
			//l_fRenderWidth += f_camera->get_x() + f_camera->get_screenWidth() - ((tile) * l_fNewWidth);
			l_fRenderHeight += f_camera->get_y() + f_camera->get_screenHeight() - (l_iLocationY + m_iScaledWidthAndHeight);
			l_iSourceHeight = l_fRenderHeight / m_fScaleSize;
			}

			//Full Cull:
			if ((int)l_fRenderWidth == 0 || (int)l_fRenderHeight == 0 || l_iSourceHeight == 0 || l_iSourceWidth == 0)
				continue;

			//CAMERA NUDE:

			if (m_bLockToCamera && f_camera != nullptr)
			{
				l_iLocationX -= f_camera->get_x();
				l_iLocationX += f_camera->get_screenX();
				l_iLocationY -= f_camera->get_y();
				l_iLocationY += f_camera->get_screenY();
			}

			/*if (tile == 10 && line == 6)
				cout << " out : l_iLocationX: " << l_iLocationX << " l_fRenderWidth: " << l_fRenderWidth << " l_iSourceWidth: " << l_iSourceWidth << endl;*/

				//if (tile == 10 && line == 6)
				//{
				//	l_iSourceX = 0;
				//	l_iSourceY = 0;
				//}

			SDL_Rect renderLocation = { (int)l_iLocationX, (int)l_iLocationY, (int)l_fRenderWidth, (int)l_fRenderHeight };
			SDL_Rect sourceLocation = { (int)l_iSourceX, (int)l_iSourceY, (int)l_iSourceWidth, (int)l_iSourceHeight };

			//SDL_RenderCopyEx(m_sb->get_renderer(), m_tsmTexture->get_texturePointer(), &sourceLocation, &renderLocation, 0, NULL, SDL_FLIP_NONE);
			if (l_cFlipFlag == '`')
				SDL_RenderCopyEx(m_sb->get_renderer(), m_tsmTexture->get_texturePointer(), &sourceLocation, &renderLocation, 0, NULL, SDL_FLIP_NONE);
			else
			{
				switch (l_cFlipFlag)
				{
				case 'H':
					SDL_RenderCopyEx(m_sb->get_renderer(), m_tsmTexture->get_texturePointer(), &sourceLocation, &renderLocation, 0, NULL, SDL_FLIP_HORIZONTAL);
					break;
				case 'V':
					SDL_RenderCopyEx(m_sb->get_renderer(), m_tsmTexture->get_texturePointer(), &sourceLocation, &renderLocation, 0, NULL, SDL_FLIP_VERTICAL);
					break;
				case 'B':
					SDL_RendererFlip flip = (SDL_RendererFlip)(SDL_FLIP_HORIZONTAL | SDL_FLIP_VERTICAL);
					SDL_RenderCopyEx(m_sb->get_renderer(), m_tsmTexture->get_texturePointer(), &sourceLocation, &renderLocation, 0, NULL, flip);
					break;
				}
			}
			l_cFlipFlag = '`';
			//delete renderLocation; delete sourceLocation;
		}
	}
	return true;
}

bool Tilemap::Draw_specialBlocks(bool f_bRenderAbovePlayers)//Just draws Special Blocks
{
	for (int i = 0; i < m_specialBlocksLength; i++)
		if (m_specialBlocks[i] != nullptr)
			if (m_specialBlocks[i]->get_renderAbovePlayer() == f_bRenderAbovePlayers)
				m_specialBlocks[i]->RenderToScreen(m_sb->get_renderer());

	return true;
}

bool Tilemap::update_specialCollisions(int f_tileX, int f_tileY, int f_iDirection, int f_iActor, int f_iPlayer)
{
	bool l_bIsPlayerBig = false;
	if (f_iPlayer != -1)
	{
		switch (f_iPlayer)
		{
		case 1:
			l_bIsPlayerBig = *m_bPlayerOneSize;
			break;
		case 2:
			l_bIsPlayerBig = *m_bPlayerTwoSize;
			break;
		}
	}
	for (int i = 0; i < m_specialBlocksLength; i++)
		if (m_specialBlocks[i] != nullptr)
		{
			if(m_specialBlocks[i]->get_didDrawThisFrame())
				m_specialBlocks[i]->update_actionActivation(f_tileX, f_tileY, (int)f_iDirection, f_iActor, l_bIsPlayerBig);
		}

	return true;
}

bool Tilemap::update_entiesReactions(SDL_Rect f_rectCollision, int f_iDirection, int f_iActor, int f_iPlayer, int *f_iFeedback, bool f_bAlsoSpecialBlocks, int f_iAdditionalNumber)
{
	if (f_iActor <= -1) return false;
	if (f_iActor >= 2)
	{
		if (m_enemies[f_iPlayer] != nullptr)
			if (m_enemies[f_iPlayer]->get_isBeingHeld())return false;
	}
	bool l_bIsPlayerBig = false;
	if (f_iActor == 0) //If player
	{
		//React to Players:
		if (f_iPlayer != -1)
		{
			switch (f_iPlayer)
			{
			case 1:
				if(m_bPlayerOneSize != nullptr)
					l_bIsPlayerBig = *m_bPlayerOneSize;
				break;
			case 2:
				if(m_bPlayerTwoSize != nullptr)
					l_bIsPlayerBig = *m_bPlayerTwoSize;
				break;
			}
		}
	}
	if (f_iActor <= 1)
	{
		for (int i = 0; i < m_enemiesLength; i++)
			if (m_enemies[i] != nullptr)
			{
				if(f_iActor == 0 && m_enemies[i]->get_didDrawThisFrame() || f_iActor >= 1)
				{
					if (f_iPlayer == m_enemies[i]->get_id()) continue;//Don't activate yourself
					if (m_enemies[i]->get_isBeingHeld()) continue;//Don't activate if' you're being held
				}
				m_enemies[i]->update_actionActivation(f_rectCollision, (int)f_iDirection, f_iActor, l_bIsPlayerBig, f_iFeedback, f_iAdditionalNumber);
			}
	}
	//React for special blocks and we're sending 
	if (f_bAlsoSpecialBlocks)
	{
		Tilemap_Tile l_ttMyLocation = get_tileFromLocation(f_rectCollision.x + (f_rectCollision.w / 2), f_rectCollision.y + (f_rectCollision.h / 2));

		switch (f_iDirection)
		{
		case 4:
			l_ttMyLocation.x -= 1;
			break;
		case 6:
			l_ttMyLocation.x += 1;
			break;
		case 8:
			l_ttMyLocation.y -= 1;
			break;
		case 2:
			l_ttMyLocation.y += 1;
			break;
		}

		for (int i = 0; i < m_specialBlocksLength; i++)
			if (m_specialBlocks[i] != nullptr)
			{
				if (f_iActor == 0 && m_specialBlocks[i]->get_didDrawThisFrame() || f_iActor >= 1)
					m_specialBlocks[i]->update_actionActivation(l_ttMyLocation.x, l_ttMyLocation.y, f_iDirection, f_iActor);
			}
	}
	//PolyMorph Blocks:
	//if (f_bAlsoSpecialBlocks)
	//{
		for (int i = 0; i < m_specialBlocksLength; i++)
			if (m_specialBlocks[i] != nullptr)
				if (m_specialBlocks[i]->get_polymorph() > -1)
				{
					if (f_iActor == 0 && m_specialBlocks[i]->get_didDrawThisFrame() || f_iActor >= 1)
						m_specialBlocks[i]->update_actionActivationPolymorphVersion(f_rectCollision, f_iDirection, f_iActor, f_iAdditionalNumber, p_players[f_iAdditionalNumber]);
				}
	//}
	return true;
}

Tilemap::Tilemap_Tile Tilemap::get_tileInTilesetLocation(int f_iNumber)
{
	Tilemap_Tile l_ttReturn;
	if (f_iNumber == -1) return l_ttReturn;
	Utility::GetRowAndColFromNumber(m_iNumberOfRows, m_iNumberOfCols, f_iNumber, l_ttReturn.y, l_ttReturn.x);
	return l_ttReturn;
}

bool Tilemap::load_tileset(MapLoadType f_lt, string f_sData)
{
	if (f_sData == "") return false;

	//Reset the special blocks if we're loading a new tileset
	for (int i = 0; i < m_sSpecialBlockLoadersLength; i++)
	{
		if (m_sSpecialBlockLoaders[i] != nullptr)
			delete m_sSpecialBlockLoaders[i];
		m_sSpecialBlockLoaders[i] = nullptr;
	}
	switch (f_lt)
	{
	case FromFile:
		m_iNumberOfCols = 1;
		m_iNumberOfRows = 1;
		load_tilesetFromFile(f_sData);
		break;
	case FromString:
		m_iNumberOfCols = 1;
		m_iNumberOfRows = 1;
		//load_tilesetFromString(f_sData);
		break;
	default:
		return false;
	}

	m_bNeedToLoadSpecialBlocks = true;
	return true;
}

bool Tilemap::load_tilesetFromFile(string f_sData)
{
	if (m_sb == nullptr) return false;
	if (f_sData == "") return false;
	if (Utility::SearchStringForChar(f_sData, '.') != -1)//Incase you send it with the .txt on the end. It'll grab everything before the first .
	{
		string l_sLeft = f_sData, l_sRight = "";
		Utility::SplitStringAtPoint(&l_sLeft, &l_sRight, '.');
		f_sData = l_sLeft;
	}
	if (!load_tilesetMeta(f_sData + ".txt")) { cout << "Tilemap::load_tilesetFromFile: Failed to load file(" << f_sData << ")" << endl; return false; }
	if (!load_tilesetInformation(f_sData + "_collisions.txt")) { cout << "Tilemap::load_tilesetFromFile: Failed to load file(" << f_sData << ")" << endl; return false; }

	return true;
}

bool Tilemap::load_tilesetMeta(string f_sData)
{
	ifstream infile(f_sData);
	if (!infile.is_open())
	{
		if (m_bDebug_TilemapLoad) cout << "load_tilesetFromFile(string f_sData):  Could not open file." << endl;
		return false;
	}

	string f_sGetLine = "";
	string f_sLeft = "", f_sRight = "";
	TileSetMeta l_tsmCurrent;
	while (getline(infile, f_sGetLine))
	{
		f_sLeft = f_sGetLine;
		if (Utility::SplitStringAtPoint(&f_sLeft, &f_sRight, ' '))
		{
			l_tsmCurrent = convert_stringToTileSetMeta(f_sLeft);
			if (l_tsmCurrent != TileSetMeta::None)
				set_tilesetMeta(l_tsmCurrent, f_sRight);
		}
	}
	infile.close();
	return true;
}

bool Tilemap::load_tilesetInformation(string f_sData)
{
	ifstream infile(f_sData);
	if (!infile.is_open())
	{
		if (m_bDebug_TilemapLoad) cout << "load_tilesetCollisions(string f_sData [" << f_sData << "]):  Could not open file." << endl;
		return false;
	}

	if (m_llTileInformation != nullptr)
		delete m_llTileInformation;
	m_llTileInformation = new LinkedList();

	string f_sGetLine = "";
	LinkedList *l_llCollision = nullptr;
	LinkedList *l_llCurrentLine = nullptr;
	LinkedList *l_llCurrentCell = nullptr;
	while (getline(infile, f_sGetLine))
	{
		l_llCollision = Utility::SplitStringToLL(f_sGetLine, '|');
		l_llCurrentLine = new LinkedList();
		m_llTileInformation->AddNewToEnd(l_llCurrentLine);
		for (int i = 0; i < l_llCollision->length(); i++)
		{
			if (Utility::SearchStringForChar(l_llCollision->read_data(i, f_sGetLine), '/') > -1)
			{
				LinkedList* l_llFlags = Utility::SplitStringToLL(l_llCollision->read_data(i, f_sGetLine), '/');//This creates a linked list, ensure you delete it to avoid leaks
				string f_sFirstFlag = l_llFlags->read_data(0, f_sGetLine);
				l_llCurrentCell = l_llCurrentLine->AddNewToEnd(f_sFirstFlag);
				for (int f = 1; f < l_llFlags->length(); f++)
				{
					string f_sCurrentFlag = "";
					f_sCurrentFlag = l_llFlags->read_data(f, f_sCurrentFlag);
					if (f_sCurrentFlag.length() > 1)
					{
						//Flags which aren't characters
						LinkedList* l_llInformation = Utility::SplitStringToLL(f_sCurrentFlag, '_');//This creates a linked list, ensure you delete it to avoid leaks
						int l_iDuration = 0;
						int l_iFrames  = 0;
						switch (f_sCurrentFlag[0])
						{
							case 'A':
								//Animation Flag
								if (l_llInformation->length() < 3) continue;
								
								l_iFrames = Utility::convert_stringToInt(l_llInformation->read_data(1, f_sGetLine));
								l_iDuration = Utility::convert_stringToInt(l_llInformation->read_data(2, f_sGetLine));
							
								l_llCurrentCell->set_data(add_animation(l_iFrames, l_iDuration, (i * f), f_sCurrentFlag));
								break;
							case 'S':
							case 'E':
							case 'I':
								//Special Block flag
								if (l_llInformation->length() < 2) continue;
								int l_iNextIndex = l_llCurrentCell->get_data(i);
								if (l_iNextIndex != -1) continue;//Can only have one
								for (int i = 0; i < m_sSpecialBlockLoadersLength; i++)
									if (m_sSpecialBlockLoaders[i] == nullptr)
									{
										l_iNextIndex = i;
										break;
									}
								if (l_iNextIndex == -1)
								{
									cout << "load_tilesetInformation: Too many SpecialBlock files, extend m_sSpecialBlockLoaders in Tilemap to increase the storage" << endl;
									continue;
								}

								m_sSpecialBlockLoaders[l_iNextIndex] = new string(l_llInformation->read_data(1, f_sGetLine));
								m_cSpecialBlockSource[l_iNextIndex] = f_sCurrentFlag[0];
								l_llCurrentCell->set_data(l_iNextIndex + 0.0f);//Store a link to that string. This allows us to technically store two strings in one linked list
								break;
						}
						delete l_llInformation;
					}
				}
				delete l_llFlags;
			}
			else
				l_llCurrentCell = l_llCurrentLine->AddNewToEnd(l_llCollision->read_data(i, f_sGetLine));
		}
			
		delete l_llCollision;
	}

	/*
		Basics of getting into the multi dimentional array:
	for (int i = 1; i < m_llTileCollisions->length(); i++)
	{
		l_llCurrentLine = m_llTileCollisions->read_data(i, nullptr);
		if (l_llCurrentLine == nullptr) continue;
		for (int j = 1; j < l_llCurrentLine->length(); j++)
			f_sGetLine = l_llCurrentLine->read_data(j, f_sGetLine);
	}*/
	infile.close();
	return true;
}

bool Tilemap::get_collision(Tilemap_Tile f_tt, tmCollisionSide f_collisionSide)
{
	if (f_tt.x < 0) return true;
	LinkedList *l_llCurrentLine = m_llTileInformation->read_data(f_tt.y + 1, nullptr);
	if (l_llCurrentLine == nullptr) return false;
	string l_sCollisionString = "";
	l_sCollisionString = l_llCurrentLine->read_data(f_tt.x + 1, l_sCollisionString);//Yeah you need to actually send a string not a char*
	if (l_sCollisionString == "") return false;
	if (l_sCollisionString.length() == 1)
	{
		if (l_sCollisionString[0] == 'X')
			return true;
		else
			return false;
	}
	else if (l_sCollisionString.length() == 4)
	{
		switch (f_collisionSide)
		{
		case Any:
			for (int i = 0; i < 4; i++)
				if (l_sCollisionString[i] == 'X') return true;
			return false;
		case Top:
		case Bottom:
		case Left:
		case Right:
			int l_iCollside = f_collisionSide;
			if (l_sCollisionString[l_iCollside - 1] == 'X') return true;
			return false;
		}
	}
	return false;
}

bool Tilemap::set_tilesetMeta(TileSetMeta f_tsmMt, string f_sData)
{
	//enum MetaType : int { None = 0, Name, Texture, SingleWidth, SingleHeight, NumberOfRows, NumberOfCols };
	bool l_bErrorCheck = false;
	int l_iData = 0;
	switch (f_tsmMt)
	{
	case TileSetMeta::Name:
		m_sName = f_sData;
		return true;
	case TileSetMeta::Texture:
		if (f_sData == "FALSE") return true;
		if (m_tsmTexture == nullptr)
			m_tsmTexture = new TextureSelfManaged(m_sb->get_renderer(), f_sData);
		else
			m_tsmTexture->LoadTextureFromLocation(m_sb->get_renderer(), f_sData);
		return true;
	case TileSetMeta::SingleWidth:
		l_iData = Utility::convert_stringToInt(f_sData, &l_bErrorCheck);
		if (!l_bErrorCheck) return false;
		m_iSingleWidth = l_iData;
		return true;
	case TileSetMeta::SingleHeight:
		l_iData = Utility::convert_stringToInt(f_sData, &l_bErrorCheck);
		if (!l_bErrorCheck) return false;
		m_iSingleHeight = l_iData;
		return true;
	case TileSetMeta::NumberOfRows:
		l_iData = Utility::convert_stringToInt(f_sData, &l_bErrorCheck);
		if (!l_bErrorCheck) return false;
		m_iNumberOfRows = l_iData;
		return true;
	case TileSetMeta::NumberOfCols:
		l_iData = Utility::convert_stringToInt(f_sData, &l_bErrorCheck);
		if (!l_bErrorCheck) return false;
		m_iNumberOfCols = l_iData;
		return true;
	default:
		return false;
	}
	return false;
}

Tilemap::TileSetMeta Tilemap::convert_stringToTileSetMeta(string f_sData)
{
	for (int i = 0; i < f_sData.length(); i++)
		f_sData[i] = tolower(f_sData[i]);

	if (f_sData == "name:")
		return Name;
	else if (f_sData == "texture:")
		return Texture;
	else if (f_sData == "singlewidth:")
		return SingleWidth;
	else if (f_sData == "singleheight:")
		return SingleHeight;
	else if (f_sData == "numberofrows:")
		return NumberOfRows;
	else if (f_sData == "numberofcols:")
		return NumberOfCols;
	else
		return None;
}

Tilemap::Tilemap_Tile Tilemap::get_tileFromLocation(int f_iLocationX, int f_iLocationY, int f_fWorldSize)
{
	int l_fNewWidth = m_fScaleSize * m_iSingleWidth;
	int l_fNewHeight = m_fScaleSize * m_iSingleHeight;

	Tilemap_Tile l_ttReturn;
	if (!m_bTilemapLoaded) return l_ttReturn;
	l_ttReturn.x = f_iLocationX / l_fNewWidth;
	if (!m_bDrawFromBottom)
		l_ttReturn.y = f_iLocationY / l_fNewHeight;
	else
	{
		//int f_iTilesetHeight = m_llTileMap->length() * m_iScaledWidthAndHeight;
		//int f_iGapLeftAboveTileset = p_camera->get_worldHeight() - f_iTilesetHeight;

		//cout << "f_iLocationY:" << f_iLocationY << endl;

		l_ttReturn.y = (f_iLocationY) / l_fNewHeight;
		l_ttReturn.y -= 13 - m_llTileMap->length();
		/*if(m_controls != nullptr)
			if(m_controls->isGameKeyDown(Controls::BTN_A))
				cout << "l_ttReturn.y:" << l_ttReturn.y << endl;
*/
		if (l_ttReturn.y > m_llTileMap->length()) l_ttReturn.y = m_llTileMap->length();
		//cout << "l_ttReturn.y: " << l_ttReturn.y << endl;
		//l_ttReturn.y -= l_fNewHeight;
	}

	//l_ttReturn.y = f_iLocationY / l_fNewHeight;
	//if (m_bDrawFromBottom)
	//{
	//	l_ttReturn.y = (f_fWorldSize -(f_iLocationY)) / l_fNewHeight;
	//	l_ttReturn.y--;
	//}
	//cout << "l_ttReturn.y" << l_ttReturn.y << endl;
	return l_ttReturn;
}

Tilemap::Tilemap_Tile Tilemap::get_tileLocationFromReference(Tilemap::Tilemap_Tile f_ttReference, int f_fWorldSize)
{
	int l_fNewHeight = m_fScaleSize * m_iSingleHeight;
	Tilemap_Tile l_ttReturn;
	if (!m_bTilemapLoaded) return l_ttReturn;
	l_ttReturn.x = f_ttReference.x * m_iScaledWidthAndHeight;



	if (!m_bDrawFromBottom)
		l_ttReturn.y = f_ttReference.y * m_iScaledWidthAndHeight;
	//l_ttReturn.y = f_fWorldSize - l_ttReturn.y + m_iScaledWidthAndHeight;
	else
	{
		//Work out the Y given the tileset draws from the top
		//World Height is everything in the world
		//
		l_ttReturn.y = p_camera->get_worldHeight() - (l_fNewHeight * (m_llTileMap->length() - f_ttReference.y)) - ((SCREEN_HEIGHT - (l_fNewHeight * 13)) * 1);
		//l_ttReturn.y = p_camera->get_worldHeight() - (l_fNewHeight * (m_llTileMap->length() - f_ttReference.y)) - ((p_camera->get_worldHeight() - (l_fNewHeight * 13)) * 1);
		//l_ttReturn.y = p_camera->get_worldHeight() - (l_fNewHeight * (m_llTileMap->length() - f_ttReference.y)) - ((p_camera->get_worldHeight() - (l_fNewHeight * 13)) * 1);

		//l_ttReturn.y = p_camera->get_worldHeight() - (m_iScaledWidthAndHeight * (m_llTileMap->length() - f_ttReference.y) );


	}
	//cout << "l_ttReturn.y" << l_ttReturn.y << endl;

	return l_ttReturn;
}

SDL_Rect Tilemap::get_tileCollisionFromReference(Tilemap::Tilemap_Tile l_ttReference)
{
	SDL_Rect l_rectReturn = { 0,0,0,0 };
	if (!m_bTilemapLoaded) return l_rectReturn;
	Tilemap::Tilemap_Tile l_ttTopLeft = get_tileLocationFromReference(l_ttReference);

	l_rectReturn.x = l_ttTopLeft.x;
	l_rectReturn.y = l_ttTopLeft.y;
	l_rectReturn.w = m_iScaledWidthAndHeight;
	l_rectReturn.h = m_iScaledWidthAndHeight;
	return l_rectReturn;
}

int Tilemap::get_tileInfo(int f_iRow, int f_iCol)
{
	if (!m_bTilemapLoaded) return -1;
	if (m_llTileMap == nullptr) return -1;
	LinkedList *l_llLine = m_llTileMap->ReadFromList(f_iCol);
	if (l_llLine == nullptr) return -1;
	LinkedList *l_llColTop = l_llLine->read_data(f_iCol, nullptr);
	if (l_llColTop == nullptr) return -1;
	LinkedList *l_llTile = l_llColTop->ReadFromList(f_iRow + 1);
	if (l_llTile == nullptr) return -1;
	return l_llTile->get_data(0);
}

LinkedList* Tilemap::get_tileInfo(int f_iRow, int f_iCol, LinkedList *f_ll)
{
	if (!m_bTilemapLoaded) return nullptr;
	if (m_llTileMap == nullptr) return nullptr;
	LinkedList *l_llLine = m_llTileMap->ReadFromList(f_iCol);
	if (l_llLine == nullptr) return nullptr;
	LinkedList *l_llColTop = l_llLine->read_data(f_iCol, nullptr);
	if (l_llColTop == nullptr) return nullptr;
	LinkedList *l_llTile = l_llColTop->ReadFromList(f_iRow + 1);
	if (l_llTile == nullptr) return nullptr;
	//return l_llTile->get_data(f_ll);
	return l_llTile;
}

LinkedList* Tilemap::get_tileInformationFromTilesetLocation(int f_iRow, int f_iCol, LinkedList *f_ll)
{
	/*if (!m_bTilemapLoaded) return nullptr;
	if (m_llTileInformation == nullptr) return nullptr;
	LinkedList *l_llLine = m_llTileInformation->ReadFromList(f_iCol);
	if (l_llLine == nullptr) return nullptr;
	LinkedList *l_llColTop = l_llLine->read_data(f_iCol, nullptr);
	if (l_llColTop == nullptr) return nullptr;
	LinkedList *l_llTile = l_llColTop->ReadFromList(f_iRow + 1);
	if (l_llTile == nullptr) return nullptr;
	return l_llTile->get_data(f_ll);
	*/

	if (!m_bTilemapLoaded) return nullptr;
	if (m_llTileInformation == nullptr) return nullptr;
	LinkedList *l_llLine = m_llTileInformation->read_data(f_iRow + 1, nullptr);
	if (l_llLine == nullptr) return nullptr;
	return l_llLine->ReadFromList(f_iCol + 1);
}

TileInformation* Tilemap::get_tileInformation(int f_iRow, int f_iCol)
{
	if (!m_bTilemapLoaded) return nullptr;
	if (m_llTileMap == nullptr) return nullptr;
	LinkedList *l_llLine = m_llTileMap->ReadFromList(f_iCol);
	if (l_llLine == nullptr) return nullptr;
	LinkedList *l_llColTop = l_llLine->read_data(f_iCol, nullptr);
	if (l_llColTop == nullptr) return nullptr;
	LinkedList *l_llTile = l_llColTop->ReadFromList(f_iRow + 1);
	if (l_llTile == nullptr) return nullptr;
	//All that gets us the tile number
	//Now we need to search the collisions ll and then return the information
	int l_iRow = 0, l_iCol = 0;
	if (!Utility::GetRowAndColFromNumber(m_iNumberOfRows, m_iNumberOfCols, l_llTile->get_data(0), l_iRow, l_iCol))
		return false;

	if (m_llTileInformation == nullptr) return nullptr;
	LinkedList *l_llInfoLine = m_llTileInformation->ReadFromList(l_iCol);
	if (l_llInfoLine == nullptr) return nullptr;
	LinkedList *l_llInfoColTop = l_llInfoLine->read_data(l_iRow, nullptr);
	if (l_llInfoColTop == nullptr) return nullptr;
	LinkedList *l_llInfoTile = l_llInfoColTop->ReadFromList(l_iRow + 1);
	if (l_llInfoTile == nullptr) return nullptr;
	//l_llInfoTile should be the tile on the collision ll

	int l_iTileInformationIndex = l_llInfoTile->get_data(0);
	if (l_iTileInformationIndex == -1) return nullptr;
	//l_iTileInformationIndex is the index of the information

	if (l_iTileInformationIndex < 0 || l_iTileInformationIndex > 20) return nullptr;

	return m_tiInformation[l_iTileInformationIndex];
}


int Tilemap::add_animation(int f_iFrames, int f_iTickLimit, int f_iStartTileNumber, string f_sSignature)
{
	int l_iNewLocation = -1;
	for (int i = 0; i < 20; i++)
		if (m_tiInformation[i] != nullptr)
			if (f_sSignature == m_tiInformation[i]->get_signature())
				return -1;
	for (int i = 0; i < 20; i++)
		if (m_tiInformation[i] == nullptr)
		{
			l_iNewLocation = i;
			break;
		}
			
	
	m_tiInformation[++l_iNewLocation] = new TileInformation();
	m_tiInformation[l_iNewLocation]->setup_animation(f_iFrames, f_iTickLimit, f_iStartTileNumber, f_sSignature);

	//cout << "l_iNewLocation:" << l_iNewLocation << endl;
	//m_tiInformation[l_iNewLocation]->debug_animation();
	return l_iNewLocation;
}

bool Tilemap::update_modifications(Tilemap_Tile f_tt, int f_iTileNumber)
{
	//This should be called on Collision which creates desired effect as described in tileset
	int l_iTileNumber = -1;
	for (int i = 0; i < m_iTIArrayLimit; i++)
		if (m_tiInformation[i] != nullptr)
			if (m_tiInformation[i]->get_modTileNumber() == f_iTileNumber)
				l_iTileNumber = f_iTileNumber;
	if (l_iTileNumber == -1) return false;//Just avoids us searching two linked lists for something which doens't exist. Memory stuff basically
	TileInformation* l_tiTileInformation = get_tileInformation(f_tt.x, f_tt.y);
	if (l_tiTileInformation == nullptr) return false;

	bool l_bAlreadyHaveAModification = false;
	TileModifications::TileMod_Tile *l_ttPtr = nullptr;
	for (int i = 0; i < 20; i++)
		if (m_tmInformation[i] != nullptr)
			if (m_tmInformation[i]->get_tilemapLocation() != nullptr)
			{
				l_ttPtr = m_tmInformation[i]->get_tilemapLocation();
				if (l_ttPtr->x == f_tt.x)
					if (l_ttPtr->y == f_tt.y)
						l_bAlreadyHaveAModification = true;//Yeah all this to make sure we don't make two
			}
				
	if (l_bAlreadyHaveAModification) return false;

	//Now we know what to setup and we know we can set it up
	int l_iFirstEmptySlot = -1;
	for (int i = 0; i < 20; i++)
		if (m_tmInformation[i] == nullptr)
			l_iFirstEmptySlot = i;
	if (l_iFirstEmptySlot == -1) return false; //No empty slot
	if (l_iFirstEmptySlot > 19) return false; //No empty slot

	m_tmInformation[l_iFirstEmptySlot] = new TileModifications();
	TileModifications *l_tmPtr = m_tmInformation[l_iFirstEmptySlot];//Not needed but I'd like to keep things short

	return true;
}

void Tilemap::set_itemSpawnInformation(ItemSpawnInformation *f_isi)
{
	m_itemSpawnInfo = f_isi;
}

bool Tilemap::set_tileInPlace(Tilemap_Tile f_llDestination, int f_iNumber, bool f_bAddFlags, bool f_bSetNumber)
{
	if (!m_bTilemapLoaded) return false;
	bool l_bFoundTile = false;
	LinkedList *l_llTile = get_tilemapTileNeverFail(f_llDestination, &l_bFoundTile);
	//l_llTile is now the tile we're going to add however we need to add any flags
	string l_sTotalFlag = "";
	if (l_bFoundTile && f_bAddFlags)//This is the old tile information
		l_sTotalFlag = l_llTile->get_data(l_sTotalFlag);

	//New Tile information: 
	string l_sNewFlagLoader = get_tilesetFlagLoader(f_iNumber);
	
	if (l_sNewFlagLoader != "Unfound" && l_sNewFlagLoader != "Undefined")
	{
		if (l_sTotalFlag == "")
			l_sTotalFlag += l_sNewFlagLoader;
		else
			l_sTotalFlag += '_' + l_sNewFlagLoader;
	}
	
	if (Utility::SearchStringForChar(l_sTotalFlag, '_') > -1)
	{
		string l_sUniqueList = l_sTotalFlag;
		LinkedList *l_llFlags = Utility::SplitStringToLL(l_sTotalFlag, '_');//You need to do the clean up on this
		LinkedList *l_llUniqueFlags = Utility::UniqueElements(l_llFlags, l_sTotalFlag);//You need to do the clean up on this
		delete l_llFlags;
		if (l_llUniqueFlags != nullptr)
		{
			l_sUniqueList = Utility::LinkedListToString(l_llUniqueFlags, '_');
			delete l_llUniqueFlags;
		}
		l_sTotalFlag = l_sUniqueList;
	}

	if(f_bSetNumber)
		l_llTile->set_data(f_iNumber);

	l_sTotalFlag = convert_stringFlagsToTilesetFlags(l_sTotalFlag, l_llTile->get_data(0));//Converts numbers to own and any words to tileset number.

	string l_sType = Utility::charToString(get_tilesetFlagType(f_iNumber));
	l_sTotalFlag = l_sType + "_" + l_sTotalFlag;
	if(f_bAddFlags)
		l_llTile->set_data(l_sTotalFlag);
	
	return false;
}

LinkedList* Tilemap::get_tilemapTileNeverFail(Tilemap_Tile f_llDestination, bool *f_bFoundTileReturn)
{
	/*
		THIS FUNCTION WILL RETURN THE TILE YOU SEND TO IT
		IN THE TILEMAP.
		IF IT'S NOT IN THERE IT'LL MAKE IT AND THEN SEND YOU
		THE NEWLY CREATED TILE.

		THE AIR IS SET TO -1

		THE RETURN BOOL IS SIMPLY SO OTHER FUNCTIONS CAN
		DETECT IF THERE'S INFORMATION IN THE TILE INSTEAD
		OF A NEW VERSION.
	*/
	LinkedList *l_llLine, *l_llTile;
	if (m_llTileMap == nullptr) return false;
	l_llLine = m_llTileMap->read_data(f_llDestination.y, nullptr);
	if (l_llLine == nullptr)
	{
		int l_iNewElementsNeeded = f_llDestination.y - m_llTileMap->length();
		for (int i = 0; i < l_iNewElementsNeeded; i++)
			m_llTileMap->AddNewToEnd(nullptr);
		l_llLine = m_llTileMap->read_data(f_llDestination.y, nullptr);
	}
	l_llTile = l_llLine->ReadFromList(f_llDestination.x + 1);
	
	if (l_llTile == nullptr)
	{
		int l_iNewElementsNeeded = f_llDestination.x + 2 - l_llLine->length();
		for (int i = 0; i < l_iNewElementsNeeded; i++)
			l_llLine->AddNewToEnd(nullptr);

		l_llTile = l_llLine->ReadFromList(f_llDestination.x + 1);
	}
	else
		if (f_bFoundTileReturn != nullptr)*f_bFoundTileReturn = true;//Used to return weather or not we had to make this tile
	return l_llTile;
}

string Tilemap::get_tilesetFlagLoader(int f_iTile)
{
	Tilemap_Tile l_ttTilesetLocation = get_tileInTilesetLocation(f_iTile);
	return get_tilesetFlagLoader(l_ttTilesetLocation);
}

string Tilemap::get_tilesetFlagLoader(Tilemap_Tile f_ttTile)
{
	LinkedList *l_llTileInformation = get_tileInformationFromTilesetLocation(f_ttTile.y, f_ttTile.x, nullptr);
	if (l_llTileInformation == nullptr) return "Unfound";
	float l_llFlagInformationLocation = l_llTileInformation->get_data(0.0f);
	if (l_llFlagInformationLocation >= 0)//There are flags
	{
		int l_llFilIndex = (int)l_llFlagInformationLocation;
		if (m_sSpecialBlockLoaders[l_llFilIndex] != nullptr)
			return *m_sSpecialBlockLoaders[l_llFilIndex];
	}
	return "Unfound";
}

char Tilemap::get_tilesetFlagType(int f_iTile)
{
	Tilemap_Tile l_ttTilesetLocation = get_tileInTilesetLocation(f_iTile);
	return get_tilesetFlagType(l_ttTilesetLocation);
}

char Tilemap::get_tilesetFlagType(Tilemap_Tile f_ttTile)
{
	LinkedList *l_llTileInformation = get_tileInformationFromTilesetLocation(f_ttTile.y, f_ttTile.x, nullptr);
	if (l_llTileInformation == nullptr) return '~';
	float l_llFlagInformationLocation = l_llTileInformation->get_data(0.0f);
	if (l_llFlagInformationLocation >= 0)//There are flags
	{
		int l_llFilIndex = (int)l_llFlagInformationLocation;
		if (m_cSpecialBlockSource[l_llFilIndex] != '~')
			return m_cSpecialBlockSource[l_llFilIndex];
	}
	return '~';
}

int Tilemap::get_tilesetFromFlag(string f_sFlag)
{
	if (!m_bTilemapLoaded) return false;
	string l_string = "";
	LinkedList *l_llLine;
	if (m_llTileInformation == nullptr) return false;
	for (int i = 0; i < m_llTileInformation->length(); i++)
	{
		l_llLine = m_llTileInformation->read_data(i, nullptr);
		if (l_llLine != nullptr)
		{
			for (int j = 0; j < l_llLine->length(); j++)
			{
				int l_iLoader = (int)l_llLine->read_data(j, 0.0f);
				if (l_iLoader > -1 && l_iLoader < m_sSpecialBlockLoadersLength)
					if (m_sSpecialBlockLoaders[l_iLoader] != nullptr)
						if (*m_sSpecialBlockLoaders[l_iLoader] == f_sFlag)
							return (get_tilesetCols() * (i - 1)) + j - 1;
			}
		}
	}
	return -1;
}

string Tilemap::convert_stringFlagsToTilesetFlags(string f_sFlags, int f_iTilesetNumberInsertingInto)
{
	int l_iTilesetNumber = -1;
	string l_sReturn = "", l_sCurrent = "";
	if (Utility::SearchStringForChar(f_sFlags, '_') > -1)//If there is more than one flag
	{
		LinkedList *l_llFlagsAsList = Utility::SplitStringToLL(f_sFlags, '_');
		for (int i = 0; i < l_llFlagsAsList->length(); i++)
		{
			l_sCurrent = l_llFlagsAsList->read_data(i, l_sReturn);

			if (l_sCurrent != "Own")//Own remains as own
			{
				l_iTilesetNumber = Utility::convert_stringToInt(l_sCurrent);//If it is already a number
				if(l_iTilesetNumber == -1)
					l_iTilesetNumber = get_tilesetFromFlag(l_sCurrent);//If not go get the number

				if (l_iTilesetNumber == -1) continue;//If we can't find it don't write it
				if (l_iTilesetNumber == f_iTilesetNumberInsertingInto)//It's own
					l_sCurrent = "Own";
				else
					l_sCurrent = to_string(l_iTilesetNumber);//It's just fine
			}
			//Write to the return
			if (l_sReturn == "")
				l_sReturn = l_sCurrent;
			else
				l_sReturn += "_" + l_sCurrent;
		}
		delete l_llFlagsAsList;//Clean up
		return l_sReturn;
	}
	else
	{
		if (l_sCurrent != "Own")
		{
			l_iTilesetNumber = get_tilesetFromFlag(f_sFlags);
			if (l_iTilesetNumber == -1) return "";
			if (l_iTilesetNumber == f_iTilesetNumberInsertingInto)
				return "Own";
			else
				return to_string(l_iTilesetNumber);
		}
		else
			return l_sCurrent;
	}
}

bool Tilemap::write_tilemapToFile(string f_sLocation, bool f_bDebug)
{
	if (f_sLocation == "") return false;
	string f_sLocationCopy = f_sLocation + "_temp.txt";
	ofstream l_outfile;
	bool l_bFoundFile = FileMethods::open_fileForWritting(f_sLocationCopy, l_outfile);
	if (!l_bFoundFile){ cout << "Tilemap::write_tilemapToFile: open_fileForWritting failed." << endl; return false;}//The above shouldn't fail however may fail if the folder writting to is restricted within windows
	LinkedList *l_llTilemapAsLinkedList = convert_tilemapToLevel(&l_bFoundFile);
	if (!l_bFoundFile) { delete l_llTilemapAsLinkedList;  if (f_bDebug)cout << "Tilemap::write_tilemapToFile: convert_tilemapToLevel failed." << endl; return false; }
	if (l_llTilemapAsLinkedList == nullptr) { cout << "Tilemap::write_tilemapToFile: l_llTilemapAsLinkedList is nullptr." << endl; return false; }
	bool l_bWrittenToCopyFile = write_tilemapLinkedListToFile(l_llTilemapAsLinkedList, l_outfile, f_bDebug);
	if (!l_bWrittenToCopyFile) { delete l_llTilemapAsLinkedList; cout << "Tilemap::write_tilemapToFile: write_tilemapLinkedListToFile failed." << endl; return false; }
	l_outfile.close();
	if (!FileMethods::copy_file(f_sLocationCopy, f_sLocation + ".txt"))
	{
		delete l_llTilemapAsLinkedList;
		cout << "Tilemap::write_tilemapToFile: copy_file failed." << endl;
		return false;
	}
	delete l_llTilemapAsLinkedList;
	return true;
}

LinkedList* Tilemap::convert_tilemapToLevel(bool *f_bCheck, bool f_bDebug)
{
	LinkedList *l_llReturn = new LinkedList(), *l_llWriteLine = nullptr, *l_llWriteTile = nullptr;
	LinkedList *l_llLine = nullptr, *l_llCurrentTile = nullptr;
	string l_sCurrentFlag = "", l_sPreviousFlag = "";
	int l_iPreviousTileNumber = -2, l_iTileNumber = -1, l_iCurrentCount = 0;
	if (m_llTileMap == nullptr) return false;
	for (int i = 0; i < m_llTileMap->length(); i++)
	{
		l_llLine = m_llTileMap->read_data(i, nullptr);
		if (l_llLine != nullptr)
		{
			l_llWriteLine = new LinkedList();
			l_llReturn->AddNewToEnd(l_llWriteLine);
			//cout << "i: " << i << endl;
			l_iCurrentCount = 0;
			l_iPreviousTileNumber = -2;

			for (int j = 1; j < l_llLine->length(); j++)
			{
				l_llCurrentTile = l_llLine->ReadFromList(j);
				l_iTileNumber = l_llCurrentTile->get_data(0);
				if(f_bDebug) if (l_iTileNumber == 126) { cout << 1 << endl; }
				l_sCurrentFlag = l_llCurrentTile->get_data(to_string(0));
				if (l_sCurrentFlag == "Undefined" || l_sCurrentFlag == "Unfound") l_sCurrentFlag = "";
				//cout << "q: " << i << " w: " << j << " tile: " << l_iTileNumber << " previous: " << l_iPreviousTileNumber << endl;
				if (j > 1 && ((l_iTileNumber != l_iPreviousTileNumber || l_sCurrentFlag != l_sPreviousFlag) || l_iCurrentCount >= 26))//The previous data must be written
				{
					if (f_bDebug) if (l_iTileNumber == 126) { cout << 2 << endl; }
					l_llWriteTile = l_llWriteLine->AddNewToEnd();
					if (l_sPreviousFlag == "Undefined" || l_sPreviousFlag == "Unfound") l_sPreviousFlag = "";
					l_llWriteTile->set_data(convert_levelEditorFlagsToFile(l_sPreviousFlag));
					l_llWriteTile->set_data(l_iPreviousTileNumber);
					l_llWriteTile->set_data(Utility::convert_intToChar(l_iCurrentCount));

					//cout << "WRITE" << " FLAG:"<< l_sPreviousFlag << " TILE:" << l_iPreviousTileNumber << " CHAR: (" << l_iCurrentCount << ") " << Utility::convert_intToChar(l_iCurrentCount) << endl;
					
					l_iPreviousTileNumber = l_iTileNumber;
					l_sPreviousFlag = l_sCurrentFlag;
					if (l_sPreviousFlag == "Undefined" || l_sPreviousFlag == "Unfound") l_sPreviousFlag = "";
					l_iCurrentCount = 0;
				}
				else
				{
					if (f_bDebug) if (l_iTileNumber == 126) { cout << 3 << endl; }
					if (j == 1)
					{
						l_iPreviousTileNumber = l_iTileNumber;
						l_sPreviousFlag = l_sCurrentFlag;
						if (l_sPreviousFlag == "Undefined" || l_sPreviousFlag == "Unfound") l_sPreviousFlag = "";
					}
				}
				l_iCurrentCount++;
			}
			if (l_iCurrentCount > 0 && l_iPreviousTileNumber > -2)
			{
				if (f_bDebug) if (l_iTileNumber == 126) { cout << 4 << endl; }
				l_llWriteTile = l_llWriteLine->AddNewToEnd();
				if (l_sPreviousFlag == "Undefined" || l_sPreviousFlag == "Unfound") l_sPreviousFlag = "";
				l_llWriteTile->set_data(convert_levelEditorFlagsToFile(l_sPreviousFlag));
				l_llWriteTile->set_data(l_iPreviousTileNumber);
				l_llWriteTile->set_data(Utility::convert_intToChar(l_iCurrentCount));

				if (f_bDebug) cout << "END ADDITION" << " FLAG:"<< l_sPreviousFlag << " TILE:" << l_iPreviousTileNumber << " CHAR: (" << l_iCurrentCount << ") " << Utility::convert_intToChar(l_iCurrentCount) << endl;

				l_iPreviousTileNumber = l_iTileNumber;
				l_sPreviousFlag = "";
				if (l_sPreviousFlag == "Undefined" || l_sPreviousFlag == "Unfound") l_sPreviousFlag = "";
				l_iCurrentCount = 0;
			}
		}
	}
	if (f_bCheck != nullptr)
		*f_bCheck = true;
	return l_llReturn;
}

bool Tilemap::write_tilemapLinkedListToFile(LinkedList *f_llTilemapAsLinkedList, ofstream &f_outfile, bool f_bDebug)
{
	if (!f_outfile.is_open()) return false;
	//Meta Data first
	f_outfile << "META\n";
	f_outfile << "WORLDSIZE: " << m_iWorldSize << "\n";
	f_outfile << "---\n";

	LinkedList *l_llLine = nullptr;
	string l_sCurrentWriteLine = "";
	for (int i = 0; i < f_llTilemapAsLinkedList->length(); i++)
	{
		l_llLine = f_llTilemapAsLinkedList->read_data(i, nullptr);
		if (l_llLine != nullptr)
		{
			l_sCurrentWriteLine = "";
			for (int j = 1; j < l_llLine->length(); j++)
			{
				if (l_sCurrentWriteLine != "")
					l_sCurrentWriteLine += "|";
				l_sCurrentWriteLine += l_llLine->read_data(j, 'c');
				l_sCurrentWriteLine += to_string(l_llLine->read_data(j, 0));
				if (l_llLine->read_data(j, to_string(0)) != "")
					l_sCurrentWriteLine += "/" + l_llLine->read_data(j, to_string(0));
			}
			
			if (l_sCurrentWriteLine != "")
				f_outfile << l_sCurrentWriteLine + "\n";
		}
	}
	return true;
}

string Tilemap::convert_levelEditorFlagsToFile(string f_sInput, bool f_bDebug)
{
	if (f_sInput == "") return "";
	string l_sReturn = f_sInput;
	if (Utility::SearchStringForChar(f_sInput, '_'))
	{
		l_sReturn = "";
		LinkedList *l_llSplitFlags = Utility::SplitStringToLL(f_sInput, '_');
		string l_sType = "";
		for (int i = 0; i < l_llSplitFlags->length(); i++)
		{
			if(f_bDebug) cout << "l_llSplitFlags->read_data(i, to_string(0)): " << l_llSplitFlags->read_data(i, to_string(0)) << endl;
			if (i == 0)
			{
				l_sType = l_llSplitFlags->read_data(i, to_string(0));
				continue;
			}
			if(i == 1)
				l_sReturn = l_sType + "_" + l_llSplitFlags->read_data(i, to_string(0));
			else
				l_sReturn += "/" + l_sType + "_"  + l_llSplitFlags->read_data(i, to_string(0));
		}
		if(l_llSplitFlags != nullptr)
			delete l_llSplitFlags;
		if (f_bDebug) cout << "l_sReturn: " << l_sReturn << endl;
	}
	else
	{
		l_sReturn = "S_" + f_sInput;
	}
	return l_sReturn;
}

bool Tilemap::debug_dumpTilemap()
{
	if (!m_bTilemapLoaded) return false;
	LinkedList *l_llLine = nullptr;
	if (m_llTileMap == nullptr) return false;
	for (int i = 0; i < m_llTileMap->length(); i++)
	{
		cout << "i: " << i << " ";
		l_llLine = m_llTileMap->read_data(i, nullptr);
		if (l_llLine != nullptr)
		{
			for (int j = 0; j < l_llLine->length(); j++)
			{
				//cout << "l_llLine->length(): " << l_llLine->length() << endl;
				cout << l_llLine->read_data(j, 0) << ", ";
			}
		}
		cout << endl;
	}
	return true;
}

bool Tilemap::debug_dumpTileInformation()
{
	if (!m_bTilemapLoaded) return false;
	string l_string = "";
	LinkedList *l_llLine;
	if (m_llTileInformation == nullptr) return false;
	for (int i = 0; i < m_llTileInformation->length(); i++)
	{
		cout << "i: " << i << " ";
		l_llLine = m_llTileInformation->read_data(i, nullptr);
		if (l_llLine != nullptr)
		{
			for (int j = 0; j < l_llLine->length(); j++)
			{
				//cout << "l_llLine->length(): " << l_llLine->length() << endl;
				cout << l_llLine->read_data(j, l_string);
				if (l_llLine->read_data(j, 0.0f) > 0)
					cout << " (" << l_llLine->read_data(j, 0.0f) << ")" << endl;
				cout << "|";
			}
		}
		cout << endl;
	}
	return true;
}

bool Tilemap::debug_dumpTileset()
{
	if (!m_bTilemapLoaded) return false;
	cout << "Dump Tileset: " << endl;
	cout << "m_sName: " << m_sName << endl;
	cout << "m_tsmTexture (Location): " << m_tsmTexture->get_lastLocation() << endl;
	cout << "m_iSingleWidth: " << m_iSingleWidth << endl;
	cout << "m_iSingleHeight: " << m_iSingleHeight << endl;
	cout << "m_iNumberOfCols: " << m_iNumberOfCols << endl;
	cout << "m_iNumberOfRows: " << m_iNumberOfRows << endl;

	return true;
}

bool Tilemap::debug_dumpTiInformation()
{
	for (int i = 0; i < m_iTIArrayLimit; i++)
	{
		if (m_tiInformation[i] == nullptr) continue;
		cout << "i: " << i << " : " << m_tiInformation[i]->get_signature();
	}
	return true;
}
//
bool Tilemap::debug_dumpSpecialBlocks()
{
	for (int i = 0; i < m_specialBlocksLength; i++)
	{
		if (m_specialBlocks[i] == nullptr) continue;
		cout << "i: " << i << " : " << endl;
		m_specialBlocks[i]->debug_all();
	}
	return true;
}

bool Tilemap::debug_specialBlockLoaders()
{
	for (int i = 0; i < m_sSpecialBlockLoadersLength; i++)
		if (m_sSpecialBlockLoaders[i] != nullptr)
			cout << "i: " << i << " : " << *m_sSpecialBlockLoaders[i] << endl;
	return true;
}