#include "RenderInfoStorage.hpp"
#include "../RenderTools/RenderInfo.hpp"
void RenderInfoStorage::clean() {
	wasRenderInfoLoaded = false;
	for (auto &it : m_data)
		delete it.second;
	m_data.clear();
}

#include <sstream>
void RenderInfoStorage::parse_file_type_info(std::string const& line) {
	std::istringstream s(line);
	std::string string;
	s >> string;
	if (string != "RenderInfo")
		throw Exceptions::FileParsingException("File seems to be corrupted");

	m_current_render_info = nullptr;
}
void RenderInfoStorage::parse_line(std::string const& line) {
	std::istringstream s(line);
	std::string string;
	s >> string;
	if (string == "Object") {
		s >> string;
		if (m_current_render_info)
			m_data.insert(std::make_pair(string, m_current_render_info));
		m_current_render_info = new RenderInfo();
	} else if (m_current_render_info) {
		return parse_object_line(line);
	} else throw Exceptions::FileParsingException("File seems to be corrupted");
}

#include "../MyGraphicsLibrary/mgl/Math/Vector.hpp"
#include "../MyGraphicsLibrary/mgl/SharedHeaders/Color.hpp"
RenderInfoStorage::RenderInfoStorage() : m_current_render_info(nullptr), 
	m_current_color(new mgl::Color(0.f, 0.f, 0.f)), m_current_scale(new mgl::math::vectorH(1.f, 1.f, 1.f, 1.f)) {}
RenderInfoStorage::~RenderInfoStorage() {
	delete m_current_render_info;
	delete m_current_color;
	delete m_current_scale;
}

#include "../MyGraphicsLibrary/MGL/Primitive/AbstractDrawableObject.hpp"
#include "../MyGraphicsLibrary/MGL/Primitive/Primitive.hpp"
#include "../MyGraphicsLibrary/MGL/Default/DefaultPrimitives.hpp"
mgl::PoligonPlacing placing_switch(std::string const& value) {
	if (value == "zero_in_center") return mgl::PoligonPlacing::zero_in_center;
	else if (value == "zero_in_top_left") return mgl::PoligonPlacing::zero_in_top_left;
	else if (value == "zero_in_top_right") return mgl::PoligonPlacing::zero_in_top_right;
	else if (value == "zero_in_bottom_left") return mgl::PoligonPlacing::zero_in_bottom_left;
	else if (value == "zero_in_bottom_right") return mgl::PoligonPlacing::zero_in_bottom_right;
	else throw Exceptions::FileParsingException("Unsupported poligon placing type was encountered.");
}
bool filling_switch(std::string const& value) {
	if (value == "filled") return true; else return false;
}
void RenderInfoStorage::parse_object_line(std::string const& line) {
	std::istringstream iss(line);
	std::string s;
	iss >> s;
	if (s == "Color") {
		float r, g, b, a;
		iss >> r >> g >> b;
		if (iss) {
			iss >> a;
			delete m_current_color;
			m_current_color = new mgl::Color(r, g, b, a);
		} else {
			delete m_current_color;
			m_current_color = new mgl::Color(r, g, b);
		}
	} else if (s == "Scale") {
		float x, y, z, w;
		iss >> x >> y >> z;
		if (iss) {
			iss >> w;
			delete m_current_scale;
			m_current_scale = new mgl::math::vectorH(x, y, z, w);
		} else {
			delete m_current_scale;
			m_current_scale = new mgl::math::vectorH(x, y, z, 1.f);
		}
	} else if (s == "Rectangle") {
		float aspect_ratio;
		std::string filled;
		iss >> aspect_ratio >> s >> filled;
		m_current_render_info->get()->addPrimitive(mgl::generateRectangle(aspect_ratio, m_current_color, placing_switch(s), filling_switch(filled)));
	}
	else if (s == "Ellipse") {
		float aspect_ratio;
		std::string filled;
		size_t vertices;
		iss >> aspect_ratio >> vertices >> s >> filled;
		m_current_render_info->get()->addPrimitive(mgl::generateEllipse(aspect_ratio, vertices, m_current_color, placing_switch(s), filling_switch(filled)));
	} else
		throw Exceptions::FileParsingException(("Unsupported RenderStorage line was encountered:\n" + line).c_str());
}