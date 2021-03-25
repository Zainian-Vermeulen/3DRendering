#include "Mesh.h"
#include <DirectXMath.h>
#include "Utils.h"
#include "Vertex.h"
#include  <string>

using namespace DirectX;

int Mesh::init(ID3D11Device* pD3DDevice, Object obj)
{
	Vertex::MeshData meshData;

	_obj = obj;
	
	int error = createVertexBuffer(pD3DDevice, meshData);
	if (error != 0) return error;

	error = createIndexBuffer(pD3DDevice, meshData);
	if (error != 0) return error;

	return 0;
}

void Mesh::render(ID3D11DeviceContext* pD3DDeviceContext)
{
	static UINT offset = 0;
	pD3DDeviceContext->IASetVertexBuffers(0, 1, &_pVertexBuffer, &_vertexStride, &offset);
	pD3DDeviceContext->IASetIndexBuffer(_pIndexBuffer, DXGI_FORMAT_R16_UINT, 0);
	pD3DDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	pD3DDeviceContext->DrawIndexed(_indexCount, 0, 0);
}


void Mesh::deInit()
{
	safeRelease<ID3D11Buffer>(_pVertexBuffer);
	safeRelease<ID3D11Buffer>(_pIndexBuffer);
}

int Mesh::createVertexBuffer(ID3D11Device* pD3DDevice, Vertex::MeshData& meshData)
{

	switch(_obj)
	{
	case Mesh::cube:
		{
			_vertexCount = 24;
			_vertexStride = sizeof(Vertex);
			
			Vertex vertices[] =
			{
				//quad - triangle strip or triangle list with index buffer

				//front
				Vertex(-0.5f, 0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f),
				Vertex(0.5f,  0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f),
				Vertex(-0.5f,-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, -1.0f),
				Vertex(0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f),

				//right
				Vertex(0.5f, 0.5f,-0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f),
				Vertex(0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f),
				Vertex(0.5f,-0.5f,-0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f),
				Vertex(0.5f,-0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f),

				//left
				Vertex(-0.5f, 0.5f,  0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f),
				Vertex(-0.5f, 0.5f, -0.5f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f),
				Vertex(-0.5f,-0.5f,  0.5f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f),
				Vertex(-0.5f,-0.5f, -0.5f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f),

				//back
				Vertex(0.5f,  0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f),
				Vertex(-0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f),
				Vertex(0.5f, -0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f),
				Vertex(-0.5f,-0.5f, 0.5f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f),

				//top
				Vertex(-0.5f, 0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f),
				Vertex(0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f),
				Vertex(-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f),
				Vertex(0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 1.0f, -0.0f),

				//bottom
				Vertex(0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f),
				Vertex(-0.5f,-0.5f, 0.5f, 1.0f, 0.0f, 0.0f, -1.0f, 0.0f),
				Vertex(0.5f, -0.5f, -0.5f,0.0f, 1.0f, 0.0f, -1.0f, 0.0f),
				Vertex(-0.5f,-0.5f, -0.5f,1.0f, 1.0f, 0.0f, -1.0f, 0.0f)
			};

			if (!createBuffer(_vertexCount * _vertexStride, vertices, pD3DDevice)) return 30;

			return 0;
		}
	case Mesh::sphere:
		{
			
			meshData.Vertices.clear();
			meshData.Indices.clear();

			_vertexStride = sizeof(Vertex);
			
			float radius = 1.0f;
			UINT sliceCount = 30;
			UINT stackCount = 30;
			
			Vertex topVertex(0.0f, +radius, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
			Vertex bottomVertex(0.0f, -radius, 0.0f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f);

			meshData.Vertices.push_back(topVertex);
			
			float phiStep = XM_PI / stackCount;
			float thetaStep = 2.0f * sliceCount / 10;

			for (UINT i = 1; i <= stackCount - 1; ++i)
			{
				float phi = i * phiStep;

				for (UINT j = 0; j <= sliceCount; ++j)
				{
					float theta = j * thetaStep;

					Vertex v = {};
					
					v.position.x = radius * sinf(phi) * cosf(theta);
					v.position.y = radius * cosf(phi);
					v.position.z = radius * sinf(phi) * sinf(theta);

					XMVECTOR normalized_position = XMVector3Normalize(XMLoadFloat3(&v.position));
					
					XMVECTOR tangent;
					XMVECTOR c1 = XMVector3Cross(normalized_position, XMVECTOR {0,0,1});
					XMVECTOR c2 = XMVector3Cross(normalized_position, XMVECTOR {0,1,0});

					
					if (XMVector3Greater(XMVector3Length(c1), XMVector3Length(c2)))
					{
						tangent = c1;
					}
					else
					{
						tangent = c2;
					}

					XMStoreFloat3(&v.t, XMVector3Normalize(tangent));

					v.uv.x = asinf(XMVectorGetX(normalized_position)) / XM_PI + 0.5f;
					v.uv.y = asinf(XMVectorGetY(normalized_position)) / XM_PI + 0.5f;
					
					XMStoreFloat3(&v.normal, normalized_position);

					meshData.Vertices.push_back(v);
				}
			}

			meshData.Vertices.push_back(bottomVertex);

			if (!createBuffer(meshData.Vertices.size() * _vertexStride, &meshData.Vertices[0], pD3DDevice)) return 30;

			return 0;
		}
	case Mesh::skybox:
		{
		meshData.Vertices.clear();
		meshData.Indices.clear();

		_vertexStride = sizeof(Vertex);

		float radius = 1.0f;
		UINT sliceCount = 30;
		UINT stackCount = 30;


		Vertex topVertex(0.0f, +radius, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
		Vertex bottomVertex(0.0f, -radius, 0.0f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f);

		meshData.Vertices.push_back(topVertex);

		float phiStep = XM_PI / stackCount;
		float thetaStep = 2.0f * sliceCount / 10;

		for (UINT i = 1; i <= stackCount - 1; ++i)
		{
			float phi = i * phiStep;

			for (UINT j = 0; j <= sliceCount; ++j)
			{
				float theta = j * thetaStep;

				Vertex vertex = {};

				vertex.position.x = radius * sinf(phi) * cosf(theta);
				vertex.position.y = radius * cosf(phi);
				vertex.position.z = radius * sinf(phi) * sinf(theta);

				XMVECTOR normalized_position = XMVector3Normalize(XMLoadFloat3(&vertex.position));

				XMVECTOR tangent;
				XMVECTOR cross1 = XMVector3Cross(normalized_position, XMVECTOR{ 0,0,1 });
				XMVECTOR cross2 = XMVector3Cross(normalized_position, XMVECTOR{ 0,1,0 });


				if (XMVector3Greater(XMVector3Length(cross1), XMVector3Length(cross2)))
				{
					tangent = cross1;
				}
				else
				{
					tangent = cross2;
				}

				XMStoreFloat3(&vertex.t, XMVector3Normalize(tangent));
				
				vertex.uv.x = asinf(XMVectorGetX(normalized_position)) / XM_PI + 0.5f;
				vertex.uv.y = asinf(XMVectorGetY(normalized_position)) / XM_PI + 0.5f;

				XMStoreFloat3(&vertex.normal, normalized_position);

				meshData.Vertices.push_back(vertex);
			}
		}

		meshData.Vertices.push_back(bottomVertex);

		if (!createBuffer(meshData.Vertices.size() * _vertexStride, &meshData.Vertices[0], pD3DDevice)) return 30;

		return 0;
		}
	}
}

bool Mesh::createBuffer(UINT byteWidth, Vertex* vertices, ID3D11Device* pD3DDevice)
{
	D3D11_BUFFER_DESC desc = {};
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	desc.ByteWidth = byteWidth;
	desc.Usage = D3D11_USAGE_IMMUTABLE;

	D3D11_SUBRESOURCE_DATA initialData = {};
	initialData.pSysMem = vertices;

	HRESULT hr = pD3DDevice->CreateBuffer(&desc, &initialData, &_pVertexBuffer);
	if (FAILED(hr)) return false;

	return true;
}

bool Mesh::createBuffer(UINT byteWidth, WORD* vertices, ID3D11Device* pD3DDevice)
{
	D3D11_BUFFER_DESC desc = {};
	desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	desc.ByteWidth = byteWidth;
	desc.Usage = D3D11_USAGE_IMMUTABLE;

	D3D11_SUBRESOURCE_DATA initialData = {};
	initialData.pSysMem = vertices;

	HRESULT hr = pD3DDevice->CreateBuffer(&desc, &initialData, &_pIndexBuffer);
	if (FAILED(hr)) return false;

	return true;
}

int Mesh::createIndexBuffer(ID3D11Device* pD3DDevice, Vertex::MeshData& meshData)
{
	switch (_obj)
	{
		case Mesh::cube:
			{
				_indexCount = 36;

				WORD indices[] =
				{
					//front
					0, 1, 2,
					1, 3, 2,
					//right
					4, 5, 6,
					5, 7, 6,
					//left
					8, 9, 10,
					9, 11, 10,
					//back
					12, 13, 14,
					13, 15, 14,
					//top
					16, 17, 18,
					17, 19, 18,
					//bottom
					20, 21, 22,
					21, 23, 22
				};

				if (!createBuffer(_indexCount * sizeof(WORD), indices, pD3DDevice)) return 35;

				return 0;
			}
		case Mesh::sphere:
		{
			UINT sliceCount = 30;
			UINT stackCount = 30;
				
			for (UINT i = 1; i <= sliceCount; ++i)
			{
				meshData.Indices.push_back(i);
				meshData.Indices.push_back(i + 1);
				meshData.Indices.push_back(0);
			}

			UINT baseIndex = 1;
			UINT ringVertexCount = sliceCount + 1;
			for (UINT i = 0; i < stackCount - 2; ++i)
			{
				for (UINT j = 0; j < sliceCount; ++j)
				{
					meshData.Indices.push_back(baseIndex + (i + 1) * ringVertexCount + j);
					meshData.Indices.push_back(baseIndex + i * ringVertexCount + j + 1);
					meshData.Indices.push_back(baseIndex + i * ringVertexCount + j);

					meshData.Indices.push_back(baseIndex + (i + 1) * ringVertexCount + j + 1);
					meshData.Indices.push_back(baseIndex + i * ringVertexCount + j + 1);
					meshData.Indices.push_back(baseIndex + (i + 1) * ringVertexCount + j);

				}
			}
				
			UINT southPoleIndex = (UINT)meshData.Vertices.size() - 1;

			// Offset the indices to the index of the first vertex in the last ring.
			baseIndex = southPoleIndex - ringVertexCount;

			for (UINT i = 0; i < sliceCount; ++i)
			{
				meshData.Indices.push_back(baseIndex + i + 1);
				meshData.Indices.push_back(baseIndex + i);
				meshData.Indices.push_back(southPoleIndex);
			}

			_indexCount = meshData.Indices.size();
				
			if (!createBuffer(meshData.Indices.size() * sizeof(WORD), &meshData.Indices[0], pD3DDevice)) return 35;

			return 0;
		}
		case Mesh::skybox:
			{
			UINT sliceCount = 30;
			UINT stackCount = 30;

			for (UINT i = 1; i <= sliceCount; ++i)
			{
				meshData.Indices.push_back(0);
				meshData.Indices.push_back(i + 1);
				meshData.Indices.push_back(i);
			}

			UINT baseIndex = 1;
			UINT ringVertexCount = sliceCount + 1;
			for (UINT i = 0; i < stackCount - 2; ++i)
			{
				for (UINT j = 0; j < sliceCount; ++j)
				{
					meshData.Indices.push_back(baseIndex + i * ringVertexCount + j);
					meshData.Indices.push_back(baseIndex + i * ringVertexCount + j + 1);
					meshData.Indices.push_back(baseIndex + (i + 1) * ringVertexCount + j);

					meshData.Indices.push_back(baseIndex + (i + 1) * ringVertexCount + j);
					meshData.Indices.push_back(baseIndex + i * ringVertexCount + j + 1);
					meshData.Indices.push_back(baseIndex + (i + 1) * ringVertexCount + j + 1);
				}
			}

			UINT southPoleIndex = (UINT)meshData.Vertices.size() - 1;

			// Offset the indices to the index of the first vertex in the last ring.
			baseIndex = southPoleIndex - ringVertexCount;

			for (UINT i = 0; i < sliceCount; ++i)
			{
				meshData.Indices.push_back(southPoleIndex);
				meshData.Indices.push_back(baseIndex + i);
				meshData.Indices.push_back(baseIndex + i + 1);
			}

			_indexCount = meshData.Indices.size();

			if (!createBuffer(meshData.Indices.size() * sizeof(WORD), &meshData.Indices[0], pD3DDevice)) return 35;

			return 0;
			}
	}
}
