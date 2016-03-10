#pragma once

namespace GameEngine{

	class FpsLimiter{
	public:
		FpsLimiter();
		~FpsLimiter();

		void init(float maxFPS);

		void setMaxFPS(float maxFPS);

		void begin();
		float end(); // Returns current FPS
	private:
		void calculateFPS();
		float m_fps;
		float m_maxFPS;
		float m_frameTime;
		unsigned int m_startTicks;

	};

	
}