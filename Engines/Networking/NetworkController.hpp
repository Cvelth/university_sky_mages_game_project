#pragma once
#include <string>
class MainActor;
class MainActorQueue;
class ProjectileQueue;
class ObjectQueue;
enum class ControlEvent { move_up = 1, move_down = 2, move_left = 3, move_right = 4, anti_gravity = 5, left_weapon = 6, right_weapon = 7 };
namespace NetworkController {

	void send_control_event(ControlEvent ev, bool direction = false);
	void send_aim_event(float x, float y);

	void accept_control_event(std::shared_ptr<MainActor> actor, ControlEvent ev, bool direction = false);
	void accept_aim_event(std::shared_ptr<MainActor> actor, float x, float y);

	inline void send_control_event(std::shared_ptr<MainActor> actor, ControlEvent ev, bool direction = false) {
		send_control_event(ev, direction);
		accept_control_event(actor, ev, direction);
	}
	inline void send_aim_event(std::shared_ptr<MainActor> actor, float x, float y) {
		send_aim_event(x, y);
		accept_aim_event(actor, x, y);
	}
}