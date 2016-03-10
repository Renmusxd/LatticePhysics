#pragma once

#include <SDL2/SDL_mixer.h>
#include <string>
#include <map>

namespace GameEngine{

	class SoundEffect{
	public:
		friend class AudioEngine; // so it can use private variables
		void play(int loops = 0);
	private:
		Mix_Chunk* _chunk = NULL;
	};

	class Music{
	public:
		friend class AudioEngine;
		void play(int loops = 1);

		static void pause();
		static void stop();
		static void resume();
		
	private:
		Mix_Music* _music = NULL;
	};

	class AudioEngine
	{
	public:
		AudioEngine();
		~AudioEngine();
		
		void init();
		void destroy();

		SoundEffect loadSoundEffect(const std::string& filePath);
		Music loadMusic(const std::string& filePath);
	private:
		std::map<std::string, Mix_Chunk*> m_effectMap;
		std::map<std::string, Mix_Music*> m_musicMap;
		bool m_isInitialized = false;


	};
}