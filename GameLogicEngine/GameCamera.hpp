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

	bool m_wasCameraChanged;
protected:
	bool check();
	void correct();
public:
	GameCamera(GameMap* map, float aspectRatio, float blocks = 100);

	void changeAspectRatio(float aspectRatio);
	void changeZoom(float magnifier);
	void move(float x, float y);

	float minX_f() const;
	float minY_f() const;
	float maxX_f() const;
	float maxY_f() const;

	unsigned int minX_i() const;
	unsigned int minY_i() const;
	unsigned int maxX_i() const;
	unsigned int maxY_i() const;
	GameMap* map();

	inline bool wasCameraChanged() const {
		return m_wasCameraChanged;
	}
	inline void cameraChangeWasHandled() {
		m_wasCameraChanged = false;
	}

	~GameCamera();
};

