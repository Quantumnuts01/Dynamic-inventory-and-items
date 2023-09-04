#pragma once



class CBaseItemComponent
{
public:
	CBaseItemComponent();
	virtual ~CBaseItemComponent();

	virtual void Use() = 0;


	virtual void Update(float DeltaTime) { DeltaTime; }
};

