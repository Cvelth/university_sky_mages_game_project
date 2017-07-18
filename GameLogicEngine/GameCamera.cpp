#include "GameCamera.hpp"
#include "GameMap.hpp"
#include "GeometryAndPhysics\Point.hpp"

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

	if (x + m_horizontalBlocks > m_map->m_width)
		m_corner->x(m_map->m_width - m_horizontalBlocks);
	if (y + m_horizontalBlocks * m_aspectRatio > m_map->m_height)
		m_corner->y(m_map->m_height - m_horizontalBlocks * m_aspectRatio);
}

GameCamera::GameCamera(GameMap* map, float aspectRatio, float blocks) : m_corner(new Point(0, 0)), m_map(map), m_aspectRatio(aspectRatio), m_horizontalBlocks(blocks) {}

void GameCamera::changeAspectRatio(float aspectRatio) {
	this->m_aspectRatio = aspectRatio;
	correct();
}

void GameCamera::changeZoom(float magnifier) {
	m_horizontalBlocks *= magnifier;
	correct();
}

void GameCamera::move(float x, float y) {
	*m_corner += Point(x, y);
	correct();
}

GameCamera::~GameCamera() {
	if (m_corner)
		delete m_corner;
}
