// ======================================================================
// File Name:		Mesh.h
// Project Name:	Zoodiac
// Author(s):		Low Zhi Sheng Kitson(60%), Lim Geng Yang(20%),
//					Lye Pin Liang Xavier(20%)
// Brief:			Contains Mesh functionality
// 
// All content © 2023 DigiPen Institute of Technology Singapore. All 
// rights reserved.
// ======================================================================

#ifndef MESH_H_
#define MESH_H_

#include <AEEngine.h>

namespace Backend
{
	/**
	 * \brief	Mesh class for rendering 2D objects
	 */
	class Mesh
	{
	private:
		AEGfxVertexList* m_MeshVertexes;	//	Vertex list for the mesh
	public:
		/**
		 * \brief	Constructor
		 */
		Mesh();

		/**
		 * \brief	Destructor
		 */
		~Mesh();

		/**
		 * \brief	Initializes the mesh
		 */
		void InitMesh();

		/**
		 * \brief				Renders a mesh.
		 * \param _transform	Transformation matrix of the mesh.
		 * \param _texture		Texture of the mesh.
		 * \param _r			Red color value.
		 * \param _g			Green color value.
		 * \param _b			Blue color value.
		 * \param _a			Alpha value.
		 */
		void RenderMesh(AEMtx33& _transform, AEGfxTexture* _texture, float _r, float _g, float _b, float _a) const;
	};
}

#endif