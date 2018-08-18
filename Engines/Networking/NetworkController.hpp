#pragma once
#include <string>
class MainActor;
namespace NetworkController {
	enum NetworkChannels { ImportantData = 0, ActorData = 1, ControlsData = 2, ProjectileData = 3 };
	enum class ControlEvent { move_up = 1, move_down = 2, move_left = 3, move_right = 4, anti_gravity = 5, left_weapon = 6, right_weapon = 7 };

	void send_control_event(ControlEvent ev, bool direction = false);
	std::pair<ControlEvent, bool> parse_control_event(std::string const& data);
	void accept_control_event(MainActor *actor, ControlEvent ev, bool direction = false);
	inline void accept_control_event(MainActor *actor, std::pair<ControlEvent, bool> ev) { return accept_control_event(actor, ev.first, ev.second); }
	inline void accept_control_event(MainActor *actor, std::string const& data) { return accept_control_event(actor, parse_control_event(data)); }

	inline void send_control_event(MainActor *actor, ControlEvent ev, bool direction = false) {
		accept_control_event(actor, ev, direction);
		send_control_event(ev, direction);
	}
}