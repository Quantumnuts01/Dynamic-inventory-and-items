//////////////////////////////////////////////////////////////////////////////////////////
//  File name: CPlayer.cpp                                                              //
//  Created:   2022-02-18 16:54:46                                                      //
//                                                                                      //
//                                                                                      //
//  Copyright (c) 2022 Tension Graphics AB                                              //
//                                                                                      //
//////////////////////////////////////////////////////////////////////////////////////////

#include	<tgSystem.h>

#include	"CPlayer.h"

#include	"Camera/CCamera.h"
#include	"Managers/CModelManager.h"
#include	"Managers/CWorldManager.h"
#include	"CApplication.h"
#include	"Inventory/CInventory.h"
#include	"CClock.h"
#include	"tgMath.h"

#include	<tgCDebugManager.h>
#include	<tgCPluginInput.h>
#include	<tgCTextureManager.h>
#include	<tgCAnimation.h>
#include	<tgCInterpolator.h>
#include	<tgCAnimationManager.h>
#include	<tgCCore.h>
#include	<tgCLine3D.h>
#include	<tgCCollision.h>
#include	<tgLog.h>


////////////////////////////// CPlayer //////////////////////////////
//                                                                 //
//  Info:
//                                                                 //
//*//////////////////////////////////////////////////////////////////
CPlayer::CPlayer( void )
: m_pModel( NULL )
//, m_Animations
, m_pInventory(NULL)
, m_MovementDirection( tgCV3D::Zero )
, m_RowLenght(5)
, m_CameraRotation( 45.0f, 180.0f, 0.0f )
, m_Position( 3.0f, 0.0f, -1.0f )
, m_Rotation( 0.0f, 180.0f, 0.0f )
, m_Velocity( 0.0f, 0.0f, 0.0f )
, m_CameraMinAngle( 10.0f )
, m_CameraMaxAngle( 90.0f )
, m_WalkSpeed( 2.0f )
, m_RunSpeed( 5.0f )
, m_Blend( 0.0f )
, m_Radius( 0.5f )
, m_SlopeThreshold( 0.8f )
, m_AttackTime( 0.0f )
, m_HurtTime( 0.0f )
, m_State( STATE_IDLE )
, m_LastState( STATE_IDLE )
, m_ControlState(STATE_PLAYING)
, m_MenuState(STATE_MAIN_MENU)
, m_IsControlling( true )
, m_IsBlending( false )
, m_Running( false )
, m_Grounded(false)
, m_CursorIsHidden(true)
{
	ImGui::SetNextWindowSize({ 500,500 });
	// Load model
	tgCCamera& r3DCamera = *CApplication::GetInstance().Get3DCamera()->GetCamera();
	tgCMatrix& rCameraMatrix = r3DCamera.GetTransform().GetMatrixLocal();

	m_pModel		= CModelManager::GetInstance().LoadModel( "models/box", "Player", false );

	tgCV3D modelp = r3DCamera.GetTransform().GetMatrixLocal().Pos;
	modelp.y -= 0.1;
	modelp.x -= 0.1;
	modelp.z += 0.7;

	m_pModel->GetTransform().GetMatrixLocal().Scale(tgCV3D::Zero, tgCMatrix::COMBINE_POST_MULTIPLY);
	m_pModel->GetTransform().GetMatrixLocal().Pos = modelp;

	r3DCamera.GetTransform().AddChild(m_pModel->GetTransform());
	tgInput::AddListener( this );
	m_pInventory = new CInventory(this);

}	// */ // CPlayer


////////////////////////////// ~CPlayer //////////////////////////////
//                                                                  //
//  Info:
//                                                                  //
//*///////////////////////////////////////////////////////////////////
CPlayer::~CPlayer( void )
{
	tgInput::RemoveListener( this );

	CInventory* Tempinv = m_pInventory;
	delete m_pInventory; //= nullptr;
	//delete Tempinv;
	// Destroy model
	if( m_pModel )
		CModelManager::GetInstance().DestroyModel( m_pModel );

}	// */ // ~CPlayer


////////////////////////////// Update //////////////////////////////
//                                                                //
//  Info:
//                                                                //
//*/////////////////////////////////////////////////////////////////
void
CPlayer::Update( const tgFloat DeltaTime )
{

	//switch( m_State )
	//{
	//	case STATE_IDLE:
	//	{
	//		if( m_MovementDirection.DotProduct() )
	//			m_State	= m_Running ? STATE_RUN : STATE_WALK;
	//	}
	//	break;
	//
	//	case STATE_WALK:
	//	{
	//		if( !m_MovementDirection.DotProduct() )
	//			m_State	= STATE_IDLE;
	//		else if( m_Running )
	//			m_State	= STATE_RUN;
	//	}
	//	break;
	//
	//	case STATE_RUN:
	//	{
	//		if( !m_MovementDirection.DotProduct() )
	//			m_State	= STATE_IDLE;
	//		else if( !m_Running )
	//			m_State	= STATE_WALK;
	//	}
	//	break;
	//
	//	case STATE_ATTACK_1:
	//	case STATE_ATTACK_2:
	//	{
	//		const tgFloat	AttackLength	 = m_State == STATE_ATTACK_1 ? 1.25f : 2.75f;
	//
	//		m_AttackTime					+= DeltaTime;
	//
	//		if( m_AttackTime >= AttackLength )
	//		{
	//			if( m_MovementDirection.DotProduct() )
	//				m_State	= m_Running ? STATE_RUN : STATE_WALK;
	//			else
	//				m_State	= STATE_IDLE;
	//		}
	//	}
	//	break;
	//
	//	case STATE_HURT_1:
	//	case STATE_HURT_2:
	//	{
	//		const tgFloat	HurtLength	 = m_State == STATE_HURT_1 ? 0.6f : 1.0f;
	//
	//		m_HurtTime					+= DeltaTime;
	//
	//		if( m_HurtTime >= HurtLength )
	//		{
	//			if( m_MovementDirection.DotProduct() )
	//				m_State	= m_Running ? STATE_RUN : STATE_WALK;
	//			else
	//				m_State	= STATE_IDLE;
	//		}
	//	}
	//	default:
	//	break;
	//}

//////////////////////////////////////////////////////////////////////////

	tgCCamera& r3DCamera = *CApplication::GetInstance().Get3DCamera()->GetCamera();
	tgCMatrix& rCameraMatrix = r3DCamera.GetTransform().GetMatrixLocal();

	if( m_IsControlling )
	{
		tgCV3D		MoveDir( -rCameraMatrix.Left.z, 0.0f, rCameraMatrix.Left.x );
		tgCMatrix	RotationMatrix	 = tgCMatrix::Identity;
		RotationMatrix.At			 = m_MovementDirection.Normalized();
		RotationMatrix.OrthoNormalize();
		MoveDir.TransformVector( RotationMatrix );

		tgFloat	TargetRotation	= ( TG_RAD_TO_DEG( tgMathATan2( MoveDir.z, -MoveDir.x ) ) - 90.0f );
		tgFloat	SpeedModifier	= ( m_State == STATE_RUN ) ? m_RunSpeed : m_WalkSpeed;

		while( ( TargetRotation - m_Rotation.y ) > 180.0f )
			TargetRotation	-= 360.0f;
		while( ( TargetRotation - m_Rotation.y ) < -180.0f )
			TargetRotation	+= 360.0f;

		m_Position		+= MoveDir * SpeedModifier * DeltaTime;
		m_Rotation.y	 = tgMathInterpolateLinear( m_Rotation.y, TargetRotation, DeltaTime * 15 );
		

//////////////////////////////////////////////////////////////////////////

		HandleCollision( DeltaTime );
		//HandleAnimation( DeltaTime );

//////////////////////////////////////////////////////////////////////////

		m_CameraRotation.x	= tgMathClamp( -90.0f, m_CameraRotation.x, m_CameraMaxAngle );

		rCameraMatrix.Translate( 0, tgCMatrix::COMBINE_REPLACE );
		rCameraMatrix.RotateXYZ( m_CameraRotation, tgCMatrix::COMBINE_POST_MULTIPLY );
		rCameraMatrix.Translate( m_Position + tgCV3D::PositiveY * 0.8f, tgCMatrix::COMBINE_POST_MULTIPLY );
		m_Forward = rCameraMatrix.At;
	}

	r3DCamera.GetTransform().Update();
	r3DCamera.CalcFrustum();
//////////////////////////////////////////////////////////////////////////

	m_pInventory->Update(DeltaTime);
	m_pModel->GetTransform().GetMatrixLocal().Scale( 0.01f, tgCMatrix::COMBINE_REPLACE );
	m_pModel->GetTransform().GetMatrixLocal().RotateXYZTranslate(m_CameraRotation,m_Position, tgCMatrix::COMBINE_REPLACE);
	m_pModel->GetTransform().GetMatrixLocal().RotateXYZ(m_CameraRotation, tgCMatrix::COMBINE_POST_MULTIPLY);
	m_pModel->GetTransform().Update();
}	// */ // Update


////////////////////////////// HandleCollision //////////////////////////////
//                                                                         //
//  Info:
//                                                                         //
//*//////////////////////////////////////////////////////////////////////////
void
CPlayer::HandleCollision( const tgFloat DeltaTime )
{
	const tgCWorld&	rWorld	 = *CWorldManager::GetInstance().GetWorld( "Collision" );

	m_Position				+= m_Velocity * DeltaTime;
	m_Grounded				 = CheckGrounded( rWorld );

	if( !m_Grounded )
	{
		const tgFloat	Gravity	 = 9.81f;

		m_Velocity.y			-= Gravity * DeltaTime;
	}

//////////////////////////////////////////////////////////////////////////

	const tgCSphere	Sphere( m_Position + tgCV3D( 0, m_Radius, 0 ), m_Radius );
	tgCCollision	Collision( true );
	Collision.SetType( tgCMesh::EType::TYPE_WORLD );

	if( Collision.SphereAllMeshesInWorld( Sphere, rWorld ) )
	{
		const tgCV3D	Normal	= Collision.GetLocalNormal();
		const tgFloat	Slope	= Normal.DotProduct( tgCV3D::PositiveY );

		if( Slope <= m_SlopeThreshold )
		{
			const tgFloat	InvFraction	 = ( 1.0f - Collision.GetFraction() );
			const tgCV3D	Push		 = ( Normal * InvFraction ) * Sphere.GetRadius();
			m_Position.x				+= Push.x;
			m_Position.z				+= Push.z;
		}
	}

}	// */ // HandleCollision

////////////////////////////// InputEvent //////////////////////////////
//                                                                    //
//  Info:
//                                                                    //
//*/////////////////////////////////////////////////////////////////////
void
CPlayer::InputEvent( const tgInput::EType Type, const tgInput::SEvent* pEvent)
{

#if defined( TG_WINDOWS )
	switch (m_ControlState)
	{
	case CPlayer::STATE_PLAYING:
		switch (Type)
		{
			case tgInput::TYPE_MOUSE_MOVE_RELATIVE:
			{
				m_CameraRotation.y -= (pEvent->CurrPos.x / tgCCore::GetInstance().GetWindowWidth()) * 135.0f;
				m_CameraRotation.x += (pEvent->CurrPos.y / tgCCore::GetInstance().GetWindowHeight()) * 135.0f;
			}
			
			break;

			case tgInput::TYPE_MOUSE_SINGLE_CLICK:
			{
				if (pEvent->Mouse.ButtonId > 2)
					break;

				if (CanAttack())
				{
					m_State = pEvent->Mouse.ButtonId == 1 ? STATE_ATTACK_1 : STATE_ATTACK_2;
					m_AttackTime = 0.0f;
				}
			}

			break;

			case tgInput::TYPE_KEY_DOWN:
			{
				if (pEvent->Keyboard.Repeat != 0)
					break;

				switch (pEvent->Keyboard.VKey)
				{
				case 'W': { m_MovementDirection.z += 1.0f;	} break;
				case 'S': { m_MovementDirection.z -= 1.0f;	} break;
				case 'A': { m_MovementDirection.x += 1.0f;	} break;
				case 'D': { m_MovementDirection.x -= 1.0f;	} break;
				case VK_SHIFT: { m_Running = true;	} break;
				case 'I':
				{
					//This toggles the cursor and inventory ui
					if (m_CursorIsHidden == true)
					{
						m_CursorIsHidden = false;
						tgCPluginInput::GetInstance().SetAutoCenterMouse(false);
						tgCCore::GetInstance().SetWinCursorVisibility(true);
						m_IsControlling = false;
					}
					else
					{
						m_CursorIsHidden = true;
						tgCPluginInput::GetInstance().SetAutoCenterMouse(true);
						tgCCore::GetInstance().SetWinCursorVisibility(false);
						m_IsControlling = true;
					}
					//m_pInventory->ToggleInventory();
				}
				break;
				}
			}
			break;

			case tgInput::TYPE_KEY_UP:
			{
				switch (pEvent->Keyboard.VKey)
				{
				case 'W': { m_MovementDirection.z -= 1.0f;	} break;
				case 'S': { m_MovementDirection.z += 1.0f;	} break;
				case 'A': { m_MovementDirection.x -= 1.0f;	} break;
				case 'D': { m_MovementDirection.x += 1.0f;	} break;
				case VK_SHIFT: { m_Running = false;	} break;
				}
			}
			break;
			}
		break;
	case CPlayer::STATE_UI:
		break;
	}
	

#endif // TG_WINDOWS

}	// */ // InputEvent


void CPlayer::DrawImGui(void)
{
	if (m_pInventory)
	{
		//ImGui::SetNextWindowSize({ 700,500 });
		ImGui::Begin("Window");
		m_pInventory->DrawImGUI();
	


		ImGui::End();
	}
}

void CPlayer::UseItem(void)
{

}

void CPlayer::Render2D(void)
{
	m_pInventory->Render2D();
}


////////////////////////////// CanMove //////////////////////////////
//                                                                 //
//  Info:
//                                                                 //
//*//////////////////////////////////////////////////////////////////
tgBool
CPlayer::CanMove( void ) const
{
	if( !m_IsControlling)
		return false;
	return true;

}	// */ // CanMove


////////////////////////////// CanAttack //////////////////////////////
//                                                                   //
//  Info:
//                                                                   //
//*////////////////////////////////////////////////////////////////////
tgBool
CPlayer::CanAttack( void ) const
{
	if( !CanMove() )
		return false;

	return true;

}	// */ // CanAttack


////////////////////////////// CheckGrounded //////////////////////////////
//                                                                       //
//  Info:
//                                                                       //
//*////////////////////////////////////////////////////////////////////////
tgBool
CPlayer::CheckGrounded( const tgCWorld& rCollisionWorld )
{
	if( m_Velocity.y > 0.0f )
		return false;

//////////////////////////////////////////////////////////////////////////

	tgCLine3D		Line( m_Position + tgCV3D( 0.0f, m_Radius, 0.0f ), m_Position + tgCV3D( 0.0f, -0.05f, 0.0f ) );
	tgCCollision	Collision( true );
	Collision.SetType( tgCMesh::EType::TYPE_WORLD );

	if( Collision.LineAllMeshesInWorld( Line, rCollisionWorld ) )
	{
		const tgFloat	Slope	= Collision.GetLocalNormal().DotProduct( tgCV3D::PositiveY );

		if( Slope > m_SlopeThreshold )
		{
			m_Position		= Collision.GetLocalIntersection();
			m_Velocity.y	= 0.0f;

			return true;
		}
	}

	return false;

}	// */ // CheckGrounded
