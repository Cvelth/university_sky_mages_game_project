#pragma once
#include "Graphics\RenderInfo\GetRenderInfo.hpp"

class RenderInfo;
class AbstractBlock {
private:
	float m_speedMultiplier;
	RenderInfo* m_renderInfo;
public:
	AbstractBlock(float multiplier, RenderInfo* renderInfo) : m_speedMultiplier(multiplier), m_renderInfo(renderInfo) {};
	float get() {
		return m_speedMultiplier;
	}
	RenderInfo* renderInfo() { return m_renderInfo; }
};

class EmptyBlock : public AbstractBlock {
public:
	EmptyBlock(RenderInfoStorage* ri) : AbstractBlock(1.f, ri->getEmptyBlockRenderInfo()) {};
};
class WallBlock : public AbstractBlock {
public:
	WallBlock(RenderInfoStorage* ri) : AbstractBlock(0.f, ri->getWallBlockRenderInfo()) {};
};