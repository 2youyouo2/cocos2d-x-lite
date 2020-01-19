#include "GLES3Std.h"
#include "GLES3CommandAllocator.h"

NS_CC_BEGIN

GLES3CommandAllocator::GLES3CommandAllocator(GFXDevice* device)
    : GFXCommandAllocator(device) {
}

GLES3CommandAllocator::~GLES3CommandAllocator() {
}

bool GLES3CommandAllocator::Initialize(const GFXCommandAllocatorInfo& info) {
  return true;
}

void GLES3CommandAllocator::Destroy() {
}

void GLES3CommandAllocator::ClearCmds(GLES3CmdPackage* cmd_package) {
  if (cmd_package->begin_render_pass_cmds.Size()) {
    begin_render_pass_cmd_pool.FreeCmds(cmd_package->begin_render_pass_cmds);
  }
  if (cmd_package->bind_states_cmds.Size()) {
    bind_states_cmd_pool.FreeCmds(cmd_package->bind_states_cmds);
  }
  if (cmd_package->draw_cmds.Size()) {
    draw_cmd_pool.FreeCmds(cmd_package->draw_cmds);
  }
  if (cmd_package->update_buffer_cmds.Size()) {
    update_buffer_cmd_pool.FreeCmds(cmd_package->update_buffer_cmds);
  }
  if (cmd_package->copy_buffer_to_texture_cmds.Size()) {
    copy_buffer_to_texture_cmd_pool.FreeCmds(cmd_package->copy_buffer_to_texture_cmds);
  }

  cmd_package->cmd_types.Clear();
}

NS_CC_END