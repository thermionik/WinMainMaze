//-----------------------------------------------------------------------------
//
// @doc
//
// @module	SkyDome.cpp - Sky dome |
//
// This module contains the support for the sky dome.
//
// Copyright (c) 1999 - Descartes Systems Sciences, Inc.
//
// @end
//
// $History: DcComEngGenConfigurePage.h $
//
//-----------------------------------------------------------------------------

#include "stdafx.h"
#include "SkyDome.h"
#include "../_CoreLib/Matrix.h"
#include "../_CoreLib/Utility.h"
#include "Frustum.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//
// Force all render code into own seg
//

#pragma code_seg ("renderer_segment")

//-----------------------------------------------------------------------------
//
// @mfunc <c CSkyDome> constructor.
//
// @rdesc None.
//
//-----------------------------------------------------------------------------

CSkyDome::CSkyDome () 
{

	//
	// Invoke the helper routine
	//

	Initialize ();
}

//-----------------------------------------------------------------------------
//
// @mfunc <c CSkyDome> destructor.
//
// @rdesc None.
//
//-----------------------------------------------------------------------------

CSkyDome::~CSkyDome () 
{

	//
	// Invoke the helper routine
	//

	Destroy ();
}

//-----------------------------------------------------------------------------
//
// @mfunc Initialize the sky dome
//
// @rdesc None.
//
//-----------------------------------------------------------------------------

void CSkyDome::Initialize () 
{

	//
	// Initialize the variables
	//

	m_nVertex = 0;
	m_pVertex = NULL;
	m_nResolution = 0;

	//
	// Initialize the sky light information
	//

	m_vBaseDayAmbient    = CVector3 (1, 1, 1);
	m_vBaseNightAmbient  = CVector3 (0, 0, 0);
	m_vBaseDayColor      = CVector3 (0, 0, 1);
	m_vBaseNightColor    = CVector3 (0, 0, 0);
}

//-----------------------------------------------------------------------------
//
// @mfunc Destroy the sky dome
//
// @rdesc None.
//
//-----------------------------------------------------------------------------

void CSkyDome::Destroy () 
{

	//
	// Remove all bodies and clouds
	//

	m_sBodyList .RemoveAllChildren ();
	m_sCloudsList .RemoveAllChildren ();

	//
	// Deallocate the vertex array
	//

	if (m_pVertex)
		delete [] m_pVertex;

	//
	// Initialize
	//

	Initialize ();
}

//-----------------------------------------------------------------------------
//
// @mfunc Create a new sky dome
//
// @parm int | nResolution | Defines the number of subdivisions in a 90
//		degree slice of the sky dome.  This applies to the horizontal and
//		vertical aspects of the dome.
//
// @parm float | fVertSweep | Vertical sweep.  Must be greater than zero 
//		and less or equal to 90.  This defines the amount of the hemisphere
//		that will be included in the dome.
//
// @parm float | fRadius | Radius of the sky dome at the base
//
// @parm float | fHeightScale | Height scale of the sky dome.  Used to either
//		increase or decrease the height of the dome after generation.
//
// @parm const CVector3 & | vOrigin | Origin of the center of the sky dome.
//		Usually, the sky dome is place slightly below the landscape.
//
// @parm const CVector3 & | vDayAmbient | Base ambient color for the sun light.
//
// @parm const CVector3 & | vNightAmbient | Base ambient color for the night light.
//
// @parm const CVector3 & | vDayColor | Base color for the day sky
//
// @parm const CVector3 & | vNightColor | Base color for the night sky
//
// @rdesc None.
//
//-----------------------------------------------------------------------------

void CSkyDome::Create (int nResolution, float fVertSweep, float fRadius, 
		float fHeightScale, const CVector3 &vOrigin, const CVector3 &vDayAmbient,
		const CVector3 &vNightAmbient, const CVector3 &vDayColor,
		const CVector3 &vNightColor)
{
	//
	// Validate the arguments
	//

	_ASSERT (nResolution > 0 && fRadius > 0);
	_ASSERT (fVertSweep > 0 && fVertSweep <= 90);

	//
	// Destroy the old dome
	//

	Destroy ();

	//
	// Save the generation arguments
	//

	m_nResolution = nResolution;
	m_fVertSweep = fVertSweep;
	m_fRadius = fRadius;
	m_fHeightScale = fHeightScale;
	m_vOrigin = vOrigin;
	m_vBaseDayAmbient = vDayAmbient;
	m_vBaseNightAmbient = vNightAmbient;
	m_vBaseDayColor = vDayColor;
	m_vBaseNightColor = vNightColor;


	//
	// Compute the number of vertices
	//

	m_nVertex = 1 + 4 * nResolution * nResolution;

	//
	// Allocate the buffer to contain vertices
	//

	m_pVertex = new _Vertex [m_nVertex];


	//
	// Adjust the radius based on the vertical sweep
	//

	float fRadAngle = (90 - fVertSweep) / 180 * PI;
	fRadius /= cos (fRadAngle);

	//
	// Compute the z adjustment
	//

	float fZAdj = fRadius * sin (fRadAngle);

	//
	// Start the vertex list with the very top of the dome
	//

	m_pVertex [0] .vVertex = CVector3 (0, 0, 
		(fRadius - fZAdj) * fHeightScale) + vOrigin;

	//
	// From the resolution, compute the angular sweep of one section
	// of the dome
	//

	float fHorzSweep = 90.0 / nResolution;

	//
	// Adjust the vertical resolution
	//

	fVertSweep /= nResolution;
	
	//
	// Starting from the top, populate with nResolution number of rings
	//

	int nVertex = 1;
	for (int i = 0; i < nResolution; i++)
	{

		//
		// Compute the vertex that will be rotated around to make a ring
		//

		CVector3 vPoint (0, 0, fRadius);
		CMatrix m;
		m .MakeHPR (0, fVertSweep * (i + 1), 0);
		m .PreMultiply (vPoint, vPoint);
		_ASSERT (vPoint .m_z >= fZAdj - ON_EPSILON);
		vPoint .m_z = (vPoint .m_z - fZAdj) * fHeightScale;

		//
		// Loop through the ring creating the points
		//

		for (int j = 0; j < nResolution * 4; j++)
		{

			//
			// Map the point
			//

			m .SetHPR (fHorzSweep * j, 0, 0);
			m .PreMultiply (vPoint, m_pVertex [nVertex] .vVertex);
			m_pVertex [nVertex] .vVertex += vOrigin;
			nVertex++;
		}
	}

	//
	// All done
	//

	return;
}

//-----------------------------------------------------------------------------
//
// @mfunc Map the vertices
//
// @parm const CMatrix & | sModelToEye | Matrix used to map the sky dome
//		vertices to the eye space.  The translation part of the matrix is
//		not used.
//
// @rdesc None.
//
//-----------------------------------------------------------------------------

void CSkyDome::MapVertices (const CMatrix &sModelToEye)
{


	//
	// Loop through the vertices
	//

	for (int i = 0; i < m_nVertex; i++)
	{

		//
		// Map
		//

		sModelToEye .PreNormalMultiply (
//		sModelToEye .PreMultiply (
			m_pVertex [i] .vVertex,
			m_pVertex [i] .vMapVertex);
//		m_pVertex [i] .vMapVertex .m_y += sModelToEye .m_m [3] [1];
	}
}

//-----------------------------------------------------------------------------
//
// @mfunc Compute the position of the sky objects
//
// @parm float | fTimeInDays | The current time in days
//
// @rdesc None.
//
//-----------------------------------------------------------------------------

void CSkyDome::ComputePositions (float fTimeInDays)
{

	CSkyBody *pSun = NULL;

	//
	// Loop through the sky bodies computing positions
	//

	CDoubleLink *pStart = &m_sBodyList;
	for (CDoubleLink *pLink = pStart ->GetNext (); 
	pLink != pStart; pLink = pLink ->GetNext ())
	{

		//
		// Get a pointer to the body
		//

		CSkyBody *pBody = CSkyBody::FromLink (pLink);

		//
		// Compute the position.  If the time is -1, just compute
		// a constant position in the sky
		//

		if (fTimeInDays == -1)
		{
			CVector3 vNormal (1, -1, 1);
			vNormal .Normalize ();
			pBody ->SetPosition (vNormal * pBody ->GetA ());
		}
		else
		{
			pBody ->ComputePosition (fTimeInDays);
		}

		//
		// If this is a sun, save the pointer
		//

		if (pBody ->GetType () == CSkyBody::Type_Sun)
			pSun = pBody;
	}

	//
	// If a sun was found, then compute the sun light
	//

	float fDayFactor;
	if (pSun)
	{
		//
		// Get the normal
		//

		CVector3 vNormal (pSun ->GetNormal ());
		m_vCurrentSunNormal = vNormal;

		//
		// Compute the angle of the sun
		//

		float fSunAngle = asin (vNormal .m_z) * 180.0f / PI;
		bool fSunrise = vNormal .m_x >= 0.0;

		//
		// Compute the day factor
		//

		if (fTimeInDays != -1) 
		{
			if (fSunAngle > 20.0) 
			{
				fDayFactor = 1.0;
				m_vCurrentSkyColor = m_vBaseDayColor;
			} 
			else if (fSunAngle >= -10.0)
			{
				fDayFactor = (fSunAngle + 10.0f) / 30.0f;
			} 
			else 
			{
				fDayFactor = 0.0;
				m_vCurrentSkyColor = m_vBaseNightColor;
			}
		}
		else
		{
			fDayFactor = 1.0;
			m_vCurrentSkyColor = m_vBaseDayColor;
		}

		//
		// Compute the sky color
		//

		CVector3 vDeltaSky (m_vBaseDayColor - m_vBaseNightColor);
		m_vCurrentSkyColor = m_vBaseNightColor + vDeltaSky * fDayFactor;

		//
		// Compute the ambient
		//

		CVector3 vDeltaAmbient (m_vBaseDayAmbient - m_vBaseNightAmbient);
		m_vCurrentAmbient = m_vBaseNightAmbient + vDeltaAmbient * fDayFactor;
	}

	//
	// If not, just used the default values
	//

	else
	{
		m_vCurrentSunNormal = CVector3 (1,0,1);
		fDayFactor = 1.0;
		m_vCurrentAmbient = m_vBaseDayAmbient;
		m_vCurrentSkyColor = m_vBaseDayColor;
	}

	//
	// Force the w value for the colors to 1
	//

	m_vCurrentAmbient .m_w = 1.0;
	m_vCurrentSkyColor .m_w = 1.0;

	//
	// Compute the positions of the clouds
	//

	pStart = &m_sCloudsList;
	for (pLink = pStart ->GetNext (); 
	pLink != pStart; pLink = pLink ->GetNext ())
	{

		//
		// Get a pointer to the clouds
		//

		CSkyClouds *pClouds = CSkyClouds::FromLink (pLink);

		//
		// Compute the position.
		//

		pClouds ->ComputePosition (fTimeInDays, fDayFactor);
	}

	//
	// Compute the colors of the bodies
	//

	pStart = &m_sBodyList;
	for (pLink = pStart ->GetNext (); 
	pLink != pStart; pLink = pLink ->GetNext ())
	{

		//
		// Get a pointer to the body
		//

		CSkyBody *pBody = CSkyBody::FromLink (pLink);

		//
		// Compute the color.
		//

		pBody ->ComputeColor (fDayFactor);
	}

}

//-----------------------------------------------------------------------------
//
// @mfunc Prepare a sky dome
//
//		Prepare a sky dome.
//
// @rdesc None.
//
//-----------------------------------------------------------------------------

void CSkyDome::Prep ()
{

	//
	// Loop through the bodies in the sky dome
	//

	CDoubleLink *pStart = &m_sBodyList;
	for (CDoubleLink *pLink = pStart ->GetNext (); 
	pLink != pStart; pLink = pLink ->GetNext ())
	{
		CSkyBody *pBody = CSkyBody::FromLink (pLink);
		pBody ->Prep ();
	}

	//
	// Loop through the clounds in the sky dome
	//

	pStart = &m_sCloudsList;
	for (pLink = pStart ->GetNext (); 
	pLink != pStart; pLink = pLink ->GetNext ())
	{
		CSkyClouds *pClouds = CSkyClouds::FromLink (pLink);
		pClouds ->Prep ();
	}
}

