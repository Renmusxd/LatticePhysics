#pragma once

#include <functional>
#include <glm/glm.hpp>
#include "Vertex.h"
#include "SpriteBatch.h"
#include "GLTexture.h"

namespace GameEngine{

	class Particle2D{
	public:
		Particle2D(){}
		~Particle2D(){}
		glm::vec2 position = glm::vec2(0.0f);
		glm::vec2 velocity = glm::vec2(0.0f);
		ColorRGBA8 color;
		float life = 0.0f;
		float width = 0.0f;
	};

	inline void defaultParticleUpdate(Particle2D& particle, float deltatime){
		particle.position += particle.velocity * deltatime;		
	}

	class ParticleBatch2D
	{
	public:
		ParticleBatch2D();
		~ParticleBatch2D();

		void init(
			int maxParticles, float decayRate, 
			GLTexture texture, 
			std::function<void(Particle2D&, float)> updateFunc = defaultParticleUpdate);

		void addParticle(
						const glm::vec2& position,
						const glm::vec2& velocity,
						const ColorRGBA8& color,
						float width);

		void update(float deltatime);

		void draw(SpriteBatch* spriteBatch);
		
	private:
		int findFreeParicle();

		std::function<void(Particle2D&, float)> m_updateFunc;

		float m_decayRate = 0.1f;
		Particle2D* m_particles = NULL; // Cache friendlyness - minimizes "new" and "delete"
		int m_maxParticles = 0;
		GLTexture m_texture;
		int m_lastFreeParticle = 0;
	};
}