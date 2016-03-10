#include "ParticleEngine2D.h"
#include "SpriteBatch.h"
#include "ParticleBatch2D.h"

namespace GameEngine{
	ParticleEngine2D::ParticleEngine2D(){}

	ParticleEngine2D::~ParticleEngine2D(){
		for (auto& b : m_batches){
			delete b;
		}
	}

	void ParticleEngine2D::addParticleBatch(ParticleBatch2D* particleBatch){
		m_batches.push_back(particleBatch);
	}

	void ParticleEngine2D::draw(SpriteBatch* spriteBatch){
		spriteBatch->begin();
		for (auto& b : m_batches){
			b->draw(spriteBatch);
		}
		spriteBatch->end();
   		spriteBatch->renderBatch();
	}

	void ParticleEngine2D::update(float deltatime){
		for (auto& b : m_batches){
			b->update(deltatime);
		}
	}
}