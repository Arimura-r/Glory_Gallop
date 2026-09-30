#include "ColorFunc.h"

COLOR_F ColorFunc::ColorCodeConvCOLOR_F(unsigned int _color, float _alpha)
{
    int blue = _color % 256;
    int green = (_color / 256) % 256;
    int red = ((_color / 256)/256) % 256;
    COLOR_F color{ red / 256.0f,green / 256.0f,blue / 256.0f,_alpha };

    return color;
}

unsigned int ColorFunc::COLOR_FConvColorCode(COLOR_F _color)
{
    return GetColor((int)(_color.r * 256), (int)(_color.g * 256), (int)(_color.b * 256));
}

unsigned int ColorFunc::ColorMix(unsigned int _color1, unsigned int _color2)
{
    COLOR_F color1 = ColorCodeConvCOLOR_F(_color1);
    COLOR_F color2 = ColorCodeConvCOLOR_F(_color2);
    
    return COLOR_FConvColorCode(ColorMix(color1, color2));
}

COLOR_F ColorFunc::ColorMix(COLOR_F _color1, COLOR_F _color2)
{
    return COLOR_F{ (_color1.r + _color2.r) / 2.0f , (_color1.g + _color2.g) / 2.0f , (_color1.b + _color2.b) / 2.0f };
}
