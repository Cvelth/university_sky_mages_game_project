#pragma once
namespace mgl {
	class AbstractDrawableObject;
	class Primitive;
	class VertexArray;
	enum class DataUsage;
	class InstancingArray;
	class InstancingMultiArray;
}

class RenderInfo {
private:
	mgl::AbstractDrawableObject *m_object;
	bool is_initialized;
public:
	explicit RenderInfo();
	virtual ~RenderInfo();
	virtual void addPrimitive(mgl::Primitive *v);
	virtual void insertVertexArray(mgl::VertexArray *vao);
	virtual void send(mgl::DataUsage const& u);
	virtual void draw();
	virtual void draw(mgl::InstancingArray *instances);
	virtual void draw(mgl::InstancingMultiArray *instances);
	virtual void clean();

	virtual bool isInitialized() const { return is_initialized; }
};