#include "SMB_ScreenUI.h"


SMB_ScreenUI::SMB_ScreenUI(SpriteBatch *f_sb)
{
	p_sb = f_sb;
	m_tsmTexture = new TextureSelfManaged();
	if(p_sb != nullptr)
		m_tsmTexture->LoadTextureFromLocation(p_sb->get_renderer(),"Images/Mario3/LowerUI.png");

	m_sprLowerBackground = new Sprite();
	m_sprLowerBackground->set_singleSize(304,56);
	m_sprLowerBackground->set_textureSelfManageFromPointer(m_tsmTexture);
	m_sprLowerBackground->set_location(m_fOriginalLocationX, m_fOriginalLocationY);

	m_sprSmallerElements = new Sprite();
	m_sprSmallerElements->set_singleSize(32,16);
	m_sprSmallerElements->set_textureSelfManageFromPointer(m_tsmTexture);
	m_sprSmallerElements->set_location(m_sprLowerBackground->get_locationX(), m_sprLowerBackground->get_locationY());

	m_sprCharacters = new SprLetters();//Letters is basically just a sprite but the render can render text/numbers
	m_sprCharacters->set_singleSize(16, 16);
	m_sprCharacters->set_textureSelfManageFromPointer(m_tsmTexture);
	m_sprCharacters->set_location(m_sprLowerBackground->get_locationX(), m_sprLowerBackground->get_locationY());
	m_sprCharacters->set_lettersRow(4);
	m_sprCharacters->set_lettersCol(0);

	m_sprCharacters->set_numbersRow(5);
	m_sprCharacters->set_numbersCol(7);

	m_v2dNewLocation = new Vector2D();
}


SMB_ScreenUI::~SMB_ScreenUI()
{
	if(m_tsmTexture != nullptr)
		delete m_tsmTexture;

	if (m_sprLowerBackground != nullptr)
		delete m_sprLowerBackground;

	if (m_sprSmallerElements != nullptr)
		delete m_sprSmallerElements;

	if (m_sprCharacters != nullptr)
		delete m_sprCharacters;

	if (m_v2dNewLocation != nullptr)
		delete m_v2dNewLocation;
}

bool SMB_ScreenUI::Update()
{
	m_sprLowerBackground->set_location(m_fOriginalLocationX + m_fOffsetX, m_fOriginalLocationY + m_fOffsetY);
	return true;
}
bool SMB_ScreenUI::Draw()
{
	if (p_sb == nullptr) return false;
	//cout << "DR" << endl;
	if (m_sprLowerBackground != nullptr)
	{
		switch (m_cPlayerCounter)
		{
		case 'm':
			m_sprLowerBackground->set_singleLocation(2, 0);
			break;
		case 'l':
			m_sprLowerBackground->set_singleLocation(3, 0);
			break;
		case 'p':
			m_sprLowerBackground->set_singleLocation(4, 0);
			break;
		case 'w':
			m_sprLowerBackground->set_singleLocation(5, 0);
			break;
		default:
			m_sprLowerBackground->set_singleLocation(0, 0);
			break;
		}
		m_sprLowerBackground->RenderToScreen(p_sb->get_renderer());
	}
		

	if (m_sprSmallerElements != nullptr && m_sprCharacters != nullptr)
	{
		//Player Icon:
		m_v2dNewLocation->set_value(m_sprLowerBackground->get_locationX() + 8, m_sprLowerBackground->get_locationY() + 29);
		switch (m_cPlayerCounter)
		{
		case 'w':
			m_sprSmallerElements->set_singleLocation(6, 3);
			break;
		case 'p':
			m_sprSmallerElements->set_singleLocation(6, 2);
			break;
		case 'l':
			m_sprSmallerElements->set_singleLocation(6, 1);
			break;
		default:
		case 'm':
			m_sprSmallerElements->set_singleLocation(6, 0);
			break;
		}
		m_sprSmallerElements->RenderToScreen(p_sb->get_renderer(), m_v2dNewLocation);

		update_pointersToMembers();

		//P Arrows:
		m_sprCharacters->set_singleLocation(5,6);
		m_v2dNewLocation->set_value(m_sprLowerBackground->get_locationX()+104, m_sprLowerBackground->get_locationY() + 13);
		for (int i = 1; i <= 6; i++)
		{
			if (m_iPSpeed < i) continue;
			m_sprCharacters->RenderToScreen(p_sb->get_renderer(), m_v2dNewLocation);
			m_v2dNewLocation->add_valueX(16);
		}

		//P 7th Arrow
		if (m_iPSpeed == 7)
		{
			m_v2dNewLocation->set_value(m_sprLowerBackground->get_locationX() + 200, m_sprLowerBackground->get_locationY() + 13);
			m_sprSmallerElements->set_singleLocation(5, 2);
			m_sprSmallerElements->RenderToScreen(p_sb->get_renderer(), m_v2dNewLocation);
		}
		

		//World Number:
		m_v2dNewLocation->set_value(m_sprLowerBackground->get_locationX() + 74, m_sprLowerBackground->get_locationY() + 13);
		m_sprCharacters->RenderAsLetters(p_sb->get_renderer(), m_v2dNewLocation, to_string(m_iWorldNumber));

		//Points
		m_v2dNewLocation->set_value(m_sprLowerBackground->get_locationX() + 104, m_sprLowerBackground->get_locationY() + 29);
		m_sprCharacters->RenderAsLetters(p_sb->get_renderer(), m_v2dNewLocation, Utility::AddLeadingZeros(m_iPoints,7));

		//Coins
		m_v2dNewLocation->set_value(m_sprLowerBackground->get_locationX() + 264, m_sprLowerBackground->get_locationY() + 13);
		m_sprCharacters->RenderAsLetters(p_sb->get_renderer(), m_v2dNewLocation, Utility::AddLeadingZeros(m_iCoins, 2));

		//Lives
		m_v2dNewLocation->set_value(m_sprLowerBackground->get_locationX() + 58, m_sprLowerBackground->get_locationY() + 29);
		m_sprCharacters->RenderAsLetters(p_sb->get_renderer(), m_v2dNewLocation, Utility::AddLeadingZeros(m_iLives, 2));

		//Time
		m_v2dNewLocation->set_value(m_sprLowerBackground->get_locationX() + 248, m_sprLowerBackground->get_locationY() + 29);
		m_sprCharacters->RenderAsLetters(p_sb->get_renderer(), m_v2dNewLocation, Utility::AddLeadingZeros(m_iTime, 3));
	}
	return true;
}

void SMB_ScreenUI::update_pointersToMembers()
{
	if (m_cPtrPlayerCounter != nullptr)
		m_cPlayerCounter = *m_cPtrPlayerCounter;
	if(m_iPtrSpeed != nullptr)
		m_iPSpeed = *m_iPtrSpeed;
	if(m_iPtrWorldNumber != nullptr)
		m_iWorldNumber = *m_iPtrWorldNumber;
	if(m_iPtrPoints != nullptr)
		m_iPoints = *m_iPtrPoints;
	if(m_iPtrCoins != nullptr)
		m_iCoins = *m_iPtrCoins;
	if(m_iPtrLives != nullptr)
		m_iLives = *m_iPtrLives;
	if(m_iPtrTime != nullptr)
		m_iTime = *m_iPtrTime;
}