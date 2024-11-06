//============================================
//
//	�X�����u�̏��� [SlopeDevice.h]
//	Author:sakai minato
//
//============================================
#ifndef _SLOPE_DEVICE_H_
#define _SLOPE_DEVICE_H_

#include "object.h"

#define MODEL_NUM		(64)	// ���f���̐�

class CModel;
class CMotion;

//�I�u�W�F�N�g�v���C���[�N���X
class CSlopeDevice : public CObject
{
public:

	CSlopeDevice(int nPriority = 3);
	~CSlopeDevice();

	// ���
	enum STATE
	{
		STATE_NORMAL = 0,	// �ʏ�
		STATE_ACTION,		// �s��
		STATE_MAX,
	};

	// ���[�V����
	enum ACTION_TYPE
	{
		ACTION_NEUTRAL = 0,	// �ҋ@���
		ACTION_MAX,			// �ő�
	};

	static CSlopeDevice* Create(const char* pModelNameSlopeDevice, const char* pModelNameEnemy);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void TitleUpdate(void);
	void GameUpdate(void);
	void Draw(void);

	void SetPos(D3DXVECTOR3 pos) { m_pos = pos; }
	D3DXVECTOR3 GetPos(void) { return m_pos; }

	void SetMultiMatrix(bool Set) { m_bMultiMatrix = Set; }
	bool GetMultiMatrix(void) { return m_bMultiMatrix; }
	void SetUseMultiMatrix(D3DXMATRIX Set) { m_UseMultiMatrix = Set; }

	// �ÓI�����o�֐�
	static CListManager<CSlopeDevice>* GetList(void); // ���X�g�擾

private:
	void InitModel(const char* pModelNameSlopeDevice, const char* pModelNameEnemy);

	void StateManager(void);		// ��ԊǗ�

	void LoadModel(const char* pFilename);

	D3DXVECTOR3 m_pos;			//�ʒu
	D3DXVECTOR3 m_posOld;		//�ߋ��̈ʒu
	D3DXVECTOR3 m_rot;			//����
	D3DXVECTOR3 m_size;			//�傫��

	D3DXMATRIX m_mtxWorld;		//���[���h�}�g���b�N�X

	STATE m_State;					//���
	int m_nStateCount;				//��ԊǗ��p�ϐ�

	//�K�w�\���ƃ��[�V�����̃|�C���^
	CModel* m_apModel[MODEL_NUM];
	CMotion* m_pMotion;
	char* m_aModelName[MODEL_NUM];
	int m_nNumModel;
	char m_cFileName[128];		//�t�@�C���̖��O

	//�}�b�v�Ƃ̃}�g���b�N�X���
	bool m_bMultiMatrix;					// �}�g���b�N�X�̊|�����킹�����邩�ǂ���
	D3DXMATRIX m_UseMultiMatrix;			// �|�����킹��}�g���b�N�X

	// �ÓI�����o�ϐ�
	static CListManager<CSlopeDevice>* m_pList; // �I�u�W�F�N�g���X�g

	// �����o�ϐ�
	CListManager<CSlopeDevice>::AIterator m_iterator; // �C�e���[�^�[

};
#endif