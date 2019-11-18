#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>

#include "SPRITEBATCH.h"
#include "TextureSelfManaged.h"
#include "LinkedList.h"
#include "Utility.h"
#include "Camera.h"
#include "Vector2D.h"
#include "Controls.h"
#include "TileInformation.h"
#include "TileModifications.h"
#include "SpecialBlock.h"
#include "ItemSpawnInformation.h"
#include "FileMethods.h"
#include "Sprite.h"
#include "Audio.h"
#include "EndOfLevelBlock.h"
//#include "Enemy.h"

#include <iostream>
#include <string>
#include <fstream>

class Enemy;//Needs to know it exists but not what it is until the implimentation
class SMB_PlayerPlatformer;//Needs to know it exists but not what it is until the implimentation
class ParticleManager;

class Tilemap
{
public:
	enum tmCollisionSide : int {Any = 0, Top, Bottom, Left, Right };
	//Debug
	bool m_bDebug_TilemapLoad = false;
private:
	SpriteBatch *m_sb = nullptr;
	TextureSelfManaged *m_tsmTexture = nullptr;
	Controls *m_controls = nullptr;
	Audio *m_audio = nullptr;
	bool m_bDebugReturnOnce = false;

	int m_iLocationX = 0, m_iLocationY = 0;
	float m_fScaleSize = 1;
	int m_iScaledWidthAndHeight = 0;

	string m_sName = "";
	int m_iSingleWidth = 0, m_iSingleHeight = 0;
	int m_iNumberOfCols = 1, m_iNumberOfRows = 1;

	bool m_bVisable = true;
	bool m_bDrawFromBottom = false;
	bool m_bLockToCamera = false;
	bool m_bTilemapLoaded = false;
	bool m_bRenderSpecialBlocks = true;//Used for level editor so it doesn't load the special blocks instead it displays them

	int m_iCameraYLowest = -1;
	
	bool m_bDebugMessage = false;

	bool *m_bPlayerOneSize = nullptr;
	bool *m_bPlayerTwoSize = nullptr;

	//World Size
	int m_iWorldSize = 1920 * 10;//Only dealing with X

	//End of Level (these are passed to the blocks which manage such)
	int *m_iPlayerOneFinishedLevel = 0;
	int *m_iPlayerTwoFinishedLevel = 0;
protected:
	Camera *p_camera = nullptr;
	Camera *p_alternativeCameras[10] = { nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr };

	LinkedList *m_llTileMap = nullptr;
	LinkedList *m_llTileInformation = nullptr;
	TileInformation *m_tiInformation[20] = {nullptr};
	int m_iTIArrayLimit = 20;
	TileModifications *m_tmInformation[20] = {nullptr};

	char m_cSpecialBlockLoaded[100] = { '\n' };//Stores the character which created it. Used for the difforence between blocks and enemys.
	SpecialBlock *m_specialBlocks[100] = { nullptr };
	int m_specialBlocksLength = 100;
	ItemSpawnInformation *m_itemSpawnInfo = nullptr;
	string *m_sSpecialBlockLoaders[100] = { nullptr };//Stores the filenames of the loaders
	char m_cSpecialBlockSource[100] = { '\n' };//Stores the character which created it. Used for the difforence between blocks and enemys.
	int m_sSpecialBlockLoadersLength = 100;
	bool m_bNeedToLoadSpecialBlocks = true;

	Enemy *m_enemies[100] = { nullptr };
	int m_enemiesLength = 100;

	SMB_PlayerPlatformer *p_players[2] = { nullptr };
	ParticleManager *p_particleManager = nullptr;

	//Straightup item spawns
	LinkedList *m_llItemsToSpawn = nullptr;
	SpecialBlock *m_sbDisplayNothing = nullptr;
public:
	struct Tilemap_Tile
	{
		int x = 0, y = 0;
		void set_position(int f_iX, int f_iY) { x = f_iX; y = f_iY; }
	};
public:
	Tilemap();
	~Tilemap();

	enum MapLoadType : int {FromString = 0, FromFile};
	enum TileSetMeta : int { None = 0, Name, Texture, SingleWidth, SingleHeight, NumberOfRows, NumberOfCols };

	TileSetMeta convert_stringToTileSetMeta(string f_sData);

	//SETS:

	void set_spritebatch(SpriteBatch *f_sb) { m_sb = f_sb; }
	void set_audio(Audio *f_audio) { m_audio = f_audio; }
	bool load_tilemap(MapLoadType f_lt, string f_sData);
	bool load_tileset(MapLoadType f_lt, string f_sData);

	bool set_tilesetMeta(TileSetMeta f_tsmMt, string f_sData);
	void set_drawFromBottom(bool f_bDraw) { m_bDrawFromBottom = f_bDraw; }
	void set_camera(Camera *f_camera) { p_camera = f_camera; }
	void set_camera(Camera *f_camera, bool f_bLock) { p_camera = f_camera; m_bLockToCamera = f_bLock;}
	void set_camera(bool f_bLock, Camera *f_camera) { p_camera = f_camera; m_bLockToCamera = f_bLock;}
	void set_cameraLock(bool f_bLock) { m_bLockToCamera = f_bLock; }
	//bool set_tilesetMeta(TileSetMeta::MetaType f_tsmMt, int f_iData);
	void set_worldSize(int f_iData) { m_iWorldSize = f_iData; }
	void set_playerOneEndCard(int *f_iCard) { m_iPlayerOneFinishedLevel = f_iCard; }
	void set_playerTwoEndCard(int *f_iCard) { m_iPlayerTwoFinishedLevel = f_iCard; }

	void set_altCamera(Camera *f_camera, int f_iIndex) { p_alternativeCameras[f_iIndex] = f_camera; }

	void set_controls(Controls *f_controls) { m_controls = f_controls; }

	void set_itemSpawnInformation(ItemSpawnInformation *f_isi);

	void set_playerOneSize(bool *f_b) { m_bPlayerOneSize = f_b; }
	void set_playerTwoSize(bool *f_b) { m_bPlayerTwoSize = f_b; }

	void set_renderSpecialBlocks(bool f_b) { m_bRenderSpecialBlocks = f_b; }//Used if you want to render special blocks as simply blocks

	void set_particleManager(ParticleManager *f_pm) { p_particleManager = f_pm; }

	//More Complicated Sets
	bool set_tileInPlace(Tilemap_Tile f_llDestination, int f_iNumber, bool f_bAddFlags = false, bool f_bSetNumber = true);

	void set_playerReference(SMB_PlayerPlatformer *f_ppReference, int f_iNumber) { p_players[f_iNumber] = f_ppReference; }
	//GETS:

	int get_scaleSize() { return (int)m_fScaleSize; }
	Tilemap_Tile get_tileFromLocation(int f_iLocationX, int f_iLocationY, int f_fWorldSize = 720);
	TextureSelfManaged *get_textureSelfManaged() { return m_tsmTexture; }
	int get_actualWandH(){return m_iScaledWidthAndHeight;}

	int get_tileInfo(int f_iRow, int f_iCol);
	LinkedList* get_tileInfo(int f_iRow, int f_iCol, LinkedList *f_ll);

	TileInformation* get_tileInformation(int f_iRow, int f_iCol);

	LinkedList* get_tileInformationFromTilesetLocation(int f_iRow, int f_iCol, LinkedList *f_ll);

	Tilemap_Tile get_tileLocationFromReference(Tilemap::Tilemap_Tile f_ttReference, int f_fWorldSize = 720);

	SDL_Rect get_tileCollisionFromReference(Tilemap::Tilemap_Tile l_ttReference);
	
	int get_cameraYLowest() { return m_iCameraYLowest; }

	Tilemap_Tile get_tileInTilesetLocation(int f_iNumber);

	int get_tilesetRows() { return m_iNumberOfRows; }
	int get_tilesetCols() { return m_iNumberOfCols; }

	LinkedList* get_tileInformationLL() { return m_llTileInformation; }
	LinkedList* get_tilemapTileNeverFail(Tilemap_Tile f_llDestination, bool *f_bFoundTileReturn = nullptr);

	int get_tilesetFromFlag(string f_sFlag);
	int get_worldSize() { return m_iWorldSize; }

	//Collision Gets:
	///True means collision, false means no collision///
	bool get_collision(Tilemap_Tile f_tt, tmCollisionSide f_collisionSide);
	string get_tilesetFlagLoader(int f_iTile);
	string get_tilesetFlagLoader(Tilemap_Tile f_ttTile);

	char get_tilesetFlagType(int f_iTile);
	char get_tilesetFlagType(Tilemap_Tile f_ttTile);

	//DEFAULTS:
	bool Draw();
	bool Update(float f_deltaTime);

	bool Draw_specialBlocks(bool f_bRenderAbovePlayers = true);//Just draws Special Blocks

	//Modifications:
	///The Tile in this case is Tilemap reference ///
	bool update_modifications(Tilemap_Tile f_tt, int f_iTileNumber);

	bool update_specialCollisions(int f_tileX, int f_tileY, int f_iDirection, int f_iActor, int f_iPlayer = -1);
	bool update_entiesReactions(SDL_Rect f_rectCollision, int f_iDirection, int f_iActor, int f_iPlayer, int *f_iFeedback, bool f_bAlsoSpecialBlocks = false, int f_iAdditionalNumber = -1);

	bool update_cameraYLowest();

	//Convert
	string convert_stringFlagsToTilesetFlags(string f_sFlags, int f_iTilesetNumberInsertingInto = -1);

	//Write to file
	bool write_tilemapToFile(string f_sLocation, bool f_bDebug = false);

	//Load
	bool load_metaData(string f_sData);
	bool load_metaDataActions();

	//Debug:
	bool debug_dumpTilemap();
	bool debug_dumpTileset();
	bool debug_dumpTileInformation();
	bool debug_dumpTiInformation();
	bool debug_dumpSpecialBlocks();
	bool debug_specialBlockLoaders();
private:
	bool load_tilemapFromString(string f_sData);
	bool load_tilemapFromFile(string f_sData);

	//bool load_tilesetFromString(string f_sData);
	bool load_tilesetFromFile(string f_sData);
	bool load_tilesetMeta(string f_sData);
	bool load_tilesetInformation(string f_sData);

	bool load_tilemapFromSingleLine(string f_sData, int f_iLine);

	bool Draw_eachCamera(Camera *f_camera = nullptr);

	bool update_animations(float f_deltaTime);
	int add_animation(int f_iFrames, int f_iTickLimit, int f_iStartTileNumber, string f_sSignature);

	//Special Blocks:
	bool setup_specialBlocks(float f_deltaTime);
	bool update_specialBlocks(float f_deltaTime);

	//Enemies:
	bool setup_enemiesS(float f_fBlockIndex, float f_deltaTime, string f_sFlags, LinkedList *f_llTile, int f_iTile, int f_iLine);//Special Blocks begining with a S
	bool update_enemies(float f_deltaTime);

	//Items:
	bool setup_itemsS(float f_fBlockIndex, float f_deltaTime, string f_sFlags, LinkedList *f_llTile, int f_iTile, int f_iLine);//Special Blocks begining with an I
	bool update_items();

	//Loaders
	bool setup_specialBlocksS(float f_fBlockIndex, float f_deltaTime, string f_sFlags, LinkedList *f_llTile, int f_iTile, int f_iLine);//Special Blocks begining with an S

	//Loading from file
	LinkedList* convert_tilemapToLevel(bool *f_bCheck, bool f_bDebug = false);
	string convert_levelEditorFlagsToFile(string f_sInput, bool f_bDebug = false);
	bool write_tilemapLinkedListToFile(LinkedList *f_llTilemapAsLinkedList, ofstream &f_outfile, bool f_bDebug = false);

	//Polymorph blocks
	bool handle_polymorphOfSpecialBlocks(int f_iSpecialBlockIndex, int f_iPolymorph);
};