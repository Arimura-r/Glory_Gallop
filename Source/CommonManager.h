#pragma once
#include "../Library/GameObject.h"
#include <typeinfo>

/// <summary>
/// シーンを跨いで存在させておきたいクラスをまとめておくクラス
/// </summary>
class CommonManager :public GameObject
{
public:
	CommonManager() 
	{
		DontDestroyOnSceneChange(true);//シーンを跨いでも消えないようにする
	};

	/*void Update()override
	{
		if (SceneManager::IsExit())
		{
			DestroyMe();
			for (auto& c : commonObjects)
			{
				c->DestroyMe();
			}
		}
	}*/

	template<typename C>
	C* AddCommonObject(C* _object);

	template<typename C>
	int RemoveCommonObject();

	template<typename C>
	C* FindCommonObject();

private:
	std::list<GameObject*> commonObjects;
};

template<typename C>
inline C* CommonManager::AddCommonObject(C* _object)
{
	commonObjects.push_back(_object);

	return _object;
}

template<typename C>
inline int CommonManager::RemoveCommonObject()
{
	int ret = 0;

	for (auto it = commonObjects.begin(); it != commonObjects.end();)
	{
		if (typeid(C) == typeid(*it))
		{
			ret++;
			(*it).DestroyMe();
			it = commonObjects.erase(it);
		}
		else
		{
			it++;
		}
	}

	return ret;
}

template<typename C>
inline C* CommonManager::FindCommonObject()
{
	C* ret = nullptr;

	for (auto it = commonObjects.begin(); it != commonObjects.end(); it++)
	{
		if (typeid(C) == typeid(**it))
		{
			return dynamic_cast<C*>(*it);
		}
	}

	return ret;
}