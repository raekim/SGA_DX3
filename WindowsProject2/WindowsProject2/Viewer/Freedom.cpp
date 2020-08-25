#include "framework.h"
#include "Freedom.h"

Freedom::Freedom(float moveSpeed, float rotationSpeed)
	: moveSpeed(moveSpeed), rotationSpeed(rotationSpeed)
{
}

Freedom::~Freedom()
{
}

void Freedom::Update()
{
	D3DXVECTOR3 forward, right, up;

	Forward(&forward);
	Right(&right);
	Up(&up);

	//捞悼贸府
	if (!IsPivot())
	{
		{
			D3DXVECTOR3 position;
			Position(&position);

			if (Mouse::Get()->Press(1))
			{
				if (Keyboard::Get()->Press('W'))
					position += forward * moveSpeed * Time::Delta();
				else if (Keyboard::Get()->Press('S'))
					position += -forward * moveSpeed * Time::Delta();

				if (Keyboard::Get()->Press('A'))
					position += -right * moveSpeed * Time::Delta();
				else if (Keyboard::Get()->Press('D'))
					position += right * moveSpeed * Time::Delta();

				if (Keyboard::Get()->Press('E'))
					position += up * moveSpeed * Time::Delta();
				else if (Keyboard::Get()->Press('Q'))
					position += -up * moveSpeed * Time::Delta();

				Position(position.x, position.y, position.z);
			}
		}

		//雀傈贸府
		{
			D3DXVECTOR2 rotation;
			Rotation(&rotation);

			if (Mouse::Get()->Press(1))
			{

				D3DXVECTOR3 val = Mouse::Get()->GetMoveValue();

				rotation.x += val.y * rotationSpeed * Time::Delta();
				rotation.y += val.x * rotationSpeed * Time::Delta();
			}

			Rotation(rotation.x, rotation.y);
		}
	}
	else
	{
		D3DXVECTOR3 position;
		D3DXVECTOR2 rotation;
		Position(&position);
		Rotation(&rotation);

		D3DXVECTOR3 pivot;
		D3DXVECTOR3 temp;
		D3DXVECTOR3 tempUp;
		D3DXVECTOR3 tempRight;
		float distance;
		Pivot(&pivot);

		D3DXVec3Normalize(&temp, &(pivot - position));
		distance = D3DXVec3Length(&(pivot - position));
		
		
		if (Mouse::Get()->Press(1))
		{
		
			if (Keyboard::Get()->Press('W') && distance >= 0.1f)
				position += temp * moveSpeed * Time::Delta();
			else if (Keyboard::Get()->Press('S'))
				position -= temp * moveSpeed * Time::Delta();

			if (Keyboard::Get()->Press('D'))
				rotation.y += rotationSpeed * Time::Delta();
			else if (Keyboard::Get()->Press('A'))
				rotation.y -= rotationSpeed * Time::Delta();
			

			if (Keyboard::Get()->Press('Q'))
				rotation.x += rotationSpeed * Time::Delta();
			else if (Keyboard::Get()->Press('E'))
				rotation.x -= rotationSpeed * Time::Delta();

		}
		
		
		Position(position.x, position.y, position.z);
		Rotation(rotation.x, rotation.y);
	}
}
