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

#include "PositionUpdate.h"

#include "Components/Transform.h"
#include "Components/Velocity.h"
#include "Game.h"
#include "World.h"

#include <ranges>

namespace systems {
void PositionUpdate::update(const Game& game)
{
    auto& world = game.getWorld();

    const auto& velocities = world.velocities;
    auto& transforms = world.transforms;
    auto view = std::views::filter(world.getEntities(), [velocities, transforms](uint32_t entityId) {
        return velocities.find(entityId) != velocities.end() && transforms.find(entityId) != transforms.end();
    });

    for (const World::EntityId entityId : view) {
        const auto& velocity = velocities.at(entityId);
        if (velocity.x != 0 || velocity.y != 0) {
            auto& transform = transforms.at(entityId);
            transform.position.x += velocity.x;
            transform.position.y += velocity.y;
        }
    }
}
}
