//============================================
//
//	ブロックのスーパークラス [block.h]
//	Author:nakamura riku
//
//============================================
#ifndef _BLOCK_H_
#define _BLOCK_H_

#include "main.h"
#include "object2D.h"

// 前方宣言
class CPlayer2D;

// ブロックスーパークラス
class CBlock : public CObject2D
{
public:

	CBlock(int nPriority = 5);
	~CBlock();

	virtual HRESULT Init(void);
	virtual void Uninit(void);
	virtual void Update(void);
	virtual void Draw(void);

	virtual void Collision(CPlayer2D* pPlayer2D) = 0;

	// 取得処理・設定処理
	void SetMove(D3DXVECTOR3 move) { m_move = move; }
	D3DXVECTOR3 GetMove(void) { return m_move; }
	D3DXVECTOR3 GetPosOld(void) { return m_posOld; }

private:
	D3DXVECTOR3 m_posOld;
	D3DXVECTOR3 m_move;							// 移動量
};

// 足場クラス
class CBlockBase : public CBlock
{
public:

	CBlockBase(int nPriority = 6) {};
	~CBlockBase() {};

	static CBlockBase* Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, float fWight, float fHeight, int nPriority = 6);

	HRESULT Init(void)
	{
		CBlock::Init();
		SetTexture("data\\TEXTURE\\HGS\\block.png");
		return S_OK;
	};
	void Uninit(void) { CBlock::Uninit(); };
	void Update(void) { CBlock::Update(); };
	void Draw(void) { CBlock::Draw(); };

	void Collision(CPlayer2D* pPlayer2D);

	// 取得処理・設定処理

private:

};

// 棘クラス
class CBlockSpike : public CBlock
{
public:

	CBlockSpike(int nPriority = 6) {};
	~CBlockSpike() {};

	static CBlockSpike* Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, float fWight, float fHeight, int nPriority = 6);

	HRESULT Init(void) 
	{
		CBlock::Init();
		SetTexture("data\\TEXTURE\\HGS\\gimmick000.png");
		return S_OK;
	};
	void Uninit(void) { CBlock::Uninit(); };
	void Update(void) { CBlock::Update(); };
	void Draw(void) { CBlock::Draw(); };

	void Collision(CPlayer2D* pPlayer2D);

	// 取得処理・設定処理

private:

};

// コインクラス
class CBlockCoin : public CBlock
{
public:

	CBlockCoin(int nPriority = 6) {};
	~CBlockCoin() {};

	static CBlockCoin* Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, float fWight, float fHeight, int nPriority = 6);

	HRESULT Init(void) 
	{
		CBlock::Init();
		SetTexture("data\\TEXTURE\\HGS\\coin000.png");
		return S_OK;
	};
	void Uninit(void) { CBlock::Uninit(); };
	void Update(void) { CBlock::Update(); };
	void Draw(void) { CBlock::Draw(); };

	void Collision(CPlayer2D* pPlayer2D);

	// 取得処理・設定処理

private:

};

// バネクラス
class CBlockSpring : public CBlock
{
public:

	CBlockSpring(int nPriority = 6) {};
	~CBlockSpring() {};

	static CBlockSpring* Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, float fWight, float fHeight, int nPriority = 6);

	HRESULT Init(void) 
	{
		CBlock::Init();
		SetTexture("data\\TEXTURE\\HGS\\gimmick001.png");
		return S_OK;
	};
	void Uninit(void) { CBlock::Uninit(); };
	void Update(void) { CBlock::Update(); };
	void Draw(void) { CBlock::Draw(); };

	void Collision(CPlayer2D* pPlayer2D);

	// 取得処理・設定処理

private:

};

// 崩れかけクラス
class CBlockBroken : public CBlock
{
public:

	CBlockBroken(int nPriority = 6) {};
	~CBlockBroken() {};

	static CBlockBroken* Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, float fWight, float fHeight, int nPriority = 6);

	HRESULT Init(void)
	{
		CBlock::Init();
		SetTexture("data\\TEXTURE\\HGS\\gimmick002.png");
		return S_OK;
	};
	void Uninit(void) { CBlock::Uninit(); };
	void Update(void) { CBlock::Update(); };
	void Draw(void) { CBlock::Draw(); };

	void Collision(CPlayer2D* pPlayer2D);

	// 取得処理・設定処理

private:
	int m_nLife = 15;
};

// タイトルクラス
class CBlockTitle : public CBlock
{
public:

	CBlockTitle(int nPriority = 6) {};
	~CBlockTitle() {};

	static CBlockTitle* Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, float fWight, float fHeight, int nPriority = 6);

	HRESULT Init(void)
	{
		CBlock::Init();
		SetTexture("data\\TEXTURE\\HGS\\Pause_logo.png");
		return S_OK;
	};
	void Uninit(void) { CBlock::Uninit(); };
	void Update(void) { CBlock::Update(); };
	void Draw(void) { CBlock::Draw(); };

	void Collision(CPlayer2D* pPlayer2D);

	// 取得処理・設定処理

private:

};

// チュートリアルクラス
class CBlockTutorial : public CBlock
{
public:

	CBlockTutorial(int nPriority = 6) {};
	~CBlockTutorial() {};

	static CBlockTutorial* Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, float fWight, float fHeight, int nPriority = 6);

	HRESULT Init(void) 
	{
		CBlock::Init();
		SetTexture("data\\TEXTURE\\HGS\\tutorial000.png");
		return S_OK;
	};
	void Uninit(void) { CBlock::Uninit(); };
	void Update(void) { CBlock::Update(); };
	void Draw(void) { CBlock::Draw(); };

	void Collision(CPlayer2D* pPlayer2D);

	// 取得処理・設定処理

private:

};
#endif