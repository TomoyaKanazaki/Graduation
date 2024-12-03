//============================================
//
//	転がる岩の処理 [RollRock.cpp]
//	Author:sakamoto kai
//
//============================================
#include "RollRock.h"
#include "renderer.h"
#include "manager.h"
#include "texture.h"
#include "XModel.h"
#include "game.h"
#include "tutorial.h"
#include "Devil.h"
#include "MapSystem.h"
#include "CubeBlock.h"
#include "objmeshField.h"
#include "wall.h"

//==========================================
//  定数定義
//==========================================
namespace
{
	const D3DXVECTOR3 SAMPLE_SIZE = D3DXVECTOR3(40.0f, 40.0f, 40.0f);		//当たり判定
	const float GRIT_OK = 45.0f;			//移動可能なグリットの範囲内
}

//====================================================================
//静的メンバ変数宣言
//====================================================================
CListManager<CRollRock>* CRollRock::m_pList = nullptr; // オブジェクトリスト

//====================================================================
//コンストラクタ
//====================================================================
CRollRock::CRollRock(int nPriority) : CObjectX(nPriority),
m_pEffect(nullptr)
{
	SetSize(SAMPLE_SIZE);
	SetPos(INITVECTOR3);
	m_nIdxXModel = 0;			//マテリアルの数
	m_CollisionPos = INITVECTOR3;
	m_bCollision = false;
	m_nStateCount = 0;
	m_Scaling = 1.0f;
	m_fColorA = 0.0f;
	m_pos = INITVECTOR3;
	m_posOld = INITVECTOR3;
	m_move = INITVECTOR3;
	m_rot = INITVECTOR3;
	m_Grid.x = 0;
	m_Grid.z = 0;

	m_OKL = false;
	m_OKR = false;
	m_OKU = false;
	m_OKD = false;
}

//====================================================================
//コンストラクタ(オーバーロード)
//====================================================================
CRollRock::CRollRock(int nPriority, CMapSystem::GRID gridCenter) : CObjectX(nPriority)
{
	SetSize(SAMPLE_SIZE);
	SetPos(INITVECTOR3);
	m_nIdxXModel = 0;			//マテリアルの数
	m_CollisionPos = INITVECTOR3;
	m_bCollision = false;
	m_nStateCount = 0;
	m_Scaling = 1.0f;
	m_fColorA = 0.0f;
	m_pos = INITVECTOR3;
	m_posOld = INITVECTOR3;
	m_move = INITVECTOR3;
	m_rot = INITVECTOR3;
	m_Grid = gridCenter;		// グリッド

	m_OKL = false;
	m_OKR = false;
	m_OKU = false;
	m_OKD = false;
}

//====================================================================
//デストラクタ
//====================================================================
CRollRock::~CRollRock()
{

}

//====================================================================
//生成処理
//====================================================================
CRollRock* CRollRock::Create(CMapSystem::GRID gridCenter)
{
	CRollRock* pSample = nullptr;

	if (pSample == nullptr)
	{
		//オブジェクト2Dの生成
		pSample = new CRollRock(3, gridCenter);
	}

	//オブジェクトの初期化処理
	if (FAILED(pSample->Init("data\\MODEL\\rock.x")))
	{//初期化処理が失敗した場合
		return nullptr;
	}

	return pSample;
}

//====================================================================
//初期化処理
//====================================================================
HRESULT CRollRock::Init(char* pModelName)
{
	m_pEffect = nullptr;
	// 位置
	m_pos = m_Grid.ToWorld();
	m_pos.y = 50.0f;

	SetType(CObject::TYPE_ENEMY3D);

	CObjectX::Init(pModelName);

	//マップとのマトリックスの掛け合わせをオンにする
	SetUseMultiMatrix(CObjmeshField::GetListTop()->GetMatrix());

	// 位置設定
	CObjectX::SetPos(m_pos);

	SetSize(SAMPLE_SIZE);

	//モードごとに初期値を設定出来る
	switch (CScene::GetMode())
	{
	case CScene::MODE_TITLE:
		break;

	case CScene::MODE_GAME:
	case CScene::MODE_TUTORIAL:

		break;

	case CScene::MODE_RESULT:
		break;
	}

	if (m_pList == nullptr)
	{// リストマネージャー生成
		m_pList = CListManager<CRollRock>::Create();
		if (m_pList == nullptr) { assert(false); return E_FAIL; }
	}

	// リストに自身のオブジェクトを追加・イテレーターを取得
	m_iterator = m_pList->AddList(this);


	return S_OK;
}

//====================================================================
//終了処理
//====================================================================
void CRollRock::Uninit(void)
{
	// リストから自身のオブジェクトを削除
	m_pList->DelList(m_iterator);

	if (m_pList->GetNumAll() == 0)
	{ // オブジェクトが一つもない場合

		// リストマネージャーの破棄
		m_pList->Release(m_pList);
	}

	// エフェクトを消去
	if (m_pEffect != nullptr)
	{
		m_pEffect->SetDeath();
		m_pEffect = nullptr;
	}

	CObjectX::Uninit();
}

//====================================================================
//更新処理
//====================================================================
void CRollRock::Update(void)
{
	switch (CScene::GetMode())
	{
	case CScene::MODE_TITLE:
		TitleUpdate();
		break;

	case CScene::MODE_GAME:
	case CScene::MODE_TUTORIAL:

		GameUpdate();
		break;

	case CScene::MODE_RESULT:
		break;
	}
}

//====================================================================
//タイトルでの更新処理
//====================================================================
void CRollRock::TitleUpdate(void)
{
	D3DXVECTOR3 pos = GetPos();

	//位置更新
	pos += m_move;

	SetPos(pos);

	//頂点情報の更新
	CObjectX::Update();
}

//====================================================================
//ゲームでの更新処理
//====================================================================
void CRollRock::GameUpdate(void)
{
	//更新前の位置を過去の位置とする
	m_posOld = m_pos;
	CObjectX::SetPosOld(m_pos);

	Move();

	//// 移動量を位置に反映
	//m_pos.x += m_move.x;

	// X軸の当たり判定
	CollisionWall(useful::COLLISION_X);

	// 移動した分だけ回転
	m_rot.z -= (m_pos.x - m_posOld.x) * D3DX_PI * 0.01f;

	//// 移動量を位置に反映
	//m_pos.z += m_move.z;

	// Z軸の当たり判定
	CollisionWall(useful::COLLISION_Z);

	// 移動した分だけ回転
	m_rot.x += (m_pos.z - m_posOld.z) * D3DX_PI * 0.01f;

	// ステージ外との当たり判定
	CollisionOut();

	// グリット番号の設定
	m_Grid = CMapSystem::GetInstance()->CalcGrid(m_pos);

	//位置更新
	CObjectX::SetPos(m_pos);
	CObjectX::SetRot(m_rot);

	//大きさの設定
	SetScaling(D3DXVECTOR3(m_Scaling, m_Scaling, m_Scaling));

	//頂点情報の更新
	CObjectX::Update();

	DebugProc::Print(DebugProc::POINT_RIGHT, "[岩]横 %d : 縦 %d\n", m_Grid.x, m_Grid.z);
	DebugProc::Print(DebugProc::POINT_RIGHT, "[岩]左 %d\n", m_OKL);
	DebugProc::Print(DebugProc::POINT_RIGHT, "[岩]右 %d\n", m_OKR);
	DebugProc::Print(DebugProc::POINT_RIGHT, "[岩]上 %d\n", m_OKU);
	DebugProc::Print(DebugProc::POINT_RIGHT, "[岩]下 %d\n", m_OKD);
}

//====================================================================
//描画処理
//====================================================================
void CRollRock::Draw(void)
{
	CObjectX::Draw();
}

//====================================================================
//傾き時の移動処理
//====================================================================
void CRollRock::Move(void)
{
	D3DXVECTOR3 SlopeRot = INITVECTOR3;

	SlopeRot = CDevil::GetListTop()->GetDevilRot();

	// 傾きによる移動量設定
	m_move.x = -SlopeRot.z * 10.0f;
	m_move.z = SlopeRot.x * 10.0f;

	//自分の立っているグリットの中心位置を求める
	D3DXVECTOR3 MyGritPos = m_Grid.ToWorld();
	float MapGritSize = CMapSystem::GetInstance()->GetGritSize();

	if (useful::CollisionCircle(MyGritPos, D3DXVECTOR3(m_pos.x, MyGritPos.y, m_pos.z), 5.0f) == true)
	{// ブロックの中心にある時に上下か左右のどちらかになるまでに移動する

		m_pos.x += m_move.x;
		m_pos.z += m_move.z;
	}
	else
	{// ブロックの中心にないとき

		//上下移動
		if (MyGritPos.x - m_pos.x >= -5.0f && MyGritPos.x - m_pos.x <= 5.0f)
		{
			m_pos.z += m_move.z;
		}
		else
		{
			m_pos.z = MyGritPos.z;
		}

		//左右移動
		if (MyGritPos.z - m_pos.z >= -5.0f && MyGritPos.z - m_pos.z <= 5.0f)
		{
			m_pos.x += m_move.x;
		}
		else
		{
			m_pos.x = MyGritPos.x;
		}
	}

	if (m_pos.x <= MyGritPos.x + ((MapGritSize * 0.5f) - (GRIT_OK * m_OKR)) &&	//左
		m_pos.x >= MyGritPos.x - ((MapGritSize * 0.5f) - (GRIT_OK * m_OKL)) &&	//右
		m_pos.z <= MyGritPos.z + ((MapGritSize * 0.5f)) &&	//上
		m_pos.z >= MyGritPos.z - ((MapGritSize * 0.5f)))	//下
	{// グリットの中心位置に立っているか

		int nRGridX = m_Grid.x + 1;
		int nLGridX = m_Grid.x - 1;

		nRGridX = useful::RangeNumber(CMapSystem::GetInstance()->GetWightMax(), 0, nRGridX);
		nLGridX = useful::RangeNumber(CMapSystem::GetInstance()->GetWightMax(), 0, nLGridX);

		if (CMapSystem::GetInstance()->GetGritBool(nRGridX, m_Grid.z) == true)
		{//右
			m_OKR = false;
		}
		else
		{
			m_OKR = true;
		}

		if (CMapSystem::GetInstance()->GetGritBool(nLGridX, m_Grid.z) == true)
		{//左
			m_OKL = false;
		}
		else
		{
			m_OKL = true;
		}
	}

	if (m_pos.x <= MyGritPos.x + ((MapGritSize * 0.5f)) &&	//左
		m_pos.x >= MyGritPos.x - ((MapGritSize * 0.5f)) &&	//右
		m_pos.z <= MyGritPos.z + ((MapGritSize * 0.5f) - (GRIT_OK * m_OKD)) &&	//上
		m_pos.z >= MyGritPos.z - ((MapGritSize * 0.5f) - (GRIT_OK * m_OKU)))	//下
	{// グリットの中心位置に立っているか

		int nUGridZ = m_Grid.z - 1;
		int nDGridZ = m_Grid.z + 1;

		nUGridZ = useful::RangeNumber(CMapSystem::GetInstance()->GetHeightMax(), 0, nUGridZ);
		nDGridZ = useful::RangeNumber(CMapSystem::GetInstance()->GetHeightMax(), 0, nDGridZ);

		if (CMapSystem::GetInstance()->GetGritBool(m_Grid.x, nUGridZ) == true)
		{//上
			m_OKU = false;
		}
		else
		{
			m_OKU = true;
		}

		if (CMapSystem::GetInstance()->GetGritBool(m_Grid.x, nDGridZ) == true)
		{//下
			m_OKD = false;
		}
		else
		{
			m_OKD = true;
		}
	}

	if (!m_OKR && m_move.x > 0.0f)
	{
		if (m_pos.x > MyGritPos.x)
		{
			m_pos.x = MyGritPos.x;
			m_move.x = 0.0f;
		}
	}
	if (!m_OKL && m_move.x < 0.0f)
	{
		if (m_pos.x < MyGritPos.x)
		{
			m_pos.x = MyGritPos.x;
			m_move.x = 0.0f;
		}
	}
	if (!m_OKU && m_move.z > 0.0f)
	{
		if (m_pos.z > MyGritPos.z)
		{
			m_pos.z = MyGritPos.z;
			m_move.z = 0.0f;
		}
	}
	if (!m_OKD && m_move.z < 0.0f)
	{
		if (m_pos.z < MyGritPos.z)
		{
			m_pos.z = MyGritPos.z;
			m_move.z = 0.0f;
		}
	}

	// エフェクトの生成
	if (m_pEffect == nullptr && (fabsf(m_move.x) > 0.1f || fabsf(m_move.z) > 0.1f))
	{
		D3DXVECTOR3 rot = INITVECTOR3;
		rot.y = atan2f(-m_move.x, -m_move.z);
		D3DXMATRIX mat = *GetUseMultiMatrix();
		D3DXVECTOR3 ef = useful::CalcMatrix(m_pos, rot, mat);
		m_pEffect = MyEffekseer::EffectCreate(CMyEffekseer::TYPE_ROLL, true, ef, rot);
	}

	// エフェクトを移動
	if (m_pEffect != nullptr)
	{
		// 計算に使用する値の取得
		D3DXMATRIX mat = *GetUseMultiMatrix();

		// 座標と向きにマトリックスを反映
		D3DXVECTOR3 pos = useful::CalcMatrix(m_pos, m_rot, mat);

		// エフェクトに情報を適用
		m_pEffect->SetPosition(pos);
	}

	// エフェクトを消去
	if (m_pEffect != nullptr && fabsf(m_move.x) <= 0.1f && fabsf(m_move.z) <= 0.1f)
	{
		m_pEffect->SetDeath();
		m_pEffect = nullptr;
	}
}

//====================================================================
// 壁との当たり判定
//====================================================================
void CRollRock::CollisionWall(useful::COLLISION XYZ)
{
	// キューブブロックのリスト構造が無ければ抜ける
	if (CWall::GetList() == nullptr) { return; }
	std::list<CWall*> list = CWall::GetList()->GetList();    // リストを取得

	// キューブブロックリストの中身を確認する
	for (CWall* pWall : list)
	{
		D3DXVECTOR3 pos = pWall->GetPos();
		D3DXVECTOR3 posOld = pWall->GetPosOld();
		D3DXVECTOR3 Move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		D3DXVECTOR3 Size = pWall->GetSize();

		D3DXVECTOR3 ObjMove = INITVECTOR3;
		D3DXVECTOR3 MySize = GetSize();
		bool Jump = false;

		// 矩形の当たり判定
		if (useful::CollisionBlock(pos, pos, Move, Size, &m_pos, m_posOld, &m_move, &ObjMove, MySize, &Jump, XYZ) == true)
		{

		}
	}
}

//====================================================================
// ステージ外との当たり判定
//====================================================================
void CRollRock::CollisionOut()
{
	// キューブブロックのリスト構造が無ければ抜ける
	if (CDevil::GetList() == nullptr) { return; }
	std::list<CDevil*> list = CDevil::GetList()->GetList();    // リストを取得

	// キューブブロックリストの中身を確認する
	for (CDevil* pDevil : list)
	{
		D3DXVECTOR3 Pos = pDevil->GetDevilPos();
		D3DXVECTOR3 MapSize = CMapSystem::GetInstance()->GetMapSize();
		float GritSize = CMapSystem::GetInstance()->GetGritSize();

		// ステージ外の当たり判定
		if (Pos.x + MapSize.x < m_pos.x) // 右
		{
			m_pos.x = Pos.x - MapSize.x - GritSize;
		}
		if (Pos.x - MapSize.x - GritSize > m_pos.x) // 左
		{
			m_pos.x = Pos.x + MapSize.x;
		}
		if (Pos.z + MapSize.z + GritSize < m_pos.z) // 上
		{
			m_pos.z = Pos.z - MapSize.z;
		}
		if (Pos.z - MapSize.z > m_pos.z) // 下
		{
			m_pos.z = Pos.z + MapSize.z + GritSize;
		}
	}
}

//====================================================================
//リスト取得
//====================================================================
CListManager<CRollRock>* CRollRock::GetList(void)
{
	return m_pList;
}