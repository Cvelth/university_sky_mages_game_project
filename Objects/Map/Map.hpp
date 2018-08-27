#pragma once
#include <vector>
#include <memory>
class Block;
class Camera;
class MapGenerator;
class MapStorage;
class MessageInputStream;
class MessageOutputStream;

class Map {
	friend MapGenerator;
	friend MapStorage;
private:
	std::vector<std::shared_ptr<Block>> m_blocks;
	std::vector<std::vector<std::shared_ptr<Block>>> m_cells;
	std::shared_ptr<Block> m_default_block;

	class MapSharedPointerEnabler;
protected:
	Map(size_t width, size_t height, std::shared_ptr<Block> default_block);
	void set(std::shared_ptr<Block> block, size_t _width, size_t _height);
	void addNewBlock(std::shared_ptr<Block> block);
	bool isBorder(size_t w, size_t h) const;
public:
	~Map();

	inline size_t width() const { return m_cells.size(); }
	inline size_t height() const { return m_cells.front().size(); }
	inline size_t getSize() const { return width() * height(); }

	bool is_in_borders(size_t w, size_t h) const;
	inline std::shared_ptr<Block> operator()(size_t w, size_t h) const {
		return get(w, h);
	}
	std::shared_ptr<Block> get(size_t w, size_t h) const;
	float getSpeedMultiplier(size_t w, size_t h) const;
	std::string const& getRenderInfo(size_t w, size_t h) const;
	inline std::vector<std::shared_ptr<Block>>& get_blocks_data() {
		return m_blocks;
	}
	inline void changeDefaultBlock(std::shared_ptr<Block> new_default) {
		m_default_block = new_default;
	}
	inline std::shared_ptr<Block> default_block() const { return m_default_block; }

	friend MessageInputStream& operator>>(MessageInputStream &s, std::shared_ptr<Map> &mt);
	friend MessageOutputStream& operator<<(MessageOutputStream &s, std::shared_ptr<Map> const& mt);
};

#include "../../Shared/AbstractException.hpp"
DefineNewException(NonExistingCellException)