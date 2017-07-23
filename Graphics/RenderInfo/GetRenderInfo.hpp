#pragma once
#include "Exceptions\AbstractExceptions.hpp"

namespace Exceptions {
	class RenderInfoException : public Exceptions::AbstractStringException {
	public:
		RenderInfoException(char* error) : AbstractStringException("RenderInfoException", error) {}
	};
}
class RenderInfo;
class RenderInfoStorage {
private:
	bool isRenderInfoGenerated = false;
protected:
	RenderInfo* EmptyBlock;
	RenderInfo* WallBlock;
public:
	~RenderInfoStorage() { cleanRenderInfo(); }
	void generateRenderInfo();
	void cleanRenderInfo();

	inline void check() { if (!isRenderInfoGenerated) throw Exceptions::RenderInfoException("Render info was never generated, so it cannot be accessed. Call generateRenderInfo() first."); }
	inline RenderInfo* getEmptyBlockRenderInfo() { check(); return EmptyBlock; }
	inline RenderInfo* getWallBlockRenderInfo() { check(); return WallBlock; }
};