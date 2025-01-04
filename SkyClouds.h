#ifndef __SkyClouds__
#define __SkyClouds__

//-----------------------------------------------------------------------------
// 
// @doc
//
// @module	SkyClouds.h - Sky cloud support |
//
// This module contains the support for the sky cloud.
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

#include "../_CoreLib/Vector2.h"
#include "../_CoreLib/Vector4.h"
#include "../_CoreLib/DoubleLink.h"
#include "../_CoreLib/DcString.h"

//-----------------------------------------------------------------------------
//
// Forward definitions
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//
// @class CSkyClouds | The class provides support for the sky clouds
//
//-----------------------------------------------------------------------------

class CSkyClouds
{
// @access Types and enumerations
public:
	enum _Type {
		Type_Clouds	= 0,
		Type_Stars	= 1,
	};

// @access Constructors and Destructors
public:

	// @cmember Constructor.

	CSkyClouds ();

	// @cmember Destructor

	~CSkyClouds ();

// @access Public Methods
public:

	// @cmember Create the SkyClouds

	void Create (_Type nType, const char *pszTexture, 
	    const CVector2 &vRate, float fScale, const CVector3 &vDayColor, 
		const CVector3 &vNightColor, float fTransitionExp);

	// @cmember Compute the position of the clouds
	
	void ComputePosition (float fTimeInDays, float fDayFactor);

	// @cmember Prepare a sky dome for rendering

	void Prep ();

// @access Public Inline Methods
public:

	// @cmember Given a link, return the address of the sky cloub
	
	static CSkyClouds *FromLink (CDoubleLink *pLink)
	{
		return (CSkyClouds *) ((BYTE *) pLink - 
			offsetof (CSkyClouds, m_sLink));
	}

	// @cmember Get the type of the cloub
	
	_Type GetType () const
	{
		return m_nType;
	}

	// @cmember Attach the cloud to the end of the given list

	void InsertTail (CDoubleLink *pList)
	{
		m_sLink .InsertTail (pList);
	}

	// @cmember Get the name of the texture

	const char *GetTextureName () const
	{
		return m_strTexture;
	}

	// @cmember Get the texture index

	int GetTextureIndex () const
	{
		return m_nTexture;
	}

	// @cmember Set the texture index

	void SetTextureIndex (int nTextureIndex)
	{
		m_nTexture = nTextureIndex;
	}

	// @cmember Get the offset

	const CVector2 &GetOffset () const
	{
		return m_vOffset;
	}

	// @cmember Get the current color

	const CVector3 &GetCurrentColor () const
	{
		return m_vCurrentColor;
	}

	// @cmember Get the scale

	float GetScale () const
	{
		return m_fScale;
	}

// @access Protected variables
protected:

	// @cmember Type of the body

	_Type			m_nType;

	// @cmember Scale of the texture

	float			m_fScale;

	// @cmember Texture coordinate shift rate

	CVector2		m_vRate;

	// @cmember Base day sky color

	CVector3		m_vBaseDayColor;

	// @cmember Base night sky color

	CVector3		m_vBaseNightColor;

	// @cmember Transition exponent

	float			m_fTransitionExp;

	// @cmember Current color
	
	CVector3		m_vCurrentColor;

	// @cmember Current texture offset

	CVector2		m_vOffset;

	// @cmember List of all objects

	CDoubleLink		m_sLink;

	// @cmember Name of the body texture

	CDcTString		m_strTexture;

	// @cmember Index of the texture

	int				m_nTexture;
};

#endif // __SkyClouds__
