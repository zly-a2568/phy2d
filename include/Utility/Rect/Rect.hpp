#pragma once

#include <glm/glm.hpp>

using namespace glm;

namespace phy2d {

class Rect {
public:
	float x, y;      // 左下角坐标
	float width, height;

	Rect() : x(0), y(0), width(0), height(0) {}
	Rect(float x_, float y_, float w_, float h_)
		: x(x_), y(y_), width(w_), height(h_) {}

	// 判断点是否在矩形内
	bool contains(vec2 point) const {
		return point.x >= x && point.x <= x + width && point.y >= y && point.y <= y + height;
	}

	// 判断另一个矩形是否完全被包含
	bool contains(const Rect& other) const {
		return other.x >= x && other.x + other.width <= x + width &&
			   other.y >= y && other.y + other.height <= y + height;
	}

	// 判断与另一个矩形是否相交
	bool intersects(const Rect& other) const {
		return !(other.x > x + width ||
				 other.x + other.width < x ||
				 other.y > y + height ||
				 other.y + other.height < y);
	}
};

} // namespace phy2d
