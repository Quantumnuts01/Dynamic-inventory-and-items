//////////////////////////////////////////////////////////////////////////////////////////
//  File name: CLevel.cpp                                                               //
//  Created:   2022-02-18 15:31:49                                                      //
//                                                                                      //
//                                                                                      //
//  Copyright (c) 2022 Tension Graphics AB                                              //
//                                                                                      //
//////////////////////////////////////////////////////////////////////////////////////////

#include	<tgSystem.h>

#include	"CLevel.h"

#include	"Managers/CWorldManager.h"

#include	<tgCTextureManager.h>


////////////////////////////// CLevel //////////////////////////////
//                                                                //
//  Info:
//                                                                //
//*/////////////////////////////////////////////////////////////////
CLevel::CLevel( void )
{
	
	// Load worlds
	CWorldManager&	rWorldManager	= CWorldManager::GetInstance();
	m_pWorld						= rWorldManager.LoadWorld( "worlds/Map.tfw",				"Level" );
	m_pCollisionWorld				= rWorldManager.LoadWorld( "worlds/Map.tfw",	"Collision" );

	rWorldManager.SetActiveWorld( m_pWorld );

//////////////////////////////////////////////////////////////////////////

	// Disable normalmaps on ground meshes
	for( tgUInt32 SectorIndex = 0; SectorIndex < m_pWorld->GetNumSectors(); ++SectorIndex )
	{
		tgSWorldSector&	rSector	= *m_pWorld->GetSector( SectorIndex );

		for( tgUInt32 MeshIndex = 0; MeshIndex < rSector.NumMeshes; ++MeshIndex )
		{
			tgCMesh&	rMesh	= rSector.pMeshArray[ MeshIndex ];

			if( strncmp( rMesh.GetAssetName().String(), "TileGround_", 11 ) == 0 )
			{
				for( tgUInt32 SubMeshIndex = 0; SubMeshIndex < rMesh.GetNumSubMeshes(); ++SubMeshIndex )
				{
					tgCMesh::SSubMesh&	rSubMesh	= *rMesh.GetSubMesh( SubMeshIndex );
					tgCTexture*			pNormalmap	= rSubMesh.Material.GetNormalmap();

					tgCTextureManager::GetInstance().Destroy( &pNormalmap );
					pNormalmap	= tgCTextureManager::GetInstance().Create( "tg_embedded_lightblue" );
					rSubMesh.Material.SetNormalmap( *pNormalmap );
				}
			}
		}
	}

}	// */ // CLevel


////////////////////////////// ~CLevel //////////////////////////////
//                                                                 //
//  Info:
//                                                                 //
//*//////////////////////////////////////////////////////////////////
CLevel::~CLevel( void )
{
	CWorldManager::GetInstance().DestroyWorld( m_pCollisionWorld );
	CWorldManager::GetInstance().DestroyWorld( m_pWorld );

}	// */ // ~CLevel


////////////////////////////// Update //////////////////////////////
//                                                                //
//  Info:
//                                                                //
//*/////////////////////////////////////////////////////////////////
void
CLevel::Update( const tgFloat /*DeltaTime*/ )
{

}	// */ // Update

//void CLevel::DrawImGui(void)
//{
//}
