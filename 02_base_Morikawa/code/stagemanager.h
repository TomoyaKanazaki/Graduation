//=============================================================================
// 
//  �X�e�[�W�}�l�[�W���[�w�b�_�[ [stagemanager.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _STAGE_MANAGER_H_
#define _STAGE_MANAGER_H_		// ��d�C���N���[�h�h�~

//==========================================================================
// �N���X��`
//==========================================================================
// �X�e�[�W�}�l�[�W���[�N���X
class CStageManager
{
public:

	CStageManager();
	~CStageManager();

	// �I�[�o�[���C�h�֐�
	HRESULT Init();
	void Uninit();
	void Update();

	// �ÓI�����o�֐�
	static CStageManager* Create();

private:

	// �X�e�[�W���[�h
	HRESULT StageLoad();
};


#endif
