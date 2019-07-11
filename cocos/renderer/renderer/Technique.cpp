/****************************************************************************
 Copyright (c) 2018 Xiamen Yaji Software Co., Ltd.

 http://www.cocos2d-x.org
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#include "Technique.h"
#include "Config.h"
#include "Pass.h"
#include "gfx/Texture.h"
#include "gfx/Texture.h"

RENDERER_BEGIN

// implementation of Parameter

uint8_t Technique::Parameter::getElements(Type type)
{
    switch (type)
    {
        case Type::INT:
        return 1;
        case Type::INT2:
        return 2;
        case Type::INT3:
        return 3;
        case Type::INT4:
        return 4;
        case Type::FLOAT:
        return 1;
        case Type::FLOAT2:
        return 2;
        case Type::FLOAT3:
        return 3;
        case Type::FLOAT4:
        return 4;
        case Type::COLOR3:
        return 3;
        case Type::COLOR4:
        return 4;
        case Type::MAT2:
        return 4;
        case Type::MAT3:
        return 9;
        case Type::MAT4:
        return 16;
        default:
        return 0;
    }
}

Technique::Parameter::Parameter()
{}

void Technique::Parameter::init(const std::string& name, Type type, void* buffer, uint32_t byteOffset, uint32_t byteLength)
{
    _name = name;
    _type = type;
    
    _value = buffer;
    _bytes = byteLength;
    _byteOffset = byteOffset;
    
    uint8_t el = getElements(type);
    switch (type) {
        case Type::INT:
        case Type::INT2:
        case Type::INT3:
        case Type::INT4:
        {
            _count = (_bytes / sizeof(float)) / el;
            break;
        }
        case Type::FLOAT:
        case Type::FLOAT2:
        case Type::FLOAT3:
        case Type::FLOAT4:
        case Type::COLOR3:
        case Type::COLOR4:
        case Type::MAT2:
        case Type::MAT3:
        case Type::MAT4:
        {
            _count = (_bytes / sizeof(float)) / el;
            break;
        }
            
        case Type::TEXTURE_2D:
        case Type::TEXTURE_CUBE:
        {
            el = getElements(Type::INT);
            _count = (_bytes / sizeof(float)) / el;
            break;
        }
        default:
            assert(false);
            break;
    }

}


Technique::Parameter::Parameter(Parameter&& rh)
{
    if (this == &rh)
        return;
    
    _name = rh._name;
    _type = rh._type;
    _value = rh._value;
    _count = rh._count;
    _bytes = rh._bytes;
    
    rh._value = nullptr;
}

Technique::Parameter::Parameter(const Parameter& rh)
{
    copyValue(rh);
}

Technique::Parameter::~Parameter()
{
}

void Technique::Parameter::copy (const Parameter& other, uint8_t* buffer)
{
    _name = other._name;
    _type = other._type;
    _count = other._count;
    _bytes = other._bytes;
    _byteOffset = other._byteOffset;
    
    _value = buffer + _byteOffset;
}

Technique::Parameter& Technique::Parameter::operator=(const Parameter& rh)
{
    if (this == &rh)
        return *this;

    copyValue(rh);
    
    return *this;
}


void Technique::Parameter::setTexture(renderer::Texture *texture)
{
    assert(_type == Type::TEXTURE_2D);
    ((uint32_t* )_value)[0] = texture->getHandle();
}

void Technique::Parameter::copyValue(const Parameter& rh)
{
    _name = rh._name;
    _type = rh._type;
    _count = rh._count;
    _bytes = rh._bytes;
    _value = rh._value;
    _byteOffset = rh._byteOffset;
}

// implementation of Technique

uint32_t Technique::_genID = 0;

Technique::Technique(const std::vector<std::string>& stages,
                     const Vector<Pass*>& passes,
                     int layer)
: _id(_genID++)
, _stageIDs(Config::getStageIDs(stages))
, _passes(passes)
, _layer(layer)
{
//    RENDERER_LOGD("Technique construction: %p", this);
}

Technique::Technique()
{
    
}

Technique::~Technique()
{
//    RENDERER_LOGD("Technique destruction: %p", this);
}

void Technique::setStages(const std::vector<std::string>& stages)
{
    _stageIDs = Config::getStageIDs(stages);
}

void Technique::setPass(int index, Pass* pass)
{
    _passes.insert(index, pass);
}

void Technique::copy(const Technique& tech, uint8_t* buffer)
{
    _id = tech._id;
    _stageIDs = tech._stageIDs;
    _layer = tech._layer;
    _passes.clear();
    auto& otherPasses = tech._passes;
    for (auto it = otherPasses.begin(); it != otherPasses.end(); it++)
    {
        auto newPass = new Pass();
        newPass->autorelease();
        newPass->copy(**it, buffer);
        _passes.pushBack(newPass);
    }
}

RENDERER_END
