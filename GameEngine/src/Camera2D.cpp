#include "Camera2D.h"
using namespace GameEngine;

Camera2D::Camera2D() : 
	m_screenWidth(500),
	m_screenHeight(500),
	m_needsMatrixUpdate(true),
	m_scale(1.0f), 
	m_position(0.0f,0.0f), 
	m_cameraMatrix(1.0f),
	m_orthoMatrix(1.0f)
{}

Camera2D::~Camera2D(){}

void Camera2D::init(int screenWidth, int screenHeight)
{
	m_screenWidth = screenWidth;
	m_screenHeight = screenHeight;
	m_orthoMatrix = glm::ortho(0.0f,(float)m_screenWidth,0.0f,(float)m_screenHeight);
}

void Camera2D::update(){
	if (m_needsMatrixUpdate){
		glm::vec3 translate(-m_position.x+(m_screenWidth/2.0),-m_position.y+(m_screenHeight/2.0),0.0f);
		m_cameraMatrix = glm::translate(m_orthoMatrix,translate);

		glm::vec3 scale(m_scale,m_scale,0.0f);
		m_cameraMatrix = glm::scale(glm::mat4(1.0f),scale) * m_cameraMatrix;

		m_needsMatrixUpdate = false;
	}
}

glm::vec2 Camera2D::convertScreenToWorld(glm::vec2 screenCoords){
	screenCoords.y = m_screenHeight - screenCoords.y;
	screenCoords -= glm::vec2(m_screenWidth/2.0, m_screenHeight/2.0);
	screenCoords /= m_scale;
	screenCoords += m_position;
	return screenCoords;
}


bool Camera2D::isBoxInView(const glm::vec2& position, const glm::vec2 dimensions){

	glm::vec2 scaledScreenDimensions = glm::vec2(m_screenWidth, m_screenHeight) / m_scale;

	const float MIN_DISTANCE_X = dimensions.x/2.0f + scaledScreenDimensions.x/2.0f;
	const float MIN_DISTANCE_Y = dimensions.y/2.0f + scaledScreenDimensions.y/2.0f;

	glm::vec2 centerPos = m_position + dimensions/2.0f;
	glm::vec2 centerCameraPos = m_position;


	glm::vec2 distVec = centerPos - centerCameraPos;

	float xdepth = MIN_DISTANCE_X - std::abs(distVec.x);
	float ydepth = MIN_DISTANCE_Y - std::abs(distVec.y);

	// Colliding
	if (xdepth>0 && ydepth>0){
		return true;
	}
	return false;
}

