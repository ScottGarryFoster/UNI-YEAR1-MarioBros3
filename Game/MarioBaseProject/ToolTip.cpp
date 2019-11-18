#include "ToolTip.h"



ToolTip::ToolTip(SpriteBatch *f_sb, Controls *f_controls)
{
	if (f_sb == nullptr) cout << "Tooltip (Constructor): Sprite Batch is nullptr." << endl;
	p_sb = f_sb;
	if (f_controls == nullptr) cout << "Tooltip (Constructor): Controls is nullptr." << endl;
	p_controls = f_controls;

	m_sprBackground = new Sprite();
	if (!m_sprBackground->LoadTextureFromFile(p_sb->get_renderer(), "Images/LevelEditor/toolTipBackground.png"))
		cout << "Tooltip::construct: Couldn't load: " << "Images/LevelEditor/toolTipBackground.png" << endl;
	m_sprBackground->set_singleSize(10, 30);

	m_tsmLetters = new TextureSelfManaged();
	if (p_sb != nullptr)
		if (!m_tsmLetters->LoadTextureFromLocation(p_sb->get_renderer(), "Images/Mario3/Letters.png"))
			cout << "Tooltip::construct: Couldn't load: " << "Images/Mario3/Letters.png" << endl;

	m_sprLetters = new SprLetters();
	m_sprLetters->set_singleSize(16, 16);
	m_sprLetters->set_textureSelfManageFromPointer(m_tsmLetters);
	m_sprLetters->set_location(-1, -1);
	m_sprLetters->set_lettersRow(0);
	m_sprLetters->set_lettersCol(0);
	m_sprLetters->set_numbersRow(1);
	m_sprLetters->set_numbersCol(0);
}


ToolTip::~ToolTip()
{
	delete m_sprLetters;
	delete m_tsmLetters;
	delete m_sprBackground;
}

bool ToolTip::Update(float f_deltaTime)
{
	if (!m_bShowToolTip) return false;

	Controls::mouseInfo *l_mouseInfo = p_controls->get_mouseInfo();
	int l_iNewX = *l_mouseInfo->x - (m_sWords.length() * m_sprLetters->get_actualSizeWidth());
	if (l_iNewX < 0) l_iNewX = 0;
	int l_iNewY = *l_mouseInfo->y + (m_sprLetters->get_actualSizeWidth() * 2);
	if (l_iNewY > SCREEN_HEIGHT) l_iNewY = SCREEN_HEIGHT;
	m_v2dCursorLocation = { l_iNewX, l_iNewY };
	return true;
}

bool ToolTip::Draw()
{
	if (!m_bShowToolTip) return false;
	m_rectLetters = m_sprLetters->get_collision();
	//m_v2dCursorLocation

	m_sprBackground->set_singleLocation(0, 0);
	m_sprBackground->set_screenSize(0, 0);
	m_sprBackground->set_location(m_v2dCursorLocation.m_iX - m_sprBackground->get_actualSizeWidth(), m_v2dCursorLocation.m_iY);
	m_sprBackground->RenderToScreen(p_sb->get_renderer());

	m_sprBackground->set_singleLocation(0, 1);
	m_sprBackground->set_location(m_sprBackground->get_locationX() + m_sprBackground->get_actualSizeWidth(), m_sprBackground->get_locationY());
	Vector2D l_v2dLetterLocation = { m_sprBackground->get_locationX(),m_sprBackground->get_locationY() + 6 };
	m_sprBackground->set_screenSize(m_rectLetters.w, m_sprBackground->get_actualSizeHeight());
	m_sprBackground->RenderToScreen(p_sb->get_renderer());

	m_sprBackground->set_singleLocation(0, 2);
	m_sprBackground->set_location(m_sprBackground->get_locationX() + m_sprBackground->get_actualSizeWidth(), m_sprBackground->get_locationY());
	m_sprBackground->set_screenSize(0, 0);
	m_sprBackground->RenderToScreen(p_sb->get_renderer());

	m_sprLetters->RenderAsLetters(p_sb->get_renderer(), &l_v2dLetterLocation, m_sWords);
	return true;
}
