#pragma once
#include "GameDefine.h"
#include "EnemyShot.h"

//�{�X1
class CBoss1
{
private:
	CMeshContainer		m_Mesh;			//�{�X�{�̂̃��b�V��
	CMeshContainer		m_ShieldMesh;	//�V�[���h���b�V��
	Vector3				m_Pos;			//���W
	Vector3				m_Rot;			//��]�p�x�i���W�A���j1rad�͖�57.29deg
	CMatrix44			m_matWorld;		//�S�̂̃��[���h�}�g���N�X
	CMatrix44			m_matShield;	//�V�[���h�̍s��(�}�g���N�X)
	CMatrix44			m_matScale;		//�X�P�[���s��
	bool				m_bShow;		//�\�����

	int					m_MaxHP;		//�ő�̗�
	int					m_HP;			//�̗�
	int					m_ShotWait;		//�e���˂̑҂�����
	int					m_ShotInterval;//�e���˂̃C���^�[�o��
	Vector3				m_TargetPos;	//�e��]�A�j���[�V�����I����Ɍ������W
	float				m_AnimTime;		//��]�A�j���[�V�����̌o�ߎ���

	bool				m_bDestroyed;//�j�󂳂ꂽ��

	int					m_CurrentShotPattern;//���݂̒e���˃p�^�[���ԍ�
	int					m_PrevShotPattern;//���O�̔��˃p�^�[��
	int					m_ExplosionInterval;//�����̊Ԋu

	float				m_TargetAngleP;//�^�[�Q�b�g�p�x(+)
	float				m_TargetAngleM;//�^�[�Q�b�g�p�x(-)

	float				m_ChangeTime;//�p�^�[���ύX�Ɏg�p���鎞��
	float				m_TimeElapsed;//�J�n��̌o�ߎ���
	bool				m_bDestroyedAndRemoved;//�j�󂳂�ď��ł�����
	bool				m_bFlip;//��]���t���O
	float				m_FlipTime;//��]�Ɏg�p���鎞��
	bool				m_bUsedShield;//�V�[���h�W�J�������Ƃ����邩�ǂ���
	float				m_ShieldTime;//�V�[�h���W�J�c�莞��
	float				m_MeshScale;//���b�V���̃X�P�[��
	int					m_BossType;//�{�X�̎�ށB0�ŃX�e�[�W2�̃{�X�B1��OiC
	float				m_PatternChangeInterval;//�U���p�^�[���ύX�Ԋu
	float				m_ExtremeAttackWaitTime;//���U���O�̑҂�����
	CMeshContainer		m_AttackAreaMesh;//�U���͈͂̃��b�V��
	int					m_AttackAreaTime;//�U���͈͂̓_�ŗp
	bool				m_bShowAttackArea;//�U���͈͂̓_�ŗp

	void UpdateShot(CEnemyShot* shot, int smax);
public:
	/**
	 * �ǂݍ���
	 *
	 */
	bool Load(LPCMofChar modelName);


	/**
	 * ������
	 *
	 */
	void Initialize();

	/**
	 * �J�n
	 *
	 */
	void Start();

	/**
	 * �X�V
	 *
	 */
	void Update(CEnemyShot* shot, int smax);

	/**
	 * �_���[�W����
	 *
	 */
	void Damage(int dmg, const Vector3& bulletPos);

	/**
	 * �`��
	 *
	 */
	void Render();

	/**
	 * �f�o�b�O�`��
	 *
	 */
	void RenderDebug();

	/**
	 * �\����Ԏ擾
	 *  �߂�l��true�Ȃ�\�����Afalse�Ȃ��\��
	 */
	bool GetShow() { return m_bShow; }

	/**
	 * �Փˋ���Ԃ�
	 *
	 */
	CSphere GetSphere();

	/**
	 * �^�[�Q�b�g���W�̐ݒ�
	 *
	 */
	void SetTargetPos(const Vector3& t) { m_TargetPos = t; }

	/**
 * ���
 *
 */
	void Release();

	/**
	 * ����̕����։�]����
	 *
	 */
	void RotateTarget();

	/// <summary>
	/// �V�[���h�̍X�V
	/// </summary>
	void UpdateShield();

	/**
	 * �S���ʂւ̒e����
	 *
	 */
	void ShotAllDirShot(CEnemyShot* shot, int smax, int sCnt);

	/// <summary>
	/// �~�`����
	/// </summary>
	/// <param name="shot">�G�e�̔z��̃|�C���^</param>
	/// <param name="smax">�e�̍ő吔</param>
	/// <param name="sCnt">���ː��BsCnt�~2���݂ɔ��˂���</param>
	void ShotCircleDir(CEnemyShot* shot, int smax, int sCnt);

	/// <summary>
	/// �T�C������
	/// </summary>
	/// <param name="shot">�G�e�̔z��̃|�C���^</param>
	/// <param name="smax">�e�̍ő吔</param>
	/// <param name="sCnt">���ː��BsCnt�~2�̒e�𔭎˂���</param>
	void ShotSineWave(CEnemyShot* shot, int smax, int sCnt);

	/// <summary>
/// ���s����(��)
/// </summary>
/// <param name="shot">�G�e�̔z��̃|�C���^</param>
/// <param name="smax">�e�̍ő吔</param>
/// <param name="sCnt">0�ň�����A1�ȏ��1 + sCnt�~2�̒e�𔭎˂���</param>
	void ShotExtremeParallel(CEnemyShot* shot, int smax, int sCnt);

	/// <summary>
	/// ���s����
	/// </summary>
	/// <param name="shot">�G�e�̔z��̃|�C���^</param>
	/// <param name="smax">�e�̍ő吔</param>
	/// <param name="sCnt">0�ň�����A1�ȏ��1 + sCnt�~2�̒e�𔭎˂���</param>
	void ShotParallel(CEnemyShot* shot, int smax, int sCnt);

	/// <summary>
	/// �w�萔����
	/// </summary>
	/// <param name="shot">�G�e�̔z��̃|�C���^</param>
	/// <param name="smax">�e�̍ő吔</param>
	/// <param name="sCnt">0�ň�����A1�ȏ��1 + sCnt�~2�̒e�𔭎˂���</param>
	/// <param name="horming">�z�[�~���O���邩�ǂ���</param>
	void Shot(CEnemyShot* shot, int smax, int sCnt, bool horming);

	/// <summary>
	/// �j�󂳂�A���ł������ǂ���
	/// </summary>
	/// <returns></returns>
	bool IsDestroyedAndRemoved() { return m_bDestroyedAndRemoved; }

	/// <summary>
	/// �{�X1�̌��݂�HP���擾����
	/// </summary>
	/// <returns>�{�X1�̌��݂�HP</returns>
	int GetHP() { return m_HP; }

	/// <summary>
	/// �{�X1�̍ő�HP���擾����
	/// </summary>
	/// <returns>�{�X1�̍ő�HP</returns>
	int GetMaxHP() { return m_MaxHP; }

	/// <summary>
	/// ���b�V���̃X�P�[����ݒ肷��B
	/// </summary>
	/// <param name="value">�{�X�̃X�P�[��</param>
	void SetMeshScale(float value);

	/// <summary>
	/// �{�X�̎�ނ�ݒ肷��
	/// </summary>
	/// <param name="value">�l</param>
	void SetBossType(int value) { m_BossType = value; }

	/// <summary>
	/// �{�X�̎�ނ��擾����B
	/// </summary>
	/// <returns>�{�X�̎��</returns>
	int GetBossType() { return m_BossType; }
};

