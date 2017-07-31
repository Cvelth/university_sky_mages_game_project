#include "GameCamera.hpp"
#include "GameMap.hpp"
#include "..\MyOpenGL_Lib\Math\Vector.hpp"

bool GameCamera::check() {
	float x = m_corner->x();
	float y = m_corner->y();
	if (x < 0 || y < 0 || x > m_map->m_width || y > m_map->m_height)
		return false;
	if (x + m_horizontalBlocks > m_map->m_width)
		return false;
	if (y + m_horizontalBlocks * m_aspectRatio > m_map->m_height)
		return false;

	return true;
}

void GameCamera::correct() {
	float x = m_corner->x();
	float y = m_corner->y();
	if (x < 0) m_corner->x(0);
	if (y < 0) m_corner->y(0);
	if (x > m_map->m_width) m_corner->x(float(m_map->m_width));
	if (y > m_map->m_height) m_corner->y(float(m_map->m_height));

	if (x + m_horizontalBlocks > m_map->m_width) {
		m_horizontalBlocks = m_map->m_width;
		m_corner->x(0);
	}
	if (y + m_horizontalBlocks / m_aspectRatio > m_map->m_height) {
		m_horizontalBlocks = m_map->m_width * m_aspectRatio;
		m_corner->y(0);
	}
}

GameCamera::GameCamera(GameMap* map, float aspectRatio, float blocks) : m_corner(new mgl::math::Vector(0, 0)), m_map(map), m_aspectRatio(aspectRatio), m_horizontalBlocks(blocks) {
	correct();
}

void GameCamera::changeAspectRatio(float aspectRatio) {
	this->m_aspectRatio = aspectRatio;
	correct();
}

void GameCamera::changeZoom(float magnifier) {
	m_horizontalBlocks *= magnifier;
	correct();
}

void GameCamera::move(float x, float y) {
	*m_corner += mgl::math::Vector(x, y);
	correct();
}

float GameCamera::minX_f() const {
	return m_corner->x();
}								 
float GameCamera::minY_f() const {
	return m_corner->y();
}
float GameCamera::maxX_f() const {
	return m_corner->x() + m_horizontalBlocks;
}
float GameCamera::maxY_f() const {
	return m_corner->y() + m_horizontalBlocks / m_aspectRatio;
}
unsigned int GameCamera::minX_i() const {
	return (unsigned int) minX_f();
}
unsigned int GameCamera::minY_i() const {
	return (unsigned int) minY_f();
}
unsigned int GameCamera::maxX_i() const {
	return (unsigned int) maxX_f();
}
unsigned int GameCamera::maxY_i() const {
	return (unsigned int) maxY_f();
}

GameMap * GameCamera::map() {
	return m_map;
}

GameCamera::~GameCamera() {
	if (m_corner)
		delete m_corner;
}
