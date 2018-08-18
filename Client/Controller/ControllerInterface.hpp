#pragma once
#include <memory>
class Controller;
class Camera;
class MainActor;
struct KeyLayout;

class ControllerInterface {
	Controller* m_controller;
public:
	ControllerInterface();
	~ControllerInterface();

	inline Controller* controller() {
		return m_controller;
	}
	inline Controller* operator*() {
		return controller();
	}

	void startCameraControl(std::shared_ptr<Camera> camera);
	void stopCameraControl();

	void setMainActor(std::shared_ptr<MainActor> actor);
	void removeMainActor();

	void startKeyControl(KeyLayout const *keys);
	void stopKeyControl();
};

