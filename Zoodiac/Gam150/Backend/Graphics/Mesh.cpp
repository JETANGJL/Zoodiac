// ======================================================================
// File Name:		Mesh.cpp
// Project Name:	Zoodiac
// Author(s):		Low Zhi Sheng Kitson(60%), Lim Geng Yang(20%),
//					Lye Pin Liang Xavier(20%)
// Brief:			Contains Mesh functionality
// 
// All content © 2023 DigiPen Institute of Technology Singapore. All 
// rights reserved.
// ======================================================================

#include <Backend/Engine_Common.h>
#include <Backend/Graphics/Mesh.h>

namespace Backend
{
	Mesh::Mesh()
	{
		InitMesh();
	}

	Mesh::~Mesh()
	{
		AEGfxMeshFree(m_MeshVertexes);
	}

	void Mesh::InitMesh()
	{
		AEGfxMeshStart();

		AEGfxTriAdd(
			-30.0f, -30.0f, 0x00FF00FF, 0.0f, 1.0f,
			30.0f, -30.0f, 0x00FFFF00, 1.0f, 1.0f,
			-30.0f, 30.0f, 0x0000FFFF, 0.0f, 0.0f);
		AEGfxTriAdd(
			30.0f, -30.0f, 0x00FFFFFF, 1.0f, 1.0f,
			30.0f, 30.0f, 0x00FFFFFF, 1.0f, 0.0f,
			-30.0f, 30.0f, 0x00FFFFFF, 0.0f, 0.0f);

		m_MeshVertexes = AEGfxMeshEnd();
		AE_ASSERT_MESG(m_MeshVertexes, "Failed to create mesh!!");
	}

	void Mesh::RenderMesh(AEMtx33& _transform, AEGfxTexture* _texture, const float _r, const float _g, const float _b, float _a) const
	{
		AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
		AEGfxSetTransparency(1.0f);
		AEGfxSetTransform(_transform.m);
		AEGfxSetTintColor(_r, _g, _b, _a);
		if (_texture)
			AEGfxTextureSet(_texture, 0, 0);
		AEGfxMeshDraw(m_MeshVertexes, AE_GFX_MDM_TRIANGLES);
	}
}