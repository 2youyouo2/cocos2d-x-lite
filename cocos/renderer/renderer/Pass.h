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

#pragma once

#include <string>
#include <base/CCRef.h>
#include "../Macro.h"
#include "../Types.h"
#include "cocos/scripting/js-bindings/jswrapper/SeApi.h"

RENDERER_BEGIN

/**
 * @addtogroup renderer
 * @{
 */

/**
 * @brief Pass describes base render pass configurations, including program, cull face, blending, depth testing and stencil testing configs.\n
 * JS API: renderer.Pass
 * @code
 * let pass = new renderer.Pass('sprite');
 * pass.setDepth(false, false);
 * pass.setCullMode(gfx.CULL_NONE);
 * pass.setBlend(
 *     gfx.BLEND_FUNC_ADD,
 *     gfx.BLEND_SRC_ALPHA, gfx.BLEND_ONE_MINUS_SRC_ALPHA,
 *     gfx.BLEND_FUNC_ADD,
 *     gfx.BLEND_SRC_ALPHA, gfx.BLEND_ONE_MINUS_SRC_ALPHA
 * );
 * @endcode
 */
class Pass : public Ref
{
public:
    /**
     * @brief Constructor with linked program name.
     * @param[in] programName Shader program name
     */
    Pass(const std::string& programName, uint32_t* data, uint32_t byteOffset);
    Pass() {};
    ~Pass();
    
    /**
     *  @brief Sets cull mode.
     *  @param[in] cullMode Cull front or back or both.
     */
    void setCullMode(CullMode cullMode);
    /**
     *  @brief Sets blend mode.
     *  @param[in] blendEq RGB blend equation.
     *  @param[in] blendSrc Src RGB blend factor.
     *  @param[in] blendDst Dst RGB blend factor.
     *  @param[in] blendAlphaEq Alpha blend equation.
     *  @param[in] blendSrcAlpha Src Alpha blend equation.
     *  @param[in] blendDstAlpha Dst Alpha blend equation.
     *  @param[in] blendColor Blend constant color value.
     */
    void setBlend(BlendOp blendEq = BlendOp::ADD,
                  BlendFactor blendSrc = BlendFactor::SRC_ALPHA,
                  BlendFactor blendDst = BlendFactor::ONE_MINUS_SRC_ALPHA,
                  BlendOp blendAlphaEq = BlendOp::ADD,
                  BlendFactor blendSrcAlpha = BlendFactor::SRC_ALPHA,
                  BlendFactor blendDstAlpha = BlendFactor::ONE_MINUS_SRC_ALPHA,
                  uint32_t blendColor = 0xffffffff);
    /**
     *  @brief Switch depth test or write, and sets depth test function.
     *  @param[in] depthTest Enable depth test or not.
     *  @param[in] depthWrite Enable depth write or not.
     *  @param[in] depthFunc Depth test function.
     */
    void setDepth(bool depthTest = false,
                  bool depthWrite = false,
                  DepthFunc depthFunc = DepthFunc::LESS);
    /**
     *  @brief Sets stencil front-facing function, reference, mask, fail operation, write mask.
     */
    void setStencilFront(StencilFunc stencilFunc = StencilFunc::ALWAYS,
                         uint32_t stencilRef = 0,
                         uint8_t stencilMask = 0xff,
                         StencilOp stencilFailOp = StencilOp::KEEP,
                         StencilOp stencilZFailOp = StencilOp::KEEP,
                         StencilOp stencilZPassOp = StencilOp::KEEP,
                         uint8_t stencilWriteMask = 0xff);
    /**
     *  @brief Sets stencil back-facing function, reference, mask, fail operation, write mask.
     */
    void setStencilBack(StencilFunc stencilFunc = StencilFunc::ALWAYS,
                        uint32_t stencilRef = 0,
                        uint8_t stencilMask = 0xff,
                        StencilOp stencilFailOp = StencilOp::KEEP,
                        StencilOp stencilZFailOp = StencilOp::KEEP,
                        StencilOp stencilZPassOp = StencilOp::KEEP,
                        uint8_t stencilWriteMask = 0xff);

    /**
     *  @brief Sets linked program name.
     */
    inline void setProgramName(const std::string& programName) { _programName = programName; }
    /**
     *  @brief Gets linked program name.
     */
    inline std::string getProgramName() const { return _programName; }
    
    /**
     *  @brief deep copy from other pass.
     */
    void copy(const Pass& pass, uint8_t* buffer);
    
    
    // cull mode
    CullMode getCullMode() const { return (CullMode)__data[0]; }
    
    // blending
    inline bool isBlend () const { return __data[1]; };
    inline BlendOp getBlendEq () const { return (BlendOp)__data[2]; };
    inline BlendOp getBlendAlphaEq () const { return (BlendOp)__data[3]; };
    inline BlendFactor getBlendSrc () const { return (BlendFactor)__data[4]; };
    inline BlendFactor getBlendDst () const { return (BlendFactor)__data[5]; };
    inline BlendFactor getBlendSrcAlpha () const { return (BlendFactor)__data[6]; };
    inline BlendFactor getBlendDstAlpha () const { return (BlendFactor)__data[7]; };
    inline uint32_t getBlendColor () const { return __data[8]; };
    
    
    // depth
    inline bool isDepthTest () const { return __data[9]; };;
    inline bool isDepthWrite () const { return __data[10]; };;
    inline DepthFunc getDepthFunc () const { return (DepthFunc)__data[11]; };;
    
    // stencil
    inline void setStencilTest(bool value)      const  { __data[12] = value; }
    inline bool isStencilTest()                const { return __data[12]; }
    inline void disableStencilTest()            const { __data[12] = false; }
    
    // front
    inline StencilFunc getStencilFuncFront()     const { return (StencilFunc)__data[13]; }
    inline uint32_t getStencilRefFront()         const { return __data[14]; }
    inline uint8_t getStencilMaskFront()         const { return __data[15]; }
    inline StencilOp getStencilFailOpFront()     const { return (StencilOp)__data[16]; }
    inline StencilOp getStencilZFailOpFront()    const { return (StencilOp)__data[17]; }
    inline StencilOp getStencilZPassOpFront()    const { return (StencilOp)__data[18]; }
    inline uint8_t getStencilWriteMaskFront()    const { return __data[19]; }
    
    // back
    inline StencilFunc getStencilFuncBack()      const { return (StencilFunc)__data[20]; }
    inline uint32_t getStencilRefBack()          const { return __data[21]; }
    inline uint8_t getStencilMaskBack()          const { return __data[22]; }
    inline StencilOp getStencilFailOpBack()      const { return (StencilOp)__data[23]; }
    inline StencilOp getStencilZFailOpBack()     const { return (StencilOp)__data[24]; }
    inline StencilOp getStencilZPassOpBack()     const { return (StencilOp)__data[25]; }
    inline uint8_t getStencilWriteMaskBack()     const { return __data[26]; }
    
private:
    friend class BaseRenderer;
    
    std::string _programName = "";
    
    uint32_t* __data = nullptr;
    uint32_t __byteOffset = 0;
};

// end of renderer group
/// @}
RENDERER_END
