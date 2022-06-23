#pragma once

#include "GameDefine.h"
#include "Enemy.h"

#define BOSS_PARTS_MAX (3)

class CBoss{
private:
	CMeshContainer		m_Mesh;			//ボス本体のメッシュ
	CMeshContainer		m_PartsMesh;	//周囲のパーツのメッシュ
	Vector3				m_Pos;			//座標
	Vector3				m_Rot;			//回転角度（ラジアン）
	CMatrix44			m_matWorld;		//全体のワールドマトリクス
	bool				m_bShow;		//表示状態

	int					m_HP;			//体力
	int					m_ShotWait;		//弾発射の待ち時間
	Vector3				m_TargetPos;	//各回転アニメーション終了後に向く座標
	float				m_AnimTime;		//回転アニメーションの経過時間

	CEnemy				m_Parts[BOSS_PARTS_MAX];	//周囲のパーツ管理用CEnemyのインスタンス

public:
	CBoss();
	~CBoss();
	
	/**
	 * 読み込み
	 *
	 */
	bool Load();

	/**
	 * 初期化
	 *
	 */
	void Initialize();

	/**
	 * 開始
	 *
	 */
	void Start();

	/**
	 * 更新
	 *
	 */
	void Update(CEnemyShot* shot,int smax);

	/**
	 * ダメージ処理
	 *
	 */
	void Damage(int dmg);

	/**
	 * 描画
	 *
	 */
	void Render();

	/**
	 * デバッグ描画
	 *
	 */
	void RenderDebug();

	/**
	 * 表示状態取得
	 *  戻り値がtrueなら表示中、falseなら非表示
	 */
	bool GetShow(){ return m_bShow; }
	
	/**
	 * 衝突球を返す
	 *
	 */
	CSphere GetSphere(){ return CSphere(m_Pos,3.1f); }
	
	/**
	 * ターゲット座標の設定
	 *
	 */
	void SetTargetPos(const Vector3& t){ m_TargetPos = t; }
	
	/**
	 * i番目のパーツを取得
	 *
	 */
	CEnemy& GetParts(int i){ return m_Parts[i]; }

	/**
	 * 表示中のパーツの数を返す
	 *
	 */
	int GetPartsCount();

	/**
	 * 解放
	 *
	 */
	void Release();
	
	/**
	 * 相手の方向へ回転する
	 *
	 */
	void RotateTarget() {
		Vector3 direction = m_TargetPos - (m_Pos);
		float dr = atan2(direction.x, direction.z) + MOF_MATH_PI;
		float r = fmodf((dr - m_Rot.y) + MOF_MATH_PI, MOF_MATH_2PI);
		m_Rot.y += ((0 < r) ? r - MOF_MATH_PI : r + MOF_MATH_PI) * 0.1f;
	}
	
	/**
	 * 全方位への弾発射
	 *
	 */
	void ShotAllDirShot(CEnemyShot* shot, int smax, int sCnt) {
		if (m_ShotWait > 0) {
			m_ShotWait--;
			return;
		}

		float ad = ((float)rand() / RAND_MAX) * 360.0f / sCnt;
		for (int cnt = 0; cnt < sCnt; cnt++) {
			CEnemyShot* newShot = CEnemyShot::FindAvailableShot(shot, smax);
			if (newShot) {
				m_ShotWait = 20;
				float rad = MOF_ToRadian(ad);
				Vector3 vt(cos(rad), 0, sin(rad));
				newShot->Fire(m_Pos, vt * 0.2f);
			}
			ad += 360.0f / sCnt;
		}
	}
	
	/**
	 * パーツ更新
	 *
	 */
	void UpdateParts(int idx, CEnemyShot* shot, int smax, bool bShot) {
		CEnemy& parts = m_Parts[idx];
		float angle = -(MOF_MATH_2PI * idx / BOSS_PARTS_MAX);

		CVector3 p(0.0f, 3.5f, 0.0f);
		p.RotationZ(angle);
		p *= m_matWorld;
		parts.SetPosition(p);

		CVector3 r(m_Rot);
		r.z += angle;
		parts.SetRotation(r);

		if (g_BossAnimPosY[1].Time <= m_AnimTime && bShot) {
			parts.SetTargetPos(m_TargetPos);
			parts.Update(shot, smax);
		}
	}
};