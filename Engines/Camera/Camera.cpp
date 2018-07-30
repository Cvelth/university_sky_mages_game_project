#include "Camera.hpp"
#include "Objects/AbstractObjects/ActorObject.hpp"
#include "Objects/Map/Map.hpp"
#include "Shared/vector.hpp"

void Camera::correct() {
	if (minX() < 0) {
		m_center->at(0) = (m_horizontal_blocks_number / 2.f);
		m_camera_was_changed = true;
	}
	if (minY() < 0) {
		m_center->at(1) = (m_horizontal_blocks_number / m_aspect_ratio / 2.f);
		m_camera_was_changed = true;
	}
	if (maxX() > m_map->m_width) {
		m_center->at(0) = (m_map->m_width - m_horizontal_blocks_number) / 2.f;// -1.f;
		m_camera_was_changed = true;
	}
	if (maxY() > m_map->m_height) {
		m_center->at(1) = (m_map->m_height - m_horizontal_blocks_number / m_aspect_ratio) / 2.f;// -1.f;
		m_camera_was_changed = true;
	}

	if (m_horizontal_blocks_number >= m_map->m_width) {
		m_horizontal_blocks_number = float(m_map->m_width) - 1.f;
		m_center->at(0) = m_horizontal_blocks_number / 2.f;
		m_camera_was_changed = true;
	}
	if (int(m_horizontal_blocks_number / m_aspect_ratio) + 1 >= int(m_map->m_height)) {
		m_horizontal_blocks_number = float(m_map->m_height) * m_aspect_ratio - 1;
		m_center->at(1) = m_horizontal_blocks_number / m_aspect_ratio / 2.f;
		m_camera_was_changed = true;
	}
}

Camera::Camera(Map *map, Actor *center_figure, 
					   float aspectRatio, float blocks)
	: m_map(map), m_aspect_ratio(aspectRatio), m_horizontal_blocks_number(blocks), 
	m_center_figure(center_figure), m_camera_was_changed(true) {

	m_center = new mgl::math::vector();
	if (m_center_figure)
		*m_center = mgl::math::vector(m_center_figure->position().at(0), m_center_figure->position().at(1));
	else
		*m_center = mgl::math::vector(m_horizontal_blocks_number / 2.f, m_horizontal_blocks_number / m_aspect_ratio / 2.f);
	correct();
}

void Camera::changeAspectRatio(float aspectRatio) {
	this->m_aspect_ratio = aspectRatio;
	m_camera_was_changed = true;
	correct();
}

void Camera::changeZoom(float magnifier) {
	if (magnifier > 0.f) {
		m_horizontal_blocks_number *= magnifier;
		m_camera_was_changed = true;
		correct();
	}
}

void Camera::move_to(float x, float y) {
	move_to(mgl::math::vector(x,y));
}

void Camera::move_to(mgl::math::vector const& point) {
	*m_center = point;
	m_camera_was_changed = true;
	correct();
}

void Camera::move(float x, float y) {
	move(mgl::math::vector(x, y));
}

void Camera::move(mgl::math::vector const& point) {
	*m_center += point;
	m_camera_was_changed = true;
	correct();
}

void Camera::changeCenterFigure(Actor *center_figure) {
	m_center_figure = center_figure;
}

float Camera::minX() const {
	return m_center->at(0) - m_horizontal_blocks_number / 2.f;
}
float Camera::minY() const {
	return m_center->at(1) - m_horizontal_blocks_number / m_aspect_ratio / 2.f;
}
float Camera::maxX() const {
	return m_center->at(0) + m_horizontal_blocks_number / 2.f;
}
float Camera::maxY() const {
	return m_center->at(1) + m_horizontal_blocks_number / m_aspect_ratio / 2.f;
}
size_t Camera::minX_i() const {
	if (float t = minX(); t <= 0.f)
		return 0.f;
	else
		return t;
}
size_t Camera::minY_i() const {
	if (float t = minY(); t <= 0.f)
		return 0.f;
	else
		return t;
}
size_t Camera::maxX_i() const {
	float temp = maxX();
	size_t ret = temp;
	if (temp != float(ret))
		ret++;
	if (ret == m_map->width())
		ret--;
	return ret;
}
size_t Camera::maxY_i() const {
	float temp = maxY();
	size_t ret = temp;
	if (temp != float(ret))
		ret++;
	if (ret == m_map->height())
		ret--;
	return ret;
}

Map* Camera::map() {
	return m_map;
}

Camera::~Camera() {
	if (m_center)
		delete m_center;
}

void Camera::move() {
	if (m_center_figure)
		move_to(m_center_figure->position().at(0), m_center_figure->position().at(1));
	correct();
}