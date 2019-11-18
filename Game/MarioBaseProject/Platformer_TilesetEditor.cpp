#include "Platformer_TilesetEditor.h"


Platformer_TilesetEditor::Platformer_TilesetEditor(SpriteBatch *f_sb, Audio *f_audio, int *f_iSelectMenu) : GameInstance(f_sb, f_audio, f_iSelectMenu)
{
	
}


Platformer_TilesetEditor::~Platformer_TilesetEditor()
{
}

bool Platformer_TilesetEditor::Draw()
{
	if (m_menuStage == pteMainMenu)
		draw_mainMenu();
	return true;
}

bool Platformer_TilesetEditor::Update(float f_deltatime)
{
	if(!m_bLoaded)
		setup_tilesetEditor();

	if (m_menuStage == pteMainMenu)
		update_mainMenu();

	return true;
}

bool Platformer_TilesetEditor::setup_tilesetEditor()
{
	load_tilesetList();
	load_mainMenu();
	m_bLoaded = true;
	return true;
}

bool Platformer_TilesetEditor::load_tilesetList()
{
	string l_sLine = "";
	bool l_bDeleteCurrentList = false;
	ifstream l_ifMeta("Meta.txt");
	if (l_ifMeta.is_open())
	{
		while (getline(l_ifMeta, l_sLine))
		{
			if (!l_bDeleteCurrentList)
			{
				if (m_llNamesOfTilesets != nullptr)
					delete m_llNamesOfTilesets;
				m_llNamesOfTilesets = new LinkedList();
			}
			m_llNamesOfTilesets->AddNewToEnd(l_sLine);
		}
		l_ifMeta.close();
	}
	else
	{
		return false;
	}
	return true;
}

bool Platformer_TilesetEditor::load_mainMenu()
{
	m_sprBackgroundColor = new Sprite();
	m_sprBackgroundColor->LoadTextureFromFile(p_sb->get_renderer(), "Images/GameSelectMain/SMB_color.png");
	m_sprBackgroundColor->set_singleSize(5, 5);
	m_sprBackgroundColor->set_screenSize(1280, 720);
	m_sprBackgroundColor->set_location(0, 0);

	m_ltrsTitle = new SprLetters();
	m_ltrsTitle->LoadTextureFromFile(p_sb->get_renderer(),"Images/Mario3/Letters.png");
	m_ltrsTitle->set_singleSize(16, 16);
	m_ltrsTitle->set_scaleSize(1.5f);
	m_ltrsTitle->set_lettersLocation(0, 0);
	m_ltrsTitle->set_numbersLocation(1, 0);
	m_ltrsTitle->set_location(50,50);

	m_ltrsNewTileset = new SprLetters();
	m_ltrsNewTileset->LoadTextureFromFile(p_sb->get_renderer(), "Images/Mario3/Letters.png");
	m_ltrsNewTileset->set_singleSize(16, 16);
	m_ltrsNewTileset->set_scaleSize(1.5f);
	m_ltrsNewTileset->set_lettersLocation(4, 0);
	m_ltrsNewTileset->set_numbersLocation(4, 0);
	m_ltrsNewTileset->set_location(m_ltrsTitle->get_locationX(), m_ltrsTitle->get_locationY() + 75);
	return true;
}

bool Platformer_TilesetEditor::update_mainMenu()
{

	return false;
}

bool Platformer_TilesetEditor::draw_mainMenu()
{
	if (m_sprBackgroundColor != nullptr)
		m_sprBackgroundColor->RenderToScreen(p_sb->get_renderer());

	if (m_ltrsTitle != nullptr)
		m_ltrsTitle->RenderAsLetters(p_sb->get_renderer(), nullptr, "Tileset Editor");

	if (m_ltrsNewTileset != nullptr)
		m_ltrsNewTileset->RenderAsLetters(p_sb->get_renderer(), nullptr, "New Tileset");
	return false;
}
