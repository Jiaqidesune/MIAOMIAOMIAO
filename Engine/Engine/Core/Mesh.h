#pragma once

#include <vector>

#include "../math/Vector3.h"
#include "../math/Vector4.h"
#include "../math/Matrix4x4.h"
#include "../bvh/SplitBvh.h"

namespace Rendering
{
	class Mesh
	{
	public:
		Mesh()
			: loaded(false)
		{
			m_bvh = new SplitBvh(2.0f, 64, 0, 0.001f, 2.5f);
		}

		~Mesh()
		{
			if (m_bvh)
			{
				delete m_bvh;
				m_bvh = nullptr;
			}
		}

		void BuildBVH();
		bool LoadFromFile(const std::string& filename);

	public:
		std::vector<Vector4> m_Vertex;
		std::vector<Vector4> m_Normal;

		Bvh* m_bvh;
		std::string m_name;
		bool loaded;
	};

	class MeshInstance
	{
	public:
		MeshInstance(int meshId, Matrix4x4 xform, int matId, const std::string& inName)
			: transform(xform)
			, materialID(matId)
			, meshID(meshId)
			, name(inName) {}

		~MeshInstance()	{}

		Matrix4x4	transform;
		int			materialID;
		int			meshID;
		std::string name;
	};
}