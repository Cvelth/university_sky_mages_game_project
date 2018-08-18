#pragma once
#include <string>
namespace NetworkController {
	enum NetworkChannels { ImportantData = 0, ActorData = 1, ControlsData = 2, ProjectileData = 3 };
	enum class ControlEvent { move_up = 1, move_down = 2, move_left = 3, move_right = 4, anti_gravity = 5, left_weapon = 6, right_weapon = 7 };

	void send_control_event(ControlEvent ev, bool direction = false);
	std::pair<ControlEvent, bool> parse_control_event(std::string const& data);
}