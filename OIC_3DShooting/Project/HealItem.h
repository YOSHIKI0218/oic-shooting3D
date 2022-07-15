#pragma once
#include "ItemBase.h"
class CHealItem :
    public CItemBase
{
private:
    CMeshContainer      m_Mesh;
public:
    void Load() override;
    void Initalize() override;
    void Update() override;
    void Render() override;
    void Release() override;
    int GetType() override;
    CSphere GetSphere() override;
    void Start(const Vector3& pos) override;
};

