//==============================================================
//
//�}�l�[�W������[manager.h]
//Author:����������
//
//==============================================================
#ifndef _MANAGER_H_		//���̃}�N����`������Ă��Ȃ�������
#define _MANAGER_H_		//2�d�C���N���[�h�h�~�̃}�N�����`����

#include "main.h"
#include "scene.h"

//�O���錾
class CRenderer;		//�����_���[
class CInputKeyboard;	//�L�[�{�[�h����
class CInputJoyPad;		//�Q�[���p�b�h�̓���
class CDebugProc;		//�f�o�b�O�\��
class CSound;			//�T�E���h
class CScene;			//�V�[��
class CFade;			//�t�F�[�h
class CTexture;			//�e�N�X�`��
class CMaterial;		//�}�e���A��
class CCamera;			//�J����
class CLight;			//���C�g

//�}�l�[�W���N���X�̒�`
class CManager
{
public:

	CManager();
	~CManager();

	HRESULT Init(HINSTANCE hInstance,
		HWND hWnd,
		BOOL bWindow);		//����������
	void Uninit(void);		//�I������
	void Update(void);		//�X�V����
	void Draw(void);		//�`�揈��
	void ResetMap(void);	//�}�b�v�̍Đ���

	static CManager *GetInstance(void);		//�}�l�[�W���̏��

	CRenderer *GetRenderer(void) { return m_pRenderer; }		//�����_���[�̎擾
	CInputKeyboard *GetInputKeyboard(void) { return m_pInputKeyboard; }		//�L�[�{�[�h�̎擾
	CInputJoyPad *GetInputJoyPad(void) { return m_pInputJoyPad; }			//�Q�[���p�b�h�̎擾
	CDebugProc *GetDebugProc(void) { return m_pDebugProc; }	//�f�o�b�O�\���̎擾
	CSound *GetSound(void) { return m_pSound; }				//�T�E���h�̎擾
	CScene *GetScene(void) { return m_pScene; }				//�V�[���̏��
	CTexture *GetTexture(void) { return m_pTexture; }		//�e�N�X�`���̎擾
	CMaterial *GetMaterial(void) { return m_pMaterial; }		//�}�e���A���̎擾
	CFade *GetFade(void) { return m_pFade; }					//�t�F�[�h�̎擾

	CCamera *GetCamera(void) { return m_pCamera; }			//�J�����̏��
	CLight *GetLight(void) { return m_pLight; }				//���C�g�̏��

	void SetMode(CScene::MODE mode);		//�Q�[�����[�h�ݒ�
	CScene::MODE GetMode(void);				//�Q�[�����[�h�擾
	void SetNumScore(int nNum) { m_nScore = nNum; }		//�X�R�A�̐ݒ�
	void SetNumDeath(int nNum) { m_nNumDeath = nNum; }	//���S���̐ݒ�
	void SetNumItem(int nNum) { m_nNumItem = nNum; }	//�A�C�e�����̐ݒ�

	int GetNumScore(void) { return m_nScore; }			//�X�R�A�̎擾
	int GetNumDeath(void) { return m_nNumDeath; }		//���S���̎擾
	int GetNumItem(void) { return m_nNumItem; }			//�A�C�e�����̎擾

	void SetBgm(bool bBgm) { m_bBgm = bBgm; }		//BGM�̐ݒ�
	bool GetBgm(void) { return m_bBgm; }				//BGM�̎擾
	void SetResult(bool bResult) { m_bResult = bResult; }	//���U���g�̔���̐ݒ�
	bool GetResult(void) { return m_bResult; }				//���U���g�̔���擾

	void SetFps(int nFps);			//FPS�̐ݒ�

private:
	static CManager *m_pManager;			//�}�l�[�W���̏��

	static CRenderer *m_pRenderer;		//�����_���[�̏��
	static CDebugProc *m_pDebugProc;	//�f�o�b�O�\���̏��
	static CInputKeyboard *m_pInputKeyboard;	//�L�[�{�[�h�̏��
	static CInputJoyPad *m_pInputJoyPad;		//�Q�[���p�b�h�̏��
	static CSound *m_pSound;			//�T�E���h�̏��
	static CScene *m_pScene;			//�V�[���̏��
	static CTexture *m_pTexture;		//�e�N�X�`���̏��
	static CMaterial *m_pMaterial;		//�}�e���A���̏��
	static CFade *m_pFade;				//�t�F�[�h�̏��
	static CCamera *m_pCamera;			//�J�����̏��
	static CLight *m_pLight;			//���C�g�̏��

	static int m_nScore;				//�X�R�A
	static int m_nNumDeath;				//���S��
	static int m_nNumItem;				//�A�C�e����

	static bool m_bBgm;					//BGM�X�g�b�v���邩
	static bool m_bResult;				//���U���g�̔���
};

#endif
