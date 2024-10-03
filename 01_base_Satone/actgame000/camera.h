//==============================================================
//
//�J��������[camera.h]
//Author:����������
//
//==============================================================
#ifndef _CAMERA_H_		//���̃}�N����`������Ă��Ȃ�������
#define _CAMERA_H_		//2�d�C���N���[�h�h�~�̃}�N�����`����

#include"main.h"

//�J�����N���X�̒�`
class CCamera
{
public:
	CCamera();		//�R���X�g���N�^
	~CCamera();		//�f�X�g���N�^

	HRESULT Init(void);		//����������
	void Uninit(void);		//�I������
	void Update(void);		//�X�V����
	void SetCamera(void);	//�ݒ菈��

	void SetPositionVY(float posY) { m_posV.y = posY; }		//���_�ʒuY�ݒ�
	void SetPositionRY(float posY) { m_posR.y = posY; }		//�����_�ʒuY�ݒ�
	void SetRotation(D3DXVECTOR3 rot) { m_rot = rot; }		//�����ݒ�
	D3DXVECTOR3 GetRotation(void) { return m_rot; }			//�����擾
	void SetMode(bool bProject) { m_bProject = bProject; }	//���e���@�ݒ�
	void SetMode(void){ m_bProject = m_bProject ? false : true; }
	bool GetMode(void) { return m_bProject; }				//���e���@�擾

	void Control(void);		//�ړ�����
	void RotCorrection(void);	//�����̕␳����

private:
	D3DXMATRIX m_mtxView;			//�r���[�}�g���b�N�X
	D3DXMATRIX m_mtxProjection;		//�v���W�F�N�V�����}�g���b�N�X

	D3DXVECTOR3 m_posV;			//���_
	D3DXVECTOR3 m_posR;			//�����_
	D3DXVECTOR3 m_vecU;			//������x�N�g��
	D3DXVECTOR3 m_rot;			//����

	D3DXVECTOR3 m_posVDest;		//�ړI�̎��_
	D3DXVECTOR3 m_posRDest;		//�ړI�̒����_

	D3DXVECTOR3 m_moveV;		//���_�̈ړ���
	D3DXVECTOR3 m_moveR;		//�����_�̈ړ���
	float m_fRotMove;			//���݂̕���(�p�x)
	float m_fRotDest;			//�ڕW�̕���(�p�x)
	float m_fRotDiff;			//�ڕW�̕����܂ł̍���

	bool m_bProject;			//���e���@�̐؂�ւ�
};

//�J�����̍\����
//typedef struct
//{
//	
//	
//	float fLength;				//����
//	D3DVIEWPORT9 viewport;		//�r���[�|�[�g
//} Camera;

//�v���g�^�C�v�錾

//void SetCamera(int nIdex);
//Camera *GetCamera(void);

#endif
