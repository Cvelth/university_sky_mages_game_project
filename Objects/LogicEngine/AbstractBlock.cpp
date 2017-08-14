#include "AbstractBlock.hpp"
#include "Engines\RenderTools\RenderInfoStorage.hpp"

EmptyBlock::EmptyBlock() : AbstractBlock(1.f, RenderInfoStorage::getEmptyBlockRenderInfo()) {};

WallBlock::WallBlock() : AbstractBlock(0.f, RenderInfoStorage::getWallBlockRenderInfo()) {};
