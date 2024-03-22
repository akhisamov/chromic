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

#include "PlayerStartup.h"

#include "Camera.h"
#include "Components/DynamicRigidBody.h"
#include "Components/SpriteSheetAnimation.h"

#include "Constants.h"
#include "Game.h"
#include "World.h"

using namespace systems;

void PlayerStartup::startup(const Game& game)
{
    auto& world = game.getWorld();
    const auto& camera = game.getCamera();

    const auto& player = world.getPlayer();
    if (!camera || !player.has_value()) {
        return;
    }

    camera->attachEntity(player.value());

    const components::SpriteSheetAnimation spriteSheetAnimation = {
        .tracks = { { "idle", { 240 } }, { "run", { 241, 242, 243 } } },
        .currentTrackName = "idle",
    };
    world.spriteSheetAnimations.emplace(player.value(), spriteSheetAnimation);

    const components::DynamicRigidBody rigidBody = {
            .boundingBox = {
            .x = 2,
            .y = 0,
            .w = 12,
            .h = 16,
            },
            .speed = constants::MOVEMENT_SPEED,
        };
    world.dynamicRigidyBodies.emplace(player.value(), rigidBody);
}
