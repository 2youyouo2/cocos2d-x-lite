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

#include "Pass.h"

RENDERER_BEGIN

Pass::Pass(const std::string& programName, uint32_t* data, uint32_t byteOffset)
: _programName(programName)
, __data(data)
, __byteOffset(byteOffset)
{
//    RENDERER_LOGD("Pass constructor: %p", this);
}

Pass::~Pass()
{
//    RENDERER_LOGD("Pass destructor: %p", this);
}

void Pass::setCullMode(CullMode cullMode)
{
    if (!__data) return;
    __data[0] = (uint32_t)cullMode;
}

void Pass::setBlend(BlendOp blendEq,
                    BlendFactor blendSrc,
                    BlendFactor blendDst,
                    BlendOp blendAlphaEq,
                    BlendFactor blendSrcAlpha,
                    BlendFactor blendDstAlpha,
                    uint32_t blendColor)
{
    if (!__data) return;
    
    __data[1] = (uint32_t)true;
    __data[2] = (uint32_t)blendEq;
    __data[3] = (uint32_t)blendSrc;
    __data[4] = (uint32_t)blendDst;
    __data[5] = (uint32_t)blendAlphaEq;
    __data[6] = (uint32_t)blendSrcAlpha;
    __data[7] = (uint32_t)blendDstAlpha;
    __data[8] = (uint32_t)blendColor;
}

void Pass::setDepth(bool depthTest, bool depthWrite, DepthFunc depthFunc)
{
    if (!__data) return;
    
    __data[9] = (uint32_t)depthTest;
    __data[10] = (uint32_t)depthWrite;
    __data[11] = (uint32_t)depthFunc;
}

void Pass::setStencilFront(StencilFunc stencilFunc,
                           uint32_t stencilRef,
                           uint8_t stencilMask,
                           StencilOp stencilFailOp,
                           StencilOp stencilZFailOp,
                           StencilOp stencilZPassOp,
                           uint8_t stencilWriteMask)
{
    if (!__data) return;
    
    __data[12] = true;
    __data[13] = (uint32_t)stencilFunc;
    __data[14] = (uint32_t)stencilRef;
    __data[15] = (uint32_t)stencilMask;
    __data[16] = (uint32_t)stencilFailOp;
    __data[17] = (uint32_t)stencilZFailOp;
    __data[18] = (uint32_t)stencilZPassOp;
    __data[19] = (uint32_t)stencilWriteMask;
}

void Pass::setStencilBack(StencilFunc stencilFunc,
                          uint32_t stencilRef,
                          uint8_t stencilMask,
                          StencilOp stencilFailOp,
                          StencilOp stencilZFailOp,
                          StencilOp stencilZPassOp,
                          uint8_t stencilWriteMask)
{
    if (!__data) return;
    
    __data[12] = true;
    __data[20] = (uint32_t)stencilFunc;
    __data[21] = (uint32_t)stencilRef;
    __data[22] = (uint32_t)stencilMask;
    __data[23] = (uint32_t)stencilFailOp;
    __data[24] = (uint32_t)stencilZFailOp;
    __data[25] = (uint32_t)stencilZPassOp;
    __data[26] = (uint32_t)stencilWriteMask;
}

void Pass::copy(const Pass& pass, uint8_t* buffer)
{
    __byteOffset = pass.__byteOffset;
    __data = (uint32_t*)(buffer + __byteOffset);
    
//    for (int i = 0; i < __dataLen; i++) {
//        __data[i] = pass.__data[i];
//    }
    
    _programName = pass._programName;
}

RENDERER_END
