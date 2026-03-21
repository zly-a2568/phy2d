#ifndef PHY2D_OBJECT_HPP
#define PHY2D_OBJECT_HPP

#include <utility>
#include <glm/glm.hpp>
#include "../Utility/Rect/Rect.hpp"

using namespace glm;

namespace phy2d {

enum BodyType {
    NONE,
	CIRCLE,
	BOX,
	LINE
};

class Object {
public:
	Object() = default;
	virtual ~Object() = default;

	// 位置
	virtual vec2 getPosition() const;
	virtual void setPosition(float x, float y);

	// 速度
	virtual vec2 getVelocity() const;
	virtual void setVelocity(float vx, float vy);

	// 质量
	virtual float getMass() const;
	virtual void setMass(float m);

	//角速度
	virtual float getAngleVelocity() const;
	virtual void setAngleVelocity(float av);

	virtual float getAngle() const;
	virtual void setAngle(float a);

	virtual void applyTorque(float t);
	virtual void applyForce(vec2 f);

	virtual void addVelocity(float vx, float vy);
	virtual void addAngleVelocity(float av);

	virtual float getRotationInertia() const;

	// 更新物体状态
	virtual void update(float dt) = 0;

	BodyType getType();

	float mass;
	float angle_velocity;
	BodyType type;
	vec2 position;
	vec2 velocity;
	vec2 force;
	float torque;
	float RotationInertia;
	float angle;
	Rect bounding_box;
	vec2 tmp_v;
	float tmp_av;
};

} // namespace phy2d

#endif // PHY2D_OBJECT_HPP
