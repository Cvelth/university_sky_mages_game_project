#pragma once
namespace mgl {
	enum class Key;
}
struct KeyLayout {
	mgl::Key *move_up;
	mgl::Key *move_down;
	mgl::Key *move_left;
	mgl::Key *move_right;
	mgl::Key *anti_gravity;
	mgl::Key *shield;

	bool hold_anti_gravity_key;
	bool hold_shield_key;

	mgl::Key *pause;

	KeyLayout(const KeyLayout &other);
	KeyLayout(const size_t _move_up, const size_t _move_down, const size_t _move_left, const size_t _move_right,
			  const size_t _anti_gravity, const size_t _shield, const size_t _pause, 
			  const bool _hold_anti_gravity_key, const bool _hold_shield_key);
	KeyLayout();
	~KeyLayout();
};

#include <ios>
std::ostream& operator<<(std::ostream &s, mgl::Key *k);
std::istream& operator>>(std::istream &s, mgl::Key *k);
std::ostream& operator<<(std::ostream &s, KeyLayout &k); 
std::istream& operator>>(std::istream &s, KeyLayout &k);

#include "Shared\AbstractException.hpp"
DefineNewException(KeyParsingException)