#include "HUD_RenderInfo.hpp"
#include "Primitive\AbstractDrawableObject.hpp"
#include "Primitive\Primitive.hpp"
#include "SharedHeaders\Color.hpp"
#include "Math\Vector.hpp"

mgl::Primitive *outter, *inner, *midder;

HUD_RenderInfo::HUD_RenderInfo() {
	outter = new mgl::Primitive(mgl::VertexConnectionType::TriangleStrip, 
								new mgl::Color(0.9f, 0.9f, 0.9f));
	outter->insert(new mgl::math::Vector(0.975f, 0.01f));
	outter->insert(new mgl::math::Vector(0.995f, 0.01f));
	outter->insert(new mgl::math::Vector(0.975f, 0.99f));
	outter->insert(new mgl::math::Vector(0.995f, 0.99f));

	inner = new mgl::Primitive(mgl::VertexConnectionType::TriangleStrip,
							   new mgl::Color(0.1f, 0.1f, 0.1f));
	inner->insert(new mgl::math::Vector(0.978f, 0.015f));
	inner->insert(new mgl::math::Vector(0.992f, 0.015f));
	inner->insert(new mgl::math::Vector(0.978f, 0.985f));
	inner->insert(new mgl::math::Vector(0.992f, 0.985f));

	midder = new mgl::Primitive(mgl::VertexConnectionType::TriangleStrip,
								new mgl::Color(0.35f, 0.05f, 0.25f));
	midder->insert(new mgl::math::Vector(0.978f, 0.515f));
	midder->insert(new mgl::math::Vector(0.992f, 0.515f));
	midder->insert(new mgl::math::Vector(0.978f, 0.985f));
	midder->insert(new mgl::math::Vector(0.992f, 0.985f));

	get()->addPrimitive(outter);
	get()->addPrimitive(inner);
	get()->addPrimitive(midder);
}

HUD_RenderInfo::~HUD_RenderInfo() {}