#pragma once
#include "Particle.h"
#include "LinkedList.h"
#include "Vector2D.h"
#include "TextureSelfManaged.h"
#include "SPRITEBATCH.h"

#include <string>

using namespace std;

class ParticleManager
{
public:
	enum particlePreset : int {ppBreakBlock = 0};
protected:
	SpriteBatch *p_sb = nullptr;
	Camera *p_camera = nullptr;
	Camera *p_cameraAlt[10] = { nullptr };
	float p_fScaleSize = 1;
private:
	LinkedList *m_llParticles = nullptr;
	TextureSelfManaged *m_tsmBreakblock = nullptr;
public:
	ParticleManager(SpriteBatch *f_sb);
	virtual ~ParticleManager();

	//Defaults
	virtual bool Update(float f_deltaTime);
	virtual bool Draw();

	//Sets
	void set_mainCamera(Camera *f_camera) { p_camera = f_camera; }
	void set_altCamera(Camera *f_camera, int f_iIndex) { p_cameraAlt[f_iIndex] = f_camera; }
	void set_scaleSize(float f_fData) { p_fScaleSize = f_fData; }

	bool create_particleEffect(particlePreset f_particlePreset, Vector2D f_v2dLocation);
private:
	Particle* add_particle();
	bool create_breakBlock(Vector2D f_v2dLocation);
};

