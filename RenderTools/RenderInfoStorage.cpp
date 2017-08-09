#include "RenderInfoStorage.hpp"
#include "RenderInfo.hpp"
#include "MGL\Primitive\AbstractDrawableObject.hpp"
#include "MGL\Default\DefaultPrimitives.hpp"
#include "MGL\SharedHeaders\Color.hpp"

void RenderInfoStorage::generateRenderInfo() {
	EmptyBlock = new RenderInfo();
	WallBlock = new RenderInfo();

	WallBlock->get()->addPrimitive(mgl::generateRectangle(1.f, new mgl::Color(0.3f, 0.3f, 0.3f), mgl::PoligonPlacing::zero_TopLeftCorner, true));

	MainActor = new RenderInfo();
	MainActor->get()->addPrimitive(mgl::generateEllipse(2.f, 60, new mgl::Color(0.9f, 0.0f, 0.0f), mgl::PoligonPlacing::zero_Center, true));

	isRenderInfoGenerated = true;
}

void RenderInfoStorage::cleanRenderInfo() {
	delete EmptyBlock;
	delete WallBlock;
	delete MainActor;
}
