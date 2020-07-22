#pragma once

#include "SDL.h"

#include "../cell.h"

class AbstractUnit {
public:
	// ������� ����� �� ������ ������
	virtual void Move(const Cell& destination) = 0;

	// ���������� ����� ��������� �������
	virtual void Attack(AbstractUnit& other) = 0;
	
	virtual SDL_Texture* Draw() const = 0;  // ������ � ���������� �������� �����
};