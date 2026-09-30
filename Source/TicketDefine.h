#pragma once
#include "../Library/myDxLib.h"
#include <string>
#include <assert.h>

namespace TicketDefine
{
	//馬券の券種
	enum TicketType
	{
		NONE = -1,//何も選択されていない状態
		WIN = 0,//単勝
		PLACE,//複勝
		QUINELLA,//馬連
		//BRACKET_QUINELLA,//枠連
		EXACTA,//馬単
		QUINELLA_PLACE,//ワイド
		TRIO,//三連複
		TRIFECTA,//三連単

		TYPE_NUM,
	};

	//チケットの種類を文字列に変換する
	inline std::string TicketTypeConvString(TicketType _ticketType)
	{
		{
			std::string ret;
			switch (_ticketType)
			{
			case WIN:
				ret = "単勝";
				break;
			case PLACE:
				ret = "複勝";
				break;
			case QUINELLA:
				ret = "馬連";
				break;
			case EXACTA:
				ret = "馬単";
				break;
			case QUINELLA_PLACE:
				ret = "ワイド";
				break;
			case TRIO:
				ret = "三連複";
				break;
			case TRIFECTA:
				ret = "三連単";
				break;
			default:
				assert(false && "異常な馬券タイプが入力されています");
				break;
			}
			return ret;
		}
	}

	/// <summary>
	/// 馬券データ
	/// </summary>
	/// <param name="_price">購入する金額　最低100円～</param>
	/// <param name="_ticketType">馬券の券種</param>
	/// <param name="_firstHorse">購入する1列目の馬</param>
	/// <param name="_secondHorse">購入する2列目の馬(単勝、複勝なら必要ないので入力せず-1でok)</param>
	/// <param name="_thirdHorse">購入する3列目の馬(三連複、三連単以外なら必要ないので入力せず-1でok)</param>
	struct TicketData
	{
		int price;
		TicketType type;
		int firstHorse;
		int secondHorse;
		int thirdHorse;

		TicketData(int _price = -1, TicketType _ticketType = WIN, int _firstHorse = -1, int _secondHorse = -1, int _thirdHorse = -1) :
			price(_price), type(_ticketType), firstHorse(_firstHorse), secondHorse(_secondHorse), thirdHorse(_thirdHorse)
		{
		}

		//馬券の内容を文字列に変換する
		std::string TicketContentToString()const
		{
			std::string ret;
			switch (type)
			{
			case WIN:
			case PLACE:
				ret = std::to_string(firstHorse);
				break;
			case QUINELLA:
			case QUINELLA_PLACE:
				ret = std::to_string(firstHorse) + "-" + std::to_string(secondHorse);
				break;
			case EXACTA:
				ret = std::to_string(firstHorse) + "→" + std::to_string(secondHorse);
				break;
			case TRIO:
				ret = std::to_string(firstHorse) + "-" + std::to_string(secondHorse) + "-" + std::to_string(thirdHorse);
				break;
			case TRIFECTA:
				ret = std::to_string(firstHorse) + "→" + std::to_string(secondHorse) + "→" + std::to_string(thirdHorse);
				break;
			default:
				assert(false && "異常な馬券の種類が入力されています");
				break;
			}

			return ret;
		}
	};

}