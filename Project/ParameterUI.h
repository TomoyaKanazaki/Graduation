//============================================
//
//	�v���C���[�̃p�����[�^���Ǘ�����UI [ParamererUI.h]
//	Author:sakamoto kai
//
//============================================
#ifndef _PARAMETERUI_H_
#define _PARAMETERUI_H_

#include "main.h"
#include "object2D.h"

// �O���錾
class CObjGauge2D;

// �}�N����`
#define STAMINA_MAX (2)		//�X�^�~�i�Q�[�W�̐�

//�I�u�W�F�N�g�v���C���[�N���X
class CParameterUI : public CObject2D
{
public:

	CParameterUI(int nPriority = 6);
	~CParameterUI();

	static CParameterUI* Create(int nPriority = 6);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	//�擾�����E�ݒ菈��
	D3DXVECTOR3 GetPos(void) { return m_pos; }
	void SetPos(D3DXVECTOR3 pos);
	void SetLife(float Life);
	float GetLifeMax(void) { return m_fLifeMax; }

	void SetStamina(float Stamina);
	void SetStaminaCnt(int Count) { m_nStaminaCnt = Count; }
	int GetUseStamina(void) { return m_nUseStamina; }

private:
	void LifeGaugeCreate();			//���C�t�Q�[�W����
	void DelLifeGaugeCreate(float Damage);		//���C�t�Q�[�W����
	void DelLifeGaugeUpdate();		//���C�t�Q�[�W�X�V
	void StaminaGaugeCreate();		//�X�^�~�i�Q�[�W����
	void StaminaGaugeUpdate();		//�X�^�~�i�Q�[�W�X�V

	D3DXVECTOR3 m_pos;				//�ʒu

	float m_fLife;					//���C�t
	float m_fLifeMax;				//���C�t�̍ő�l
	int m_fDelLifeCoolTime;			//�������C�t�̃N�[���^�C��
	float m_fDelLifeCount;			//�������C�t�̃J�E���g
	float m_fDelLifeCountMax;		//�������C�t�̃J�E���g�̍ő�l
	float m_fStamina[STAMINA_MAX];	//�X�^�~�i
	float m_fStaminaMax;			//�X�^�~�i�ő�l
	int m_nStaminaCnt;				//�X�^�~�i�񕜃J�E���g
	int m_nUseStamina;				//�g�p����X�^�~�i�Q�[�W

	CObjGauge2D* m_pLifeGauge;			//���C�t�Q�[�W�̃|�C���^
	CObjGauge2D* m_pDelLifeGauge;		//������ƌ��郉�C�t�Q�[�W�̃|�C���^
	CObjGauge2D* m_pStaminaGauge[STAMINA_MAX];	//�X�^�~�i�Q�[�W�̃|�C���^
};
#endif