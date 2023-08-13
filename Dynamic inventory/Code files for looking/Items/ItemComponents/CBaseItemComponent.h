#pragma once



class CBaseItemComponent
{
public:
	CBaseItemComponent();
	virtual ~CBaseItemComponent();

	virtual void Use() = 0;

};

