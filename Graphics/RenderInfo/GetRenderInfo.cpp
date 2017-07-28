#include "GetRenderInfo.hpp"
#include "RenderInfo.hpp"
#include "Primitive\AbstractDrawableObject.hpp"
#include "Primitive\Primitive.hpp"
#include "Primitive\DefaultPrimitives.hpp"
#include "OpenGL_Mirror\BasicTypes\BasicColors.hpp"

void RenderInfoStorage::generateRenderInfo() {
	EmptyBlock = new RenderInfo();
	WallBlock = new RenderInfo();
	WallBlock->get()->addPrimitive(mgl::generateRectangle(1.f, mgl::Colors::Grey(0.3f), true));

	isRenderInfoGenerated = true;
}

void RenderInfoStorage::cleanRenderInfo() {
	delete EmptyBlock;
	delete WallBlock;
}
