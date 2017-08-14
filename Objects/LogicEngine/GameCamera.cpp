#include "GameCamera.hpp"
#include "GameMap.hpp"
#include "MGL\Math\Vector.hpp"
#include "Objects\GameObjects\AbstractActors.hpp"

void GameCamera::correct() {
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

GameCamera::GameCamera(GameMap *map, AbstractActor *center_figure, 
					   float aspectRatio, float blocks)
	: m_map(map), m_aspect_ratio(aspectRatio), m_horizontal_blocks_number(blocks), 
	m_center_figure(center_figure), m_camera_was_changed(true) {

	m_center = new mgl::math::vector();
	// [[deprecated]]
	//if (center_figure) {
	//	auto position = center_figure->position();
	//	*m_center = mgl::math::vector(position.h, position.v);
	//} else
		*m_center = mgl::math::vector(m_horizontal_blocks_number / 2.f, m_horizontal_blocks_number / m_aspect_ratio / 2.f);
	correct();
}

void GameCamera::changeAspectRatio(float aspectRatio) {
	this->m_aspect_ratio = aspectRatio;
	m_camera_was_changed = true;
	correct();
}

void GameCamera::changeZoom(float magnifier) {
	if (magnifier > 0.f) {
		m_horizontal_blocks_number *= magnifier;
		m_camera_was_changed = true;
		correct();
	}
}

void GameCamera::move_to(float x, float y) {
	move_to(mgl::math::vector(x,y));
}

void GameCamera::move_to(mgl::math::vector const& point) {
	*m_center = point;
	m_camera_was_changed = true;
	correct();
}

void GameCamera::move(float x, float y) {
	move(mgl::math::vector(x, y));
}

void GameCamera::move(mgl::math::vector const& point) {
	*m_center += point;
	m_camera_was_changed = true;
	correct();
}

void GameCamera::changeCenterFigure(AbstractActor * center_figure) {
	m_center_figure = center_figure;
}

float GameCamera::minX() const {
	return m_center->at(0) - m_horizontal_blocks_number / 2.f;
}								 
float GameCamera::minY() const {
	return m_center->at(1) - m_horizontal_blocks_number / m_aspect_ratio / 2.f;
}
float GameCamera::maxX() const {
	return m_center->at(0) + m_horizontal_blocks_number / 2.f;
}
float GameCamera::maxY() const {
	return m_center->at(1) + m_horizontal_blocks_number / m_aspect_ratio / 2.f;
}
unsigned int GameCamera::minX_i() const {
	return (unsigned int) minX();
}
unsigned int GameCamera::minY_i() const {
	return (unsigned int) minY();
}
unsigned int GameCamera::maxX_i() const {
	return (unsigned int) maxX();
}
unsigned int GameCamera::maxY_i() const {
	return (unsigned int) maxY();
}

GameMap * GameCamera::map() {
	return m_map;
}

GameCamera::~GameCamera() {
	if (m_center)
		delete m_center;
}

void GameCamera::move() {
	if (m_center_figure) {
		// [[deprecated]]
		//auto position = m_center_figure->position();
		//move_to(position.h, position.v);
	}
}