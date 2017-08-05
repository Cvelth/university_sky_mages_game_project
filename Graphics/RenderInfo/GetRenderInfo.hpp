#pragma once
#include "Shared\AbstractException.hpp"
DefineNewException(RenderInfoException);
class RenderInfo;
class RenderInfoStorage {
private:
	bool isRenderInfoGenerated = false;
protected:
	RenderInfo* EmptyBlock;
	RenderInfo* WallBlock;
	RenderInfo* MainActor;
public:
	~RenderInfoStorage() { cleanRenderInfo(); }
	void generateRenderInfo();
	void cleanRenderInfo();

	inline void check() { if (!isRenderInfoGenerated) throw Exceptions::RenderInfoException("Render info was never generated, so it cannot be accessed. Call generateRenderInfo() first."); }
	inline RenderInfo* getEmptyBlockRenderInfo() { check(); return EmptyBlock; }
	inline RenderInfo* getWallBlockRenderInfo() { check(); return WallBlock; }
	inline RenderInfo* getMainActorRenderInfo() { check(); return MainActor; }
};