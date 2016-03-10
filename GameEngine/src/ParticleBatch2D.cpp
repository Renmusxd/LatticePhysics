#include "ParticleBatch2D.h"
#include "SpriteBatch.h"

namespace GameEngine{

	ParticleBatch2D::ParticleBatch2D(){

	}

	ParticleBatch2D::~ParticleBatch2D(){
		delete[] m_particles;
	}


	void ParticleBatch2D::init(
								int maxParticles, 
								float decayRate,
	 							GLTexture texture, 
	 							std::function<void(Particle2D&, float)> updateFunc)
	{
		m_maxParticles = maxParticles;
		m_particles = new Particle2D[maxParticles];
		m_decayRate = decayRate;
		m_texture = texture;
		m_updateFunc = updateFunc;
	}

	void ParticleBatch2D::update(float deltatime){
		for (int i = 0; i<m_maxParticles; i++){
			if (m_particles[i].life > 0.0f){
				m_updateFunc(m_particles[i],deltatime);
				m_particles[i].life -= m_decayRate*deltatime;
			}
		}

	}

	void ParticleBatch2D::draw(SpriteBatch* spriteBatch){
		glm::vec4 uvRect(0.0f,0.0f,1.0f,1.0f);
		for (int i = 0; i<m_maxParticles; i++){
			auto& p = m_particles[i];
			if (p.life > 0.0f){
				glm::vec4 destRect(p.position.x, p.position.y, p.width, p.width);
				spriteBatch->draw(
								destRect,
								uvRect,
								m_texture.id,
								0.0f,
								p.color
								);
			}
		}
	}

	int ParticleBatch2D::findFreeParicle(){
		for (int i = m_lastFreeParticle; i<m_maxParticles; i++){
			if (m_particles[i].life <= 0.0f){
				m_lastFreeParticle = i;
				return i;
			}
		}
		for (int i = 0; i<m_lastFreeParticle; i++){
			if (m_particles[i].life <= 0.0f){
				m_lastFreeParticle = i;
				return i;
			}
		}
		// No particles are free, overwrite first
		return 0;
	}

	void ParticleBatch2D::addParticle(
					const glm::vec2& position,
					const glm::vec2& velocity,
					const ColorRGBA8& color,
					float width)
	{
		int particleIndex = findFreeParicle();
		auto& p = m_particles[particleIndex];
		p.life = 1.0f;
		p.position = position;
		p.velocity = velocity;
		p.color = color;
		p.width = width;
	}

}