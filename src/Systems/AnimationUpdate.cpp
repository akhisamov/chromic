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

#include "AnimationUpdate.h"

#include "Components/Sprite.h"
#include "Components/SpriteSheetAnimation.h"

#include "Constants.h"
#include "Game.h"
#include "GameTime.h"
#include "World.h"

#include <cmath>
#include <ranges>

using namespace systems;

namespace {
void updateAnimation(const Game& game, uint32_t entityId, components::SpriteSheetAnimation& animation)
{
    auto& world = game.getWorld();
    const auto& gameTime = game.getGameTime();

    auto it = animation.tracks.find(animation.currentTrackName);
    if (it == animation.tracks.end()) {
        return;
    }

    const components::SpriteSheetAnimation::FramesType& frames = it->second;

    if (frames.empty()) {
        return;
    }

    const float dt = (gameTime.getCurrentTime() - animation.lastUpdate) / 1000.f;

    const auto framesToUpdate = static_cast<int>(std::floor(dt / (1.0f / constants::ANIMATION_FRAME_RATE)));
    if (framesToUpdate > 0) {
        animation.currentTrackFrameIdx += framesToUpdate;
        animation.currentTrackFrameIdx %= frames.size();
        animation.lastUpdate = gameTime.getCurrentTime();
    }

    const components::SpriteSheetAnimation::TileId& tileId = frames[animation.currentTrackFrameIdx];

    auto& sprites = world.sprites;
    const SDL_Rect sourceRect = {
        .x = (static_cast<int>(tileId) % constants::TOTAL_TILES) * constants::TILE_SIZE,
        .y = (static_cast<int>(tileId) / constants::TOTAL_TILES) * constants::TILE_SIZE,
        .w = constants::TILE_SIZE,
        .h = constants::TILE_SIZE,
    };
    auto spriteIt = sprites.find(entityId);
    if (spriteIt != sprites.end()) {
        spriteIt->second.sourceRect = sourceRect;
    } else {
        sprites.emplace(entityId, components::Sprite { .sourceRect = sourceRect });
    }
}
}

void AnimationUpdate::update(const Game& game)
{
    auto& world = game.getWorld();
    auto& spriteSheetAnimations = world.spriteSheetAnimations;

    auto view = std::views::filter(world.getEntities(),
        [&](uint32_t entityId) { return spriteSheetAnimations.find(entityId) != spriteSheetAnimations.end(); });
    for (const uint32_t entityId : view) {
        auto spriteSheetAnimationIt = spriteSheetAnimations.find(entityId);
        updateAnimation(game, entityId, spriteSheetAnimationIt->second);
    }
}
