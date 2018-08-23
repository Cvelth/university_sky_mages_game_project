#include "NetworkController.hpp"
#include "Networking.hpp"
#include "Message.hpp"
#include "Shared/GameStateController.hpp"

void NetworkController::send_control_event(ControlEvent ev, bool direction) {
	if (GameStateController::mode() != ProgramMode::Client)
		throw Exceptions::GameStateException("Only client-mode applications can send control events.");
	Networking::send_to_server(make_control_event_message(ev, direction));
}
void NetworkController::send_aim_event(float x, float y) {
	if (GameStateController::mode() != ProgramMode::Client)
		throw Exceptions::GameStateException("Only client-mode applications can send control events.");
	Networking::send_to_server(make_aim_event_message(x, y));
}

#include "Objects/Actors/MainActor.hpp"
#include "Objects/EquipableItems/FlyEngine.hpp"
#include "Objects/EquipableItems/Weapon.hpp"
#include "Objects/EquipableItems/Shield.hpp"
void NetworkController::accept_control_event(std::shared_ptr<MainActor> actor, ControlEvent ev, bool direction) {
	switch (ev) {
		case ControlEvent::move_up:
			if (actor->fly_engine())
				if (direction)
					actor->fly_engine()->accelerate_up(false);
				else
					actor->fly_engine()->accelerate_up(true);
			break;
		case ControlEvent::move_down:
			if (actor->fly_engine())
				if (direction)
					actor->fly_engine()->accelerate_down(false);
				else
					actor->fly_engine()->accelerate_down(true);
			break;
		case ControlEvent::move_left:
			if (actor->fly_engine())
				if (direction)
					actor->fly_engine()->accelerate_left(false);
				else
					actor->fly_engine()->accelerate_left(true);
			break;
		case ControlEvent::move_right:
			if (actor->fly_engine())
				if (direction)
					actor->fly_engine()->accelerate_right(false);
				else
					actor->fly_engine()->accelerate_right(true);
			break;
		case ControlEvent::anti_gravity:
			if (actor->fly_engine())
				actor->fly_engine()->activate_anti_gravity(actor->mass());
			break;
		case ControlEvent::left_weapon:
			if (direction)
				actor->activateLeftWeapon();
			else
				actor->deactivateLeftWeapon();
			break;
		case ControlEvent::right_weapon:
			if (direction)
				actor->activateRightWeapon();
			else
				actor->deactivateRightWeapon();
			break;
		case ControlEvent::shield:
			if (actor->shield()) {
				if (actor->shield()->is_active())
					actor->shield()->deactivate();
				else
					actor->shield()->activate();
			}
			break;
	}
}
void NetworkController::accept_aim_event(std::shared_ptr<MainActor> actor, float x, float y) {
	actor->aim(x, y);
}