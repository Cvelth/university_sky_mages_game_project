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

	KeyLayout(const KeyLayout &other);
	KeyLayout(const size_t up, const size_t down, const size_t left, const size_t right, const size_t antiGravity);
	KeyLayout();
	~KeyLayout();
};

#include <ios>
std::ostream& operator<<(std::ostream &s, mgl::Key *k);
std::istream& operator>>(std::istream &s, mgl::Key *k);
std::ostream& operator<<(std::ostream &s, KeyLayout &k); 
std::istream& operator>>(std::istream &s, KeyLayout &k);