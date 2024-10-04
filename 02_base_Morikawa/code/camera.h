//=======================================
//
//�J����[camera.h]
//Author�@:�@MORIKAWA SHUNYA
//
//=======================================

#ifndef _CAMERA_H_     //���̃}�N����`����`����Ă��Ȃ�������
#define _CAMERA_H_     //2�A�C���N���[�h�h�~�̃}�N�����`����

#include "main.h"

//=======================================
//�J�����N���X
//=======================================
class CCamera
{
public:
	CCamera();		//�R���X�g���N�^
	~CCamera();		//�f�X�g���N�^

	//�����o�֐�
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void SetCamera(void);
	void CameraMoveV(void);
	float RotNor(float RotN);
	void Following(D3DXVECTOR3 pos, D3DXVECTOR3 rot);
	void TitleCamera();
	void CollisionArena();

	D3DXVECTOR3 GetRot(void);				//�ʒu�擾
	D3DXVECTOR3 GetPosV(void);				//���_�̎擾
	D3DXVECTOR3 GetPosR(void);				//�����_�̎擾
	D3DXVECTOR3 GetVecU() { return m_vecU; }

private:

	//�����o�ϐ�
	D3DXMATRIX m_mtxProjection;		//�v���W�F�N�V�����}�g���b�N�X
	D3DXMATRIX m_mtxView;			//�r���[�}�g���b�N�X

	D3DXVECTOR3 m_posV;		//���_
	D3DXVECTOR3 m_posR;		//�����_
	D3DXVECTOR3 m_vecU;		//������x�N�g��
	D3DXVECTOR3 m_rot;		//����
	D3DXVECTOR3 m_move;		//�ړ���
	D3DXVECTOR3 m_VDest;	//�ړI�̎��_
	D3DXVECTOR3 m_RDest;	//�ړI�̒����_
	float m_fDistance;		//����
};

#endif
