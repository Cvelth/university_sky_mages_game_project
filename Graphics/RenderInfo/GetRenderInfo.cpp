#include "GetRenderInfo.hpp"
#include "RenderInfo.hpp"
#include "Primitive\AbstractDrawableObject.hpp"
#include "Primitive\Primitive.hpp"
#include "Default\DefaultPrimitives.hpp"
#include "Default\DefaultColors.hpp"

void RenderInfoStorage::generateRenderInfo() {
	EmptyBlock = new RenderInfo();
	WallBlock = new RenderInfo();

	WallBlock->get()->addPrimitive(mgl::generateRectangle(1.f, mgl::Colors::Grey(0.3f), mgl::PoligonPlacing::zero_TopLeftCorner, true));

	MainActor = new RenderInfo();
	MainActor->get()->addPrimitive(mgl::generateEllipse(2.f, 60, mgl::Colors::Red(), mgl::PoligonPlacing::zero_Center, true));

	isRenderInfoGenerated = true;
}

void RenderInfoStorage::cleanRenderInfo() {
	delete EmptyBlock;
	delete WallBlock;
	delete MainActor;
}
