#include "EffectManager.h"

CDynamicArray<CParticle*> CEffectManager::m_Effects;
extern bool gbShowDebugLog;

CParticle* CEffectManager::Spawn(int id)
{
    for (int i = 0; i < m_Effects.GetArrayCount(); i++)
    {
        if (m_Effects[i]->GetID() == id && m_Effects[i]->IsEnd()) 
        {
            m_Effects[i]->Start();
            return m_Effects[i];
        }
    }
    MOF_PRINTLOG("active particle ID %d not found\n", id);
    return nullptr;
}

CParticle* CEffectManager::Spawn(int id, CParticle::CREATEINFO& info)
{
    for (int i = 0; i < m_Effects.GetArrayCount(); i++)
    {
        if (m_Effects[i]->GetID() == id && m_Effects[i]->IsEnd())
        {
            m_Effects[i]->Initalize(info);
            m_Effects[i]->Start();
            return m_Effects[i];
        }
    }
    CParticle* temp = new CParticle();
    temp->Initalize(info);
    m_Effects.Add(temp);
    return temp;
}

CParticle* CEffectManager::Create(CParticle::CREATEINFO& info)
{
    CParticle* temp = new CParticle();
    temp->Initalize(info);
    m_Effects.Add(temp);
    return temp;
}

void CEffectManager::Update()
{
    if(gbShowDebugLog)
    MOF_PRINTLOG("Effectmanager Update\n");
    for (int i = 0; i < m_Effects.GetArrayCount(); i++)
    {
        m_Effects[i]->Update();
    }
}

void CEffectManager::Render()
{
    if (gbShowDebugLog)
        MOF_PRINTLOG("Effectmanager Render\n");
    for (int i = 0; i < m_Effects.GetArrayCount(); i++)
    {
        m_Effects[i]->Render();
    }
}

void CEffectManager::Release()
{
    if (gbShowDebugLog)
        MOF_PRINTLOG("Effectmanger Release\n");
    for (int i = 0; i < m_Effects.GetArrayCount(); i++)
    {
        m_Effects[i]->Release();
    }

    m_Effects.Release();
}
