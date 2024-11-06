//============================================
//
//	�����̏��� [bible.h]
//	Author:morikawa shunya
//
//============================================
#ifndef _BIBLE_H_
#define _BIBLE_H_

#include "main.h"
#include "item.h"

//�O���錾
class CObjGauge2D;

//�I�u�W�F�N�g�����N���X
class CBible : public CItem
{
public:

	CBible(int nPriority = 3);
	~CBible();

	//enum POS_TYPE
	//{
	//	POS_TYPE_LEFTUP = 0,	//����
	//	POS_TYPE_RIGHTUP,		//�E��
	//	POS_TYPE_LEFTDOWN,		//����
	//	POS_TYPE_RIGHTDOWN,		//�E��
	//	POS_TYPE_MAX,			//�񋓂̍ő吔
	//};

	HRESULT Init();
	void Uninit(void);
	void Update(void);
	void TitleUpdate(void);
	void GameUpdate(void);
	void Draw(void);

	void Move();

	void SetGrid(const GRID& pos) override;
	bool Hit(CPlayer* pPlayer) override;

	// �ÓI�����o�֐�
	static CListManager<CBible>* GetList(void); // ���X�g�擾

private:
	void StateManager(void);		//��ԊǗ�

	// �ÓI�����o�ϐ�
	static CListManager<CBible>* m_pList; // �I�u�W�F�N�g���X�g

	// �����o�ϐ�
	CListManager<CBible>::AIterator m_iterator; // �C�e���[�^�[

	float m_fMove;

	// �����o�ϐ�
	D3DXVECTOR3 m_posBase; // �ړ��̒��S���W
	float m_fMoveTime; // �ړ�����
};

#endif
