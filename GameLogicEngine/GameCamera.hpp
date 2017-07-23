#pragma once
namespace mgl {
	namespace math {
		class Vector;
	}
}
class GameMap;

class GameCamera {
private:
	mgl::math::Vector *m_corner;
	float m_aspectRatio;
	float m_horizontalBlocks;
	GameMap* m_map;
protected:
	bool check();
	void correct();
public:
	GameCamera(GameMap* map, float aspectRatio, float blocks = 50);

	void changeAspectRatio(float aspectRatio);
	void changeZoom(float magnifier);
	void move(float x, float y);

	int beginX() const;
	int beginY() const;
	int endX() const;
	int endY() const;
	GameMap* map();

	~GameCamera();
};

