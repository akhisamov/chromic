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

#include "GroundStartup.h"

#include "Components/Ground.h"
#include "Components/Sprite.h"
#include "Components/StaticRigidBody.h"

#include "Constants.h"
#include "Game.h"
#include "World.h"

#include <ranges>

using namespace systems;

constexpr uint32_t getTileIdByType(components::Ground::Type type)
{
    using T = components::Ground::Type;
    switch (type) {
    case T::TOP_LEFT:
        return 270;
    case T::TOP_MIDDLE:
        return 271;
    case T::TOP_RIGHT:
        return 272;
    case T::MIDDLE_LEFT:
        return 290;
    case T::MIDDLE:
        return 291;
    case T::MIDDLE_RIGHT:
        return 292;
    case T::BOTTOM_LEFT:
        return 310;
    case T::BOTTOM_MIDDLE:
        return 311;
    case T::BOTTOM_RIGHT:
        return 312;
    case T::NONE:
    default:
        return 0;
    }
}

void GroundStartup::startup(const Game& game)
{
    auto& world = game.getWorld();

    const auto& grounds = world.grounds;
    auto view = std::views::filter(
        world.getEntities(), [grounds](uint32_t entityId) { return grounds.find(entityId) != grounds.end(); });

    for (const World::EntityId entityId : view) {
        const auto& ground = grounds.at(entityId);
        const auto tileId = static_cast<int>(getTileIdByType(ground.type));

        const SDL_Rect sourceRect = {
            .x = (tileId % constants::TOTAL_TILES) * constants::TILE_SIZE,
            .y = (tileId / constants::TOTAL_TILES) * constants::TILE_SIZE,
            .w = constants::TILE_SIZE,
            .h = constants::TILE_SIZE,
        };
        constexpr SDL_Rect boundingBox = {
            .x = 0,
            .y = 0,
            .w = 16,
            .h = 16,
        };

        auto spriteIt = world.sprites.find(entityId);
        if (spriteIt != world.sprites.end())
        {
            spriteIt->second.sourceRect = sourceRect;
        }
        else
        {
            components::Sprite sprite;
            sprite.sourceRect = sourceRect;
            world.sprites.emplace(entityId, sprite);
        }

        auto rigidBodyIt = world.staticRigidBodies.find(entityId);
        if (rigidBodyIt != world.staticRigidBodies.end())
        {
            rigidBodyIt->second.boundingBox = boundingBox;
        }
        else
        {
            components::StaticRigidBody rigidBody;
            rigidBody.boundingBox = boundingBox;
            world.staticRigidBodies.emplace(entityId, rigidBody);
        }
    }
}
