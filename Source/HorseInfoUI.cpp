#include "HorseInfoUI.h"
#include "HorseManager.h"
#include "Horse.h"
#include "GameManager.h"
#include "BeforeRaceState.h"
#include "HorseDataDrawManager.h"
#include "HorseCommentData.h"
#include "CsvReader.h"
#include "TicketManager.h"

namespace
{
	const VECTOR2 BACK_BOX_SIZE = VECTOR2(900, 800);
	const int BACK_BOX_ALPHA = 128;

	const int HORSE_NAME_FONT_SIZE = 40;
	const VECTOR2 HORSE_NAME_OFFSET = VECTOR2(50, 25);

	const int FAVORITE_FONT_SIZE = 32;
	const VECTOR2 FAVORITE_OFFSET = VECTOR2(50, 100);
	const float RED_ODDS_MAX = 10.0f;//これ以下ならオッズ部を赤く表示する

	const int RUNNING_STYLE_FONT_SIZE = 32;
	const VECTOR2 RUNNING_STYLE_OFFSET = VECTOR2(650, 80);
	const float FRONTRUNNER_FRONT_TARGET_POSITION = 0.0f;//目標順位がこれ以上なら逃げ馬と定義する
	const float PACECHASER_FRONT_TARGET_POSITION = 0.4f;//目標順位の上限がこれ以上なら先行馬と定義する
	//const float PACECHASER_MAX_TARGET_POSITION = 0.5f;//目標順位の下限がこれ以上なら先行馬と定義する
	//const float LATESURGER_MIN_TARGET_POSITION = 0.5f;//目標順位の上限がこれ以下なら差し馬と定義する
	const float LATESURGER_BACK_TARGET_POSITION = 0.5f;//目標順位の下限がこれ以下なら差し馬と定義する
	const float ENDCLOSER_BACK_TARGET_POSITION = 0.85f;//目標順位の下限がこれ以下なら追込馬と定義する


	const int BEFORE_RACE_RESULT_FONT_SIZE = 32;
	const VECTOR2 BEFORE_RACE_RESULT_OFFSET = VECTOR2(50, 200);
	const int BEFORE_RACE_RESULT_INTERVAL = 60;
	const int BEFORE_RACE_RESULT_NUM = 5;//過去何レースの情報を表示するか
	
	const int FIRST_COLOR = 0xffff00;//1着になった際の表示カラー
	const int SECOND_COLOR = 0xaaaaaa;//2着になった際の表示カラー
	const int THIRD_COLOR = 0xff7f50;//3着になった際の表示カラー

	const int HORSE_COMMENT_CATEGORY_FONT_SIZE = 32;//カテゴリ名のフォントサイズ
	const int HORSE_COMMENT_FONT_SIZE = 24;
	const VECTOR2 HORSE_COMMENT_OFFSET = VECTOR2(50, 550);
	const int COMMENT_CATEGORY_INTERVAL = 50;//カテゴリ名と文章本体の間の大きさ

	const std::string RACE_RESULT_DATA_FILE_PATH = "data/csv/goalData.csv";
}


HorseInfoUI::HorseInfoUI(VECTOR2 _position, HorseManager* _horseManager, GameManager* _gameManager, TicketManager* _ticketManager)
	:UIBase(_position),horseManager(_horseManager),gameManager(_gameManager),ticketManager(_ticketManager)
{
	BeforeRaceResultLoad();
}

HorseInfoUI::~HorseInfoUI()
{
}

void HorseInfoUI::DrawUI()
{
	if (gameManager->GetBeforeRaceState()->GetCurrentBeforeRaceSelect() != BeforeRaceState::ALL_HORSE_DATA)
		return;

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, BACK_BOX_ALPHA);
	//テキストの背景に黒四角を置く
	DrawBox((int)position.x, (int)position.y, (int)position.x + (int)BACK_BOX_SIZE.x, (int)position.y + (int)BACK_BOX_SIZE.y, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);

	auto& h = horseManager->GetHorses()[gameManager->GetBeforeRaceState()->GetHorseDataDrawManager()->GetCursorHorseNum()];

	SetFontSize(HORSE_NAME_FONT_SIZE);
	//名前の表示
	DrawFormatStringF(position.x+HORSE_NAME_OFFSET.x, position.y+HORSE_NAME_OFFSET.y, 0xffffff, "%2d番　%s",h->GetHorseNumber(), h->GetParam().name.c_str());
	
	//脚質表示
	RunningStyleDraw();
	//人気表示
	FavoriteDraw();
	//過去の着順表示
	BeforeRaceResultDraw();
	//コメント表示
	HorseCommentDraw();

	SetFontSize(DEFAULT_FONT_SIZE);
}

void HorseInfoUI::BeforeRaceResultLoad()
{
	CsvReader* csv = new CsvReader();
	csv->Load(RACE_RESULT_DATA_FILE_PATH);
	//1行目を飛ばし、逆順で情報を得る(前走を一番上にするため逆順でループを回すを行う)
	for (int l = (int)csv->GetLines() - 1; 0 < l; l--)
	{
		std::vector<int> raceResult;
		for (int c = 0; c < csv->GetColums(l); c++)
		{
			raceResult.push_back(csv->GetInt(l, c));
		}
		beforeRaceResults.push_back(raceResult);
	}

	delete csv;
}

void HorseInfoUI::FavoriteDraw()
{
	SetFontSize(RUNNING_STYLE_FONT_SIZE);

	int horseIndex = gameManager->GetBeforeRaceState()->GetHorseDataDrawManager()->GetCursorHorseNum();
	float odds = ticketManager->GetOdds(TicketDefine::TicketData(-1, TicketDefine::WIN, horseIndex + 1));
	int favOrder=ticketManager->GetFavoriteOrder(horseIndex + 1);
	int color = 0xffffff;

	if (odds < RED_ODDS_MAX)
		color = 0xff4444;

	DrawFormatStringF(position.x + FAVORITE_OFFSET.x, position.y + FAVORITE_OFFSET.y, color, "単勝オッズ：%.1f倍　%2d人気",odds,favOrder);
}

void HorseInfoUI::RunningStyleDraw()
{
	auto& h = horseManager->GetHorses()[gameManager->GetBeforeRaceState()->GetHorseDataDrawManager()->GetCursorHorseNum()];
	SetFontSize(RUNNING_STYLE_FONT_SIZE);

	bool isFrontrunner = FRONTRUNNER_FRONT_TARGET_POSITION >= h->GetParam().boost.frontChargeRange;
	bool isPaceChaser = PACECHASER_FRONT_TARGET_POSITION >= h->GetParam().boost.frontChargeRange;
	bool isLateSurger = LATESURGER_BACK_TARGET_POSITION <= h->GetParam().boost.backChargeRange;
	bool isEndCloser = ENDCLOSER_BACK_TARGET_POSITION <= h->GetParam().boost.backChargeRange;

	DrawFormatStringF(position.x + RUNNING_STYLE_OFFSET.x, position.y + RUNNING_STYLE_OFFSET.y,0xffffff, "脚質\n\n %s\n %s\n %s\n %s",
		isFrontrunner ? "▲":"△", isPaceChaser ? "▲" : "△", isLateSurger ? "▲" : "△", isEndCloser ? "▲" : "△");
}

void HorseInfoUI::BeforeRaceResultDraw()
{
	SetFontSize(BEFORE_RACE_RESULT_FONT_SIZE);
	for (int i = 0; i < BEFORE_RACE_RESULT_NUM; i++)
	{
		std::string raceName = "前走";
		if (i != 0)
			raceName = std::to_string(i + 1) + "走前";

		//指定した馬の前走～指定したところまでの着順を得る
		int result = beforeRaceResults[i][gameManager->GetBeforeRaceState()->GetHorseDataDrawManager()->GetCursorHorseNum()] + 1;
		std::string raceResult = std::to_string(result) + "着";

		//着順によって表示色を変える
		int color = 0xffffff;
		switch (result)
		{
		case 1:
			color = FIRST_COLOR;
			break;
		case 2:
			color = SECOND_COLOR;
			break;
		case 3:
			color = THIRD_COLOR;
			break;
		}

		DrawFormatStringF(position.x + BEFORE_RACE_RESULT_OFFSET.x, position.y + BEFORE_RACE_RESULT_OFFSET.y + BEFORE_RACE_RESULT_INTERVAL * i, color, "%6s：%6s", raceName.c_str(), raceResult.c_str());
	}
}

void HorseInfoUI::HorseCommentDraw()
{
	SetFontSize(HORSE_COMMENT_CATEGORY_FONT_SIZE);
	DrawFormatStringF(position.x + HORSE_COMMENT_OFFSET.x, position.y + HORSE_COMMENT_OFFSET.y, 0xffffff, "陣営コメント");
	SetFontSize(HORSE_COMMENT_FONT_SIZE);
	DrawFormatStringF(position.x + HORSE_COMMENT_OFFSET.x, position.y + HORSE_COMMENT_OFFSET.y+COMMENT_CATEGORY_INTERVAL, 0xffffff, "%s",horseManager->GetHorseCommentData()->GetHorseComments()[gameManager->GetBeforeRaceState()->GetHorseDataDrawManager()->GetCursorHorseNum()].c_str());
}
