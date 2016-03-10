#pragma once

#include <unordered_map>
#include <glm/glm.hpp>

namespace GameEngine{

	class InputManager {
	public:
		InputManager();
		~InputManager();

		// Updates to next frame
		void update();
		void pressKey(unsigned int keyID);
		void releaseKey(unsigned int keyID);

		// Returns true if key was just pressed
		bool isKeyPressed(unsigned int keyID);
		// Returns true if key is held down
		bool isKeyDown(unsigned int keyID);

		void setMouseCoords(float x, float y);
		glm::vec2 getMouseCoords() const;

	private:
		bool wasKeyDown(unsigned int keyID);
		std::unordered_map<unsigned int, bool> m_keyMap;
		std::unordered_map<unsigned int, bool> m_previousKeyMap;
		glm::vec2 m_mouseCoords;
	};

}