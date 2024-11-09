//==========================================
//
//  �ڋʏĂ��N���X(friedegg.h)
//  Author : Tomoya Kanazaki
//
//==========================================
#pragma once
#include "item.h"
#include "enemy.h"

//==========================================
//  �N���X��`
//==========================================
class CFriedEgg : public CItem
{
public:

	// �����o�֐�
	CFriedEgg(int nPriority = 3);
	~CFriedEgg();

	HRESULT Init();
	void Uninit(void);
	void Update(void);
	void Draw(void);

	bool Hit(CPlayer* pPlayer) override;

	void SetEnemy(const CEnemy::ENEMY_TYPE eType) { m_eCreateType = eType; };

	// �ÓI�����o�֐�
	static CListManager<CFriedEgg>* GetList(void); // ���X�g�擾
	static CFriedEgg* Create(const CEnemy::ENEMY_TYPE eType, const CMapSystem::GRID& pos);

private:

	// �����o�֐�
	void Move(D3DXVECTOR3& pos) override;

	// �ÓI�����o�ϐ�
	static CListManager<CFriedEgg>* m_pList; // �I�u�W�F�N�g���X�g

	// �����o�ϐ�
	CListManager<CFriedEgg>::AIterator m_iterator; // �C�e���[�^�[
	float m_fDeleteTimer; // �������Ń^�C�}�[
	bool m_bMove; // �ړ��\�t���O
	CEnemy::ENEMY_TYPE m_eCreateType; // ���ł������ɐ�������G�̎��

};
