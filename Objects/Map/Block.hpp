#pragma once

class RenderInfo;
class Block {
private:
	float m_speedMultiplier;
	RenderInfo* m_renderInfo;
public:
	Block(float multiplier, RenderInfo* renderInfo) : m_speedMultiplier(multiplier), m_renderInfo(renderInfo) {};
	inline float get() { return m_speedMultiplier; }
	inline RenderInfo* renderInfo() { return m_renderInfo; }
	inline bool operator==(Block const& b) {
		return m_renderInfo == b.m_renderInfo;
	}
};

class EmptyBlock : public Block {
public:
	EmptyBlock();
};
class WallBlock : public Block {
public:
	WallBlock();
};