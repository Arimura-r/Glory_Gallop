#include "HorseModelLoader.h"
#include <assert.h>

namespace
{
    const int BODY_TEXTURE_INDEX = 0;
}

HorseModelLoader::HorseModelLoader()
{
    AllTextureLoad();
    hModel = -1;
    hModel=MV1LoadModel(MODEL_FILE_PATH.c_str());
   // int i=MV1SetTextureGraphHandle(hModel, BODY_TEXTURE_INDEX, gl,true);
}

HorseModelLoader::~HorseModelLoader()
{
    MV1DeleteModel(hModel);
    for (auto& b : bodyTextureImages)
    {
        DeleteGraph(b.second);
    }
}

int HorseModelLoader::GetDupliModel(BodyColor _color) const
{
    int ret = MV1DuplicateModel(hModel);
    auto itr=bodyTextureImages.find(_color);
    if (itr == bodyTextureImages.end())
        assert("àŸèÌÇ»BodyColorÇ™éwíËÇ≥ÇÍÇƒÇ¢Ç‹Ç∑" && false);

    MV1SetTextureGraphHandle(ret, BODY_TEXTURE_INDEX, (*itr).second, true);

    return ret;
}

void HorseModelLoader::AllTextureLoad()
{
    for (const auto& b : BODY_TEXTURE_NAME_LIST)
    {
        bodyTextureImages.emplace((BodyColor)b.first, LoadGraph((TEXTURE_FILE_PATH + b.second).c_str()));
    }
}
