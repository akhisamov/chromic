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

/*
 * MY_TODO
 * replace txt level file to csv format
*/

#include "LevelStartup.h"

#include <SDL2/SDL_filesystem.h>

#include "Game.h"

#include "Components/Ground.h"
#include "Components/Transform.h"
#include "Constants.h"
#include "Vec2.h"
#include "World.h"

#include <algorithm>
#include <fstream>
#include <ranges>
#include <string>
#include <vector>

using namespace systems;

enum class TileType {
    NONE = 0,

    PLAYER = 1,

    GROUND = 5,
};

constexpr TileType parseTile(const std::string_view& tileStr)
{
    const bool isNumber = std::all_of(tileStr.begin(), tileStr.end(), [](char c) { return std::isdigit(c); });
    if (isNumber) {
        const uint32_t tileId = std::stoul(tileStr.data());
        return static_cast<TileType>(tileId);
    }
    return TileType::NONE;
}

using LevelType = std::vector<std::vector<TileType>>;

LevelType parseLevel(const std::string_view& filename)
{
    std::string path(SDL_GetBasePath());
    path += filename;
    std::ifstream file(path);
    if (!file.is_open()) {
        return {};
    }

    constexpr std::string_view delim = " ";
    LevelType result;
    std::string line;
    while (std::getline(file, line)) {
        std::vector<TileType> row;
        for (const auto type : std::views::split(line, delim)) {
            row.push_back(parseTile(std::string_view(type.begin(), type.end())));
        }
        if (!row.empty()) {
            result.push_back(row);
        }
    }
    file.close();
    return result;
}

components::Ground::Type getGroundType(size_t x, size_t y, const LevelType& level)
{
    using T = components::Ground::Type;
    if (level.empty()) {
        return T::NONE;
    }

    const bool isGroundOnLeft = x > 0 && level[y][x - 1] == TileType::GROUND;
    const bool isGroundOnRight = x < level[y].size() - 1 && level[y][x + 1] == TileType::GROUND;
    const bool isGroundOnTop = y > 0 && level[y - 1][x] == TileType::GROUND;
    const bool isGroundOnBottom = y < level.size() - 1 && level[y + 1][x] == TileType::GROUND;

    if (!isGroundOnLeft && isGroundOnRight && !isGroundOnTop && isGroundOnBottom) {
        return T::TOP_LEFT;
    }
    if (isGroundOnLeft && isGroundOnRight && !isGroundOnTop && isGroundOnBottom) {
        return T::TOP_MIDDLE;
    }
    if (isGroundOnLeft && !isGroundOnRight && !isGroundOnTop && isGroundOnBottom) {
        return T::TOP_RIGHT;
    }
    if (!isGroundOnLeft && isGroundOnRight && isGroundOnTop && isGroundOnBottom) {
        return T::MIDDLE_LEFT;
    }
    if (isGroundOnLeft && isGroundOnRight && isGroundOnTop && isGroundOnBottom) {
        return T::MIDDLE;
    }
    if (isGroundOnLeft && !isGroundOnRight && isGroundOnTop && isGroundOnBottom) {
        return T::MIDDLE_RIGHT;
    }
    if (!isGroundOnLeft && isGroundOnRight && isGroundOnTop && !isGroundOnBottom) {
        return T::BOTTOM_LEFT;
    }
    if (isGroundOnLeft && isGroundOnRight && isGroundOnTop && !isGroundOnBottom) {
        return T::BOTTOM_MIDDLE;
    }
    if (isGroundOnLeft && !isGroundOnRight && isGroundOnTop && !isGroundOnBottom) {
        return T::BOTTOM_RIGHT;
    }

    return T::NONE;
}

void LevelStartup::startup(const Game& game)
{
    auto& world = game.getWorld();
    const LevelType level = parseLevel("res/level.txt");
    constexpr Vec2 TILE_SIZE = { constants::TILE_SIZE, constants::TILE_SIZE };
    for (size_t i = 0; i < level.size(); ++i) {
        const auto& row = level[i];
        for (size_t j = 0; j < row.size(); ++j) {
            const bool isTile = row[j] == TileType::GROUND;
            World::EntityId entityId;
            if (isTile) {
                entityId = world.createEntity();
            } else if (row[j] == TileType::PLAYER && !world.hasPlayer()) {
                entityId = world.createPlayer();
            } else {
                continue;
            }

            const components::Transform transform = {
                .position = TILE_SIZE * Vec2(static_cast<float>(j), static_cast<float>(i)),
                .size = TILE_SIZE,
            };
            world.transforms.emplace(entityId, transform);

            if (isTile) {
                world.grounds.emplace(entityId, components::Ground { .type = getGroundType(j, i, level) });
            }
        }
    }
}
