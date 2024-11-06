//==========================================
//
//  �o�ߎ��Ԍv��
//  Author : Tomoya Kanazaki
//
//==========================================
#include "deltatime.h"

//==========================================
//  �������O���
//==========================================
namespace
{
	DWORD m_dwOldTime; // �O��t���[���̎���
	float m_fDeltaTime; // �u�Ԃ̌o�ߎ���(�b)
}

//==========================================
//  ����������
//==========================================
void DeltaTime::Init()
{
	m_dwOldTime = timeGetTime();
	m_fDeltaTime = 0.0f;
}

//==========================================
//  �X�V
//==========================================
void DeltaTime::Update()
{
	// ���݂̎��Ԃ��擾
	DWORD Time = timeGetTime();

	// �O�񂩂�̍��������߂�
	float Delta = (float)(Time - m_dwOldTime);

	// �������o�ߎ��ԂƂ��Ďg����`�ɕϊ�
	m_fDeltaTime = Delta * 0.001f;

	// ���ݎ��Ԃ�ۑ�����
	m_dwOldTime = Time;

	// �o�ߎ��Ԃ�\������
	DebugProc::Print(DebugProc::POINT_CENTER, "��Time : %f\n", m_fDeltaTime);
}

//==========================================
//  �擾
//==========================================
float DeltaTime::Get()
{
	return m_fDeltaTime;
}
