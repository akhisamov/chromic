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

#include "GravityUpdate.h"

#include "Components/DynamicRigidBody.h"
#include "Components/Velocity.h"
#include "Constants.h"
#include "Game.h"
#include "GameTime.h"
#include "World.h"

#include <ranges>

namespace systems {
void GravityUpdate::update(const Game& game)
{
    auto& world = game.getWorld();
    const auto& gameTime = game.getGameTime();

    const auto& dynamicRigidyBodies = world.dynamicRigidyBodies;
    auto view = std::views::filter(world.getEntities(),
        [&](World::EntityId entityId) { return dynamicRigidyBodies.find(entityId) != dynamicRigidyBodies.end(); });
    if (view.empty()) { 
        return;
    }

    auto& velocities = world.velocities;
    for (const World::EntityId& entityId : view) {
        const components::Velocity velocity = { .x = 0.0f, .y = constants::GRAVITY * gameTime.getDeltaTime() };
        auto it = velocities.find(entityId);
        if (it != velocities.end()) {
            it->second.y += velocity.y;
        } else {
            velocities.emplace(entityId, velocity);
        }
    }
}
}
