#include "DxLib.h"
#include "Font.h"

namespace
{
	// フォント名
	const char* kFontName = "Makinas-4-Square";

	// 読み込むフォントファイル名
	const LPCSTR kFontDataPath[] =
	{
		"data/font/Makinas-4-Square.ttf",
	};

	// フォントのデータ
	struct FontData
	{
		const char* name;	// フォント名
		int size;			// フォントサイズ
		int thick;			// フォントの太さ(-1:デフォルト)
		int type;			// フォントのタイプ(-1:デフォルト)
	};
}

namespace Font
{
	std::vector<int> m_fontHandle;

	FontData data[] =
	{
		//kSize100_4
		{kFontName,100,4, DX_FONTTYPE_ANTIALIASING_8X8},
		// Size96_4
		{kFontName, 96, 4, DX_FONTTYPE_ANTIALIASING_8X8},
		// Size64_4
		{kFontName, 64, 4, DX_FONTTYPE_ANTIALIASING_8X8},
		// Siz55_4
		{kFontName, 55, 4, DX_FONTTYPE_ANTIALIASING_8X8},
	};


	/// <summary>
	/// フォントのロード
	/// </summary>
	void Font::Load()
	{
		Font::m_fontHandle.resize(static_cast<int>(Font::FontId::kNum));

		for (auto& fontPath : kFontDataPath)
		{
			if (AddFontResourceEx(fontPath, FR_PRIVATE, NULL) > 0) {
			}
			else
			{
				// フォント読込エラー処理
				MessageBox(NULL, "フォント読込失敗", "", MB_OK);
			}
		}

		// フォントデータ生成
		for (int i = 0; i < Font::m_fontHandle.size(); i++)
		{
			Font::m_fontHandle[i] = CreateFontToHandle(data[i].name, data[i].size, data[i].thick, data[i].type);
		}
	}


	/// <summary>
	/// フォントのアンロード
	/// </summary>
	void Font::UnLoad()
	{
		for (auto& fontPath : kFontDataPath)
		{
			if (RemoveFontResourceEx(fontPath, FR_PRIVATE, NULL))
			{
			}
			else
			{
#ifdef _DEBUG
				MessageBox(NULL, "フォント削除失敗", "", MB_OK);
#endif
			}
		}

		// フォントデータ削除
		for (const auto& handle : m_fontHandle)
		{
			DeleteFontToHandle(handle);
		}
	}
}
