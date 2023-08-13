//////////////////////////////////////////////////////////////////////////////////////////
//  File name: CLevel.h                                                                 //
//  Created:   2022-02-18 15:31:40                                                      //
//                                                                                      //
//                                                                                      //
//  Copyright (c) 2022 Tension Graphics AB                                              //
//                                                                                      //
//////////////////////////////////////////////////////////////////////////////////////////

#ifndef __CLEVEL_H__
#define __CLEVEL_H__

#include	<tgMemoryDisable.h>
#include	<imgui.h>
#include	<tgMemoryEnable.h>
class CLevel
{
public:

	// Constructor / Destructor
	 CLevel( void );
	~CLevel( void );

//////////////////////////////////////////////////////////////////////////

	void	Update	( const tgFloat DeltaTime );

//////////////////////////////////////////////////////////////////////////
	//void	DrawImGui(void);
private:

	tgCWorld*	m_pWorld;
	tgCWorld*	m_pCollisionWorld;

};	// CLevel

#endif // __CLEVEL_H__
