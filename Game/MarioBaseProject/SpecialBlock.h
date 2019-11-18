#pragma once
/*
	Future Scott or someone marking this note:
	Special Blocks are things like the quesiton mark blocks or the breakable ones.
	There are animations and the beginings of modifications to the tilemap.
	However the blocks are so complicated I felt it needed it's own classes.
	I've left the modifications in the tilemap class for now but at the moment
	I'm not too bothered as it may be useful in future.
*/
#include "Sprite.h"
#include "SprLinearAnimation.h"
#include "BlockAction.h"
#include "SPRITEBATCH.h"
#include "ItemSpawnInformation.h"
#include "Audio.h"

#include <string>
#include <iostream>
#include <fstream>
using namespace::std;

class SMB_PlayerPlatformer;
class ParticleManager;

class SpecialBlock : public Sprite
{
private:
	SprLinearAnimation *m_sprLinearAnimation = nullptr;//Optional and so is has-a not is-a
	BlockAction *m_blockAction[10] = { nullptr };
	int m_blockActionLength = 10;
	bool m_bDeleteMe = false;

	int m_iTilemapLocationX = -1, m_iTilemapLocationY = -1;

	int m_iScaledWidthAndHeight = 0;

	ItemSpawnInformation *m_itemSpawnInformation = nullptr;

	bool m_bDebugOnce = false;

	int m_iActiveItem = -1;
	bool m_bShouldSpawnItem = false;
	bool m_bHaveSpawnedItem = false;
	string m_sItem = "";
	float m_fItemCooldown = 0;

	string m_sPreset = "";

	//Audio
	int m_iItemSpawnSound = -1;
	int m_iCoinSpawnSound = -1;
	int m_iBreakBlockSound = -1;

	//RenderAboveThePlayer?
	bool m_bRenderAbovePlayer = true;

	//Polymorph State
	int m_iPolymorph = -1;
protected:
	SpriteBatch *p_sb = nullptr;
	Audio *p_audio = nullptr;
	ParticleManager *p_partcleManager = nullptr;

	int *s_iPolymorph = nullptr;
public:
	SpecialBlock();
	virtual ~SpecialBlock();

	virtual bool Update(float f_deltaTime);

	//SETS
	void set_audio(Audio *f_audio) { p_audio = f_audio; }
	void set_polymorphSignal(int *f_iPtr) { s_iPolymorph = f_iPtr; }
	void set_tilemapLocation(int f_iX, int f_iY) { m_iTilemapLocationX = f_iX; m_iTilemapLocationY = f_iY;}
	void set_itemSpawnInformation(ItemSpawnInformation *f_isi) { m_itemSpawnInformation = f_isi; }
	void set_polymorph(int f_iData) { m_iPolymorph = f_iData; }
	void set_renderAbovePlayer(bool f_bData) { m_bRenderAbovePlayer = f_bData; }
	virtual void set_playerOneEndCard(int *f_iCard) {}//Used only by end of level
	virtual void set_playerTwoEndCard(int *f_iCard) {}//Used only by end of level
	void set_particleManager(ParticleManager *f_pb) { p_partcleManager = f_pb; }

	//GETS
	int get_polymorph() { return m_iPolymorph; }
	bool get_renderAbovePlayer() { return m_bRenderAbovePlayer; }
	bool get_deleteMe() { return m_bDeleteMe; }
	int get_tilemapLocationX() { return m_iTilemapLocationX; }
	int get_tilemapLocationY() { return m_iTilemapLocationY; }
	virtual int get_drawThisTileUnderMe() { return -1; }

	//Loads
	bool load_specialBlock(string f_sLocation);
	bool load_audio();

	//Updates
	bool update_actionActivation(int f_tileX, int f_tileY, int f_iDirection, int f_iActor, bool f_bPlayerSize = false);
	virtual bool update_actionActivationPolymorphVersion(SDL_Rect f_rectActor, int f_iDirection, int f_iActor, int f_iAdditionalNumber, SMB_PlayerPlatformer *f_player){return false;}

	//SETUP
	bool setup_sprite(TextureSelfManaged *f_tsm = nullptr);
	bool setup_sprite(SpriteBatch *f_sb, TextureSelfManaged *f_tsm = nullptr);
private:
	//Updates
	bool update_agnostic(float f_deltaTime);

	bool setup_location();
	bool setup_partcleEffect(BlockAction *f_blockAction);
protected:
	bool setup_locationInner();
};

