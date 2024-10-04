//========================================
//
// ���C��[main.h]
// Author�F�X��x��
//
//========================================
#include "main.h"
#include "renderer.h"
#include "object.h"
#include "object2D.h"
#include "manager.h"

//=======================================
// �萔��`
//=======================================
namespace
{
const char* CLASS_NAME = "WindowClass";	// ���C�g�̍ő吔
const char* WINDOW_NAME = "�������Ƃ��߂ƐX�Ɛ�";	// ���C�g�̍ő吔
}

//========================================
//�v���g�^�C�v�錾
//========================================
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

//=======================================
//�O���[�o���ϐ�
//=======================================
int g_nCountFPS = 0;                        //FPS�J�E���^
LPD3DXFONT g_pFont = NULL;                  //�t�H���g�ւ̃|�C���^

//========================================
//���C���֐�
//========================================
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hInstancePrev, LPSTR lpCmdLine, int nCmdShow)
{
	//��ʃT�C�Y�̍\����
	RECT rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };

	//���ݎ���
	DWORD dwCurrentTime;
	//�Ō�ɏ�����������
	DWORD dwExecLastTime;
	//�t���[���J�E���g
	DWORD dwFrameCount;
	//�Ō��FPS���v����������
	DWORD dwFPSLastTime;

	WNDCLASSEX wcex =
	{
		sizeof(WNDCLASSEX),					//WNDCLASSEX�̃������T�C�Y
		CS_CLASSDC,							//�E�B���h�E�̃X�^�C��
		WindowProc,							//�E�B���h�E�v���V�[�W��
		0,									//0�ɂ���
		0,									//0�ɂ���
		hInstance,							//�C���X�^���X�n���h��
		LoadIcon(nullptr,IDI_APPLICATION),		//�^�X�N�o�[�̃A�C�R��
		LoadCursor(nullptr,IDC_ARROW),			//�}�E�X�J�[�\��
		(HBRUSH)(COLOR_WINDOW + 1),			//�N���C�A���g�̈�̔w�i�F
		nullptr,								//���j���[�o�[
		CLASS_NAME,							//�E�B���h�E�N���X�̖��O
		LoadIcon(nullptr,IDI_APPLICATION)		//�t�@�C���A�C�R��
	};

	//�E�B���h�E�n���h��(���ʎq)
	HWND hWnd;

	//���b�Z�[�W���i�[����
	MSG msg;

	//�E�B���h�E�N���X�̓o�^
	RegisterClassEx(&wcex);

	//�N���C�A���g�̈���w��T�C�Y�ɒ���
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, TRUE);

	//�E�B���h�E����
	hWnd = CreateWindowEx
	(0,									//�g���E�B���h�E�X�^�C��
		CLASS_NAME,						//�E�B���h�E�N���X�̖��O
		WINDOW_NAME,					//�E�B���h�E�̖��O
		WS_OVERLAPPEDWINDOW,			//�E�B���h�E�X�^�C��
		CW_USEDEFAULT,					//�E�B���h�E�̍���X���W
		CW_USEDEFAULT,					//�E�B���h�E�̍���Y���W
		(rect.right - rect.left),		//�E�B���h�E�̕�
		(rect.bottom - rect.top),		//�E�B���h�E�̍���
		nullptr,						//�e�E�B���h�E�̃n���h��
		nullptr,						//���j���[�n���h���܂��͎q�E�B���h�EID
		hInstance,						//�C���X�^���X�n���h��
		nullptr);						//�E�B���h�E�쐬�f�[�^

	//�}�l�[�W���̃|�C���^
	CManager *pManager = CManager::GetInstance();

	{//�}�l�[�W��������
		pManager->Init(hInstance, hWnd, TRUE);
	}

	//���ݎ������擾(�ۑ�)
	dwExecLastTime = timeGetTime();

	//�E�B���h�E�̕\��
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	dwFrameCount = 0;
	dwFPSLastTime = timeGetTime();

	//���b�Z�[�W���[�v
	while (1)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE) != 0)
		{//windows�̏���
			if (msg.message == WM_QUIT)
			{//WM_QUIT���b�Z�[�W���󂯎�����烁�b�Z�[�W���[�v�𔲂���
				break;
			}
			else
			{
				TranslateMessage(&msg);

				DispatchMessage(&msg);
			}
		}
		else
		{//DirectX�̏���
		 //���݂̎������擾
			dwCurrentTime = timeGetTime();

			if ((dwCurrentTime - dwFPSLastTime) >= 500)
			{//0.5�b�o��
				//FPS���v��
				g_nCountFPS = (dwFrameCount * 1000) / (dwCurrentTime - dwFPSLastTime);

				//FPS�𑪒肵��������ۑ�
				dwFPSLastTime = dwCurrentTime;
				//�t���[�����[�g���N���A
				dwFrameCount = 0;
			}
			if ((dwCurrentTime - dwExecLastTime) >= (1000 / 60))
			{//60����1�b�o��
				//�����J�n�̎���[���ݎ���]��ۑ�
				dwExecLastTime = dwCurrentTime;

				//�}�l�[�W���̍X�V����
				pManager->Update();

				//�}�l�[�W���̕`�揈��
				pManager->Draw();

				//�t���[�����[�g���v�Z
				dwFrameCount++;
			}
		}

	}
	//�}�l�[�W���̔j��
	if (pManager != nullptr)
	{
		pManager->Uninit();

		delete pManager;

		pManager = nullptr;
	}

	//�E�B���h�E�N���X�̓o�^������
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	return (int)msg.wParam;
}

//========================================
//�E�B���h�E�v���V�[�W��
//========================================
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	int nID; //�Ԃ�l���i�[

	switch (uMsg)
	{
	case WM_DESTROY://�E�B���h�E�j�����b�Z�[�W
		//WM_QUIT�𑗂�
		PostQuitMessage(0);
		break;

		//�L�[�����̃��b�Z�[�W
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_ESCAPE://[ESC]�L�[�������ꂽ
			nID = MessageBox(hWnd, "�I�����܂����H", "�I�����b�Z�[�W", MB_YESNO | MB_ICONQUESTION);

			if (nID == IDYES)
			{
				DestroyWindow(hWnd);
			}
		}
		break;

	case  WM_CLOSE://����{�^�������̃��b�Z�[�W
		nID = MessageBox(hWnd, "�I�����܂����H", "�I�����b�Z�[�W", MB_YESNO | MB_ICONQUESTION);
		if (nID == IDYES)
		{
			//�E�B���h�E��j������
			DestroyWindow(hWnd);
		}
		else
		{
			return 0;   //0��Ԃ��Ȃ��ƏI�����Ă��܂�
		}
		break;
	}
	//����̏������J��Ԃ�
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}
