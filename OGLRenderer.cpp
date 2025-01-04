//-----------------------------------------------------------------------------
// 
// @doc
//
// @module	OGLRenderer.cpp - OpenGL renderer |
//
// This module contains the support for OpenGL rendering.
//
// Copyright (c) 1999 - Descartes Systems Sciences, Inc.
//
// @end
//
// $History: DcComEngGenConfigurePage.h $
//
//-----------------------------------------------------------------------------

#include "stdafx.h"
#include "OGLRenderer.h"
#include "RenderGlobals.h"
#include "../_CoreLib/Utility.h"
#include "../_CoreLib/Matrix.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//
// Globals used by the renderer
//

HWND COGLRenderer::gs_hWnd;
HDC COGLRenderer::gs_hDC;
HGLRC COGLRenderer::gs_hrc;

//
// Force all render code into own seg
//

#pragma code_seg ("renderer_segment")

//
// Helper macros
//

#ifdef _DEBUG
#define CheckGlError() \
{ \
    GLenum nError = glGetError (); \
    _ASSERT (nError == GL_NO_ERROR); \
}
#else
#define CheckGlError()
#endif

inline void DoSkyCoord (const CSkyDome::_Vertex &sVertex, const TextureMatrix &tm)
{
	CVector2 vTC;
	vTC .m_x = sVertex .vVertex .Dot (tm .vsTexture) + tm .fsTexShift;
	vTC .m_y = sVertex .vVertex .Dot (tm .vtTexture) + tm .ftTexShift;
	glTexCoord2fv (vTC .m_v);
	glVertex3fv (sVertex .vMapVertex .m_v);
}

inline void COGLRenderer::SetColor (const CVector4 &vColor)
{
	if (!(vColor == g_vLastColor))
	{
		glColor4fv (vColor .m_v);
		g_vLastColor = vColor;
	}
}

//-----------------------------------------------------------------------------
//
// @mfunc Draw a cloud layer
//
//		Draw a cloud layer
//
// @rdesc None.
//
//-----------------------------------------------------------------------------

void COGLRenderer::DrawSkyClouds (CSkyClouds *pSkyClouds, 
									const CVector3 &vColor)
{
	int i;

	//
	// Selected the proper texture
	//

	BindTextureIndex (pSkyClouds ->GetTextureIndex ());

	//
	// Create the texture mapping object
	//

	CVector2 vOffset (pSkyClouds ->GetOffset ());
	float fScale = 1.0 / (g_sSkyDome .m_fRadius * 2.0f * pSkyClouds ->GetScale ());
	TextureMatrix tm;
	tm .fsTexShift = .5f + vOffset .m_x;
	tm .ftTexShift = .5f + vOffset .m_y;
	tm .vsTexture = CVector3 (fScale, 0, 0);
	tm .vtTexture = CVector3 (0, fScale, 0);

	//
	// Get a pointer to the vertex list to make things easier
	//

	CSkyDome::_Vertex *pVertex = g_sSkyDome .m_pVertex;
	int nResolution = g_sSkyDome .m_nResolution;

	//
	// Loop through top row of triangles
	//

	SetColor (vColor);
	glBegin (GL_TRIANGLE_FAN);
	DoSkyCoord (pVertex [0], tm);
	for (i = 1; i < nResolution * 4 + 1; i++)
	{
		DoSkyCoord (pVertex [i], tm);
	}
	DoSkyCoord (pVertex [1], tm);
	glEnd ();

	//
	// Loop through squares
	//

	int nDelta = nResolution * 4;
	for (int j = 1; j < nResolution; j++)
	{
		int nStart = nDelta * j + 1;
		int nEnd = nStart + nDelta;
		glBegin (GL_TRIANGLE_STRIP);
		for (i = nStart; i < nEnd; i++)
		{
			DoSkyCoord (pVertex [i - nDelta], tm);
			DoSkyCoord (pVertex [i], tm);
		}
		DoSkyCoord (pVertex [nStart - nDelta], tm);
		DoSkyCoord (pVertex [nStart], tm);
		glEnd ();
	}
}

//-----------------------------------------------------------------------------
//
// @mfunc Draw a sky body
//
//		Draw a sky body to the screen
//
// @rdesc None.
//
//-----------------------------------------------------------------------------

void COGLRenderer::DrawSkyBody (CSkyBody *pSkyBody)
{

	//
	// Distance factor
	//

	static const float fDistanceFactor = 1000.0f;

	//
	// Compute the position of the body
	//

	CVector3 vPos;
	g_sRenderCtx .m_sModelToEye .PreNormalMultiply (pSkyBody ->GetNormal (), vPos);
	vPos *= fDistanceFactor;
	float fScale = fDistanceFactor * 16 * 
		pSkyBody ->GetRadius () / pSkyBody ->GetDistance ();
	CVector3 vXDel (fScale, 0, 0);
	CVector3 vYDel (0, fScale, 0);

	//
	// Draw the body as a billboard
	//

	CVector3 v;
	glBegin (GL_QUADS);
	glTexCoord2f (0, 0);
	v = vPos - vXDel - vYDel;
	glVertex3fv (v .m_v);
	glTexCoord2f (0, 1);
	v = vPos - vXDel + vYDel;
	glVertex3fv (v .m_v);
	glTexCoord2f (1, 1);
	v = vPos + vXDel + vYDel;
	glVertex3fv (v .m_v);
	glTexCoord2f (1, 0);
	v = vPos + vXDel - vYDel;
	glVertex3fv (v .m_v);
	glEnd ();

	//
	// Check for an error
	//

	CheckGlError ();
}

//-----------------------------------------------------------------------------
//
// @mfunc Draw the sky
//
//		Draw the sky to the screen
//
// @rdesc None.
//
//-----------------------------------------------------------------------------

void COGLRenderer::DrawSky ()
{
	CDoubleLink *pStart;
	CDoubleLink *pLink;

	//
	// Initialize opengl.  Turn on blending and disable the z buffer
	//

	glEnable (GL_BLEND);
	glDepthMask (GL_FALSE);

	//
	// Map the sky dome triangles
	//

	g_sSkyDome .MapVertices (g_sRenderCtx .m_sModelToEye);

	//
	// Loop through the clouds in the sky dome and count the
	// number of clouds in the system
	//

	int nClouds = 0;
	pStart = &g_sSkyDome .m_sCloudsList;
	for (pLink = pStart ->GetNext (); 
	pLink != pStart; pLink = pLink ->GetNext ())
	{
		CSkyClouds *pClouds = CSkyClouds::FromLink (pLink);
		if (pClouds ->GetType () == CSkyClouds::Type_Clouds)
			nClouds++;
	}

	//
	// Loop through the clouds in the sky dome and draw the stars
	//

	glBlendFunc (GL_ONE, GL_ONE);
	pStart = &g_sSkyDome .m_sCloudsList;
	for (pLink = pStart ->GetNext (); 
	pLink != pStart; pLink = pLink ->GetNext ())
	{
		CSkyClouds *pClouds = CSkyClouds::FromLink (pLink);
		if (pClouds ->GetType () != CSkyClouds::Type_Stars) 
			continue;
		DrawSkyClouds (pClouds, pClouds ->GetCurrentColor ());
	}

	//
	// Loop through the bodies in the sky dome
	//

	pStart = &g_sSkyDome .m_sBodyList;
	for (pLink = pStart ->GetNext (); 
	pLink != pStart; pLink = pLink ->GetNext ())
	{
		CSkyBody *pBody = CSkyBody::FromLink (pLink);
		switch (pBody ->GetType ())
		{
			case CSkyBody::Type_Sun:
				BindTextureIndex (pBody ->GetTextureIndex ());
				glBlendFunc (GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
				SetColor (pBody ->GetCurrentColor ());
				DrawSkyBody (pBody);
				break;
			case CSkyBody::Type_Moon:
				BindTextureIndex (pBody ->GetMaskTextureIndex ());
				glBlendFunc (GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
				SetColor (g_sSkyDome .m_vCurrentSkyColor);
				DrawSkyBody (pBody);
				BindTextureIndex (pBody ->GetTextureIndex ());
				glBlendFunc (GL_ONE, GL_ONE);
				SetColor (pBody ->GetCurrentColor ());
				DrawSkyBody (pBody);
				break;
			case CSkyBody::Type_Flare:
				BindTextureIndex (pBody ->GetTextureIndex ());
				glBlendFunc (GL_ONE, GL_ONE);
				SetColor (pBody ->GetCurrentColor ());
				DrawSkyBody (pBody);
				break;
			default:
				_ASSERT (FALSE);
				break;
		}
	}

	//
	// Compute the basic cloud color
	//

	CVector3 vBlendColor;
	vBlendColor .m_x = 1.0 - g_sSkyDome .m_vCurrentSkyColor .m_x;
	vBlendColor .m_y = 1.0 - g_sSkyDome .m_vCurrentSkyColor .m_y;
	vBlendColor .m_z = 1.0 - g_sSkyDome .m_vCurrentSkyColor .m_z;
	CVector3 vCloudColor (vBlendColor * (1.0f / (float) nClouds));

	//
	// Loop through the clouds in the sky dome rendering the clouds
	//

	glBlendFunc (GL_ONE, GL_ONE);
	pStart = &g_sSkyDome .m_sCloudsList;
	for (pLink = pStart ->GetNext (); 
	pLink != pStart; pLink = pLink ->GetNext ())
	{
		CSkyClouds *pClouds = CSkyClouds::FromLink (pLink);
		if (pClouds ->GetType () != CSkyClouds::Type_Clouds) 
			continue;
		CVector3 vColor;
		DrawSkyClouds (pClouds, pClouds ->GetCurrentColor () * vCloudColor);
	}

	//
	// Restore any changed settings
	//

	glDisable (GL_BLEND);
	glDepthMask (GL_TRUE);
	CheckGlError ();
}

