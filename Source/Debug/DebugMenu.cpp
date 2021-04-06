#include "DebugMenu.h"
#include "DebugUtility.h"
#include <string.h>
#include <Input/InputUtility.h>

//----------
// 定数
//----------
const int ShowMaxMenu = 8;			///< 最大表示数
const int ShowStrLength = 15;		///< 表示文字数
const int FontSize = 23;			///< フォントサイズ
const float ShowPosX = 10.0f;		///< 表示位置X
const float ShowPosY = 5.0f;		///< 表示位置Y
const float CursorMargin = 23.0f;	///< カーソルからの位置


DebugMenu *DebugMenu::m_pInstance = NULL;


/**
 * @brief インスタンス取得
 * @return インスタンス
 */
DebugMenu *DebugMenu::Instance()
{
	return m_pInstance;
}
/**
 * @brief インスタンス生成
 */
void DebugMenu::Create()
{
	if (m_pInstance == NULL) {
		m_pInstance = NEW DebugMenu();
	}
}
/**
 * @brief インスタンス削除
 */
void DebugMenu::Delete()
{
	SAFE_DELETE(m_pInstance);
}


/**
 * @brief コンストラクタ
 */
DebugMenu::DebugMenu()
	: m_isOpen(false)
	, m_pTable(NULL)
	, m_pCurrent(NULL)
	, m_inputCount(0)
{
	m_menu.index = 0;
	m_menu.start = 0;
	m_menu.parent = NULL;
	m_pFont = NEW WinFont(FontSize, 1, 200);
}
/**
 * @brief デストラクタ
 */
DebugMenu::~DebugMenu()
{
	RecursionDelete(&m_menu);
	SAFE_DELETE(m_pFont);
}

/**
 * @brief 更新処理
 */
void DebugMenu::Update()
{
	if (m_menu.list.empty()) {
		return;
	}

	bool debug = InputUtil(0)->IsTrigger(INP_SELECT);
	if (IsOpenMenu()) {
		UpdateTable();
		if (m_pTable == NULL) {
			UpdateMenu();
		}		
	}
	else {
		if (debug) {
			m_isOpen = true;
			m_pCurrent = &m_menu;
		}
	}
}
/**
 * @brief 描画処理
 */
void DebugMenu::Draw()
{
	if (m_menu.list.empty()) {
		return;
	}

	// メニューが開いてなければ描画しない
	if (!IsOpenMenu()) {
		return;
	}

	// テーブルが選択されてる場合はテーブルを優先描画
	if (m_pTable != NULL) {
		DrawTable();
	}
	else {
		// メニュー描画
		DrawMenu();
	}
}

/**
 * @brief メニューが開いているか
 * @return true:開いてる
 */
bool DebugMenu::IsOpenMenu()
{
	return m_isOpen;
}
/**
 * @brief メニューに登録されているか
 * @param[in] name 登録名
 * @return true:登録されている
 */
bool DebugMenu::IsFindName(const char *name)
{
	return FindData(name) != NULL;
}

/**
 * @brief bool値登録
 * @param[in] name 登録名
 * @param[in] value 初期値
 */
void DebugMenu::EntryBool(const char *name, bool value)
{
	DataSet *p = CreateMenu(name);
	if (p != NULL) {
		p->dType = DT_BOOL;
		p->uData.bValue = value;
	}
}
/**
 * @brief int値登録
 * @param[in] name 登録名
 * @param[in] value 初期値
 * @param[in] max 最大値
 * @param[in] min 最小値
 */
void DebugMenu::EntryInt(const char *name, int value, int max, int min)
{
	DataSet *p = CreateMenu(name);
	if (p != NULL) {
		p->dType = DT_INT;
		p->uData.iData.value = value;
		p->uData.iData.max = max;
		p->uData.iData.min = min;
	}
}
/**
 * @brief float値登録
 * @param[in] name 登録名
 * @param[in] value 初期値
 * @param[in] max 最大値
 * @param[in] min 最小値
 */
void DebugMenu::EntryFloat(const char *name, float value, float max, float min)
{
	DataSet *p = CreateMenu(name);
	if (p != NULL) {
		p->dType = DT_FLOAT;
		p->uData.fData.value = value;
		p->uData.fData.max = max;
		p->uData.fData.min = min;
	}
}
/**
 * @brief テーブル登録
 * @param[in] name 登録名
 * @param[in] value テーブルリスト
 * @param[in] num テーブル数
 * @param[in] index 初期選択位置
 */
void DebugMenu::EntryTable(const char *name, const char *value[], int num, int index)
{
	DataSet *p = CreateMenu(name);
	if (p != NULL) {
		p->dType = DT_TABLE;
		p->uData.pTable = NEW TableData();
		p->uData.pTable->index = index;
		p->uData.pTable->start = index;
		// 文字リストへコピー
		for (int i = 0; i < num; ++i) {
			p->uData.pTable->list.push_back(value[i]);
		}
	}
}
/**
 * @brief コールバック登録
 * @param[in] name 登録名
 * @param[in] func 関数ポインタ
 * @param[in] arg コールバック引数
 */
void DebugMenu::EntryCallBack(const char *name, CallBack func, void *arg)
{
	DataSet *p = CreateMenu(name);
	if (p != NULL) {
		p->dType = DT_CALLBACK;
		p->uData.callback.func = func;
		p->uData.callback.arg = arg;
	}
}


/**
 * @brief bool値取得
 * @param[in] name 登録名
 * @return 現在の値
 */
bool DebugMenu::IsBool(const char *name)
{
	DataSet *p = FindData(name);
	if (p != NULL && p->dType == DT_BOOL) {
		return p->uData.bValue;
	}
	return false;
}
/**
 * @brief int値取得
 * @param[in] name 登録名
 * @return 現在の値
 */
int DebugMenu::GetInt(const char *name)
{
	DataSet *p = FindData(name);
	if (p != NULL && p->dType == DT_INT) {
		return p->uData.iData.value;
	}
	return 0;
}
/**
 * @brief float値取得
 * @param[in] name 登録名
 * @return 現在の値
 */
float DebugMenu::GetFloat(const char *name)
{
	DataSet *p = FindData(name);
	if (p != NULL && p->dType == DT_FLOAT) {
		return p->uData.fData.value;
	}
	return 0.0f;
}
/**
 * @brief 選択中のテーブルインデックス取得
 * @param[in] name 登録名
 * @return 選択中のテーブルインデックス,見つからなければ-1
 */
int DebugMenu::GetTableIndex(const char *name)
{
	DataSet *p = FindData(name);
	if (p != NULL && p->dType == DT_TABLE) {
		return p->uData.pTable->index;
	}
	return -1;
}
/**
 * @brief 選択中のテーブル名取得
 * @param[in] name 登録名
 * @return 選択中のテーブル名,見つからなければ""
 */
const char *DebugMenu::GetTableName(const char *name)
{
	DataSet *p = FindData(name);
	if (p != NULL && p->dType == DT_TABLE) {
		return p->uData.pTable->list[p->uData.pTable->index].c_str();
	}
	return "";
}


/**
 * @brief 再帰処理でデータ削除
 * @param[in] menu 処理中のメニュー
 */
void DebugMenu::RecursionDelete(MenuData *menu)
{
	MenuList::iterator it = menu->list.begin();
	while (it != menu->list.end()) {
		switch (it->dType) {
		case DT_TABLE:
			SAFE_DELETE(it->uData.pTable);
			break;
		case DT_MENU:
			RecursionDelete(it->uData.pMenu);
			SAFE_DELETE(it->uData.pMenu);
			break;
		default:
			break;
		}
		++it;
	}
}
//! @brief テーブル更新
void DebugMenu::UpdateTable()
{
	const InputInterface *pInput = InputUtil(0);
	bool up = pInput->IsRepeat(INP_UP);
	bool down = pInput->IsRepeat(INP_DOWN);
	bool enter = pInput->IsTrigger(INP_A);
	bool cancel = pInput->IsTrigger(INP_B);
	const DataSet *pData = &m_pCurrent->list[m_pCurrent->index];

	// テーブル展開中
	if (m_pTable != NULL) {
		// 項目上移動
		if (up) {
			--m_pTable->index;
			if (m_pTable->index < 0) {
				m_pTable->index = 0;
			}
			if (m_pTable->index < m_pTable->start) {
				m_pTable->start = m_pTable->index;
			}
		}
		// 項目下移動
		if (down) {
			++m_pTable->index;
			if (m_pTable->index >= m_pTable->list.size()) {
				m_pTable->index = static_cast<int>(m_pTable->list.size() - 1);
			}
			if (m_pTable->index - m_pTable->start >= ShowMaxMenu) {
				m_pTable->start = m_pTable->index - (ShowMaxMenu - 1);
			}
		}
		// 決定
		if (enter) {
			m_pTable = NULL;
		}
		// キャンセル
		if (cancel) {
			m_pTable->index = m_tempTable;
			m_pTable = NULL;
		}
	}
	else {
		if (enter && pData->dType == DT_TABLE) {
			// テーブル展開
			m_pTable = pData->uData.pTable;
			m_tempTable = m_pTable->index;
			m_pTable->start = min(max(0, m_tempTable - ShowMaxMenu / 3), static_cast<int>(m_pTable->list.size()) - ShowMaxMenu);
		}
	}
}
//! @brief メニュー更新
void DebugMenu::UpdateMenu()
{
	const InputInterface *pInput = InputUtil(0);
	bool debug = pInput->IsTrigger(INP_SELECT);
	bool up = pInput->IsRepeat(INP_UP);
	bool down = pInput->IsRepeat(INP_DOWN);
	bool left = pInput->IsRepeat(INP_LEFT);
	bool right = pInput->IsRepeat(INP_RIGHT);
	bool enter = pInput->IsTrigger(INP_A);
	bool cancel = pInput->IsTrigger(INP_B);
	bool shift = pInput->IsPress(INP_R1);
	DataSet *pMenu = &m_pCurrent->list[m_pCurrent->index];
	
	// 項目上移動
	if (up) {
		--m_pCurrent->index;
		if (m_pCurrent->index < 0) {
			m_pCurrent->index = 0;
		}
		if (m_pCurrent->index < m_pCurrent->start) {
			m_pCurrent->start = m_pCurrent->index;
		}
	}
	// 項目下移動
	if (down) {
		++m_pCurrent->index;
		if (m_pCurrent->index >= m_pCurrent->list.size()) {
			m_pCurrent->index = static_cast<int>(m_pCurrent->list.size() - 1);
		}
		if (m_pCurrent->index - m_pCurrent->start >= ShowMaxMenu) {
			m_pCurrent->start = m_pCurrent->index - (ShowMaxMenu - 1);
		}
	}

	// 値増減
	if (left || right) {
		int dir = left ? -1 : 1;
		dir *= shift ? 10 : 1;
		dir *= min(10, (m_inputCount / 30) + 1);
		switch (pMenu->dType) {
		case DT_BOOL:
			if (left) { pMenu->uData.bValue = false; }
			if (right) { pMenu->uData.bValue = true; }
			break;
		case DT_INT:
			pMenu->uData.iData.value += dir;
			if (pMenu->uData.iData.max > pMenu->uData.iData.min) {
				pMenu->uData.iData.value =
					min(pMenu->uData.iData.max,
					max(pMenu->uData.iData.min, pMenu->uData.iData.value));
			}
			break;
		case DT_FLOAT:
			pMenu->uData.fData.value += dir * 0.01f;
			if (pMenu->uData.fData.max > pMenu->uData.fData.min) {
				pMenu->uData.fData.value =
					min(pMenu->uData.fData.max,
					max(pMenu->uData.fData.min, pMenu->uData.fData.value));
			}
			break;
		}
		++ m_inputCount;
	}
	else {
		if (!(InputUtil(0)->IsPress(INP_LEFT)
			|| InputUtil(0)->IsPress(INP_RIGHT)))
		{
			m_inputCount = 0;
		}
	}

	// 決定
	if (enter) {
		switch (pMenu->dType) {
		case DT_BOOL:
			pMenu->uData.bValue ^= true;
			break;
		case DT_CALLBACK: // 関数呼び出し
			pMenu->uData.callback.func(pMenu->uData.callback.arg);
			m_isOpen = false;
			break;
		case DT_MENU: // 階層移動
			m_pCurrent = pMenu->uData.pMenu;
			break;
		}
	}

	// キャンセル
	if (cancel) {
		if (m_pCurrent->parent != NULL) {
			m_pCurrent = m_pCurrent->parent;
		}
		else {
			m_isOpen = false;
		}
	}
}
/**
 * @brief テーブル描画
 */
void DebugMenu::DrawTable()
{
	int showNum = min(ShowMaxMenu, static_cast<int>(m_pTable->list.size()));
	int currentIdx = m_pTable->index - m_pTable->start;
	float x = ShowPosX;
	float y = ShowPosY;
	for (int i = 0; i < showNum; ++i) {
		std::string &str = m_pTable->list[m_pTable->start + i];
		// 矢印
		if (i == currentIdx) {
			m_pFont->SetPos(x, y);
			m_pFont->Draw(">");
		}
		// テーブル名
		m_pFont->SetPos(x + CursorMargin, y);
		m_pFont->Draw(str.c_str());

		y += FontSize;
	}
}
/**
 * @brief メニュー描画
 */
void DebugMenu::DrawMenu()
{
	int showNum = min(ShowMaxMenu, static_cast<int>(m_pCurrent->list.size()));
	int currentIdx = m_pCurrent->index - m_pCurrent->start;
	char strVal[ShowStrLength];
	float x = ShowPosX;
	float y = ShowPosY;
	for (int i = 0; i < showNum; ++i) {
		DataSet *p = &m_pCurrent->list[m_pCurrent->start + i];
		// 矢印
		if (i == currentIdx) {
			m_pFont->SetPos(x, y);
			m_pFont->Draw(">");
		}
		// メニュー名
		m_pFont->SetPos(x + CursorMargin, y);
		m_pFont->Draw(p->name.c_str());
		// データ別値表示
		m_pFont->SetPos(x + (ShowStrLength / 2) * FontSize + CursorMargin, y);
		switch (p->dType) {
		case DT_BOOL:
			m_pFont->Draw(p->uData.bValue ? "TRUE" : "FALSE");
			break;
		case DT_INT:
			sprintf_s(strVal, sizeof(strVal), "%d", p->uData.iData.value);
			m_pFont->Draw(strVal);
			break;
		case DT_FLOAT:
			sprintf_s(strVal, sizeof(strVal), "%.2f", p->uData.fData.value);
			m_pFont->Draw(strVal);
			break;
		case DT_TABLE:
			m_pFont->Draw(p->uData.pTable->list[p->uData.pTable->index].c_str());
			break;
		case DT_CALLBACK:
			m_pFont->Draw(">>>");
			break;
		case DT_MENU:
			m_pFont->Draw("---");
			break;
		}

		y += FontSize;
	}
}
/**
 * @brief メニューパス取得
 * @param[in] name 登録名
 * @return パスリスト
 */
DebugMenu::StringList DebugMenu::GetMenuPath(const char *name)
{
	// '/'で文字分割
	std::string str = name;
	StringList strList;
	int start = 0;
	int index = 0;
	while (name[index] != '\0') {
		if (name[index] == '/') {
			// '/'までの長さで文字取得
			strList.push_back(str.substr(start, index - start));
			start = index + 1;
		}
		++index;
	}
	// 最後の'/'以降のデータを文字列で取得
	if (start != index) {
		strList.push_back(str.substr(start, index - start));
	}
	return strList;
}
/**
 * @brief メニュー作成
 * @param[in] name 登録名
 * @return 作成したメニュー
 */
DebugMenu::DataSet *DebugMenu::CreateMenu(const char *name)
{
	// 登録先リスト取得
	StringList strList = GetMenuPath(name);

	// 登録できなければ終了
	if (strList.empty()) {
		return NULL;
	}

	// 登録先構築
	StringList::iterator sIt = strList.begin();
	MenuData *pMenu = &m_menu;
	while (sIt != strList.end()) {
		// メニュー内に指定文字があるか探索
		MenuList::iterator mIt = pMenu->list.begin();
		while (mIt != pMenu->list.end()) {
			if (mIt->name == *sIt) {
				if (mIt->dType == DT_MENU) {
					pMenu = mIt->uData.pMenu;
					mIt = pMenu->list.begin();
					break;
				}
				else {
					// メニュー以外の登録済み文字が見つかったので終了
					return NULL;
				}
			}
			++mIt;
		}

		// 指定された文字のメニューが見つからなかった
		if (mIt == pMenu->list.end()) {
			// INTで仮作成
			pMenu->list.push_back({ *sIt, DT_INT, 0 });
			DataSet *p = &pMenu->list[pMenu->list.size() - 1];
			if (sIt == strList.end() - 1) {
				// 最終文字なので登録先データとして返却
				return p;
			}
			else {
				// 登録先探索途中なのでメニューとして作成
				p->dType = DT_MENU;
				p->uData.pMenu = NEW MenuData();
				p->uData.pMenu->parent = pMenu;
				pMenu = p->uData.pMenu;
				pMenu->start = 0;
				pMenu->index = 0;
			}
		}
		++sIt;
	}
	return NULL;
}
/**
 * @brief 登録データ探索
 * @param[in] name 登録名
 * @return データ
 */
DebugMenu::DataSet *DebugMenu::FindData(const char *name)
{
	// メニューに何も登録されていない
	if (m_menu.list.empty()) {
		return NULL;
	}

	// メニュー内探索
	MenuData *p = &m_menu;
	MenuList::iterator mIt = p->list.begin();
	StringList strList = GetMenuPath(name);
	StringList::iterator sIt = strList.begin();
	while (sIt != strList.end()) {
		while (mIt != p->list.end()) {
			if (mIt->name == *sIt) {
				if (sIt == strList.end() - 1) {
					// 探索文字が最終パスだったので該当データを返却
					return mIt._Ptr;
				}
				else if (mIt->dType == DT_MENU) {
					// 探索文字がメニューなので次の階層へ移動
					p = mIt->uData.pMenu;
					mIt = p->list.begin();
					break;
				}
			}
			++mIt;
		}
		++sIt;
	}

	return NULL;
}

// EOF