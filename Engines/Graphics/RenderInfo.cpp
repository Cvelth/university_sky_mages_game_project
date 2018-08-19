#include "RenderInfo.hpp"
#include "../MyGraphicsLibrary/MGL/Primitive/AbstractDrawableObject.hpp"

RenderInfo::RenderInfo() : is_initialized(false), m_object(new mgl::AbstractDrawableObject()) {}
RenderInfo::~RenderInfo() { delete m_object; }

void RenderInfo::addPrimitive(mgl::Primitive *v) {
	is_initialized = false;
	m_object->addPrimitive(v);
}
void RenderInfo::insertVertexArray(mgl::VertexArray *vao) {
	is_initialized = false;
	m_object->insertVertexArray(vao);
}
void RenderInfo::send(mgl::DataUsage const& u) {
	m_object->send(u);
	is_initialized = true;
}
void RenderInfo::draw() {
	//does it need to be checked?
	m_object->draw();
}
void RenderInfo::draw(mgl::InstancingArray *instances) {
	//does it need to be checked?
	m_object->draw(instances);
}
void RenderInfo::draw(mgl::InstancingMultiArray *instances) {
	//does it need to be checked?
	m_object->draw(instances);
}
void RenderInfo::clean() {
	is_initialized = false;
	m_object->clean();
}
