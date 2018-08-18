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
