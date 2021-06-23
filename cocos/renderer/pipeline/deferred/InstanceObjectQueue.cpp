/****************************************************************************
 Copyright (c) 2020-2021 Xiamen Yaji Software Co., Ltd.

 http://www.cocos.com

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
****************************************************************************/

#include "InstanceObjectQueue.h"
#include "gfx-base/GFXCommandBuffer.h"

#include "../InstancedBuffer.h"
#include "../PipelineStateManager.h"
#include "../helper/SharedMemory.h"
#include "../RenderPipeline.h"

namespace cc {
namespace pipeline {

void InstanceObjectQueue::clear() {
    for (auto *it : _queues) {
        it->clear();
    }
    _queues.clear();
}

void InstanceObjectQueue::uploadBuffers() {
    gfx::CommandBuffer *cmdBuffer = RenderPipeline::getInstance()->getCommandBuffers()[0];

    for (auto *instanceBuffer : _queues) {
        if (instanceBuffer->hasPendingModels()) {
            instanceBuffer->uploadBuffers(cmdBuffer);
        }
    }
}

void InstanceObjectQueue::recordCommandBuffer(gfx::Device * /*device*/, gfx::RenderPass *renderPass, gfx::CommandBuffer *cmdBuffer, Camera* camera) {
    if (camera != nullptr && !(camera->visibility & _layer)) {
        return;
    }

    for (auto *instanceBuffer : _queues) {
        if (!instanceBuffer->hasPendingModels()) continue;

        const auto &instances = instanceBuffer->getInstances();
        const auto *pass = instanceBuffer->getPass();
        cmdBuffer->bindDescriptorSet(materialSet, pass->getDescriptorSet());
        gfx::PipelineState *lastPSO = nullptr;
        for (const auto& instance : instances) {
            if (!instance.count) {
                continue;
            }
            auto *pso = PipelineStateManager::getOrCreatePipelineState(pass, instance.shader, instance.ia, renderPass);
            if (lastPSO != pso) {
                cmdBuffer->bindPipelineState(pso);
                lastPSO = pso;
            }
            cmdBuffer->bindDescriptorSet(localSet, instance.descriptorSet, instanceBuffer->dynamicOffsets());
            cmdBuffer->bindInputAssembler(instance.ia);
            cmdBuffer->draw(instance.ia);
        }
    }
}

void InstanceObjectQueue::add(InstancedBuffer *instancedBuffer) {
    _queues.emplace(instancedBuffer);
}

void InstanceObjectQueue::setNativeDataArray(se::Object *dataArray) {
    if (_dataArray != nullptr) {
        _dataArray->decRef();
    }
    if (dataArray != nullptr) {
        dataArray->incRef();
    }
    _dataArray = dataArray;
}
void InstanceObjectQueue::processNativeDataArray(uint count) {
    uint8_t *data = nullptr;
    size_t   size = 0;
    _dataArray->getTypedArrayData(&data, &size);

    uint *originData = (uint *)data;
    for (auto i = 0; i < count; i++) {
        uint              modelHandle   = *(originData + i);
        const auto *model = GET_MODEL(modelHandle);
        const auto *const subModelID    = model->getSubModelID();
        const auto        subModelCount = subModelID[0];
        for (auto m = 1; m <= subModelCount; ++m) {
            const auto *const subModel = cc::pipeline::ModelView::getSubModelView(subModelID[m]);
            for (auto p = 0; p < subModel->passCount; ++p) {
                const PassView *pass = subModel->getPassView(p);
                if (pass->phase != _phase) continue;

                auto *instancedBuffer = InstancedBuffer::get(subModel->passID[p]);
                instancedBuffer->merge(model, subModel, p);
                this->add(instancedBuffer);
            }
        }
    }
}

void InstanceObjectQueue::mergeInstance(InstancedBuffer *buffer, uint modelHandle, uint subModelHandle, uint passIdx) {
    uint phase = 0;
    
    const auto *      model         = GET_MODEL(modelHandle);
    const auto *const subModelID    = model->getSubModelID();
    const auto        subModelCount = subModelID[0];
    for (auto m = 1; m <= subModelCount; ++m) {
        const auto *const subModel = cc::pipeline::ModelView::getSubModelView(subModelID[m]);
        for (auto p = 0; p < subModel->passCount; ++p) {
            const PassView *pass = subModel->getPassView(p);
            if (pass->phase != phase) continue;

            buffer->merge(model, subModel, p);
        }
    }
}

InstancedBuffer* InstanceObjectQueue::createInstanceBuffer(uint passHandle) {
    auto *pass = GET_PASS(passHandle);
    return new InstancedBuffer(pass);
}

} // namespace pipeline
} // namespace cc
