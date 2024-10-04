#pragma once
//==========================================
//
//  �^�C�}�[�N���X(timer.h)
//  Author : Tomoya Kanazaki
//
//==========================================
#include "object.h"

//==========================================
//  �O���錾
//==========================================
class CNumber;

//==========================================
//  �N���X��`
//==========================================
class CTimer : public CObject
{
public:

	CTimer();
	~CTimer();

	HRESULT Init();
	void Uninit();
	void Update();
	void Draw();

	bool GetTimeZero(); // �^�C���A�b�v���Ă锻��

	// �ÓI�����o�֐�
	static CTimer* Create(); // ��������
	static float GetTime() { return m_fTimer; } // ���Ԃ̎擾
	static float GetInitTime(); // �������Ԃ̎擾

private:

	// �萔��`
#define DIGIT (5) // ����

	// �����o�֐�
	void CalcNum(); // �v�Z

	// �����o�ϐ�
	static float m_fTimer; // ���݂̎���
	CNumber* m_pNumber[DIGIT]; // ����

};
