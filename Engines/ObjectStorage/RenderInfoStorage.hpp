#pragma once
#include <string>
#include <map>
#include "Shared/AbstractException.hpp"
DefineNewException(RenderInfoException);
#include "FileLoader.hpp"
class RenderInfo;
namespace mgl {
	class Color;
	namespace math {
		class vectorH;
	}
}
class RenderInfoStorage : protected FileLoader {
private:
	static bool wasRenderInfoLoaded;
protected:
	static std::map<std::string, RenderInfo*> m_data;
	RenderInfo *m_current_render_info;
	mgl::Color *m_current_color;
	mgl::math::vectorH *m_current_scale;
protected:
	virtual void parse_file_type_info(std::string const& line) override;
	virtual void parse_line(std::string const& line) override;
	virtual void parse_object_line(std::string const& line);
public:
	RenderInfoStorage();
	~RenderInfoStorage();
	static inline void check() { if (!wasRenderInfoLoaded) throw Exceptions::RenderInfoException("Render info was never loaded, so it cannot be accessed. Call loadRenderInfo() first."); }
	static inline RenderInfo* getRenderInfo(std::string const& obj) { return m_data[obj]; }
	void load(std::string const& path = "/") { FileLoader::load(path, ".stg"); }
	static void clean();
};