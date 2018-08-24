#pragma once
#include <string>
class Block {
private:
	float m_speedMultiplier;
	std::string m_renderInfo;
public:
	Block(float multiplier, std::string const& renderInfo) : m_speedMultiplier(multiplier), m_renderInfo(renderInfo) {};
	inline float get() { return m_speedMultiplier; }
	inline std::string const& renderInfo() { return m_renderInfo; }
	inline bool operator==(Block const& b) {
		return m_speedMultiplier == b.m_speedMultiplier && m_renderInfo == b.m_renderInfo;
	}
};

class EmptyBlock : public Block {
public:
	EmptyBlock() : Block(1.f, "EmptyBlock") {}
};
class WallBlock : public Block {
public:
	WallBlock() : Block(0.f, "WallBlock") {}
};