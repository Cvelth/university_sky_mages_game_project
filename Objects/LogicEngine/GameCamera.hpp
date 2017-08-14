#pragma once
namespace mgl {
	namespace math {
		class vector;
	}
}
class GameMap;
class AbstractActor;

class GameCamera {
private:
	GameMap *m_map;

	mgl::math::vector *m_center;
	AbstractActor *m_center_figure;

	float m_horizontal_blocks_number;
	float m_aspect_ratio;

	bool m_camera_was_changed;
protected:
	void correct();
public:
	GameCamera(GameMap *map, AbstractActor *center_figure, float aspectRatio, float blocks = 100);

	void changeAspectRatio(float aspectRatio);
	void changeZoom(float magnifier);
	void move_to(float x, float y);
	void move_to(mgl::math::vector const& point);
	void move(float x, float y);
	void move(mgl::math::vector const& point);
	void move();
	void changeCenterFigure(AbstractActor *center_figure);

	inline float aspectRatio() const {
		return m_aspect_ratio;
	}

	float minX() const;
	float minY() const;
	float maxX() const;
	float maxY() const;

	unsigned int minX_i() const;
	unsigned int minY_i() const;
	unsigned int maxX_i() const;
	unsigned int maxY_i() const;
	GameMap* map();

	inline bool wasCameraChanged() const {
		return m_camera_was_changed;
	}
	inline void cameraChangeWasHandled() {
		m_camera_was_changed = false;
	}

	~GameCamera();
};

