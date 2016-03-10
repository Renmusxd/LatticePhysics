#include "AudioEngine.h"
#include "Errors.h"

namespace GameEngine{
	

void SoundEffect::play(int loops /* =0 */){
	if (Mix_PlayChannel(-1,_chunk,loops)==-1){
		std::printf("Mix_PlayChannel error: %s\n", Mix_GetError());
	}
}

void Music::play(int loops /* =1 */){
	if (Mix_PlayMusic(_music,loops)==-1){
		std::printf("Mix_PlayMusic error: %s\n", Mix_GetError());
	}
}


void Music::pause(){
	Mix_PauseMusic();
}
void Music::stop(){
	Mix_HaltMusic();
}
void Music::resume(){
	Mix_ResumeMusic();
}

AudioEngine::AudioEngine(){ /* Empty */ }
AudioEngine::~AudioEngine(){
	destroy();
}

void AudioEngine::init(){
	//const int CHANNELS_MONO = 1;
	const int CHANNELS_STEREO = 2;
	if (!m_isInitialized){
		// Paramet is a bitwise combination of MIX_INIT_FAC, MIX_INIT_MOD, MIX_INIT_MP3, MIX_INIT_OGG
		if (Mix_Init(MIX_INIT_OGG)==-1){
			fatalError("Mix_Init error: " + std::string(Mix_GetError()));
		}

		if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, CHANNELS_STEREO, 1024)==-1){
			fatalError("Mix_OpenAudio error: " + std::string(Mix_GetError()));
		}
		printf("AudioEngine loaded sucessfully");
		m_isInitialized = true;
	} else {
		fatalError("Tried to initialize audio engine twice");
	}
}

void AudioEngine::destroy(){
	if (m_isInitialized){
		m_isInitialized = false;
		for (auto& it : m_effectMap){
			Mix_FreeChunk(it.second);
		}
		for (auto& it : m_musicMap){
			Mix_FreeMusic(it.second);
		}
		m_effectMap.clear();
		m_musicMap.clear();
		Mix_CloseAudio();
		Mix_Quit();
	}
}

SoundEffect AudioEngine::loadSoundEffect(const std::string& filePath){
	// Try to find audio
	auto it = m_effectMap.find(filePath);
	SoundEffect effect;
	if (it == m_effectMap.end()){
		// Failed to find it, must load
		Mix_Chunk* chunk = Mix_LoadWAV(filePath.c_str());
		if (chunk==NULL){
			std::printf("Mix_LoadWAV error: %s\n", Mix_GetError());
		}
		effect._chunk = chunk;
		m_effectMap[filePath] = chunk;
	} else {
		// Found it
		effect._chunk = it->second;
	}
	return effect;
}

Music AudioEngine::loadMusic(const std::string& filePath){
	// Try to find audio
	auto it = m_musicMap.find(filePath);
	Music music;
	if (it == m_musicMap.end()){
		// Failed to find it, must load
		Mix_Music* mixmusic = Mix_LoadMUS(filePath.c_str());
		if (mixmusic==NULL){
			std::printf("Mix_LoadMUS error: %s\n", Mix_GetError());
		}
		music._music = mixmusic;
		m_musicMap[filePath] = mixmusic;
	} else {
		// Found it
		music._music = it->second;
	}
	return music;
}

}