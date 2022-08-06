#pragma once

class WithHitbox {
public:
	virtual ~WithHitbox() = default;

	virtual int GetX() = 0;
	virtual int GetY() = 0;
	virtual int GetCenterX() = 0;
	virtual int GetCenterY() = 0;

	virtual void GetHitbox(double& x1, double& y1, double& x2, double& y2) = 0;
	virtual int GetLegsY() = 0;
	virtual void GetCollisionbox(double& x1, double& y1, double& x2, double& y2) = 0;

	virtual void GetVector(double& dx, double& dy) = 0;
	virtual void AddVector(double dx, double dy) = 0;
	virtual void SetPosition(int x, int y) = 0;
	virtual void SetPosition(double x, double y) = 0;

	virtual WithHitbox* GetClosestUnit(WithHitbox* unit1, WithHitbox* unit2) = 0;
	// return first enemy unit in radius
	virtual WithHitbox* FindEnemyInRadius(int radius) = 0;
	// return first enemy unit in block
	virtual WithHitbox* GetEnemyInPoint(int x, int y) = 0;

	virtual void UnitCollide(WithHitbox* unit) = 0;

	virtual void DamageApply(int damage) = 0;

	virtual void DeadCheck() = 0;
	virtual bool IsAlive() = 0;
	virtual void Die() = 0;
};