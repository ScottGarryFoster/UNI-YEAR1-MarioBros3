#include "Sprite.h"



Sprite::Sprite()
{
	m_iScreenWidth = 0, m_iScreenHeight = 0;
	m_bHasLoadedTexture = false;
	m_tsmTexture = new TextureSelfManaged();

}


Sprite::~Sprite()
{
	if(!m_bLoadedTextureFromPointer && m_tsmTexture != nullptr)
	delete m_tsmTexture; //This has the destory texture in
	m_tsmTexture = nullptr;
}

bool Sprite::move_location(float f_iX, float f_iY)
{ 
	bool f_bReturn = false;//We'll set this to true if we had to lock them to the world.

	m_iLocationX += f_iX;
	if (m_iLocationX < 0 && m_bLocationLockXLeft)
	{
		m_iLocationX = 0;
		f_bReturn = true;
	}
	if(p_camera != nullptr && m_bLocationLockXRight)
		if (m_iLocationX + m_iActualWidth > p_camera->get_worldWidth())
		{
			m_iLocationX = p_camera->get_worldWidth() - m_iActualWidth;
			f_bReturn = true;
		}
	m_iLocationY += f_iY; 
	if (m_iLocationY < 0 && m_bLocationLockYTop)
	{
		m_iLocationY = 0;
		f_bReturn = true;
	}
	if (p_camera != nullptr && m_bLocationLockYBottom)
		if (m_iLocationY > p_camera->get_worldHeight())
		{
			m_iLocationY = p_camera->get_worldHeight();
			f_bReturn = true;
		}
	return f_bReturn;
}

bool Sprite::LoadTextureFromFile(SDL_Renderer* f_sdlRenderer, string f_sLocation)
{
	m_bHasLoadedTexture = m_tsmTexture->LoadTextureFromLocation(f_sdlRenderer, f_sLocation);
	return m_bHasLoadedTexture;
}

void Sprite::set_textureFromPointer(SDL_Texture* f_sdlTexture, string f_sLocation)
{
	m_bHasLoadedTexture = m_tsmTexture->set_textureFromPointer(f_sdlTexture, f_sLocation);
}

bool Sprite::RenderToScreen(SDL_Renderer* f_sdlRenderer, Vector2D* f_v2dLocation)
{
	/*
		BUGS ARE COMMON HERE
		SO THIS IS THE DEBUG AND MESSAGES:
	*/
	bool l_bCanLoadTexture = true;
	if (&m_bHasLoadedTexture == nullptr)
	{
		if (!m_bDebugTextureNull)
		{
			cout << "Sprite::RenderToScreen(SDL_Renderer* f_sdlRenderer, Vector2D* f_v2dLocation): m_bHasLoadedTexture NULL" << endl;
			m_bDebugTextureNull = true;
		}
		return false;
	}
	else if (!m_bHasLoadedTexture)
	{
		if (!m_bDebugTextureNull)
		{
			//cout << "Sprite::RenderToScreen(SDL_Renderer* f_sdlRenderer, Vector2D* f_v2dLocation): No texture loaded" << endl;
			m_bDebugTextureNull = true;
		}
		return false;
	}
	else if (get_texturePointer() == nullptr)
	{
		if (!m_bDebugTextureNull)
		{
			cout << "Sprite::RenderToScreen(SDL_Renderer* f_sdlRenderer, Vector2D* f_v2dLocation): Texture set to nullptr" << endl;
			m_bDebugTextureNull = true;
		}
		return false;
	}
	m_bDebugTextureNull = false;//Ensures I don't flood the screen

	if (p_camera == nullptr)
		return RenderToScreenInner(f_sdlRenderer, f_v2dLocation);
	else
	{
		m_bDidDrawThisFrame = false;
		if (p_camera->get_visable())
			if (RenderToScreenInner(f_sdlRenderer, f_v2dLocation, p_camera))
				m_bDidDrawThisFrame = true;//Resets
				

		for (int i = 0; i < 10; i++)
		{
			if (p_alternativeCameras[i] != nullptr)
				if (p_alternativeCameras[i]->get_visable())
					if (RenderToScreenInner(f_sdlRenderer, f_v2dLocation, p_alternativeCameras[i]))
						m_bDidDrawThisFrame = true;
						
		}
		return true;
	}
	return false;
}

bool Sprite::RenderToScreenInner(SDL_Renderer* f_sdlRenderer, Vector2D* f_v2dLocation, Camera *f_camera)
{

	if (m_bVisable)
	{
		m_bDebugTextureNull = false;

		update_actualsize();

		int l_iLocationX = m_iLocationX, l_iLocationY = m_iLocationY;
		if (f_v2dLocation != nullptr)
		{
			l_iLocationX = f_v2dLocation->m_iX;
			l_iLocationY = f_v2dLocation->m_iY;
		}

		SDL_Rect renderLocation = { l_iLocationX, l_iLocationY, m_iActualWidth, m_iActualHeight };
		SDL_Rect sourceLocation = { m_iSpriteCol * m_iSingleWidth, m_iSpriteRow * m_iSingleHeight, m_iSingleWidth, m_iSingleHeight };

		float l_fRenderWidth, l_fRenderHeight = 0, l_fScreenLocationX, l_fScreenLocationY;
		int l_iSourceX, l_iSourceY, l_iSourceWidth, l_iSourceHeight;

		if (f_camera != nullptr)
		{
			if (!f_camera->get_visable())
				return false;

			l_fRenderWidth = m_iActualWidth;
			l_fRenderHeight = m_iActualHeight;
			l_fScreenLocationX = l_iLocationX - f_camera->get_x() + f_camera->get_screenX();
			l_fScreenLocationY = l_iLocationY - f_camera->get_y() + f_camera->get_screenY();

			//l_iSourceX = m_iSpriteCol * m_iSingleHeight;
			l_iSourceX = m_iSpriteCol * m_iSingleWidth;
			//l_iSourceY = m_iSpriteRow * m_iSingleWidth;
			l_iSourceY = m_iSpriteRow * m_iSingleHeight;
			l_iSourceWidth = m_iSingleWidth;
			l_iSourceHeight = m_iSingleHeight;

			if (l_fScreenLocationX + m_iActualWidth < f_camera->get_screenX())
			{
				return false;
			}
			else if (l_fScreenLocationX < f_camera->get_screenX())
			{
				l_fRenderWidth -= f_camera->get_x() - l_iLocationX;
				l_iLocationX -= l_fRenderWidth;
				l_iLocationX += m_iActualWidth;

				l_iSourceWidth = l_fRenderWidth / m_iScaleSize;
				if (m_flip == None)
					l_iSourceX += m_iSingleWidth - l_iSourceWidth;
			}

			if (l_fScreenLocationX > f_camera->get_screenX() + f_camera->get_screenWidth())
			{
				//If they're off screen on the Right
				return false;
			}
			else if (l_fScreenLocationX + m_iActualWidth > f_camera->get_screenX() + f_camera->get_screenWidth())
			{
				//If they're moving on screen on the right
				//l_fRenderWidth += f_camera->get_x() + f_camera->get_screenWidth() - ((tile) * l_fNewWidth);
				//l_fRenderWidth += f_camera->get_x() + f_camera->get_screenWidth() - ((tile) * l_fNewWidth);
				l_fRenderWidth += f_camera->get_x() + f_camera->get_screenWidth() - (l_iLocationX + l_fRenderWidth);
				l_iSourceWidth = l_fRenderWidth / m_iScaleSize;
				if (m_flip == Horizontal)
					l_iSourceX += m_iSingleWidth - l_iSourceWidth;
			}

			//Y:
			if (l_fScreenLocationY + m_iActualHeight < f_camera->get_screenY())
			{
				return false;
			}
			else if (l_fScreenLocationY < f_camera->get_screenY())
			{

				//If they're moving off screen on the left
				//l_fRenderWidth -= f_camera->get_x() - ((tile - 1) * l_fNewWidth);
				l_fRenderHeight -= f_camera->get_y() - (l_iLocationY);
				l_iLocationY -= l_fRenderHeight;
				l_iLocationY += m_iActualHeight;
				l_iSourceHeight = l_fRenderHeight / m_iScaleSize;
				l_iSourceY += m_iSingleHeight - l_iSourceHeight;
			}

			if (l_fScreenLocationY > f_camera->get_screenY() + f_camera->get_screenHeight())
			{

				//If they're off screen on the Right
				return false;
			}
			else if (l_fScreenLocationY + m_iActualHeight > f_camera->get_screenY() + f_camera->get_screenHeight())
			{

				//If they're moving on screen on the right
				//l_fRenderWidth += f_camera->get_x() + f_camera->get_screenWidth() - ((tile) * l_fNewWidth);
				//l_fRenderWidth += f_camera->get_x() + f_camera->get_screenWidth() - ((tile) * l_fNewWidth);
				l_fRenderHeight += f_camera->get_y() + f_camera->get_screenHeight() - (l_iLocationY + l_fRenderHeight);
				l_iSourceHeight = l_fRenderHeight / m_iScaleSize;
			}

			//Full Cull:
			if ((int)l_fRenderWidth == 0 || (int)l_fRenderHeight == 0 || l_iSourceHeight == 0 || l_iSourceWidth == 0)
				return false;
		}


		if (m_bLockToCamera && f_camera != nullptr)
		{
			l_iLocationX -= f_camera->get_x();
			l_iLocationX += f_camera->get_screenX();
			l_iLocationY -= f_camera->get_y();
			l_iLocationY += f_camera->get_screenY();
		}

		/*
			renderLocation:
			NAME			PURPOSE
			locationX		Where the sprite will appear on the screen
			locationY		Where the sprite will appear on the screen
			sizeX			The size of image after projection
			sizeY			The size of image after projeciton
		*/

		if (f_camera == nullptr)
			renderLocation = { l_iLocationX, l_iLocationY, m_iActualWidth, m_iActualHeight };
		else
			renderLocation = { l_iLocationX, l_iLocationY, (int)l_fRenderWidth, (int)l_fRenderHeight };

		/*
			sourceLocaiton
			NAME			PURPOSE
			startX			The starting X of the part of the texutre you'd like to render off the sheet
			startY			The starting y of the part of the texutre you'd like to render off the sheet
			endX			The size of the image you'd like to display so, startX + endX = width
			endY			The size of the image you'd like to display so, startY + endY = height
		*/

		if (f_camera == nullptr)
			sourceLocation = { m_iSpriteCol * m_iSingleWidth, m_iSpriteRow * m_iSingleHeight, m_iSingleWidth, m_iSingleHeight };
		else
			sourceLocation = { l_iSourceX, l_iSourceY, l_iSourceWidth, l_iSourceHeight };



		//cout << "m_fSingleWidth: " << m_fSingleWidth;
		//SDL_Rect sourceLocation = { m_iSpriteRow * m_fSingleHeight, m_iSpriteCol * m_fSingleWidth, 100, 100 };
		switch (m_flip)
		{
		case None:
			SDL_RenderCopyEx(f_sdlRenderer, get_texturePointer(), &sourceLocation, &renderLocation, 0, NULL, SDL_FLIP_NONE);
			break;
		case Horizontal:
			SDL_RenderCopyEx(f_sdlRenderer, get_texturePointer(), &sourceLocation, &renderLocation, 0, NULL, SDL_FLIP_HORIZONTAL);
			break;
		case Vertical:
			SDL_RenderCopyEx(f_sdlRenderer, get_texturePointer(), &sourceLocation, &renderLocation, 0, NULL, SDL_FLIP_VERTICAL);
			break;
		case Both:
			SDL_RendererFlip flip = (SDL_RendererFlip)(SDL_FLIP_HORIZONTAL | SDL_FLIP_VERTICAL);
			SDL_RenderCopyEx(f_sdlRenderer, get_texturePointer(), &sourceLocation, &renderLocation, 0, NULL, flip);
			break;
		}
			
		return true;
	}

	return false;
}

bool Sprite::update_actualsize()
{
	if (m_iScreenWidth == 0)m_iActualWidth = m_iSingleWidth * m_iScaleSize; else m_iActualWidth = m_iScreenWidth;
	if (m_iScreenHeight == 0)m_iActualHeight = m_iSingleHeight * m_iScaleSize; else m_iActualHeight = m_iScreenHeight;
	return true;
}

void Sprite::debug_all()
{
	cout << "m_bDebugTextureNull: "; if (m_bDebugTextureNull) cout << "TRUE"; else cout << "FALSE"; cout << endl;
	cout << "m_iScreenWidth: " << m_iScreenWidth << " m_iScreenHeight:" << m_iScreenHeight << endl;
	cout << "m_iActualWidth: " << m_iActualWidth << " m_iActualHeight:" << m_iActualHeight << endl;
	cout << "m_iLocationX: " << m_iLocationX << " m_iLocationY:" << m_iLocationY << endl;
	cout << "m_iSingleWidth: " << m_iSingleWidth << " m_iSingleWidth:" << m_iSingleWidth << endl;
	cout << "m_iSpriteRow: " << m_iSpriteRow << " m_iSpriteCol:" << m_iSpriteCol << endl;
	cout << "m_flip: " << m_flip << endl;
	cout << "m_iScaleSize: " << m_iScaleSize << endl;
	cout << "m_bHasLoadedTexture: "; if (m_bHasLoadedTexture) cout << "TRUE"; else cout << "FALSE"; cout << endl;
	cout << "m_sLastTextureLoaded: " << m_sLastTextureLoaded << endl;
	cout << "m_bVisable: "; if (m_bVisable) cout << "TRUE"; else cout << "FALSE"; cout << endl;
	cout << "m_bDrawFromBottom: "; if (m_bDrawFromBottom) cout << "TRUE"; else cout << "FALSE"; cout << endl;
	cout << "m_bLockToCamera: "; if (m_bLockToCamera) cout << "TRUE"; else cout << "FALSE"; cout << endl;
	cout << "m_bLocationLockYTop: "; if (m_bLocationLockYTop) cout << "TRUE"; else cout << "FALSE"; cout << endl;
	cout << "m_bLocationLockYBottom: "; if (m_bLocationLockYBottom) cout << "TRUE"; else cout << "FALSE"; cout << endl;
	cout << "m_bLocationLockXLeft: "; if (m_bLocationLockXLeft) cout << "TRUE"; else cout << "FALSE"; cout << endl;
	cout << "m_bLocationLockXRight: "; if (m_bLocationLockXRight) cout << "TRUE"; else cout << "FALSE"; cout << endl;
	cout << "m_bLoadedTextureFromPointer: "; if (m_bLoadedTextureFromPointer) cout << "TRUE"; else cout << "FALSE"; cout << endl;
	cout << "p_camera: "; if (p_camera != nullptr) cout << "TRUE"; else cout << "FALSE"; cout << endl;
	for (int i = 0; i < 10; i++)
	{
		cout << "p_alternativeCameras: "; if (p_alternativeCameras[i] != nullptr) cout << "TRUE"; else cout << "FALSE"; cout << endl;
	}
}