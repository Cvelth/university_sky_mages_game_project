#pragma once
class Point;
class GameMap;

class GameCamera {
private:
	Point *corner;
	float aspectRatio;
	float horizontalBlocks;
	GameMap* map;
protected:
	bool check();
	void correct();
public:
	GameCamera(GameMap* map, float aspectRatio, float blocks = 50);

	void changeAspectRatio(float aspectRatio);
	void changeZoom(float magnifier);
	void move(float x, float y);

	~GameCamera();
};

