//============================================
//
//	�J�����̏��� [camera.h]
//	Author:sakamoto kai
//
//============================================
#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "main.h"

//�J�����N���X
class CCamera
{
public:
	CCamera();
	~CCamera();

	//�J�����̃��[�h
	enum CAMERAMODE
	{
		CAMERAMODE_CONTROL = 0,		//����
		CAMERAMODE_FOLLOW,			//�Ǐ]
		CAMERAMODE_FIXEDNOW,		//���݂̃J�����ŌŒ�
		CAMERAMODE_BETWEEN,			//2�_�̊ԂɈʒu����J����
		CAMERAMODE_DOWNVIEW,		//�����낵
		CAMERAMODE_SIDEVIEW,		//2D
		CAMERAMODE_EVENTBOSS,		//�C�x���g���̃{�X����
		CAMERAMODE_FPS,				//��l�̎��_
		CAMERAMODE_FPSCOMPLEMENT,	//��l�̎��_�ύX���̕⊮�p
		CAMERAMODE_ULTIMATE,		//�p�������̎��_
		CAMERAMODE_SEAMLESS,		//�V�[�����X
		CAMERAMODE_MAX,
	};
	
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void SetCamera(void);
	void SetCameraPos(D3DXVECTOR3 Pos) { m_posV = Pos; m_posR = Pos; }
	D3DXVECTOR3 GetCameraPos(void) { return m_posR;}
	void SetCameraPosY(float PosY) { m_EventPosY = PosY; }
	void ResetCamera(void);
	void SetBib(bool Set) { m_bBib = Set; }
	float GetPosY(void) { return m_posR.y; }
	void SetRot(D3DXVECTOR3 Rot) { m_rot = Rot; }
	D3DXVECTOR3 GetRot(void) { return m_rot; }
	void SetCameraMode(CAMERAMODE Mode) { CameraMode = Mode; }
	CAMERAMODE GetCameraMode(void) { return CameraMode; }
	void SetFollowTime(int Time) { m_FollowTime = Time; }
	D3DXVECTOR3 GetPosV(void) { return m_posV; }
	D3DXVECTOR3 GetPosR(void) { return m_posR; }
	D3DXVECTOR3 GetVecU(void) { return m_vecU; }
	void SetAttention(bool Set) { m_bAttention = Set; }
	bool GetAttention(void) { return m_bAttention; }
	D3DXMATRIX GetViewMatrix() { return m_mtxView; }
	D3DXMATRIX GetProjectionMatrix() { return m_mtxProjection; }
	void SetHomingSpeed(float Speed) { m_fHomingSpeed = Speed; }
	void SetCameraDistance(float Dis) { m_CameraDistance = Dis; }

	void SetCameraCount(int nCount) { m_nCameraCount = nCount; }
	int GetCameraCount(void) { return m_nCameraCount; }
private:
	void ControlCamera(void);
	void FollowCamera(void);
	void FixedNow(void);
	void BetWeen(void);
	void DownviewCamera(void);
	void SideviewCamera(void);
	void EventBossCamera(void);
	void FPSCamera(void);
	void UltimateCamera(void);
	void TitleCamera(void);
	void SeamlessModeChangeCamera(void);

	void FPSComplementCamera(void);

	D3DXMATRIX m_mtxView;		//�r���[�}�g���b�N�X
	D3DXMATRIX m_mtxProjection;	//�v���W�F�N�V�����}�g���b�N�X
	D3DVIEWPORT9 m_viewport;	//�r���[�|�[�g
	D3DXVECTOR3 m_move;			//�ړ���
	D3DXVECTOR3 m_posV;			//���_
	D3DXVECTOR3 m_posR;			//�����_
	D3DXVECTOR3 m_vecU;			//������x�N�g��
	D3DXVECTOR3 m_rot;			//���݂̌���
	D3DXVECTOR3 m_rotOld;		//�ߋ��̌���
	D3DXVECTOR3 m_posVDest;		//�ړI�̎��_
	D3DXVECTOR3 m_posRDest;		//�ړI�̒����_
	D3DXVECTOR3 m_PlayerPos;	//�v���C���[�̈ʒu
	D3DXVECTOR3 m_DelCameraPos;	//���X�|�[���J�����ʒu
	CAMERAMODE CameraMode;		//�J�����̏��
	D3DXVECTOR3 m_fRotMove;		//�����̃J�����ړ����x
	float m_CameraDistance;		//�����_�ƃJ�����̋���
	float m_EventPosY;			//�C�x���g���̃J�����̍���
	float m_fHomingSpeed;		//�J�����ړ��̃z�[�~���O���x
	bool m_bFollowY;			//�J�����̏㉺�Ǐ]�����邩�ǂ���
	bool m_bBib;				//�J�������U�����邩�ǂ���
	float m_fBibPowor;			//�J�����U���̋���
	int m_FollowTime;			//��荞�ݎ���
	int m_FollowDistance;		//�Ǐ]���̃J��������
	bool m_bAttention;			//���ڏ��
	int m_nCameraCount;			//�ǂ̂��炢���̃J�������ێ����邩�Ƃ����J�E���g
};

#endif