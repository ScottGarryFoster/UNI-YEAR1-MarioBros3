#include "SpecialBlock.h"
#include "ParticleManager.h"


SpecialBlock::SpecialBlock()
{
}


SpecialBlock::~SpecialBlock()
{
	delete m_sprLinearAnimation;
	for (int i = 0; i < m_blockActionLength; i++)
		if (m_blockAction[i] != nullptr)
			delete m_blockAction[i];

}
bool SpecialBlock::Update(float f_deltaTime)
{
	if (!m_bDebugOnce)
	{
	}
	//m_bDebugOnce = true;

	if (m_sPreset == "")
		return update_agnostic(f_deltaTime);
	return true;
}

bool SpecialBlock::update_agnostic(float f_deltaTime)
{
	int l_iCompleteCol = -1 , l_iCompleteRow = -1;
	for (int i = 0; i < 10; i++)
		if (m_blockAction[i] != nullptr)
		{
			if (m_blockAction[i]->get_activated())
			{
				m_blockAction[i]->Update(f_deltaTime);
				if (m_blockAction[i]->get_deleteMe())
				{
					setup_partcleEffect(m_blockAction[i]);
					m_bDeleteMe = true;
				}
				set_location(m_blockAction[i]->get_locationXMod(), m_blockAction[i]->get_locationYMod());
				if (m_iActiveItem != i)
				{
					m_sItem = m_blockAction[i]->get_itemName();
					m_bShouldSpawnItem = true;
					m_iActiveItem = i;
					m_blockAction[m_iActiveItem]->set_activated(false);
					for (int j = 0; j < 10; j++)
						if (m_blockAction[j] != nullptr)
							m_blockAction[j]->add_frequency(-1);
				}
			}
			else if (m_iActiveItem == i)
				m_iActiveItem = -1;

			if (m_blockAction[i]->get_completedCol() != -1)
				l_iCompleteCol = m_blockAction[i]->get_completedCol();
			if (m_blockAction[i]->get_completedRow() != -1)
				l_iCompleteRow = m_blockAction[i]->get_completedRow();
		}

	if (m_sprLinearAnimation != nullptr)
		if (l_iCompleteCol != -1|| l_iCompleteRow != -1)
		{
			//cout << "yh" << endl;
			if (l_iCompleteCol != -1)
				set_singleLocation(get_sourceRow(), l_iCompleteCol);

			if (l_iCompleteRow != -1)
				set_singleLocation(l_iCompleteRow, get_sourceCol());
		}
		else
		{
			m_sprLinearAnimation->Update(f_deltaTime);
			set_singleLocation(get_sourceRow(), m_sprLinearAnimation->get_currentLocation());
		}

	if (m_bShouldSpawnItem)
	{
		if (m_sItem != "")
			m_bHaveSpawnedItem = m_itemSpawnInformation->setup_all(m_sItem, get_locationX(), get_locationY(), ItemSpawnInformation::iseFromBlock);
		else
			m_bHaveSpawnedItem = true;
		if (m_bHaveSpawnedItem)
		{
			m_bShouldSpawnItem = false;
			m_bHaveSpawnedItem = false;
			m_blockAction[m_iActiveItem]->set_activated(true);
			m_fItemCooldown = 200;

			if (p_audio != nullptr)
			{
				if (m_sItem == "MUSHROOM" || m_sItem == "FEATHER" || m_sItem == "ONEUPMUSHROOM")
					if (m_iItemSpawnSound > -1)
						p_audio->play_soundEffect(m_iItemSpawnSound);

				if (m_sItem == "COININSTANT")
					if (m_iCoinSpawnSound > -1)
						p_audio->play_soundEffect(m_iCoinSpawnSound);
			}
		}
	}
	if (m_fItemCooldown > 0)
		m_fItemCooldown -= f_deltaTime;

	if (m_fItemCooldown < 0)
		m_fItemCooldown = 0;

	return true;
}

bool SpecialBlock::load_specialBlock(string f_sLocation)
{
	if (f_sLocation == "") return false;

	ifstream infile(f_sLocation);
	if (!infile.is_open())
	{
		cout << "load_specialBlock(string " << f_sLocation << "):  Could not open file." << endl;
		return false;
	}

	string f_sGetLine = "", f_sContent = "";
	int f_iLine = 0;
	while (getline(infile, f_sGetLine))
	{
		if (f_sGetLine == "Preset")
		{
			getline(infile, f_sGetLine);
			m_sPreset = Utility::StringToUpperCase(f_sGetLine);
			if (m_sPreset == "ENDOFLEVEL")
				*s_iPolymorph = 0;
			return true;

			break;
		}
		else if (f_sGetLine == "Animation")
		{
			if (m_sprLinearAnimation == nullptr)
				m_sprLinearAnimation = new SprLinearAnimation();

			while (f_sGetLine != "---")
			{
				getline(infile, f_sGetLine);
				f_sContent += f_sGetLine + "~";
			}
			if (!m_sprLinearAnimation->setup_animation(f_sContent))
				cout << "Animation not loaded" << endl;
		}
		else if (f_sGetLine == "Action")
		{
			int f_iNextIndex = -1;
			for (int i = 0; i < m_blockActionLength; i++)
				if (m_blockAction[i] == nullptr)
				{
					f_iNextIndex = i; break;
				}
			if (f_iNextIndex == -1) continue;
			m_blockAction[f_iNextIndex] = new BlockAction();
			m_blockAction[f_iNextIndex]->set_audio(p_audio);//Give them the right to own audio
			while (f_sGetLine != "---")
			{
				getline(infile, f_sGetLine);
				f_sContent += f_sGetLine + "~";
			}

			if (!m_blockAction[f_iNextIndex]->setup_action(f_sContent))
				cout << "Action not loaded" << endl;
		}
	}
	infile.close();

	load_audio();

	return true;
}

bool SpecialBlock::load_audio()
{
	if (p_audio == nullptr) return false;

	if (m_iItemSpawnSound == -1)
		m_iItemSpawnSound = p_audio->find_soundEffectNeverFail("Audio/SoundEffect/Mario3/smb3_mushroom_appears.wav");

	if(m_iCoinSpawnSound == -1)
		m_iCoinSpawnSound = p_audio->find_soundEffectNeverFail("Audio/SoundEffect/Mario3/smb3_coin.wav");

	if(m_iBreakBlockSound == -1)
		m_iBreakBlockSound = p_audio->find_soundEffectNeverFail("Audio/SoundEffect/Mario3/smb3_break_brick_block.wav");
	return true;
}

bool SpecialBlock::setup_sprite(SpriteBatch *f_sb, TextureSelfManaged *f_tsm)
{
	if (f_sb != nullptr)
		p_sb = f_sb;

	if (m_sprLinearAnimation == nullptr) return false;
	if (p_sb != nullptr)
		if (f_tsm == nullptr)
			LoadTextureFromFile(p_sb->get_renderer(), m_sprLinearAnimation->get_textureLocation());
		else
			set_textureSelfManageFromPointer(f_tsm);
	setup_location();
	return true;
}

bool SpecialBlock::setup_sprite(TextureSelfManaged *f_tsm)
{
	if (m_sprLinearAnimation == nullptr) return false;
	if (f_tsm != nullptr)
	{
		set_textureSelfManageFromPointer(f_tsm);
	}
	setup_location();
	return true;
}

bool SpecialBlock::update_actionActivation(int f_tileX, int f_tileY, int f_iDirection, int f_iActor, bool f_bPlayerSize)
{
	if (m_fItemCooldown > 0) return false;
	if (f_tileX != m_iTilemapLocationX || f_tileY != m_iTilemapLocationY) return false;
	for (int i = 0; i < 10; i++)
		if (m_blockAction[i] != nullptr)
			if (m_blockAction[i]->update_checkActivation(f_iDirection, f_iActor, f_bPlayerSize))
			{
				//cout << "YH" << endl;
				m_blockAction[i]->set_spriteCurrentLocation(get_locationX(), get_locationY());
				m_blockAction[i]->set_widthHeight(get_actualSizeHeight());
			}
				
	return true;
}

bool SpecialBlock::setup_location()
{
	setup_locationInner();
	if (m_sprLinearAnimation == nullptr) return false;
	set_singleLocation(m_sprLinearAnimation->get_startLocationRow(), m_sprLinearAnimation->get_startLocation());
	return true;
}

bool SpecialBlock::setup_locationInner()
{
	float l_fScale = ((SCREEN_HEIGHT / get_singleHeight()) * TILE_HEIGHT_RATIO);// *0.25f;
	m_iScaledWidthAndHeight = l_fScale * get_singleHeight();
	set_location(m_iTilemapLocationX * m_iScaledWidthAndHeight, m_iTilemapLocationY * m_iScaledWidthAndHeight);
	return true;
}

bool SpecialBlock::setup_partcleEffect(BlockAction *f_blockAction)
{
	if (p_partcleManager != nullptr)
	{
		p_partcleManager->create_particleEffect(ParticleManager::particlePreset::ppBreakBlock, { get_locationX(),get_locationY() });
		if (m_iBreakBlockSound > -1)
			p_audio->play_soundEffect(m_iBreakBlockSound);
	}
	return true;
}