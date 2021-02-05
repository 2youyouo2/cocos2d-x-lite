/*
 Copyright (c) Huawei Technologies Co., Ltd. 2020-2021.

 https://www.cocos.com/

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated engine source code (the "Software"), a limited,
 worldwide, royalty-free, non-assignable, revocable and non-exclusive license
 to use Cocos Creator solely to develop games on your target platforms. You shall
 not use Cocos Creator software for developing other software or tools that's
 used for developing games. You are not granted to publish, distribute,
 sublicense, and/or sell copies of Cocos Creator.

 The software or tools in this License Agreement are licensed, not sold.
 Xiamen Yaji Software Co., Ltd. reserves all rights not expressly granted to you.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 */

#include "PostprocessStage.h"
#include "DeferredPipeline.h"
#include "gfx/GFXFramebuffer.h"
#include "gfx/GFXCommandBuffer.h"
#include "../helper/SharedMemory.h"
#include "../PipelineStateManager.h"
#include "gfx/GFXDevice.h"

namespace cc {
namespace pipeline {
RenderStageInfo PostprocessStage::_initInfo = {
    "PostprocessStage",
    static_cast<uint>(DeferredStagePriority::POSTPROCESS),
    0,
};

bool PostprocessStage::initialize(const RenderStageInfo &info) {
    RenderStage::initialize(info);
    return true;
}

void PostprocessStage::activate(RenderPipeline *pipeline, RenderFlow *flow) {
    RenderStage::activate(pipeline, flow);
}

void PostprocessStage::destroy() {
}

void PostprocessStage::render(Camera *camera) {
    DeferredPipeline *pp = dynamic_cast<DeferredPipeline *>(_pipeline);
    assert(pp != nullptr);
    gfx::Device *device = pp->getDevice();
    gfx::CommandBuffer *cmdBf = pp->getCommandBuffers()[0];

    _pipeline->getPipelineUBO()->updateCameraUBO(camera, camera->getWindow()->hasOffScreenAttachments);
    gfx::Rect renderArea = pp->getRenderArea(camera, !camera->getWindow()->hasOffScreenAttachments);

    gfx::Color color = {0, 0, 0, 1};
    gfx::ColorList clst;
    clst.push_back(color);

    gfx::Framebuffer *fb = camera->getWindow()->getFramebuffer();
    gfx::RenderPass *rp = fb->getRenderPass() ?
        fb->getRenderPass() : pp->getOrCreateRenderPass(static_cast<gfx::ClearFlags>(camera->clearFlag));

    cmdBf->beginRenderPass(rp, fb, renderArea, clst, camera->clearDepth, camera->clearStencil);
    cmdBf->bindDescriptorSet(static_cast<uint>(SetIndex::GLOBAL), pp->getDescriptorSet());

    // post proces
    const auto sceneData = _pipeline->getPipelineSceneData();
    PassView *pv = sceneData->getSharedData()->getDeferredPostPass();
    gfx::Shader *sd = sceneData->getSharedData()->getDeferredPostPassShader();

    gfx::InputAssembler *ia = camera->getWindow()->hasOffScreenAttachments ? pp->getQuadIAOffScreen() : pp->getQuadIAOnScreen();

    gfx::PipelineState *pso = PipelineStateManager::getOrCreatePipelineState(pv, sd, ia, rp);
    assert(pso != nullptr);

    cmdBf->bindPipelineState(pso);
    cmdBf->bindInputAssembler(ia);
    cmdBf->draw(ia);
    cmdBf->endRenderPass();
}
} // namespace pipeline
} // namespace cc