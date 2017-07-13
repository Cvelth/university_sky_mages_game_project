#include "GameCamera.hpp"
#include "GameMap.hpp"
#include "Point.hpp"

bool GameCamera::check() {
	float x = corner->x();
	float y = corner->y();
	if (x < 0 || y < 0 || x > map->width || y > map->height)
		return false;
	if (x + horizontalBlocks > map->width)
		return false;
	if (y + horizontalBlocks * aspectRatio > map->height)
		return false;

	return true;
}

void GameCamera::correct() {
	float x = corner->x();
	float y = corner->y();
	if (x < 0) corner->x(0);
	if (y < 0) corner->y(0);
	if (x > map->width) corner->x(float(map->width));
	if (y > map->height) corner->y(float(map->height));

	if (x + horizontalBlocks > map->width)
		corner->x(map->width - horizontalBlocks);
	if (y + horizontalBlocks * aspectRatio > map->height)
		corner->y(map->height - horizontalBlocks * aspectRatio);
}

GameCamera::GameCamera(GameMap* map, float aspectRatio, float blocks) : corner(new Point(0, 0)), map(map), aspectRatio(aspectRatio), horizontalBlocks(blocks) {}

void GameCamera::changeAspectRatio(float aspectRatio) {
	this->aspectRatio = aspectRatio;
	correct();
}

void GameCamera::changeZoom(float magnifier) {
	horizontalBlocks *= magnifier;
	correct();
}

void GameCamera::move(float x, float y) {
	*corner += Point(x, y);
	correct();
}

GameCamera::~GameCamera() {
	if (corner)
		delete corner;
}
