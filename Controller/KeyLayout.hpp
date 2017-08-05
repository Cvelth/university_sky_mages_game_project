#pragma once
#include "Events\EventEnums.hpp"
struct KeyLayout {
	mgl::Key move_up;
	mgl::Key move_down;
	mgl::Key move_left;
	mgl::Key move_right;
};

#include <ios>
std::ostream& operator<<(std::ostream &s, mgl::Key &k);
std::istream& operator>>(std::istream &s, mgl::Key &k);
std::ostream& operator<<(std::ostream &s, KeyLayout &k); 
std::istream& operator>>(std::istream &s, KeyLayout &k);