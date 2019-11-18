#pragma once
#include "Item.h"
#include "LinkedList.h"
#include "SPRITEBATCH.h"
#include "SMB_PlayerPlatformer.h"
#include "ItemSpawnInformation.h"
#include "TextureSelfManaged.h"
#include "Camera.h"
#include "Tilemap.h"
#include "ScoreOnScreen.h"

#include <string>
using namespace std;
class ItemManager
{
private:
	SpriteBatch *m_sb = nullptr;
	TextureSelfManaged *m_tsm = nullptr;
	SMB_PlayerPlatformer *m_players[2] = { nullptr };
	Tilemap *m_tilemap = nullptr;
	int m_playersLength = 2;
	Camera *m_camera = nullptr;
	Camera *m_cameraAlt[10] = { nullptr };
	float m_fScaleSize = 1;
	
	Item *m_items[100] = { nullptr };
	int m_itemsLength = 100;
	ItemSpawnInformation *m_itemSpawnInformation = nullptr;
	ScoreOnScreen *m_scoreOnScreen = nullptr;

	int m_iCollisionFrame = 0;
	int m_iCollisionFrameTime = 50;
public:
	ItemManager(SpriteBatch *f_sb, SMB_PlayerPlatformer *f_players[2], int f_iNumberOfPlayers);
	virtual ~ItemManager();

	virtual bool Update(float f_deltaTime);
	virtual bool Draw(bool f_bBelowSpecialBlocks = false);

	void set_itemSpawnInformation(ItemSpawnInformation *f_isi) { m_itemSpawnInformation = f_isi; }
	void set_defaultTexture(TextureSelfManaged *f_tsm) { m_tsm = f_tsm; }
	void set_mainCamera(Camera *f_camera) { m_camera = f_camera; }
	void set_altCamera(Camera *f_camera, int f_iIndex) { m_cameraAlt[f_iIndex] = f_camera; }
	void set_scaleSize(float f_fScaleSize) { m_fScaleSize = f_fScaleSize; }
	void set_tilemap(Tilemap *f_tilemap){m_tilemap = f_tilemap;}
	void set_scoreOnScreen(ScoreOnScreen *f_scoreOnScreen) { m_scoreOnScreen = f_scoreOnScreen; }

	bool update_itemSpawn();
	bool spawn_item(string f_sItemName, float f_fX, float f_fY, ItemSpawnInformation::ItemSpawnEvent f_ise);
};

