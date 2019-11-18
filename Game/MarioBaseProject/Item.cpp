#include "Item.h"



Item::Item(SMB_PlayerPlatformer *f_players[2], int f_iNumberOfPlayers)
{
	*m_players = *f_players;
	m_playersLength = f_iNumberOfPlayers;
}


Item::~Item()
{
	delete m_sprLinearAnimation;
	delete m_itemAction;
}


bool Item::Update(float f_deltaTime)
{
	if (!get_didDrawThisFrame() && m_bDestoryNotOnScreen)
		m_bDelete = true;//Delete me when not on screen. Use this var so the class who manage this can nullptr this

	if (m_itemAction != nullptr)
		update_action(f_deltaTime);

	if (m_sprLinearAnimation != nullptr)
		update_animation(f_deltaTime);

	Platformer_Character::Update(f_deltaTime);

	if(get_locationY() > p_camera->get_worldHeight())
		m_bDelete = true;

	return false;
}

bool Item::update_action(float f_deltaTime)
{
	if (m_itemAction == nullptr) return false;

	//Setup
	if (!m_bHaveSetup)
	{
		set_location(m_itemAction->get_locationX(), m_itemAction->get_locationY());
		if (m_itemAction->get_actualHeight() == -1)
			m_itemAction->set_actualHeight(get_actualSizeHeight());
	}
	m_bHaveSetup = true;

	//Update Gravity
	set_disableGravity(m_itemAction->get_gravity());
	//Update Action
	m_itemAction->Update(f_deltaTime);
	set_flip((SpriteFlip)m_itemAction->get_flipImage());

	//Check Gravity Adjustments
	if (m_itemAction->get_gravityAdjustment() != -9001)
	{
		set_gravityPull(m_itemAction->get_gravityAdjustment());
		m_itemAction->set_gravityAdjustment(-9001);
	}
	if(m_bAlwaysCollectable) m_bCollectable = true;

	//If we're not using gravity Action holds the new location
	if (m_itemAction->get_gravity())
		set_location(m_itemAction->get_locationX(), m_itemAction->get_locationY());
	else
	{
		if (m_itemAction->get_moveX() != 0 || m_itemAction->get_moveY() != 0)
		{
			Direction *l_directionReturn = new Direction();
			bool l_bCouldMove = move_character(m_itemAction->get_moveX(), m_itemAction->get_moveY(), l_directionReturn);
			p_animation = Falling;
			bool l_bMoveCharacterReaction = false;
			
			m_itemAction->set_moveCharacterReaction(l_bCouldMove);
			delete l_directionReturn;
		}
		
		m_itemAction->set_currentLocation(get_locationX(), get_locationY());//If we're are then update action so it can react

		m_bCollectable = true;//If gravity then it's moving around and we can collect
	}

	if (m_itemAction->get_delete())//Allow Action to signal a delete
	{
		if (m_bScoreOnDelete == true)//There is a score to be had
			m_scoreOnScreen->new_score(m_itemAction->get_originalX(), m_itemAction->get_originalY() - m_itemAction->get_actualHeight(), m_bScoreAdditive);
		m_bDelete = true;
	}
	return true;
}

bool Item::update_animation(float f_deltaTime)
{
	if (m_sprLinearAnimation == nullptr) return false;

	m_sprLinearAnimation->Update(f_deltaTime);
	set_singleLocation(m_sprLinearAnimation->get_startLocationRow(), m_sprLinearAnimation->get_currentLocation());
	return true;
}

bool Item::load_fromFile(string f_sLocation)
{
	if (f_sLocation == "") return false;

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
				cout << "bool Item::load_fromFile: Animation not loaded" << endl;
		}
		else if (f_sGetLine == "ItemAction")
		{
			if (m_itemAction == nullptr)
				m_itemAction = new ItemAction();

			while (f_sGetLine != "---")
			{
				getline(infile, f_sGetLine);
				f_sContent += f_sGetLine + "~";
			}
			if (!m_itemAction->setup_action(f_sContent))
				cout << "bool Item::load_fromFile: Item Action not loaded" << endl;
			m_itemAction->set_canCollect(&m_bCollectable);
		}
	}
	infile.close();

	set_didDrawThisFrame(true);

	return true;
}

void Item::set_intialLocation(float f_fX, float f_fY)
{
	if (m_itemAction != nullptr)
	{
		m_itemAction->set_currentLocation(f_fX,f_fY);
	}
}

bool Item::setup_meta(string f_sData)
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

	return true;
}

bool Item::set_byString(string f_sParameter, string f_sValue)
{
	string f_sParaUpper = Utility::StringToUpperCase(f_sParameter);//Shouldn't be case sensative
	string f_sValueUpper = Utility::StringToUpperCase(f_sValue);//Shouldn't be case sensative

	//cout << "para: " << f_sParaUpper << endl;
	//cout << "value: " << f_sValueUpper << endl;

	if (f_sParaUpper == "" || f_sValueUpper == "")
		return false;
	else if (f_sParaUpper == "SCORE:")
	{
		if (f_sValueUpper == "TRUE"){m_bScoreOnDelete = true; return true;}
		if (f_sValueUpper == "FALSE"){m_bScoreOnDelete = false; return true;}
		return false;
	}
	else if (f_sParaUpper == "ADDITIVE:")
	{
		if (f_sValueUpper == "TRUE") { m_bScoreAdditive = true; return true; }
		if (f_sValueUpper == "FALSE") { m_bScoreAdditive = false; return true; }
		return false;
	}
	else if (f_sParaUpper == "DESTROYNOTONSCREEN:")
	{
		if (f_sValueUpper == "TRUE") { m_bDestoryNotOnScreen = true; return true; }
		if (f_sValueUpper == "FALSE") { m_bDestoryNotOnScreen = false; return true; }
		return false;
	}
	else if (f_sParaUpper == "DRAWBELOWSPECIALBLOCKS:")
	{
		if (f_sValueUpper == "TRUE") { m_bDrawBelowSpecialBlocks = true; return true; }
		if (f_sValueUpper == "FALSE") { m_bDrawBelowSpecialBlocks = false; return true; }
		return false;
	}
	else if (f_sParaUpper == "ALWAYSCOLLECTABLE:")
	{
		if (f_sValueUpper == "TRUE") { m_bAlwaysCollectable = true; return true; }
		if (f_sValueUpper == "FALSE") { m_bAlwaysCollectable = false; return true; }
		return false;
	}

	return false;
}