#include "RenderInfoStorage.hpp"
#include "../Graphics/RenderInfo.hpp"
std::unordered_map<std::string, std::shared_ptr<RenderInfo>> RenderInfoStorage::m_data;
std::unordered_map<std::string, RenderInfoStorage::Palette> RenderInfoStorage::m_palettes;
bool RenderInfoStorage::wasRenderInfoLoaded;

void initialize_render_info() {
	RenderInfoStorage render_info_storage;
	render_info_storage.load();
}
void RenderInfoStorage::clean() {
	wasRenderInfoLoaded = false;
	m_data.clear();
}
void RenderInfoStorage::parse_file_type_info(std::string const& line) {
	if (line != " with RenderInfo")
		throw Exceptions::FileParsingException("File seems to be corrupted");

	m_current_render_info.clear();
}
#include <sstream>
enum Mode { empty_mode = 0u, object_mode = 1u, palette_mode = 2u, object_array_mode = 4u, palette_object_array_mode = 8u };
std::string name(std::string name, size_t index) {
	std::ostringstream s;
	s << name << '[' << index << ']';
	return s.str();
}
void RenderInfoStorage::finalize_object(size_t mode) {
	switch (m_current_mode) {
		case object_mode:
			if (!m_current_render_info.empty())
				m_data.insert(std::make_pair(m_current_name, m_current_render_info.front()));
			break;
		case palette_mode:
			m_palettes.insert(std::make_pair(m_current_name, m_current_palette));
			break;
		case object_array_mode:
			if (!m_current_render_info.empty())
				m_data.insert(std::make_pair(name(m_current_name, m_current_index++), m_current_render_info.front()));
			break;
		case palette_object_array_mode:
			for (size_t i = 0; i < m_current_render_info.size(); i++)
				m_data.insert(std::make_pair(name(m_current_name, i), m_current_render_info.at(i)));
		case empty_mode:
			break;
		default:
			throw Exceptions::FileParsingException("Unsupported object mode was encountered while parsing the file");
	}
	m_current_mode = mode;
}
bool RenderInfoStorage::parse_special_line(std::string const & line) {
	switch (m_current_mode) {
		case object_mode:
			parse_object_line(line);
			return true;
		case palette_mode:
			parse_palette_line(line);
			return true;
		case object_array_mode:
			parse_object_array_line(line);
			return true;
		case palette_object_array_mode:
			parse_palette_object_array_line(line);
			return true;
		default:
			return false;
	}
}
void RenderInfoStorage::parse_line(std::string const& line) {
	std::istringstream s(line);
	std::string string;
	s >> string;
	if (string == "Object") {
		finalize_object(object_mode);
		s >> m_current_name;
		m_current_render_info.clear();
		m_current_render_info.push_back(std::make_shared<RenderInfo>());
	} else if (string == "Palette") {
		finalize_object(palette_mode);
		s >> m_current_name;
		m_current_palette.clear();
	} else if (string == "ObjectArray") {
		finalize_object(object_array_mode);
		s >> m_current_name;
		m_current_render_info.clear();
		m_current_render_info.push_back(std::make_shared<RenderInfo>());
		m_current_index = 0u;
	} else if (string == "PaletteObjectArray") {
		finalize_object(palette_object_array_mode);
		s >> m_current_name >> string;
		m_current_palette = getPalette(string);
		m_current_render_info.clear();
		for (auto &it : m_current_palette)
			m_current_render_info.push_back(std::make_shared<RenderInfo>());
	} else if (!parse_special_line(line))
		throw Exceptions::FileParsingException("File seems to be corrupted");
}

#include "../MyGraphicsLibrary/mgl/Math/Vector.hpp"
#include "../MyGraphicsLibrary/mgl/SharedHeaders/Color.hpp"
RenderInfoStorage::RenderInfoStorage() : m_current_mode(0u), m_current_color(std::make_shared<mgl::Color>(0.f, 0.f, 0.f)), 
										 m_current_scale(std::make_shared<mgl::math::vectorH>(1.f, 1.f, 1.f, 1.f)) {}
std::shared_ptr<RenderInfo> RenderInfoStorage::getRenderInfo(std::string const& obj) {
	if (auto temp = m_data.find(obj); temp != m_data.end())
		return temp->second;
	else throw Exceptions::RenderInfoException("Unloaded RenderInfo was requested.");
}
std::shared_ptr<RenderInfo> RenderInfoStorage::getRenderInfo(std::string const& obj, size_t index) {
	std::ostringstream s;
	s << obj << '[' << index << ']';
	return getRenderInfo(s.str());
}
std::string RenderInfoStorage::getRenderInfo(std::shared_ptr<RenderInfo> inf) {
	auto res = std::find_if(m_data.begin(), m_data.end(), [inf](std::pair<std::string, std::shared_ptr<RenderInfo>> it) {
		return it.second == inf;
	});
	if (res != m_data.end())
		return res->first;
	else
		throw Exceptions::RenderInfoException("Unloaded RenderInfo was requested.");
}
RenderInfoStorage::Palette& RenderInfoStorage::getPalette(std::string const& obj) {
	if (auto temp = m_palettes.find(obj); temp != m_palettes.end())
		return temp->second;
	else throw Exceptions::RenderInfoException("Unloaded Palette was requested.");
}
std::string RenderInfoStorage::getPalette(Palette &inf) {
	auto res = std::find_if(m_palettes.begin(), m_palettes.end(), [inf](std::pair<std::string, Palette> it) {
		return it.second == inf;
	});
	if (res != m_palettes.end())
		return res->first;
	else
		throw Exceptions::RenderInfoException("Unloaded RenderInfo was requested.");
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
void RenderInfoStorage::parse_object_line(std::string const& line, size_t index) {
	if (m_current_render_info.empty()) throw Exceptions::FileParsingException("File seems to be corrupted.");
	std::istringstream iss(line);
	std::string s;
	char placeholder;
	iss >> s;
	if (s == "Color") {
		float r, g, b, a = 1.f;
		iss >> r >> placeholder >> g >> placeholder >> b;
		if (!iss.eof()) 
			iss >> placeholder >> a;
		m_current_color = std::make_shared<mgl::Color>(r, g, b, a);
	} else if (s == "Scale") {
		float x, y, z, w = 1.f;
		iss >> x >> placeholder >> y >> placeholder >> z;
		if (!iss.eof()) 
			iss >> placeholder >> w;
		m_current_scale = std::make_shared<mgl::math::vectorH>(x, y, z, w);
	} else if (s == "Rectangle") {
		float aspect_ratio;
		std::string filled;
		iss >> aspect_ratio >> placeholder >> s >> filled;
		auto primitive = mgl::generateRectangle(aspect_ratio, &*m_current_color, placing_switch(s), filling_switch(filled));
		*primitive *= *m_current_scale;
		m_current_render_info.at(index)->addPrimitive(primitive);
	}
	else if (s == "Ellipse") {
		float aspect_ratio;
		std::string filled;
		size_t vertices;
		iss >> aspect_ratio >> placeholder >> vertices >> placeholder >> s >> filled;
		auto primitive = mgl::generateEllipse(aspect_ratio, vertices, &*m_current_color, placing_switch(s), filling_switch(filled));
		*primitive *= *m_current_scale;
		m_current_render_info.at(index)->addPrimitive(primitive);
	} else
		throw Exceptions::FileParsingException(("Unsupported RenderStorage line was encountered:\n" + line).c_str());
}

void RenderInfoStorage::parse_palette_line(std::string const & line) {
	std::istringstream iss(line);
	std::string s;
	char placeholder;
	iss >> s;
	if (s == "Color") {
		float r, g, b, a = 1.f;
		iss >> r >> placeholder >> g >> placeholder >> b;
		if (!iss.eof())
			iss >> placeholder >> a;
		m_current_palette.push_back(std::make_shared<mgl::Color>(r, g, b, a));
	} else
		throw Exceptions::FileParsingException(("Unsupported RenderStorage line was encountered while parsing palette '" + m_current_name + "':\n" + line).c_str());
}

void RenderInfoStorage::parse_object_array_line(std::string const& line) {
	if (line == "<>") {
		finalize_object(object_array_mode);
		m_current_render_info.clear();
		m_current_render_info.push_back(std::make_shared<RenderInfo>());
	} else
		return parse_object_line(line);
}

void RenderInfoStorage::parse_palette_object_array_line(std::string const& line) {
	size_t index = 0u;
	for (auto &it : m_current_palette) {
		m_current_color = it;
		parse_object_line(line, index++);
	}
}
