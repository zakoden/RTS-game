#pragma once

#include <bitset>
#include <memory>

#include "SDL.h"

#include "../behaviors/behavior.h"
#include "../cell.h"
#include "../status_effects.h"

class AbstractUnit {
protected:
	// �����, ������, ������� ��������, ������������ ��������
	int attack_ = 0, defense_ = 0, health_ = 0, max_health_ = 0;
	uint32_t speed_ = 0;				  // ��������
	SDL_Texture* texture_ = NULL;		  // ��������
	Cell position_ = {-1, -1};		  	  // �������
	std::bitset<EFFECT_SIZE> effects_;    // ������ �� ��������
	std::string name_;					  // ���
	std::shared_ptr<Behavior> behavior_;  // ���������

public:
	// ������� ����� �� ������ ������
	virtual void Move(const Cell& destination) = 0;

	// ���������� ����� ��������� �������
	virtual void Attack(AbstractUnit& other) = 0;
	
	virtual SDL_Texture* Draw() const = 0;  // ������ � ���������� �������� �����

	void SetPosition(const Cell& position);	 // ����������� ����� �������
	void SetBehavior(Behavior* behavior);    // ����������� ���������

	std::string GetName() const;  // ���������� ��� �����

	void AddEffect(Effect effect);		  // ��������� ������ � �����
	void RemoveEffect(Effect effect);	  // ������� ������ � ����� (���� �� ����)
	bool HasEffect(Effect effect) const;  // ���������� 1 ���� ����� ������ ���� � 0 �����

	virtual ~AbstractUnit() = default;
};