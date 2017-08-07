#pragma once
namespace mgl {
	class AbstractDrawableObject;
}

class RenderInfo {
private:
	mgl::AbstractDrawableObject* m_object;
public:
	explicit RenderInfo();
	~RenderInfo();
	inline mgl::AbstractDrawableObject* operator*() {
		return m_object;
	}
	inline mgl::AbstractDrawableObject* operator->() {
		return m_object;
	}
	inline mgl::AbstractDrawableObject* get() {
		return m_object;
	}
};