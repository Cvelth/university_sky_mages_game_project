#include "RenderInfoStorage.hpp"
#include "RenderInfo.hpp"
#include "../MyGraphicsLibrary/MGL/Primitive/AbstractDrawableObject.hpp"
#include "../MyGraphicsLibrary/MGL/Default/DefaultPrimitives.hpp"
#include "../MyGraphicsLibrary/MGL/SharedHeaders/Color.hpp"
#include "../MyGraphicsLibrary/MGL/Math/vector.hpp"
#include "../MyGraphicsLibrary/MGL/Primitive/Primitive.hpp"

RenderInfo* RenderInfoStorage::EmptyBlock;
RenderInfo* RenderInfoStorage::WallBlock;
RenderInfo* RenderInfoStorage::MainActor;
RenderInfo* RenderInfoStorage::AbstractBullet;

bool RenderInfoStorage::isRenderInfoGenerated = false;

void RenderInfoStorage::generateRenderInfo() {
	EmptyBlock = new RenderInfo();
	WallBlock = new RenderInfo();

	WallBlock->get()->addPrimitive(mgl::generateRectangle(1.f, new mgl::Color(0.3f, 0.3f, 0.3f), mgl::PoligonPlacing::zero_TopLeftCorner, true));

	MainActor = new RenderInfo();
	MainActor->get()->addPrimitive(mgl::generateEllipse(2.f, 60, new mgl::Color(0.9f, 0.0f, 0.0f), mgl::PoligonPlacing::zero_Center, true));

	AbstractBullet = new RenderInfo();
	auto bullet = mgl::generateCircle(60, new mgl::Color(0.9f, 0.0f, 0.0f), mgl::PoligonPlacing::zero_Center, true);
	*bullet *= mgl::math::vectorH(.1f, .1f, .1f, 1.f);
	AbstractBullet->get()->addPrimitive(bullet);

	isRenderInfoGenerated = true;
}

void RenderInfoStorage::cleanRenderInfo() {
	delete EmptyBlock;
	delete WallBlock;
	delete MainActor;
}
