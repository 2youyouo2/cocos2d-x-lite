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

#include "PostEffect.h"

RENDERER_BEGIN

// PostEffectCommand
void PostEffectCommand::init(int passIndex, cocos2d::ValueMap& properties)
{
    _passIndex = passIndex;
    _properties = properties;
}
void PostEffectCommand::setProperty(std::string& name, cocos2d::Value& value)
{
    _properties[name] = value;
}


// PostEffectRenderer
void PostEffectRenderer::init(cocos2d::Vector<PostEffectCommand *> commands, EffectVariant* effect)
{
    _commands = commands;
    _effect = effect;
}

void PostEffectRenderer::setEffect(EffectVariant *effect)
{
    _effect = effect;
}

void PostEffectRenderer::setEnabled(bool enabled)
{
    _enabled = enabled;
}



// PostEffect
void PostEffect::init(cocos2d::Vector<PostEffectRenderer *> renderers)
{
    _renderers = renderers;
}

RENDERER_END
