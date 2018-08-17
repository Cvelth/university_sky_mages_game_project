#pragma once
#include <string>
#include <unordered_map>
#include "Shared/AbstractException.hpp"
DefineNewException(RenderInfoException);
#include "FileLoader.hpp"
#include "DefaultObjectStorageData.hpp"
#include <memory>
class RenderInfo;
namespace mgl {
	class Color;
	namespace math {
		class vectorH;
	}
}
class RenderInfoStorage : protected FileLoader {
	using Palette = std::vector<std::shared_ptr<mgl::Color>>;
private:
	static bool wasRenderInfoLoaded;
protected:
	static std::unordered_map<std::string, std::shared_ptr<RenderInfo>> m_data;
	static std::unordered_map<std::string, Palette> m_palettes;
	bool m_current_mode;
	Palette m_current_palette;
	std::shared_ptr<RenderInfo> m_current_render_info;
	std::shared_ptr<mgl::Color> m_current_color;
	std::shared_ptr<mgl::math::vectorH> m_current_scale;
	std::string m_current_name;
protected:
	virtual void parse_file_type_info(std::string const& line) override;
	virtual void parse_line(std::string const& line) override;
	virtual void parse_object_line(std::string const& line);
	virtual void parse_palette_line(std::string const& line);

	virtual std::string generate_file_type_info() override { return "with RenderInfo"; }
public:
	RenderInfoStorage();
	static inline void check() { if (!wasRenderInfoLoaded) throw Exceptions::RenderInfoException("Render info was never loaded, so it cannot be accessed. Call loadRenderInfo() first."); }
	static std::shared_ptr<RenderInfo> getRenderInfo(std::string const& obj);
	static std::string getRenderInfo(std::shared_ptr<RenderInfo> inf);
	static Palette& getPalette(std::string const& obj);
	static std::string getPalette(Palette& inf);
	void load(std::string const& path = "/") { FileLoader::load(path, RenderInfoFileExtention); wasRenderInfoLoaded = true; }
	static void clean();
};