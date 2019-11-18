#include "ParticleManager.h"



ParticleManager::ParticleManager(SpriteBatch *f_sb)
{
	p_sb = f_sb;

	m_tsmBreakblock = new TextureSelfManaged();
	if (!m_tsmBreakblock->LoadTextureFromLocation(p_sb->get_renderer(), "Images/Mario3/breakblockParticle.png"))
		cout << "ParticleManager::ParticleManager: Could not load " << "Images/Mario3/breakblockParticle.png" << endl;

}


ParticleManager::~ParticleManager()
{
	delete m_tsmBreakblock;

	LinkedList *l_llCurrent = m_llParticles;
	for (int i = 0; i < m_llParticles->length(); i++)
	{
		if (l_llCurrent->get_data(nullptr, false) != nullptr)
		{
			Particle* l_particleCurrent = (Particle*)l_llCurrent->get_data(nullptr, false);
			delete l_particleCurrent;
			l_llCurrent->set_data(nullptr, false);
		}
		l_llCurrent = l_llCurrent->get_ahead();
	}
	delete m_llParticles;
}

bool ParticleManager::Update(float f_deltaTime)
{
	LinkedList *l_llCurrent = m_llParticles;
	if (m_llParticles != nullptr)
	{
		for (int i = 0; i < m_llParticles->length(); i++)
		{
			void* l_voidPtr = l_llCurrent->get_data(nullptr, true);
			if (l_voidPtr != nullptr)
			{
				Particle* l_particleCurrent = (Particle*)l_llCurrent->get_data(nullptr, false);
				l_particleCurrent->Update(f_deltaTime);
				if (l_particleCurrent->get_deleteMe())
				{
					delete l_particleCurrent;
					l_llCurrent->set_data(nullptr, false);
				}
			}

			if (l_llCurrent->get_ahead() == nullptr)
				break;
			else
				l_llCurrent = l_llCurrent->get_ahead();
		}
	}
	

	return true;
}

bool ParticleManager::Draw()
{
	LinkedList *l_llCurrent = m_llParticles;
	if (m_llParticles != nullptr)
	{
		for (int i = 0; i < m_llParticles->length(); i++)
		{
			void* l_voidPtr = l_llCurrent->get_data(nullptr, true);
			if (l_voidPtr != nullptr)
			{
				Particle* l_particleCurrent = (Particle*)l_llCurrent->get_data(nullptr, false);
				l_particleCurrent->RenderToScreen(p_sb->get_renderer());
			}

			if (l_llCurrent->get_ahead() == nullptr)
				break;
			else
				l_llCurrent = l_llCurrent->get_ahead();
		}
	}
	return true;
}

bool ParticleManager::create_particleEffect(particlePreset f_particlePreset, Vector2D f_v2dLocation)
{
	switch (f_particlePreset)
	{
	case ppBreakBlock:
		return create_breakBlock(f_v2dLocation);
		break;
	}
	return false;
}

Particle* ParticleManager::add_particle()
{
	if (m_llParticles == nullptr)
		m_llParticles = new LinkedList();
	
	LinkedList *l_llCurrent = m_llParticles;
	for (int i = 0; i < m_llParticles->length(); i++)
	{
		void* l_voidPtr = l_llCurrent->get_data(nullptr, true);
		if (l_voidPtr == nullptr)
		{
			Particle* l_particle = new Particle();
			void* l_voidPtrNew = (void*)l_particle;
			l_llCurrent->set_data(l_voidPtrNew, true);
			return l_particle;
		}

		if (l_llCurrent->get_ahead() == nullptr)
			l_llCurrent = m_llParticles->AddNewToEnd();
		else
			l_llCurrent = l_llCurrent->get_ahead();
	}
	cout << "Particle* ParticleManager::add_particle: Could not add a particle to the stack." << endl;
	return nullptr;
}

bool ParticleManager::create_breakBlock(Vector2D f_v2dLocation)
{
	Particle* l_llParticle[4];
	for (int i = 0; i < 4; i++)
	{
		l_llParticle[i] = add_particle();
		l_llParticle[i]->set_textureSelfManageFromPointer(m_tsmBreakblock);
		l_llParticle[i]->set_camera(p_camera);
		for (int j = 0; j < 10; j++)
			l_llParticle[i]->set_altCamera(p_cameraAlt[j],j);
		l_llParticle[i]->set_gravityInversionToMax();
		if (i % 2 == 0)
		{
			l_llParticle[i]->set_pDirection(Particle::partcleDirection::pdLeft);
			if (i == 0)
				l_llParticle[i]->set_gravityInversion(0.04f);
			else
				l_llParticle[i]->set_gravityInversion(0.02f);
		}
		else
		{
			l_llParticle[i]->set_pDirection(Particle::partcleDirection::pdRight);
			if (i == 1)
				l_llParticle[i]->set_gravityInversion(0.04f);
			else
				l_llParticle[i]->set_gravityInversion(0.02f);
		}
		l_llParticle[i]->set_singleSize(10, 10);
		l_llParticle[i]->set_scaleSize(p_fScaleSize);
		switch (i)
		{
		case 0:
			l_llParticle[i]->set_location(f_v2dLocation.m_iX + l_llParticle[i]->get_actualSizeWidth(), f_v2dLocation.m_iY);
			break;
		case 1:
			l_llParticle[i]->set_location(f_v2dLocation.m_iX, f_v2dLocation.m_iY);
			break;
		case 2:
			l_llParticle[i]->set_location(f_v2dLocation.m_iX + l_llParticle[i]->get_actualSizeWidth(), f_v2dLocation.m_iY + l_llParticle[i]->get_actualSizeHeight());
			break;
		case 3:
			l_llParticle[i]->set_location(f_v2dLocation.m_iX, f_v2dLocation.m_iY + l_llParticle[i]->get_actualSizeHeight());
			break;
		}
		
	}
	return true;
}