#include "Block.hpp"
#include "Engines/ObjectStorage/RenderInfoStorage.hpp"
EmptyBlock::EmptyBlock() : Block(1.f, RenderInfoStorage::getRenderInfo("EmptyBlock")) {};
WallBlock::WallBlock() : Block(0.f, RenderInfoStorage::getRenderInfo("WallBlock")) {};