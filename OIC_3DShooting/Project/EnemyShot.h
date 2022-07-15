#pragma once
#include "Mof.h"

class CEnemyShot
{
private:
	CMeshContainer*		m_pMesh;
	Vector3				m_Pos;
	Vector3				m_Spd;
	Vector3				m_PlayerPos;
	bool				m_bShow;
	float				m_RotY;
	float				m_HormingWaitTime;
	float					m_LifeTime;
	bool				m_bHorming;
	Vector3				m_HormingSpeed;

public:
	CEnemyShot();
	~CEnemyShot();
	void Initalize();

	/// <summary>
	/// 弾を発射する
	/// </summary>
	/// <param name="p">開始位置</param>
	/// <param name="s">速度</param>
	void Fire(const Vector3& p, const Vector3& s);
	void Update();
	void Render();
	void RenderDebug();
	void SetMesh(CMeshContainer* pm) { m_pMesh = pm; }
	bool GetShow() { return m_bShow; }
	void SetShow(bool bs) { m_bShow = bs; }
	void SetPlayerPos(const Vector3& pos) {
		m_PlayerPos = pos;
	};
	void SetHorming(bool b) { m_bHorming = b; }

	/// <summary>
	/// 当たり判定の球の取得
	/// </summary>
	/// <returns></returns>
	CSphere GetSphere() { return CSphere(m_Pos, 0.2f); }

	/// <summary>
	/// 利用可能な弾を探す
	/// </summary>
	/// <param name="shot">敵弾配列のポインタ</param>
	/// <param name="smax">弾の最大数</param>
	/// <returns>非表示の弾のポインタ</returns>
	static CEnemyShot* FindAvailableShot(CEnemyShot* shot, int smax);
	void Move();
	void SetLifeTime(int time) { m_LifeTime = time; }
};

