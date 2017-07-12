#pragma once
class AbstractBlock {
private:
	float speedMultiplier;
public:
	AbstractBlock(float multiplier) : speedMultiplier(multiplier) {};
	float get() {
		return speedMultiplier;
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