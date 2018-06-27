#include "Block.hpp"
#include "Engines/RenderTools/RenderInfoStorage.hpp"

EmptyBlock::EmptyBlock() : Block(1.f, RenderInfoStorage::getEmptyBlockRenderInfo()) {};

WallBlock::WallBlock() : Block(0.f, RenderInfoStorage::getWallBlockRenderInfo()) {};
