#pragma once
namespace mgl {
	namespace math {
		class vector;
	}
}
class Map;
class Actor;
class Camera {
private:
	Map *m_map;

	mgl::math::vector *m_center;
	Actor *m_center_figure;

	float m_horizontal_blocks_number;
	float m_aspect_ratio;

	bool m_camera_was_changed;
protected:
	void correct_height();
	void correct_width();
	void correct();
public:
	Camera(Map *map, Actor *center_figure, float aspectRatio, float blocks = 100);

	void changeAspectRatio(float aspectRatio);
	void changeZoom(float magnifier);
	void move_to(float x, float y);
	void move_to(mgl::math::vector const& point);
	void move(float x, float y);
	void move(mgl::math::vector const& point);
	void move();
	void changeCenterFigure(Actor *center_figure);

	inline float aspectRatio() const {
		return m_aspect_ratio;
	}

	float minX() const;
	float minY() const;
	float maxX() const;
	float maxY() const;

	long long minX_i() const;
	long long minY_i() const;
	long long maxX_i() const;
	long long maxY_i() const;
	Map* map();

	inline bool wasCameraChanged() const {
		return m_camera_was_changed;
	}
	inline void cameraChangeWasHandled() {
		m_camera_was_changed = false;
	}

	~Camera();
};

