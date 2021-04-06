#if !defined(__FBX_LOAD_MESH_H__)
#define __FBX_LOAD_MESH_H__

#include "FBXLoader.h"
#include <algorithm>

namespace fbx {

void CleanupMesh(MeshList &meshList);
void GetMesh(FbxScene *pScene, MeshList &meshList);

}; // fbx

#endif // __FBX_LOAD_MESH_H__