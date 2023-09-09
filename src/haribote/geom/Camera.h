#pragma once

#include <GLES2/gl2.h>

#include "../math/MatrixUtils.h"

class Camera {
public:
	Camera();

	Matrix view() const;
	Matrix projection() const;

	void setTilt(float rad);
	void setPan(float rad);

	float getTilt() const { return m_tilt; }
	float getPan() const { return m_pan; }

	void setPosition(const Vector&);
	void setPosition(float x, float y, float z)
	{
		setPosition(Vector(x, y, z));
	}
	void lookAt(Vector);
	void lookAt(float x, float y, float z)
	{
		lookAt(Vector(x, y, z));
	}

protected:
	void updateViewMatrix();

	Vector m_position;//視点
	float m_tilt;
	float m_pan;
	float m_roll;

	GLfloat zNear, zFar;
	GLfloat fov;//視野角
	GLfloat aspect;//アスペクト比

	Matrix m_viewMatrix;
};
