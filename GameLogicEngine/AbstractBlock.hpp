#pragma once
class AbstractBlock {
private:
	float m_speedMultiplier;
public:
	AbstractBlock(float multiplier) : m_speedMultiplier(multiplier) {};
	float get() {
		return m_speedMultiplier;
	}
};

class EmptyBlock : public AbstractBlock {
public:
	EmptyBlock() : AbstractBlock(1.f) {};
};
class WallBlock : public AbstractBlock {
public:
	WallBlock() : AbstractBlock(0.f) {};
};