/* 
 * File:   DebugRenderer.h
 * Author: Sumner
 *
 * Created on June 27, 2015, 10:56 PM
 */

#ifndef DEBUGRENDERER_H
#define	DEBUGRENDERER_H

#include "Vertex.h"
#include <glm/glm.hpp>
#include <vector>
#include "GLSLProgram.h"

namespace GameEngine{

class DebugRenderer {
        
struct DebugVertex {
    glm::vec2 position;
    ColorRGBA8 color;
};

public:
    DebugRenderer();
    virtual ~DebugRenderer();
    
    void init();
    void end();
    void drawBox(const glm::vec4& destRect, const ColorRGBA8& color, float angle);
    void drawCircle(const glm::vec2& center, const ColorRGBA8& color, float radius);
    void render(const glm::mat4 projectionMatrix, float lineWidth);
    void dispose();
    
private:
    GLSLProgram m_program;
    std::vector<DebugVertex> m_verts;
    GLuint m_vbo =0, m_vao =0, m_ibo =0;
    std::vector<GLuint> m_indices;
    
    int m_numElements = 0;
};

}

#endif	/* DEBUGRENDERER_H */

