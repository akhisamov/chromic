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

#include "SpriteDraw.h"

#include <SDL2/SDL.h>

#include "Camera.h"
#include "Components/Sprite.h"
#include "Components/Transform.h"
#include "Game.h"
#include "Vec2.h"
#include "World.h"

#include <ranges>

using namespace systems;

namespace {
void drawSprite(const Game& game, const components::Sprite& sprite, const components::Transform& transform)
{
    SDL_RendererFlip flip = sprite.isFlip ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;
    const Vec2 scaledSize = transform.size * game.getCamera()->getZoom();
    const Vec2 position = (transform.position * game.getCamera()->getZoom()) + game.getCamera()->getPosition();
    SDL_Rect dstRect;
    dstRect.x = static_cast<int>(position.x);
    dstRect.y = static_cast<int>(position.y);
    dstRect.w = static_cast<int>(scaledSize.x);
    dstRect.h = static_cast<int>(scaledSize.y);
    SDL_RenderCopyEx(game.getRenderer(), game.getTexture(), &sprite.sourceRect, &dstRect, 0.0, nullptr, flip);
}
}

void SpriteDraw::draw(const Game& game)
{
    const auto& world = game.getWorld();
    const auto& sprites = world.sprites;
    const auto& transforms = world.transforms;
    auto view = std::views::filter(world.getEntities(), [sprites, transforms](World::EntityId entityId) {
        return sprites.find(entityId) != sprites.end() && transforms.find(entityId) != transforms.end();
    });
    for (const World::EntityId entityId : view) {
        drawSprite(game, sprites.at(entityId), transforms.at(entityId));
    }
}
