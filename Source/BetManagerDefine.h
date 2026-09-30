#pragma once
#include "../Library/myDxLib.h"

//馬券購入画面に関するDefine
namespace BetManagerDefine
{
	//馬券購入項目でのカテゴリ
	enum BetCategory
	{
		BETTING_TICKET = 0,
		WIN,//単勝
		PLACE,//複勝
		QUINELLA,//馬連
		//BRACKET_QUINELLA,//枠連
		EXACTA,//馬単
		QUINELLA_PLACE,//ワイド
		TRIO,//三連複
		TRIFECTA,//三連単

		CATEGORY_NUM,
	};
}