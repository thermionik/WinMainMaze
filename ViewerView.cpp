//-----------------------------------------------------------------------------
//
// @doc
//
// @module	ViewerView.cpp - View main view support |
//
// This module contains the support for the main view.
//
// Copyright (c) 1999 - Descartes Systems Sciences, Inc.
//
// @end
//
// $History: DcComEngGenConfigurePage.h $
//
//-----------------------------------------------------------------------------

#include "stdafx.h"
#include "Viewer.h"
#include "ViewerView.h"
#include "../_CoreLib/Script.h"
#include "../_CoreLib/Exception.h"
#include "../_CoreLib/Matrix.h"
#include "../_CoreLib/Utility.h"
#include "../_CoreLib/PerformanceTimer.h"
#include "../_MeshLib/MeshObj.h"
#include "../_MeshLib/Hull.h"
#include "../_RenderLib/Landscape.h"
#include "../_RenderLib/RenderGlobals.h"
#include "SelectDlg.h"

//
// Debug new
//

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//-----------------------------------------------------------------------------
//
// @mfunc <c CViewerView> constructor.
//
// @rdesc None.
//
//-----------------------------------------------------------------------------

CViewerView::CViewerView ()
{

	//
	// Initialize the sun
	//

	m_sSkySun .Create (
		CSkyBody::Type_Sun,						// type
		"env/sun",								// texture
		0.0f,									// longitude of the ascending node
		23.4393f,								// Incliation to the ecliptic
		282.9404f,								// Argument of perihelion
		149600000.0f,							// Mean distance
		0.016709f,								// Eccentricity
		356.0470f,								// Mean anomaly
		0.9856002585f,							// Mean anomaly adjust
		695000.0f,								// Radius of body
		CVector3 (.7f, .7f, .5f),				// Daytime color
		CVector3 (.7f, .5f, .5f)				// Nighttime color
		);

	//
	// Initialize the sun flare
	//

	m_sSkySunFlare .Create (
		CSkyBody::Type_Flare,					// type
		"effects/flare1",						// texture
		0.0f,									// longitude of the ascending node
		23.4393f,								// Incliation to the ecliptic
		282.9404f,								// Argument of perihelion
		149600000.0f,							// Mean distance
		0.016709f,								// Eccentricity
		356.0470f,								// Mean anomaly
		0.9856002585f,							// Mean anomaly adjust
		8 * 695000.0f,							// Radius of body
		CVector3 (.4f, .4f, .0f),				// Daytime color
		CVector3 (.4f, .2f, .0f)				// Nighttime color
		);

	//
	// Initialize the moon
	//

	m_sSkyMoon .Create (
		CSkyBody::Type_Moon, 					// type
		"env/moon", 							// texture
		125.1228f, 								// longitude of the ascending node
		5.1454f, 								// Incliation to the ecliptic
		318.0634f, 								// Argument of perihelion
		384000.0f,								// Mean distance
		0.054900f,								// Eccentricity
		115.3654f, 								// Mean anomaly
		13.0649929509f, 						// Mean anomaly adjust
		1738.0f,								// Radius of body
		CVector3 (.1f, .1f, .1f), 				// Daytime color
		CVector3 (.4f, .4f, .4f)				// Nighttime color
		);

	//
	// Initialize the clouds
	//

	m_sSkyClouds1 .Create (
		CSkyClouds::Type_Clouds,				// type
		"env/clouds",							// texture
		CVector2 (0.0, 1.0),					// texture speed
		1.0f,									// texture scale
		CVector3 (0.5f, 0.5f, 0.5f),			// daytime color
		CVector3 (0.11f, 0.10f, 0.09f),			// nighttime color
		1.0f									// transition gamma
		);

	m_sSkyClouds2 .Create (
		CSkyClouds::Type_Clouds, 				// type
		"env/clouds", 							// texture
		CVector2 (1.0, 5.0), 					// texture speed
		0.25f, 									// texture scale
		CVector3 (0.3f, 0.3f, 0.3f),			// daytime color
		CVector3 (0.11f, 0.10f, 0.09f),			// nighttime color
		1.0f									// transition gamma
		);

	m_sSkyStars .Create (
		CSkyClouds::Type_Stars, 				// type
		"env/stars", 							// texture
		CVector2 (7.0f, 0.0), 					// texture speed
		1.0f / 24.0f,							// texture scale 
		CVector3 (0.0f, 0.0f, 0.0f),			// daytime color
		CVector3 (0.2f, 0.2f, 0.2f),			// nighttime color
		0.3f									// transition gamma
		);

	//
	// Initialize the sky dome
	//

	g_sSkyDome .Create (
		8,										// resolution
		55,										// sweep in degrees
		1024 * 12,								// size at horizon
		1.0f,									// height scale factor
		CVector3 (0, 0, -512 * 10),				// offset
		CVector3 (1.0f, 1.0f, 1.0f),			// day ambient
		CVector3 (0.4f, 0.4f, 0.4f),			// night ambient
		CVector3 (0.25f, 0.31f, .63f),			// day sky color
		CVector3 (0.1f, 0.1f, 0.11f)			// night sky color 
		);
	
	//
	// Add the bodies and clouds to the sky dome
	//

	g_sSkyDome .AddBody (&m_sSkyMoon);
	g_sSkyDome .AddBody (&m_sSkySunFlare);
	g_sSkyDome .AddBody (&m_sSkySun);
	g_sSkyDome .AddClouds (&m_sSkyStars);
	g_sSkyDome .AddClouds (&m_sSkyClouds1);
	g_sSkyDome .AddClouds (&m_sSkyClouds2);

	//
	// Compute starting values for the sky
	// (Must be done at start of every frame)
	//

	g_sSkyDome .ComputePositions (0);
}

