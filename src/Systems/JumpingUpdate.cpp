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

#include "JumpingUpdate.h"

#include "Components/Jumper.h"
#include "Components/Velocity.h"
#include "Game.h"
#include "GameTime.h"
#include "World.h"

#include <ranges>

namespace systems {
void JumpingUpdate::update(const Game& game)
{
    auto& world = game.getWorld();
    const auto& gameTime = game.getGameTime();

    auto& jumps = world.jumps;
    auto view = std::views::filter(
        world.getEntities(), [&jumps](const World::EntityId& entityId) { return jumps.find(entityId) != jumps.end(); });

    if (view.empty()) {
        return;
    }

    auto& velocities = world.velocities;
    for (const auto& entityId : view) {
        auto velocityIt = velocities.find(entityId);
        auto& jump = jumps.at(entityId);
        const auto newHeight = jump.height * jump.speed * gameTime.getDeltaTime();
        jump.lastHeight += newHeight;
        if (jump.lastHeight >= jump.height) {
            if (velocityIt == velocities.end() || velocityIt->second.y == 0.0f) {
                jumps.erase(entityId);
                jump.lastHeight = 0.0f;
            }
            continue;
        }

        const components::Velocity velocity = { .x = 0.0f, .y = -newHeight };

        if (velocityIt != velocities.end()) {
            velocityIt->second.y = velocity.y;
        } else {
            velocities.emplace(entityId, velocity);
        }
    }
}
}
