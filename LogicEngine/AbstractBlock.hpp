#pragma once

class RenderInfo;
class AbstractBlock {
private:
	float m_speedMultiplier;
	RenderInfo* m_renderInfo;
public:
	AbstractBlock(float multiplier, RenderInfo* renderInfo) : m_speedMultiplier(multiplier), m_renderInfo(renderInfo) {};
	inline float get() { return m_speedMultiplier; }
	inline RenderInfo* renderInfo() { return m_renderInfo; }
	inline bool operator==(AbstractBlock const& b) {
		return m_renderInfo == b.m_renderInfo;
	}
};

class EmptyBlock : public AbstractBlock {
public:
	EmptyBlock();
};
class WallBlock : public AbstractBlock {
public:
	WallBlock(); 
};