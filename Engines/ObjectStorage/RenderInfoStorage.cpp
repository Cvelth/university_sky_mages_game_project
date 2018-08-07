#include "RenderInfoStorage.hpp"
#include "../Graphics/RenderInfo.hpp"
std::unordered_map<std::string, RenderInfo*> RenderInfoStorage::m_data;
bool RenderInfoStorage::wasRenderInfoLoaded;

void initialize_render_info() {
	RenderInfoStorage render_info_storage;
	render_info_storage.load();
}
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
	s >> string >> string;
	if (string != "RenderInfo")
		throw Exceptions::FileParsingException("File seems to be corrupted");

	m_current_render_info = nullptr;
}
void RenderInfoStorage::parse_line(std::string const& line) {
	std::istringstream s(line);
	std::string string;
	s >> string;
	if (string == "Object") {
		if (m_current_render_info)
			m_data.insert(std::make_pair(m_current_name, m_current_render_info));
		s >> m_current_name;
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
RenderInfo* RenderInfoStorage::getRenderInfo(std::string const& obj) {
	check();
	if (auto temp = m_data.find(obj); temp != m_data.end())
		return m_data[obj];
	else throw Exceptions::RenderInfoException("Unloaded RenderInfo was requested.");
}

#include "../MyGraphicsLibrary/MGL/Primitive/AbstractDrawableObject.hpp"
#include "../MyGraphicsLibrary/MGL/Primitive/Primitive.hpp"
#include "../MyGraphicsLibrary/MGL/Default/DefaultPrimitives.hpp"
mgl::PoligonPlacing placing_switch(std::string const& value) {
	if (value == "zero_in_center,") return mgl::PoligonPlacing::zero_in_center;
	else if (value == "zero_in_top_left,") return mgl::PoligonPlacing::zero_in_top_left;
	else if (value == "zero_in_top_right,") return mgl::PoligonPlacing::zero_in_top_right;
	else if (value == "zero_in_bottom_left,") return mgl::PoligonPlacing::zero_in_bottom_left;
	else if (value == "zero_in_bottom_right,") return mgl::PoligonPlacing::zero_in_bottom_right;
	else throw Exceptions::FileParsingException("Unsupported poligon placing type was encountered.");
}
bool filling_switch(std::string const& value) {
	if (value == "filled") return true; else return false;
}
void RenderInfoStorage::parse_object_line(std::string const& line) {
	if (!m_current_render_info) throw Exceptions::FileParsingException("File seems to be corrupted.");
	std::istringstream iss(line);
	std::string s;
	char placeholder;
	iss >> s;
	if (s == "Color") {
		float r, g, b, a = 1.f;
		iss >> r >> placeholder >> g >> placeholder >> b;
		if (!iss.eof()) 
			iss >> placeholder >> a;
		delete m_current_color;
		m_current_color = new mgl::Color(r, g, b, a);
	} else if (s == "Scale") {
		float x, y, z, w = 1.f;
		iss >> x >> placeholder >> y >> placeholder >> z;
		if (!iss.eof()) 
			iss >> placeholder >> w;
		delete m_current_scale;
		m_current_scale = new mgl::math::vectorH(x, y, z, w);
	} else if (s == "Rectangle") {
		float aspect_ratio;
		std::string filled;
		iss >> aspect_ratio >> placeholder >> s >> filled;
		m_current_render_info->get()->addPrimitive(mgl::generateRectangle(aspect_ratio, m_current_color, placing_switch(s), filling_switch(filled)));
	}
	else if (s == "Ellipse") {
		float aspect_ratio;
		std::string filled;
		size_t vertices;
		iss >> aspect_ratio >> placeholder >> vertices >> placeholder >> s >> filled;
		m_current_render_info->get()->addPrimitive(mgl::generateEllipse(aspect_ratio, vertices, m_current_color, placing_switch(s), filling_switch(filled)));
	} else
		throw Exceptions::FileParsingException(("Unsupported RenderStorage line was encountered:\n" + line).c_str());
}