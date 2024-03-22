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

#include "PlayerAnimationUpdate.h"

#include "Components/Runner.h"
#include "Components/Sprite.h"
#include "Components/SpriteSheetAnimation.h"
#include "Game.h"
#include "World.h"

using namespace systems;

void PlayerAnimationUpdate::update(const Game& game)
{
    auto& world = game.getWorld();

    const auto& player = world.getPlayer();
    if (!player.has_value()) {
        return;
    }

    const World::EntityId& entityId = player.value();

    auto& spriteSheetAnimations = world.spriteSheetAnimations;
    auto animationIt = spriteSheetAnimations.find(entityId);
    if (animationIt == spriteSheetAnimations.end()) {
        return;
    }
    auto& animation = animationIt->second;

    const auto& runners = world.runners;
    const auto runnerIt = runners.find(entityId);
    const bool isRunning = runnerIt != runners.end() && runnerIt->second.speed != 0.0f;
    const std::string newTrack = isRunning ? "run" : "idle";
    if (animation.currentTrackName != newTrack) {
        animation.currentTrackName = newTrack;
        animation.currentTrackFrameIdx = 0;
        animation.lastUpdate = 0;
    }

    const bool isFlip = isRunning && runnerIt->second.speed < 0.0f;
    auto& sprites = world.sprites;
    auto spriteIt = sprites.find(entityId);
    if (spriteIt != sprites.end()) {
        spriteIt->second.isFlip = isFlip;
    } else {
        sprites.emplace(entityId, components::Sprite { .isFlip = isFlip });
    }
}
