#pragma once
#include "easing.h"

#include "../Library/Time.h"


class EasingUtil {
public:

	/**
		@brief		イージングで使用するために必要な情報を保持する構造体

		@details	この構造体は、イージングの進行状況を追跡するために使用されます。
					イージングの開始値、終了値、現在値、効果時間などの情報を保持します。
					また、イージングの種類も指定できます。
	*/
	template<typename T = float>
	struct EasingInfo {

		T current;					// 現在値
		T begin;					// 初期値
		T end;						// 終了値
		float duration_sec;			// 効果時間
		float duration_sec_total;	// 総効果時間
		bool isActive;				// 機能しているか
		EasingType easingType;		// 補間種類

		/**
			@brief		イージング情報の初期化コンストラクタ
		*/
		EasingInfo() :  
			current				(0.0f), 
			begin				(0.0f),  
			end					(0.0f), 
			duration_sec		(0.0f),  
			duration_sec_total	(1.0f),  
			isActive			(false),  
			easingType			(EasingType::Linear)  
		{  
		};
		
		/**
			@brief	イージング挙動を設定する関数
			@param	begin			:	開始値
			@param	end				:	終了値
			@param	duration_sec	:	効果時間（秒単位）
			@param	easingType		:	イージングの種類（EasingType 列挙型）
			@param	active			:	イージングを開始するかどうかのフラグ(デフォルトは true)

			@details	この関数は、イージングの開始値、終了値、効果時間、およびイージングの種類を設定します。
						また、イージングがアクティブかどうかを指定することもできます。
						この関数を呼び出すことで、イージングの進行状況を初期化し、指定されたパラメータに基づいてイージングを開始します。
		*/
		void SetEasing(const T& begin, const T& end, float duration_sec, EasingType easingType, bool active = true) {
			this->current			 = begin;
			this->begin				 = begin;
			this->end				 = end;
			this->duration_sec		 = duration_sec;
			this->duration_sec_total = duration_sec;
			this->easingType		 = easingType;
			this->isActive			 = active;
		}

		/**
			@brief	イージング情報をリセットする関数
		*/
		void Reset() {
			current				= 0.0f;
			begin				= 0.0f;
			end					= 0.0f;
			duration_sec		= 0.0f;
			duration_sec_total	= 1.0f;
			isActive			= false;
			easingType			= EasingType::Linear;
		}
	};
	
	/**
		@brief	イージングの進行状況を更新する静的関数
		@param	info	:	イージング情報を保持する EasingInfo 型のポインタ

		@details	この関数は、指定されたイージング情報を使用して、イージングの進行状況を更新します。
					イージングがアクティブである場合、経過時間に基づいて現在値を計算し、必要に応じてイージングを終了します。
					イージングの種類に応じて、適切なイージング関数が呼び出されます。
		
		@remarks	この関数は、ゲームのフレームごとに呼び出されることを想定しています。
					int型、double型、float型は問題なく動作することを確認しています。
					VECTORなどの新たに定義された物の場合、上手く動作しない可能性がございます。
					予めご了承ください。
	*/
	template<typename T = float>
	[[nodiscard]] static inline void Update(EasingInfo<T>* info)
	{
		// 引数が nullptr の場合は何もしない
		if (info == nullptr)
			return;

		// イージングがアクティブでない場合は何もしない
		if (info->isActive == false)
			return;

		info->duration_sec -= Time::GameDeltaTime();

		// 効果時間の割合
		float duration_rate = 1.0f - (info->duration_sec / info->duration_sec_total);

		// 効果時間が終了した場合、割合を1.0fに設定し、イージングを非アクティブにする
		if (duration_rate >= 1.0f) {
			duration_rate = 1.0f;
			info->isActive = false;
		}

		switch (info->easingType) {
		case EasingType::None:			break;
		case EasingType::InQuad:		info->current = EasingFunc::InQuad		(duration_rate, 1.0f, info->end, info->begin); break;
		case EasingType::OutQuad:		info->current = EasingFunc::OutQuad		(duration_rate, 1.0f, info->end, info->begin); break;
		case EasingType::InOutQuad:		info->current = EasingFunc::InOutQuad	(duration_rate, 1.0f, info->end, info->begin); break;
		case EasingType::InCubic:		info->current = EasingFunc::InCubic		(duration_rate, 1.0f, info->end, info->begin); break;
		case EasingType::OutCubic:		info->current = EasingFunc::OutCubic	(duration_rate, 1.0f, info->end, info->begin); break;
		case EasingType::InOutCubic:	info->current = EasingFunc::InOutCubic	(duration_rate, 1.0f, info->end, info->begin); break;
		case EasingType::InQuart:		info->current = EasingFunc::InQuart		(duration_rate, 1.0f, info->end, info->begin); break;
		case EasingType::OutQuart:		info->current = EasingFunc::OutQuart	(duration_rate, 1.0f, info->end, info->begin); break;
		case EasingType::InOutQuart:	info->current = EasingFunc::InOutQuart	(duration_rate, 1.0f, info->end, info->begin); break;
		case EasingType::InQuint:		info->current = EasingFunc::InQuint		(duration_rate, 1.0f, info->end, info->begin); break;
		case EasingType::OutQuint:		info->current = EasingFunc::OutQuint	(duration_rate, 1.0f, info->end, info->begin); break;
		case EasingType::InOutQuint:	info->current = EasingFunc::InOutQuint	(duration_rate, 1.0f, info->end, info->begin); break;
		case EasingType::InSine:		info->current = EasingFunc::InSine		(duration_rate, 1.0f, info->end, info->begin); break;
		case EasingType::OutSine:		info->current = EasingFunc::OutSine		(duration_rate, 1.0f, info->end, info->begin); break;
		case EasingType::InOutSine:		info->current = EasingFunc::InOutSine	(duration_rate, 1.0f, info->end, info->begin); break;
		//case EasingType::InExp:		info->current = EasingFunc::InExp		(duration_rate, 1.0f, info->end, info->begin); break;
		//case EasingType::OutExp:		info->current = EasingFunc::OutExp		(duration_rate, 1.0f, info->end, info->begin); break;
		//case EasingType::InOutExp:	info->current = EasingFunc::InOutExp	(duration_rate, 1.0f, info->end, info->begin); break;
		case EasingType::InCirc:		info->current = EasingFunc::InCirc		(duration_rate, 1.0f, info->end, info->begin); break;
		case EasingType::OutCirc:		info->current = EasingFunc::OutCirc		(duration_rate, 1.0f, info->end, info->begin); break;
		case EasingType::InOutCirc:		info->current = EasingFunc::InOutCirc	(duration_rate, 1.0f, info->end, info->begin); break;
		case EasingType::InBack:		info->current = EasingFunc::InBack		(duration_rate, 1.0f, info->end, info->begin); break;
		case EasingType::OutBack:		info->current = EasingFunc::OutBack		(duration_rate, 1.0f, info->end, info->begin); break;
		case EasingType::InOutBack:		info->current = EasingFunc::InOutBack	(duration_rate, 1.0f, info->end, info->begin); break;
		case EasingType::InBounce:		info->current = EasingFunc::InBounce	(duration_rate, 1.0f, info->end, info->begin); break;
		case EasingType::OutBounce:		info->current = EasingFunc::OutBounce	(duration_rate, 1.0f, info->end, info->begin); break;
		case EasingType::InOutBounce:	info->current = EasingFunc::InOutBounce	(duration_rate, 1.0f, info->end, info->begin); break;
		case EasingType::Linear:		info->current = EasingFunc::Linear		(duration_rate, 1.0f, info->end, info->begin); break;
		}
	}
};


