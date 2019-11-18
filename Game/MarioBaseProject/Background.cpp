#include "Background.h"



Background::Background()
{
}
Background::Background(SpriteBatch* f_sb)
{
	m_sb = f_sb;
	m_tsmTransparent = new TextureSelfManaged();
	m_tsmTransparent->LoadTextureFromLocation(m_sb->get_renderer(),"Images/Mario3/Transparent.png");
}

Background::~Background()
{
	if(m_tsmTexture != nullptr)
		delete m_tsmTexture;
	if(m_tsmTransparent != nullptr)
		delete m_tsmTransparent;
}
bool Background::load_background(string f_sLocation, int f_iWidth, int f_iHeight, bool f_bRepeat)
{
	//cout << "TRY LOAD BACKGROUND" << endl;
	if (f_sLocation == "") return false;
	if (m_tsmTexture == nullptr)
		m_tsmTexture = new TextureSelfManaged();
	bool l_bCheck = m_tsmTexture->LoadTextureFromLocation(m_sb->get_renderer(), f_sLocation);
	if (!l_bCheck) return false;
	//cout << "LOADED BACKGROUND" << endl;
	m_iWidth = f_iWidth;
	m_iHeight = f_iHeight;
	m_bRepeat = f_bRepeat;
	return true;
}
bool Background::Draw()
{
	if (p_camera == nullptr) return Draw_eachCamera(nullptr);

	if(p_camera->get_visable())
		Draw_eachCamera(p_camera);

	for (int i = 0; i < 10; i++)
	{
		if (p_alternativeCameras[i] != nullptr)
			if (p_alternativeCameras[i]->get_visable())
				Draw_eachCamera(p_alternativeCameras[i]);
	}
	return true;
}

bool Background::Draw_eachCamera(Camera *f_camera)
{
	//m_fScaleSize = SCREEN_HEIGHT / m_iHeight; //Scale to screenheight
	m_fScaleSize = SCREEN_HEIGHT / m_iHeight; //Scale to screenheight

	float l_fNewWidth = m_fScaleSize * m_iWidth;
	float l_fNewHeight = m_fScaleSize * m_iHeight;

	int l_iTimeHeight = 0; int l_iTimeWidth = 0;
	int l_iWorldHeight = SCREEN_HEIGHT;
	int l_iWorldWidth = SCREEN_WIDTH;

	bool l_bFoundWorld = false;
	if (f_camera != nullptr)
	{
		l_iWorldHeight = f_camera->get_worldHeight();
		l_iWorldWidth = f_camera->get_worldWidth();
	}
	l_iTimeHeight = l_iWorldHeight / m_iHeight;
	if (l_iWorldHeight % m_iHeight != 0)
		l_iTimeHeight++;

	l_iTimeWidth = l_iWorldWidth / m_iWidth;
	if (l_iWorldWidth % m_iWidth != 0)
		l_iTimeWidth++;

	for (int h = 0; h < l_iTimeHeight; h++)
	{
		for (int w = 0; w < l_iTimeHeight; w++)
		{
			int l_iLocationX = w * l_fNewWidth;
			int l_iLocationY = h * l_fNewHeight;


			SDL_Rect renderLocation = { l_iLocationX, l_iLocationY, l_fNewWidth, l_fNewHeight };

			SDL_Rect sourceLocation = { 0, 0, m_iWidth, m_iHeight };

			float l_fRenderWidth, l_fRenderHeight, l_fScreenLocationX, l_fScreenLocationY;
			int l_iSourceX, l_iSourceY, l_iSourceWidth, l_iSourceHeight;

			if (f_camera != nullptr)
			{
				l_fRenderWidth = l_fNewWidth;
				l_fRenderHeight = l_fNewHeight;
				l_fScreenLocationX = l_iLocationX - f_camera->get_x() + f_camera->get_screenX();
				l_fScreenLocationY = l_iLocationY - f_camera->get_y() + f_camera->get_screenY();

				l_iSourceX = 0;
				l_iSourceY = 0;
				l_iSourceWidth = m_iWidth;
				l_iSourceHeight = m_iHeight;

				if (l_fScreenLocationX + l_fNewWidth < f_camera->get_screenX())
				{
					continue;
				}
				else if (l_fScreenLocationX < f_camera->get_screenX())
				{
					//If they're moving off screen on the left
					l_fRenderWidth -= f_camera->get_x() - l_iLocationX;

					l_iLocationX -= l_fRenderWidth;
					l_iLocationX += l_fNewWidth;
					l_iSourceWidth = (l_fRenderWidth / m_fScaleSize);
					l_iSourceX += m_iWidth - l_iSourceWidth;

					//if (w == 0)
					//	cout << "1: l_fRenderWidth: " << l_fRenderWidth << "l_iSourceWidth: " << l_iSourceWidth << endl;
				}

				if (l_fScreenLocationX > f_camera->get_screenX() + f_camera->get_screenWidth())
				{
					//If they're off screen on the Right
					continue;
				}
				else if (l_fScreenLocationX + l_fNewWidth > f_camera->get_screenX() + f_camera->get_screenWidth())
				{
					//If they're moving on screen on the right
					l_fRenderWidth += f_camera->get_x() + f_camera->get_screenWidth() - (l_iLocationX + l_fRenderWidth);

					l_iSourceWidth = l_fRenderWidth / m_fScaleSize;

					//if (w == 0)
					//	cout << "2: l_fRenderWidth: " << l_fRenderWidth << "l_iSourceWidth: " << l_iSourceWidth << endl;
				}



				if (l_fRenderWidth < 0) continue;

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
					l_iSourceY += m_iHeight - l_iSourceHeight;
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
					l_fRenderHeight += f_camera->get_y() + f_camera->get_screenHeight() - (l_iLocationY + l_fRenderHeight);
					l_iSourceHeight = l_fRenderHeight / m_fScaleSize;
				}

				//Full Cull:
				if ((int)l_fRenderWidth == 0 || (int)l_fRenderHeight == 0 || l_iSourceHeight == 0 || l_iSourceWidth == 0)
					continue;
			}


			if (m_bLockToCamera && f_camera != nullptr)
			{
				l_iLocationX -= f_camera->get_x();
				l_iLocationX += f_camera->get_screenX();
				l_iLocationY -= f_camera->get_y();
				l_iLocationY += f_camera->get_screenY();
			}


			if (f_camera != nullptr)
			{
				renderLocation = { l_iLocationX, l_iLocationY, (int)l_fRenderWidth, (int)l_fRenderHeight };
				sourceLocation = { l_iSourceX, l_iSourceY, l_iSourceWidth, l_iSourceHeight };
			}
			else
			{
				renderLocation = { l_iLocationX, l_iLocationY, (int)l_fNewWidth, (int)l_fNewHeight };
				sourceLocation = { 0, 0, m_iWidth, m_iHeight };
			}


			//if (w > 0)
			//	SDL_RenderCopyEx(m_sb->get_renderer(), m_tsmTransparent->get_texturePointer(), &sourceLocation, &renderLocation, 0, NULL, SDL_FLIP_NONE);
			//else
			SDL_RenderCopyEx(m_sb->get_renderer(), m_tsmTexture->get_texturePointer(), &sourceLocation, &renderLocation, 0, NULL, SDL_FLIP_NONE);
		}
	}
	return true;
}