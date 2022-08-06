#pragma once

class Movable {
public:
	virtual int GetSpeed() = 0;

	virtual void SetVector(int dx, int dy) = 0;
	virtual void SetVector(double dx, double dy) = 0;
	virtual void AddVector(double dx, double dy) = 0;
	virtual void VectorApply() = 0;
	virtual void VectorApplyBullet() = 0;
	virtual void SetCommandPoint(int x, int y) = 0;
	virtual void GetCommandPoint(int& x, int& y) = 0;

	virtual void Move() = 0;
};