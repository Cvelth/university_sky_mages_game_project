#pragma once
#include <string>
class MainActor;
class MainActorQueue;
class ProjectileQueue;
class ObjectQueue;
namespace NetworkController {
	enum NetworkChannels { ImportantData = 0, ActorData = 1, ControlsData = 2, ProjectileData = 3, OtherData = 4 };
	enum class ControlEvent { move_up = 1, move_down = 2, move_left = 3, move_right = 4, anti_gravity = 5, left_weapon = 6, right_weapon = 7 };

	void send_control_event(ControlEvent ev, bool direction = false);
	void send_aim_event(float x, float y);

	std::pair<ControlEvent, bool> parse_control_event(std::string const& data);
	std::pair<float, float> parse_aim_event(std::string const& data);

	void accept_control_event(std::shared_ptr<MainActor> actor, ControlEvent ev, bool direction = false);
	void accept_aim_event(std::shared_ptr<MainActor> actor, float x, float y);

	inline void accept_control_event(std::shared_ptr<MainActor> actor, std::pair<ControlEvent, bool> ev) { return accept_control_event(actor, ev.first, ev.second); }
	inline void accept_control_event(std::shared_ptr<MainActor> actor, std::string const& data) { return accept_control_event(actor, parse_control_event(data)); }

	inline void accept_aim_event(std::shared_ptr<MainActor> actor, std::pair<float, float> ev) { return accept_aim_event(actor, ev.first, ev.second); }
	inline void accept_aim_event(std::shared_ptr<MainActor> actor, std::string const& data) { return accept_aim_event(actor, parse_aim_event(data)); }

	inline void send_control_event(std::shared_ptr<MainActor> actor, ControlEvent ev, bool direction = false) {
		accept_control_event(actor, ev, direction);
		send_control_event(ev, direction);
	}
	inline void send_aim_event(std::shared_ptr<MainActor> actor, float x, float y) {
		accept_aim_event(actor, x, y);
		send_aim_event(x, y);
	}

	void update_state(MainActorQueue *actors, ProjectileQueue *projectiles, ObjectQueue *miscellaneous);
	void update_state(std::string data, MainActorQueue *actors, ProjectileQueue *projectiles, ObjectQueue *miscellaneous);
}