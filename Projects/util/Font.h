#pragma once
#include <vector>

// フォント関連
namespace Font
{
	// フォントの種類
	enum class FontId
	{
		// フォントサイズ、太さ
		kSize100_4,
		kSize96_4,
		kSize64_4,
		kSize55_4,
		kNum,		// フォントの種類数
	};

	// フォントのロード、アンロード
	void Load();
	void UnLoad();

	// フォントのハンドル
	// MEMO:externを使用して異なるソースファイルで変数を共有できるようにする
	extern std::vector<int> m_fontHandle;
}



