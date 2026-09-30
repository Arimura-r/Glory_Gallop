#include "HorseAnimLoader.h"
#include "Animator.h"

HorseAnimLoader::HorseAnimLoader()
{
    horseAnim = new Animator();
    //horseAnim->SetModel(_hModel);
    horseAnim->AddFile(horseAnims);
}

HorseAnimLoader::~HorseAnimLoader()
{
    delete horseAnim;
}

Animator* HorseAnimLoader::GetAnimDupli() const
{
    Animator* ret = new Animator();
    horseAnim->Copy(ret);
    return ret;
}
