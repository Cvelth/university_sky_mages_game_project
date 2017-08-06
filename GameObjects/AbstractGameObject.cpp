#include "AbstractGameObject.hpp"
#include "ObjectState.hpp"

AbstractGameObject::AbstractGameObject(RenderInfo * render_info, float mass, float size_h, 
									   float size_v, float position_h, float position_v)
	: m_render_info(render_info), m_state(new IndependentObjectState(mass, size_h, size_v, position_h, position_v)),
	m_waiting_to_be_initilized(true), m_waiting_to_be_destroyed(false) {}

vector AbstractGameObject::position() const {
	return m_state->position();
}
