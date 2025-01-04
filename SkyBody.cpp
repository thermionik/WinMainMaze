//-----------------------------------------------------------------------------
//
// @doc
//
// @module	SkyBody.cpp - Sky body |
//
// This module contains the support for the sky body.
//
// Copyright (c) 1999 - Descartes Systems Sciences, Inc.
//
// @end
//
// $History: DcComEngGenConfigurePage.h $
//
//-----------------------------------------------------------------------------

#include "stdafx.h"
#include "SkyBody.h"
#include "../_CoreLib/Matrix.h"
#include "../_CoreLib/Utility.h"
#include "RenderGlobals.h"

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
// @mfunc <c CSkyBody> constructor.
//
// @rdesc None.
//
//-----------------------------------------------------------------------------

CSkyBody::CSkyBody () 
{
	//
	// Initialize
	//
}

//-----------------------------------------------------------------------------
//
// @mfunc <c CSkyBody> destructor.
//
// @rdesc None.
//
//-----------------------------------------------------------------------------

CSkyBody::~CSkyBody () 
{
	//
	// Rundown
	//
}

//-----------------------------------------------------------------------------
//
// @mfunc Create the sky body
//
// @parm _Type | nType | Type of sky body
//
// @parm const char * | pszTexture | Name of the texture
//
// @parm float | fN | Longitude of the ascending node
//
// @parm float | fi | Incliation to the ecliptic
//
// @parm float | fw | Argument of perihelion
//
// @parm float | fa | Mean distance
//
// @parm float | fe | Eccentricity (0=circle, 0-1=ellipse, 1=parabole)
//
// @parm float | fM | Mean anomaly (0 at perihelion)
//
// @parm float | fdM | Mean anomaly adjust factor for days
//
// @parm float | fRadius | Radius of the object
//
// @parm const CVector3 & | vDayColor | Color of the clouds during the day
//
// @parm const CVector3 & | vNightColor | Color of the clouds during the night
//
// @rdesc None.
//
//-----------------------------------------------------------------------------

void CSkyBody::Create (_Type nType, const char *pszTexture, float fN, 
	float fi, float fw, float fa, float fe, float fM, float fdM,
	float fRadius, const CVector3 &vDayColor, const CVector3 &vNightColor) 
{

	//
	// Initialize the variables
	//

	m_nType = nType;
	m_fN = fN;
	m_fi = fi;
	m_fw = fw;
	m_fa = fa;
	m_fe = fe;
	m_fM = fM;
	m_fdM = fdM;
	m_fRadius = fRadius;
	m_strTexture = pszTexture;
	m_vBaseDayColor = vDayColor;
	m_vBaseNightColor = vNightColor;
}

//-----------------------------------------------------------------------------
//
// @mfunc Compute the position of the sky body
//
// @parm float | fTimeInDays | The current time in days
//
// @rdesc None.
//
//-----------------------------------------------------------------------------

void CSkyBody::ComputePosition (float fTimeInDays)
{

	//
	// Compute the radian values of the settings
	//

	float M = (m_fM + m_fdM * fTimeInDays) * PI / 180.0f;
	float e = m_fe * PI / 180.0f;
	float N = m_fN * PI / 180.0f;
	float w = m_fw * PI / 180.0f;
	float i = m_fi * PI / 180.0f;
	float a = m_fa;

	//
	// Compute the eccentric anomaly
	//

	float E = M + e * sin (M) * (1.0f + e * cos (M));

	//
	// Compute the distance r and the true anomaly v
	//

	float xv = a * (cos (E) - e);
	float yv = a * (sqrt (1.0 - e * e) * sin (E));
	float r = sqrt (xv * xv + yv * yv);
	float v;
	if (xv > 0.0)
		v = atan (yv / xv);
	else if (xv < 0.0)
		v = atan (yv / xv) + PI;
	else if (yv < 0.0)
		v = -90;
	else
		v = 90;

	//
	// Compute the position in 3d space
	//

	CVector3 vDir;
	vDir .m_x = r * (cos (N) * cos (v+w) - sin (N) * sin (v+w) * cos (i));
	vDir .m_y = r * (sin (N) * cos (v+w) + cos (N) * sin (v+w) * cos (i));
	vDir .m_z = r * (sin (v + w) * sin (i));

	//
	// Rotate the position by the time of day
	//

	float fDays = floor (fTimeInDays);
	float fRotation = (fTimeInDays - fDays) * 360.0f;
	CMatrix m;
	m .MakeRotation (fRotation, 0, 0, 1);
	m .PreMultiply (vDir, vDir);

	//
	// Save the position
	//

	SetPosition (CVector3 (vDir .m_x, vDir .m_z, vDir .m_y));
}

//-----------------------------------------------------------------------------
//
// @mfunc Compute the color of the sky body
//
// @parm float | fDayFactor | Factor of the day.  0 = night, 1 = day
//
// @rdesc None.
//
//-----------------------------------------------------------------------------

void CSkyBody::ComputeColor (float fDayFactor)
{

	//
	// Set the day color
	//

	CVector3 vDelta = m_vBaseDayColor - m_vBaseNightColor;
	m_vCurrentColor = m_vBaseNightColor + vDelta * fDayFactor;
}

//-----------------------------------------------------------------------------
//
// @mfunc Prepare a sky body
//
//		Prepare a sky body.
//
// @rdesc None.
//
//-----------------------------------------------------------------------------

void CSkyBody::Prep ()
{

	//
	// Load the texture
	//

	const char *pszTexture = GetTextureName ();
	SetTextureIndex (g_sTextureManager .FindNormalN (pszTexture));

	//
	// If a moon, create a mask texture
	//

	if (GetType () == Type_Moon)
	{
		SetMaskTextureIndex (g_sTextureManager .FindMaskN (pszTexture));
	}
}

