#pragma once

#include "abstract_unit.h"

#include <bitset>

#include "../status_effects.h"
#include "../behaviors/behavior.h"

class Unit : public AbstractUnit {
protected:
	// �����, ������, ������� ��������, ������������ ��������
	int attack_, defense_, health_, max_health_;
	uint32_t speed_;					 // ��������
	SDL_Texture* texture_ = NULL;		 // ��������
	Cell position_ = {-1, -1};			 // �������
	std::bitset<EFFECT_SIZE> effects_;   // ������ �� ��������
	std::string name_;					 // ���
	std::shared_ptr<Behavior> behavior_;  // ���������

public:
	Unit() = default;

	// �����������, ������� �����, ������, ����. ��������, ��������, ��������, ���
	Unit(int attack, int defense, int max_health, uint32_t speed,
		SDL_Texture* texture, const std::string& name)
	: attack_(attack), defense_(defense), health_(max_health),
		max_health_(max_health), speed_(speed), texture_(texture), name_(name) {}

	virtual void Move(const Cell& destination) override;

	virtual void Attack(AbstractUnit& other) override;

	virtual SDL_Texture* Draw() const override;

	void SetPosition(const Cell& position);	 // ����������� ����� �������
	void SetBehavior(Behavior* behavior);    // ����������� ���������

	std::string GetName() const;  // ���������� ��� �����

	void AddEffect(Effect effect);		  // ��������� ������ � �����
	void RemoveEffect(Effect effect);	  // ������� ������ � ����� (���� �� ����)
	bool HasEffect(Effect effect) const;  // ���������� 1 ���� ����� ������ ���� � 0 �����

	virtual ~Unit() = default;
};