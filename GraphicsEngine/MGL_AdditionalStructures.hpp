#pragma once
#include <list>
#include <utility>
namespace mgl {
	class ShaderProgram;
	class ShaderVariable;
	class InstancingArray;
}
class AbstractBlock;
struct AbstractProgramStruct {
	mgl::ShaderProgram *program;
	bool operator!() const {
		return program != nullptr;
	}
	mgl::ShaderProgram* operator*() {
		return program;
	}
	const mgl::ShaderProgram* operator*() const {
		return program;
	}
	mgl::ShaderProgram* operator->() {
		return program;
	}
	mgl::ShaderProgram* operator->() const {
		return program;
	}
};
struct MapProgram : public AbstractProgramStruct {
	mgl::ShaderVariable *projection;
	std::list<std::pair<AbstractBlock*, mgl::InstancingArray*>> translationInstances;
	mgl::ShaderVariable *translation;
};
struct QueueProgram : public AbstractProgramStruct {
	mgl::ShaderVariable *translation, *scaling, *projection;
};
struct HUDProgram : public AbstractProgramStruct {
	mgl::ShaderVariable *translation, *scaling, *projection;
};

#include <string>
std::string readShader(std::string filename);