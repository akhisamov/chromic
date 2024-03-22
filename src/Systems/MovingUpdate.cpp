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

#include "MovingUpdate.h"

#include "Components/Runner.h"
#include "Components/Velocity.h"

#include "Game.h"
#include "GameTime.h"
#include "World.h"

#include <ranges>

using namespace systems;

void MovingUpdate::update(const Game& game)
{
    auto& world = game.getWorld();
    const auto& gameTime = game.getGameTime();

    auto& runners = world.runners;
    auto view = std::views::filter(
        world.getEntities(), [&runners](uint32_t entityId) { return runners.find(entityId) != runners.end(); });

    auto& velocities = world.velocities;
    for (const World::EntityId entityId : view) {
        auto& runner = runners.at(entityId);

        const components::Velocity velocity = { .x = runner.speed * std::min(0.1f, gameTime.getDeltaTime()), .y = 0.0f };
        auto it = velocities.find(entityId);
        if (it != velocities.end()) {
            it->second.x = velocity.x;
        } else if (velocity.x != 0.0f) {
            velocities.emplace(entityId, velocity);
        } else {
            runners.erase(entityId);
        }
    }
}
