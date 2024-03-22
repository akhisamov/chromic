// chromic
// Copyright (C) 2024  Khisamov Amil
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

#include "CameraDebugDraw.h"

#include <imgui.h>

#include "Camera.h"
#include "Game.h"

using namespace systems;

void CameraDebugDraw::draw(const Game& game)
{
    const auto& camera = game.getCamera();
    const auto& world = game.getWorld();
    if (!camera) {
        return;
    }

    const bool hasAttachedEntity = camera->getAttachedEntity().has_value();

    ImGui::Begin("Camera");
    if (hasAttachedEntity) {
        ImGui::Text("Camera attached to the entity");
    }
    ImGui::PushItemWidth(ImGui::CalcItemWidth() / 2);
    ImGui::DragFloat("##X", &camera->m_position.x, hasAttachedEntity ? 0.0f : 1.0f, 0.f, 0.f, "%.3f",
        hasAttachedEntity ? ImGuiSliderFlags_NoInput : 0);
    ImGui::SameLine();
    ImGui::DragFloat("##Y", &camera->m_position.y, hasAttachedEntity ? 0.0f : 1.0f, 0.f, 0.f, "%.3f",
        hasAttachedEntity ? ImGuiSliderFlags_NoInput : 0);
    ImGui::PopItemWidth();
    ImGui::DragFloat("Zoom", &camera->m_zoom, 0.1f, 1.f, 100.f);
    ImGui::End();
}
