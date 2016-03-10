#pragma once
#include <vector>

namespace GameEngine{
	class ParticleBatch2D;
	class SpriteBatch;

	class ParticleEngine2D
	{
	public:
		ParticleEngine2D();
		~ParticleEngine2D();
	
		// After adding a particle batch, ParticleEngine2D becomes
		// responsible for deallocations
		void addParticleBatch(ParticleBatch2D* particleBatch);

		void draw(SpriteBatch* spriteBatch);

		void update(float deltatime);

	private:
		std::vector<ParticleBatch2D*> m_batches;
	};

}