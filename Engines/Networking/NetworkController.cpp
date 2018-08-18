#include "NetworkController.hpp"
#include "Networking.hpp"

void NetworkController::send_control_event(ControlEvent ev, bool direction) {
	std::string data("C x");
	data[2] = char(ev);
	if (direction) data[2] |= (1 << 7);
	Networking::send_to_server(data, ControlsData, true);
}

std::pair<NetworkController::ControlEvent, bool> NetworkController::parse_control_event(std::string const& data) {
	if (data.size() != 3 || data[0] != 'C') throw Exceptions::NetworkingException("Control data seems to be corrupted.");
	return std::make_pair(ControlEvent(data[2] & ((1 << 7) - 1)), data[2] & (1 << 7));
}

#include "Objects/Actors/MainActor.hpp"
#include "Objects/EquipableItems/FlyEngine.hpp"
#include "Objects/EquipableItems/Weapon.hpp"
void NetworkController::accept_control_event(MainActor *actor, ControlEvent ev, bool direction) {
	switch (ev) {
		case NetworkController::ControlEvent::move_up:
			if (direction)
				actor->fly_engine()->accelerate_up(false);
			else
				actor->fly_engine()->accelerate_up(true);
			break;
		case NetworkController::ControlEvent::move_down:
			if (direction)
				actor->fly_engine()->accelerate_down(false);
			else
				actor->fly_engine()->accelerate_down(true);
			break;
		case NetworkController::ControlEvent::move_left:
			if (direction)
				actor->fly_engine()->accelerate_left(false);
			else
				actor->fly_engine()->accelerate_left(true);
			break;
		case NetworkController::ControlEvent::move_right:
			if (direction)
				actor->fly_engine()->accelerate_right(false);
			else
				actor->fly_engine()->accelerate_right(true);
			break;
		case NetworkController::ControlEvent::anti_gravity:
			actor->fly_engine()->activate_anti_gravity(actor->mass());
			break;
		case NetworkController::ControlEvent::left_weapon:
			if (direction)
				actor->activateLeftWeapon();
			else
				actor->deactivateLeftWeapon();
			break;
		case NetworkController::ControlEvent::right_weapon:
			if (direction)
				actor->activateRightWeapon();
			else
				actor->deactivateRightWeapon();
			break;
	}
}