//============================================
//
//	�\���˃A�C�e���̏��� [cross.h]
//	Author:sakamoto kai
//
//============================================
#ifndef _CROSS_H_
#define _CROSS_H_

#include "main.h"
#include "item.h"

//�O���錾
class CObjGauge2D;

//�I�u�W�F�N�g�\���˃N���X
class CCross : public CItem
{
public:

	CCross(int nPriority = 3);
	~CCross();

	//�T���v���̏��
	enum STATE
	{
		STATE_NORMAL = 0,	//�ʏ�
		STATE_ACTION,		//�s��
		STATE_MAX,
	};

	static CCross* Create(char* pModelName);

	HRESULT Init(char* pModelName);
	void Uninit(void);
	void Update(void);
	void TitleUpdate(void);
	void GameUpdate(void);
	void Draw(void);

	bool CollisionPlayer() override;


private:
	void StateManager(void);	//��ԊǗ�

};
#endif