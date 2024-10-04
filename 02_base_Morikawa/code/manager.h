//========================================
//
// �Ǘ�[manager.h]
// Author�F�X��x��
//
//========================================
#ifndef _MANAGER_H_
#define _MANAGER_H_
#include "main.h"
#include "object.h"

//========================================
//�O���錾
//========================================
class CObject;
class CInputKeyboard;
class CRenderer;
class CScene;
class CTexture;
class CCamera;
class CInputPad;
class CCamera;
class CInputMouse;
class CLight;
class CResult;
class CTutorial;
class CFade;
class CSound;
class CXLoad;

//========================================
//�V�[���N���X
//========================================
class CScene
{
public:
	enum MODE
	{//���[�h�̗񋓌^
		MODE_NONE = 0,	//�ݒ�Ȃ�
		MODE_TITLE,		// �^�C�g��
		MODE_TUTORIAL,	// �`���[�g���A��
		MODE_GAME,		// �Q�[��
		MODE_RESULT,	// ���U���g
		MODE_MAX
	};

	CScene();	//�R���X�g���N
	~CScene();	//�f�X�g���N�^

	//�����o�֐�
	static CScene *Create(int nMode);

	virtual HRESULT Init(void) = 0;
	virtual void Uninit(void) = 0;
	virtual void Update(void) = 0;
	virtual void Draw(void) = 0;
	MODE GetMode(void) { return m_mode; }	//���[�h�擾

private:
	//�����o�ϐ�
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	//���_�����i�[
	int m_nIdxTexture;		//�e�N�X�`���ԍ�
	MODE m_mode;			//���݂̃��[�h
};

//========================================
//�}�l�[�W���N���X
//========================================
class CManager
{
private:	//���̏ꏊ�ŃA�N�Z�X�����Ȃ�
	CManager();		//�R���X�g���N�^
public:
	virtual ~CManager();	//�f�X�g���N�^
	static CManager *GetInstance();

public:
	//�����o�֐�
	HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	CRenderer *GetRenderer(void) { return m_pRenderer; }
	CInputKeyboard *GetInputKeyboard(void) { return m_pInputKeyboard; }
	CInputPad *GetInputPad(void) { return m_pInputPad; }
	CInputMouse *GetInputMouse(void) { return m_pInputMouse; }
	CTexture *GetTexture(void) { return m_pTexture; }
	void SetMode(CScene::MODE mode);
	CScene::MODE GetMode(void) { return m_pScene->GetMode(); }	//���[�h�擾
	CCamera *GetCamera(void) { return m_pCamera; }
	CFade* GetFade() { return m_pFade; }
	CSound* GetSound() { return m_pSound; }

	float GetDeltaTime() { return m_fDeltaTime; }	// �o�ߎ��Ԏ擾

	bool bResult();

private:
	static CManager *m_pManager;

	//=============================
	//�����o�ϐ�
	//=============================
	int m_nCnt;		//�����J�ڃJ�E���^�[
	int m_state;	//��ԕύX
	int m_NowScene;	//���݂̃V�[��

	bool m_IsResult;

	CRenderer *m_pRenderer;				// �����_���[�̃|�C���^
	CInputKeyboard *m_pInputKeyboard;	// �L�[�{�[�h�̃|�C���^
	CInputPad *m_pInputPad;		// �R���g���[���[�̃|�C���^
	CInputMouse *m_pInputMouse; // �}�E�X�̃|�C���^
	CScene *m_pScene;			// �V�[���̃|�C���^
	CTexture *m_pTexture;		// �e�N�X�`���ւ̃|�C���^
	CXLoad* m_pXLoad;			// X�t�@�C��
	CCamera *m_pCamera;			// �J�����̃|�C���^
	CLight* m_pLight;			// ���C�g�̃|�C���^
	CFade* m_pFade;				// �t�F�[�h�̃|�C���^
	CSound* m_pSound;			// �T�E���h�̃|�C���^

	// �f���^�^�C���p
	DWORD m_CurrentTime;			// ���ݎ���
	DWORD m_OldTime;				// �ߋ��̎���
	float m_fDeltaTime;				// �o�ߎ���
};

#endif
