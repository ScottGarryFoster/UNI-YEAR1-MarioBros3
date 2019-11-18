#include "ItemManager.h"



ItemManager::ItemManager(SpriteBatch *f_sb, SMB_PlayerPlatformer *f_players[2], int f_iNumberOfPlayers)
{
	m_sb = f_sb;
	if(f_iNumberOfPlayers > 0)
		for (int i = 0; i < m_playersLength; i++)
			m_players[i] = f_players[i];
	m_playersLength = f_iNumberOfPlayers;
}


ItemManager::~ItemManager()
{
	for (int i = 0; i < m_itemsLength; i++)
		if (m_items[i] != nullptr)
			delete m_items[i];
}

bool ItemManager::Update(float f_deltaTime)
{
	if (m_itemSpawnInformation != nullptr)
		update_itemSpawn();
	//m_iCollisionFrame += f_deltaTime;
	for (int i = 0; i < m_itemsLength; i++)
			if (m_items[i] != nullptr)
				if (!m_items[i]->get_delete())
				{
						if (m_items[i]->get_collectable() && m_items[i]->get_didDrawThisFrame())//Can collect
							for (int p = 0; p < m_playersLength; p++)//Loop Players
								if (m_players[p] != nullptr && m_items[i] != nullptr)//Check we've players
								{
										if (Utility::ReturnCollision(m_items[i]->get_collisionRect(), m_players[p]->get_collisionRect()) && m_players[p]->get_deathState() == SMB_PlayerPlatformer::dsAlive)//Check collision
										{
											//Collect Item:
											m_players[p]->player_eatsItem(m_items[i]->get_itemName());
											delete m_items[i]; m_items[i] = nullptr;
										}
								}
					if (m_items[i] != nullptr)//Could be deleted above
						m_items[i]->Update(f_deltaTime);
				}
				else
				{
					delete m_items[i]; m_items[i] = nullptr;
				}

	//if (m_iCollisionFrame >= m_iCollisionFrameTime)
	//	m_iCollisionFrame -= m_iCollisionFrameTime;
	return true;
}

bool ItemManager::Draw(bool f_bBelowSpecialBlocks)
{
	for (int i = 0; i < m_itemsLength; i++)
		if (m_items[i] != nullptr)
			if(m_items[i]->get_drawBelowSpecialBlocks() == f_bBelowSpecialBlocks)
				m_items[i]->RenderToScreen(m_sb->get_renderer());
	return true;
}

bool ItemManager::update_itemSpawn()
{
	if (m_itemSpawnInformation == nullptr) return false;
	if (!m_itemSpawnInformation->get_isUsed()) return false;

	string l_sItemName = m_itemSpawnInformation->get_itemName();
	int l_iLocationX = m_itemSpawnInformation->get_locationX();
	int l_iLocationY = m_itemSpawnInformation->get_locationY();
	ItemSpawnInformation::ItemSpawnEvent l_ise = m_itemSpawnInformation->get_ise();
	if (!spawn_item(l_sItemName, l_iLocationX, l_iLocationY, l_ise))return false;
	m_itemSpawnInformation->reset_all();
	return true;
}

bool ItemManager::spawn_item(string f_sItemName, float f_fX, float f_fY, ItemSpawnInformation::ItemSpawnEvent f_ise)
{
	int l_iNextItemIndex = -1;
	for (int i = 0; i < m_itemsLength; i++)
		if (m_items[i] == nullptr)
		{
			l_iNextItemIndex = i;
			break;
		}
	if (l_iNextItemIndex == -1) { cout << "bool ItemManager::spawn_item: Max Items reached. Increase the array" << endl; return false; }
	m_items[l_iNextItemIndex] = new Item(m_players, m_playersLength);

	m_items[l_iNextItemIndex]->load_fromFile("GameFiles/Mario3/Items/" + f_sItemName + ".txt");
	m_items[l_iNextItemIndex]->set_itemName(f_sItemName);
	m_items[l_iNextItemIndex]->set_intialLocation(f_fX, f_fY);
	m_items[l_iNextItemIndex]->set_textureSelfManageFromPointer(m_tsm);
	if (m_camera != nullptr)
		m_items[l_iNextItemIndex]->set_camera(m_camera);
	for (int i = 0; i < 10; i++)
		if (m_cameraAlt[i] != nullptr)
			m_items[l_iNextItemIndex]->set_altCamera(m_cameraAlt[i], i);
	m_items[l_iNextItemIndex]->set_singleSize(16, 16);
	m_items[l_iNextItemIndex]->set_cameraLock(true);
	m_items[l_iNextItemIndex]->set_scaleSize(m_fScaleSize);
	m_items[l_iNextItemIndex]->set_tilemap(m_tilemap);
	//m_items[l_iNextItemIndex]->set_collision(4, 4, 12, 12);
	m_items[l_iNextItemIndex]->set_collision(2,2,14,13);
	m_items[l_iNextItemIndex]->set_scoreOnScreen(m_scoreOnScreen);//Allows this to update the score
	m_items[l_iNextItemIndex]->set_id(l_iNextItemIndex);
	return true;
}