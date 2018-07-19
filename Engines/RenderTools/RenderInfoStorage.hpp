#pragma once
#include "Shared/AbstractException.hpp"
DefineNewException(RenderInfoException);
class RenderInfo;
class RenderInfoStorage {
private:
	static bool isRenderInfoGenerated;
protected:
	static RenderInfo* EmptyBlock;
	static RenderInfo* WallBlock;
	static RenderInfo* MainActor;
	static RenderInfo* AbstractBulletProjectile;
	static RenderInfo* AbstractPhysicalProjectile;
	static RenderInfo* AbstractEnergyProjectile;

public:
	static void generateRenderInfo();
	static void cleanRenderInfo();

	static inline void check() { if (!isRenderInfoGenerated) throw Exceptions::RenderInfoException("Render info was never generated, so it cannot be accessed. Call generateRenderInfo() first."); }
	static inline RenderInfo* getEmptyBlockRenderInfo() { check(); return EmptyBlock; }
	static inline RenderInfo* getWallBlockRenderInfo() { check(); return WallBlock; }
	static inline RenderInfo* getMainActorRenderInfo() { check(); return MainActor; }
	static inline RenderInfo* getAbstractBulletProjectileRenderInfo() { check(); return AbstractBulletProjectile; }
	static inline RenderInfo* getAbstractPhysicalProjectileRenderInfo() { check(); return AbstractPhysicalProjectile; }
	static inline RenderInfo* getAbstractEnergyProjectileRenderInfo() { check(); return AbstractPhysicalProjectile; }
};