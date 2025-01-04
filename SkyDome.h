#ifndef __SkyDome__
#define __SkyDome__

//-----------------------------------------------------------------------------
// 
// @doc
//
// @module	SkyDome.h - Sky done support |
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

//-----------------------------------------------------------------------------
//
// Required include files
//
//-----------------------------------------------------------------------------

#include "../_CoreLib/Vector3.h"
#include "../_CoreLib/Vector4.h"
#include "../_CoreLib/DoubleLink.h"
#include "SkyBody.h"
#include "SkyClouds.h"

//-----------------------------------------------------------------------------
//
// Forward definitions
//
//-----------------------------------------------------------------------------

class CMatrix;
class CFrustum;

//-----------------------------------------------------------------------------
//
// @class CSkyDome | The class provides support for the sky dome
//
//-----------------------------------------------------------------------------

class CSkyDome
{
// @access Types and enumerations
public:
	struct _Vertex
	{
		CVector3 vVertex;
		CVector3 vMapVertex;
	};

// @access Constructors and Destructors
public:

	// @cmember Constructor.

	CSkyDome ();

	// @cmember Destructor

	~CSkyDome ();

// @access Public Methods
public:

	// @cmember Create the skydome

	void Create (int nResolution, float fVertSweep, float fRadius, 
		float fHeightScale, const CVector3 &vOrigin, const CVector3 &vDayAmbient,
		const CVector3 &vNightAmbient, const CVector3 &vDayColor,
		const CVector3 &vNightColor);

	// @cmember Map the vertices

	void MapVertices (const CMatrix &sModelToEye);

	// @cmember Compute positions of the sky objects

	void ComputePositions (float fTimeInDays);

	// @cmember Prepare a sky dome for rendering

	void Prep ();

// @access Public inline methods
public:

	// @cmember Insert a new body into the list

	void AddBody (CSkyBody *pBody)
	{
		pBody ->InsertTail (&m_sBodyList);
	}

	// @cmember Insert a new clounds into the list

	void AddClouds (CSkyClouds *pClouds)
	{
		pClouds ->InsertTail (&m_sCloudsList);
	}

// @access Protected methods
protected:
	
	// @cmember Initialize the skydome
	
	void Initialize ();

	// @cmember Destroy the skydome

	void Destroy ();

// @access Public variables
public:

	// @cmember Number of vertices

	int				m_nVertex;

	// @cmember List of vertices

	_Vertex			*m_pVertex;

	// @cmember Resolution of the skydome

	int				m_nResolution;

	// @cmember Vertical sweep of the skydome

	float			m_fVertSweep;

	// @cmember Radius of the skydome

	float			m_fRadius;

	// @cmember Height scale of the skydome

	float			m_fHeightScale;

	// @cmember Origin of the skydome

	CVector3		m_vOrigin;

	// @cmember Link list to all of the sky bodies

	CDoubleLink		m_sBodyList;

	// @cmember Link list to all of the sky clouds

	CDoubleLink		m_sCloudsList;

	// @cmember Base sun light ambient

	CVector3		m_vBaseDayAmbient;

	// @cmember Base sun light ambient

	CVector3		m_vBaseNightAmbient;

	// @cmember Base day sky color

	CVector3		m_vBaseDayColor;

	// @cmember Base night sky color

	CVector3		m_vBaseNightColor;

	// @cmember Current position of the sun

	CVector3		m_vCurrentSunNormal;

	// @cmember Current sun light ambient

	CVector4		m_vCurrentAmbient;

	// @cmember Current sky color
	
	CVector4		m_vCurrentSkyColor;
};

#endif // __SkyDome__
