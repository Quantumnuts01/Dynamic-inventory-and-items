//////////////////////////////////////////////////////////////////////////////////////////
//  File name: CPlayer.h                                                                //
//  Created:   2022-02-18 16:54:29                                                      //
//                                                                                      //
//                                                                                      //
//  Copyright (c) 2022 Tension Graphics AB                                              //
//                                                                                      //
//////////////////////////////////////////////////////////////////////////////////////////

#ifndef __CPLAYER_H__
#define __CPLAYER_H__

#include	<tgCInputListener.h>
#include	<tgCInterpolator.h>
#include	<tgCV3D.h>
#include	<tgMemoryDisable.h>
#include	<string>
#include	<vector>
#include	<tgMemoryEnable.h>
#include	"Inventory/CInventory.h"

class CPlayer : public tgCInputListener
{
public:

	enum EState
	{
		STATE_IDLE,
		STATE_WALK,
		STATE_RUN,
		STATE_ATTACK_1,
		STATE_ATTACK_2,
		STATE_HURT_1,
		STATE_HURT_2,

		NUM_STATES

	};	// EState
	enum EControlState
	{
		STATE_PLAYING,
		STATE_UI
	};	// EControlState

//////////////////////////////////////////////////////////////////////////

	// Constructor / Destructor
	 CPlayer( void );
	~CPlayer( void );

//////////////////////////////////////////////////////////////////////////

	void	Update			( const tgFloat DeltaTime );
	void	HandleCollision	( const tgFloat DeltaTime );

//////////////////////////////////////////////////////////////////////////

	void	InputEvent	( const tgInput::EType Type, const tgInput::SEvent* pEvent ) override;

//////////////////////////////////////////////////////////////////////////

	tgCV3D	GetPlayerPosition()									{ return m_Position; }
	tgCV3D	GetPlayerForward()									{ return m_Forward; }
	tgBool	GetIsControlling	( void ) const					{ return m_IsControlling; }
	void	SetIsControlling	( const tgBool IsControlling )	{ m_IsControlling = IsControlling; }

	void	DrawImGui(void);
	void	UseItem(void);

	void	Render2D(void);
	tgCModel* GetPlayerModel() { return	m_pModel; }
//////////////////////////////////////////////////////////////////////////

private:
	
	tgBool	CanMove			( void ) const;
	tgBool	CanAttack		( void ) const;
	tgBool	CheckGrounded	( const tgCWorld& rCollisionWorld );

//////////////////////////////////////////////////////////////////////////

	tgCModel* m_pModel;

	tgCV3D				m_MovementDirection;
	tgCV3D				m_CameraRotation;
	tgCV3D				m_Position;
	tgCV3D				m_Forward;
	tgCV3D				m_Rotation;
	tgCV3D				m_Velocity;

	tgFloat				m_CameraMinAngle;
	tgFloat				m_CameraMaxAngle;
	tgFloat				m_WalkSpeed;
	tgFloat				m_RunSpeed;
	tgFloat				m_Blend;
	tgFloat				m_Radius;
	tgFloat				m_SlopeThreshold;
	tgFloat				m_AttackTime;
	tgFloat				m_HurtTime;

	EState				m_State;
	EState				m_LastState;
	EControlState		m_ControlState;
	EMenuState			m_MenuState;

	tgBool				m_IsControlling;
	tgBool				m_CursorIsHidden;
	tgBool				m_IsBlending;
	tgBool				m_Running;
	tgBool				m_Grounded;
	CInventory*			m_pInventory;
	tgUInt32			m_RowLenght;
	tgSInt32			m_LatestPushedSlotID = 0;
};	// CPlayer

#endif // __CPLAYER_H__
