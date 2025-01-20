//===========================================================
//
// �G�t�F�N�V�A[MyEffekseer.h]
// Author �匴�叫
//
//===========================================================
#ifndef _EFFEKSEER_H_     //���̃}�N������`����ĂȂ�������
#define _EFFEKSEER_H_     //2�d�C���N���[�h��h�~�̃}�N�����`����

#include <list>

//===========================================================
// �O���錾
//===========================================================v
class CEffekseer;

//===========================================================
// �G�t�F�N�V�A�̃R���g���[���N���X��`
//===========================================================
class CMyEffekseer
{
public:
	CMyEffekseer();   // �R���X�g���N�^
	~CMyEffekseer();  // �f�X�g���N�^

	// ���
	enum TYPE
	{
		TYPE_FIRE = 0,				// �t�@�C�A�{�[��
		TYPE_CHARGE,				// �Ή����˃`���[�W
		TYPE_SMOKE,					// �X���[�N
		TYPE_HIT,					// �q�b�g
		TYPE_GETITEM,				// �����A�\���ˎ擾
		TYPE_EAT,					// �ߐH
		TYPE_RESPAWN_MEDAMAN,		// ���_�}���̃��X�|�[��
		TYPE_RESPAWN_BONBON,		// �{���{���̃��X�|�[��
		TYPE_RESPAWN_SMALLDEVIL,	// �R�f�r���̃��X�|�[��
		TYPE_BOABOA,				// �{�A�{�A�G�t�F�N�g
		TYPE_CROSS,					// �\���˃G�t�F�N�g
		TYPE_BIBLE,					// ����
		TYPE_DUSTCLOUD,				// ���������̉�
		TYPE_FALLSMOKE,				// �������̓y��
		TYPE_BORN_PLAYER_00,		// ��������00
		TYPE_BORN_PLAYER_01,		// ��������01
		TYPE_DROP,					// �^�}�S���̊�
		TYPE_ACCELE,				// �^�}�S���̉���
		TYPE_ROLL,					// �΂��]����
		TYPE_HITTHEWALL,			// �ǂɉ΂�������
		TYPE_HIT_SMALLDEVIL,		// ���f�r��������
		TYPE_STEAM,					// ������o�铒�C
		TYPE_BORNGUID1,				// �����K�C�h�@�v���C���[1
		TYPE_BORNGUID2,				// �����K�C�h�@�v���C���[2
		TYPE_GOT_THEITEM,			// �A�C�e�������Ă���Ƃ�
		TYPE_ACENTION,				// ���V
		TYPE_REVIVE_MEDAMAN,		// ���_�}���̕����K�C�h
		TYPE_REVIVE_BONBON,			// �{���{���̕����K�C�h
		TYPE_REVIVE_SMALLDEVIL,		// �q�f�r���̕����K�C�h
		TYPE_GET_SOFTCREAM,			// �\�t�g�N���[���擾
		TYPE_SOFTCREAM,				// �\�t�g�N���[���̃I�[��
		TYPE_CRASH_CROSS,			// �\���˂�������
		TYPE_SPAWN_DEVIL,			// �f�r���̏o��
		TYPE_QUAKE,					// �h��鎞�̔j��
		TYPE_TRUE,					// �f�o�b�Otrue
		TYPE_STAGE_LIMIT,			// �X�e�[�W�[
		TYPE_UNLOCKING,				// ����
		TYPE_MAX,
		TYPE_NONE					// �Ȃ���Ȃ�
	};

	void Init(void);       // ����������
	void Uninit(void);     // �I������
	void Update(void);     // �X�V����
	void Draw(void);       // �`�揈��

	// �擾
	::EffekseerRendererDX9::RendererRef GetEffekseerRenderer(void) { return m_EfkRenderer; }  // �G�t�F�N�g�����_���[
	::Effekseer::ManagerRef GetEfkManager(void) { return m_EfkManager; }                      // �G�t�F�N�g�}�l�[�W��

	// ���W���[��
	void SetupEffekseerModules(::Effekseer::ManagerRef efkManager);

	CEffekseer* CreateEffect(const TYPE eType, ::Effekseer::Vector3D pos, ::Effekseer::Vector3D rot, ::Effekseer::Vector3D scale, bool bLoop);

private:

	EffekseerRendererDX9::RendererRef m_EfkRenderer;
	::Effekseer::Matrix44 m_CameraMatrix;
	::Effekseer::Matrix44 m_ProjectionMatrix;
	::Effekseer::ManagerRef m_EfkManager;
	::Effekseer::Vector3D m_ViewerPosition;
	int m_nNum;
	int32_t m_nTime;
};

//===========================================================
// �G�t�F�N�V�A�N���X��`
//===========================================================
class CEffekseer
{
public:
	CEffekseer();     // �R���X�g���N�^
	~CEffekseer();    // �f�X�g���N�^

	void Init(Effekseer::Vector3D pos, Effekseer::Vector3D rot, Effekseer::Vector3D scale, bool bLoop); // ����������
	void Uninit(void); // �I������ 

	// �ݒ�
	void SetPosition(Effekseer::Vector3D pos) { m_pos = pos; } // �ʒu
	void SetRotation(Effekseer::Vector3D rot) { m_rot = rot; } // ����
	void SetScale(Effekseer::Vector3D scale) { m_scale = scale; } // �傫��
	void SetEffect(Effekseer::EffectRef effect) { m_effect = effect; } // �G�t�F�N�g
	void SetEfkHandle(Effekseer::Handle handle) { m_efkHandle = handle; } // �n���h��
	void SetEfkName(const char* pName) { m_EfkName = pName; } // �G�t�F�N�g�̖��O
	void SetEfkType(const CMyEffekseer::TYPE eType) { m_eType = eType; } // �G�t�F�N�g�̎��
	void SetDeath(); // �I���ݒ�

	// �擾
	Effekseer::Vector3D GetPosition(void) { return m_pos; }				// �ʒu
	Effekseer::Vector3D GetRotation(void) { return m_rot; }				// ����
	Effekseer::Vector3D GetScale(void) { return m_scale; }				// �傫��
	Effekseer::EffectRef GetEffect(void) { return m_effect; }			// �G�t�F�N�g
	Effekseer::Handle GetHandle(void) { return m_efkHandle; }			// �n���h��
	const char* GetEfkName(void) { return m_EfkName; }					// �G�t�F�N�g�̖��O
	CMyEffekseer::TYPE GetEfkType() { return m_eType; }					// �G�t�F�N�g�̎��
	bool IsLoop(void) { return m_bLoop; }								// ���[�v����t���O
	bool IsDeath(void) { return m_bDeath; }								// ���S�t���O

	// �ÓI�����o�֐�
	static CListManager<CEffekseer>* GetList(); // ���X�g�擾

private:

	// �����o�ϐ�
	Effekseer::Vector3D m_pos; // �ʒu
	Effekseer::Vector3D m_rot; // ����
	Effekseer::Vector3D m_scale; // �傫��
	Effekseer::EffectRef m_effect; // �G�t�F�N�g
	Effekseer::Handle m_efkHandle; // �n���h��
	const char* m_EfkName; // ���O
	CMyEffekseer::TYPE m_eType; // ���
	bool m_bLoop; // ���[�v����t���O
	bool m_bDeath; // ���S�t���O
	CListManager<CEffekseer>::AIterator m_iterator; // �C�e���[�^�[

	// �ÓI�����o�ϐ�
	static CListManager<CEffekseer>* m_pList; // �I�u�W�F�N�g���X�g
};

namespace MyEffekseer
{
	CEffekseer* EffectCreate(CMyEffekseer::TYPE type, 
		                     bool bLoop = false,
		                     D3DXVECTOR3 pos = { 0.0f, 0.0f, 0.0f }, 
		                     D3DXVECTOR3 rot = { 0.0f, 0.0f, 0.0f },
		                     D3DXVECTOR3 scale = { 25.0f, 25.0f, 25.0f } );
}

#endif