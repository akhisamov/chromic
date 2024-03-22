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

#include "CameraPositionUpdate.h"

#include "Camera.h"
#include "Components/Transform.h"
#include "Game.h"
#include "Vec2.h"
#include "World.h"

using namespace systems;

void CameraPositionUpdate::update(const Game& game)
{
    const auto& camera = game.getCamera();
    const auto& world = game.getWorld();
    if (!camera || !camera->getAttachedEntity().has_value()) {
        return;
    }

    const auto& attachedEntityId = camera->getAttachedEntity().value();
    const float zoom = camera->getZoom();
    const Vec2 cameraCenter = camera->getSize() * 0.5f;
    const auto& transforms = world.transforms;

    auto transformIt = transforms.find(attachedEntityId);
    if (transformIt == transforms.end()) {
        return;
    }

    const Vec2 scaledSize = transformIt->second.size * zoom;
    const Vec2 position = (transformIt->second.position * zoom) + (scaledSize * 0.5f);
    const Vec2 cameraPosition = (position - cameraCenter) * -1.0f;
    camera->setPosition(cameraPosition);
}
