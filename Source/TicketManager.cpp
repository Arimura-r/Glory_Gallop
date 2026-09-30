#include "TicketManager.h"
#include "HorseManager.h"
#include "Horse.h"
#include "CommonManager.h"
#include "GameStartData.h"
#include "OddsManager.h"

TicketManager::TicketManager(HorseManager* _horse, CommonManager* _common):horseManager(_horse),commonManager(_common)
{
	money = _common->FindCommonObject<GameStartData>()->GetData().money;
	oddsManager = new OddsManager(_horse);
}

TicketManager::~TicketManager()
{
	auto data= commonManager->FindCommonObject<GameStartData>()->GetData();
	data.money = money;
	commonManager->FindCommonObject<GameStartData>()->SetData(data);
}

void TicketManager::Update()
{
}

int TicketManager::GetTicketPayOut(const TicketDefine::TicketData& _ticket) const
{
	int ret = 0;

	int first = horseManager->GetHorse(0)->GetHorseNumber();
	int second = horseManager->GetHorse(1)->GetHorseNumber();
	int third = horseManager->GetHorse(2)->GetHorseNumber();

	//仮の処理　オッズ情報を持たせたらそこから参照するようにする
	switch (_ticket.type)
	{
	case TicketDefine::WIN:
		if (IsWinBetWinning(_ticket, first, second, third))
			ret = _ticket.price * (int)(GetOdds(_ticket) * 10)/10;//オッズの小数第2位以下を切り捨てる
		break;
	case TicketDefine::PLACE:
		if (IsPlaceBetWinning(_ticket, first, second, third))
			ret = _ticket.price * (int)(GetOdds(_ticket) * 10)/10;
		break;
	case TicketDefine::QUINELLA:
		if (IsQuinellaBetWinning(_ticket, first, second, third))
			ret = _ticket.price * 10;
		break;
	case TicketDefine::EXACTA:
		if (IsExactaBetWinning(_ticket, first, second, third))
			ret = _ticket.price * 10;
		break;
	case TicketDefine::QUINELLA_PLACE:
		if (IsQuinellaPlaceBetWinning(_ticket, first, second, third))
			ret = _ticket.price * 10;
		break;
	case TicketDefine::TRIO:
		if (IsTrioBetWinning(_ticket, first, second, third))
			ret = _ticket.price * 10;
		break;
	case TicketDefine::TRIFECTA:
		if (IsTrifectaBetWinning(_ticket, first, second, third))
			ret = _ticket.price * 10;
		break;
	default:
		break;
	}
	return ret;
}

TicketDefine::TicketData TicketManager::GetResultTicket(TicketDefine::TicketType _ticketType) const
{
	return TicketDefine::TicketData(-1, _ticketType, horseManager->GetHorse(0)->GetHorseNumber(), horseManager->GetHorse(1)->GetHorseNumber() , horseManager->GetHorse(2)->GetHorseNumber() );
}

float TicketManager::GetOdds(const TicketDefine::TicketData& _ticket)const
{	
	float ret = 1.0f;

	switch (_ticket.type)
	{
	case TicketDefine::WIN:
		ret=oddsManager->GetWinOdds(_ticket.firstHorse-1);
		break;
	case TicketDefine::PLACE:
		ret = oddsManager->GetPlaceOdds(_ticket.firstHorse - 1);
		break;
	default:
		assert(false && "オッズの入力されていない要素が選択されています");
		break;
	}

	return ret;
}

int TicketManager::GetFavoriteOrder(int _horseNumber) const
{
	return oddsManager->GetHorseFavoriteOrder(_horseNumber-1);
}

bool TicketManager::ValidateBetTicket(const TicketDefine::TicketData& _data)
{
	int maxHorseNum = (int)horseManager->GetHorses().size();

	//金額の確認
	if (_data.price < BET_MIN_MONEY || _data.price>money ||
		_data.price % BET_MIN_MONEY != 0)
	{
		assert(false && "金額に異常な値が入力されています");
		return false;
	}
	//券種の確認
	if (_data.type < TicketDefine::WIN || _data.type >= TicketDefine::TYPE_NUM)
	{
		assert(false && "券種に異常な値が入力されています");
		return false;
	}
	//1列目の馬番の確認
	if (_data.firstHorse<1 || _data.firstHorse>maxHorseNum)
	{
		assert(false && "1列目の馬に異常な値が入力されています");
		return false;
	}
	if (_data.type != TicketDefine::WIN && _data.type != TicketDefine::PLACE)
	{
		//2列目の馬番の確認(単勝、複勝以外)
		if (_data.secondHorse<1 || _data.secondHorse>maxHorseNum || _data.firstHorse == _data.secondHorse)
		{
			assert(false && "2列目の馬に異常な値が入力されています");
			return false;
		}
		//3列目の馬番の確認(三連複、三連単のみ)
		if (_data.type == TicketDefine::TRIO || _data.type == TicketDefine::TRIFECTA ||
			_data.firstHorse == _data.thirdHorse || _data.secondHorse == _data.thirdHorse)
		{
			if (_data.thirdHorse<1 || _data.thirdHorse>maxHorseNum)
			{
				assert(false && "3列目の馬に異常な値が入力されています");
				return false;
			}
		}
	}

	return true;
}

void TicketManager::Winning(const TicketDefine::TicketData& _data)
{
	money += _data.price * (int)(GetOdds(_data) * 10) / 10;
	hitTickets.push_back(_data);
}

bool TicketManager::IsWinBetWinning(const TicketDefine::TicketData& _ticket, int _first, int _second, int _third) const
{
	return _ticket.firstHorse == _first;
}

bool TicketManager::IsPlaceBetWinning(const TicketDefine::TicketData& _ticket, int _first, int _second, int _third) const
{
	return _ticket.firstHorse == _first || _ticket.firstHorse == _second || _ticket.firstHorse == _third;
}

bool TicketManager::IsQuinellaBetWinning(const TicketDefine::TicketData& _ticket, int _first, int _second, int _third) const
{
	return (_ticket.firstHorse == _first || _ticket.firstHorse == _second) && (_ticket.secondHorse == _first || _ticket.secondHorse == _second);
}

bool TicketManager::IsExactaBetWinning(const TicketDefine::TicketData& _ticket, int _first, int _second, int _third) const
{
	return _ticket.firstHorse == _first && _ticket.secondHorse == _second;
}

bool TicketManager::IsQuinellaPlaceBetWinning(const TicketDefine::TicketData& _ticket, int _first, int _second, int _third) const
{
	return (_ticket.firstHorse == _first || _ticket.firstHorse == _second||_ticket.firstHorse==_third) && 
		(_ticket.secondHorse == _first || _ticket.secondHorse == _second || _ticket.secondHorse == _third);
}

bool TicketManager::IsTrioBetWinning(const TicketDefine::TicketData& _ticket, int _first, int _second, int _third) const
{
	return (_ticket.firstHorse == _first || _ticket.firstHorse == _second || _ticket.firstHorse == _third) &&
		(_ticket.secondHorse == _first || _ticket.secondHorse == _second || _ticket.secondHorse == _third)&&
		(_ticket.thirdHorse == _first || _ticket.thirdHorse == _second || _ticket.thirdHorse == _third);
}

bool TicketManager::IsTrifectaBetWinning(const TicketDefine::TicketData& _ticket, int _first, int _second, int _third) const
{
	return _ticket.firstHorse == _first && _ticket.secondHorse == _second && _ticket.thirdHorse == _third;
}

int TicketManager::BetTicket(const TicketDefine::TicketData& _data)
{
	if (ValidateBetTicket(_data))
	{
		betTickets.push_back(_data);
		money -= _data.price;
		return 1;
	}
	return -1;
}

void TicketManager::PayOut()
{
	int first = horseManager->GetHorse(0)->GetHorseNumber();
	int second = horseManager->GetHorse(1)->GetHorseNumber();
	int third = horseManager->GetHorse(2)->GetHorseNumber();

	for (auto& t : betTickets)
	{
		switch (t.type)
		{
		case TicketDefine::WIN:
			if (IsWinBetWinning(t, first, second, third))
				Winning(t);
			break;
		case TicketDefine::PLACE:
			if (IsPlaceBetWinning(t, first, second, third))
				Winning(t);
			break;
		case TicketDefine::QUINELLA:
			if (IsQuinellaBetWinning(t, first, second, third))
				Winning(t);
			break;
		case TicketDefine::EXACTA:
			if (IsExactaBetWinning(t, first, second, third))
				Winning(t);
			break;
		case TicketDefine::QUINELLA_PLACE:
			if (IsQuinellaPlaceBetWinning(t, first, second, third))
				Winning(t);
			break;
		case TicketDefine::TRIO:
			if (IsTrioBetWinning(t, first, second, third))
				Winning(t);
			break;
		case TicketDefine::TRIFECTA:
			if (IsTrifectaBetWinning(t, first, second, third))
				Winning(t);
			break;
		default:
			break;
		}
	}
}
