#include "RenderInfo.hpp"
#include "../MyGraphicsLibrary/MGL/Primitive/AbstractDrawableObject.hpp"

RenderInfo::RenderInfo() {
	m_object = new mgl::AbstractDrawableObject();
}

RenderInfo::~RenderInfo() {
	delete m_object;
}
