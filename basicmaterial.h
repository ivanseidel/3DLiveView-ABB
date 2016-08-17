#ifndef BASICMATERIAL_H
#define BASICMATERIAL_H

#include "EasyGL/EasyMaterial.h"

class BasicMaterial : public EasyMaterial
{
public:
    BasicMaterial();
    static BasicMaterial *_singletonMaterial;
};

#endif // BASICMATERIAL_H
