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

#include "CollisionUpdate.h"

#include "Components/DynamicRigidBody.h"
#include "Components/KinematicRigidBody.h"
#include "Components/StaticRigidBody.h"
#include "Components/Transform.h"
#include "Components/Velocity.h"
#include "Game.h"
#include "Vec2.h"
#include "World.h"

#include <ranges>

using namespace systems;

struct Rectangle {
    int top;
    int bottom;
    int left;
    int right;

    Rectangle(SDL_Rect boundingBox, const Vec2& position)
        : top(static_cast<int>(position.y) + boundingBox.y)
        , bottom(top + boundingBox.h)
        , left(static_cast<int>(position.x) + boundingBox.x)
        , right(left + boundingBox.w)
    {
    }
};

bool checkCollision(const Rectangle& a, const Rectangle& b)
{
    if (a.top >= b.bottom) {
        return false;
    }

    if (a.bottom <= b.top) {
        return false;
    }

    if (a.left >= b.right) {
        return false;
    }

    if (a.right <= b.left) {
        return false;
    }

    return true;
}

void tryCollideWithStatic(World& world, World::EntityId a, World::EntityId b)
{
    try {
        auto& velocity = world.velocities[a];
        const auto& dynamicRB = world.dynamicRigidyBodies[a];
        const auto& dynamicTransform = world.transforms[a];
        const auto& staticRB = world.staticRigidBodies[b];
        const auto& staticTransform = world.transforms[b];
        const Rectangle staticRectangle(staticRB.boundingBox, staticTransform.position);
        if (velocity.x != 0) {
            const Rectangle dynamicRectangle(
                dynamicRB.boundingBox, dynamicTransform.position + Vec2(velocity.x, 0.f));
            if (checkCollision(dynamicRectangle, staticRectangle)) {
                velocity.x = 0;
            }
        }
        if (velocity.y != 0) {
            const Rectangle dynamicRectangle(
                dynamicRB.boundingBox, dynamicTransform.position + Vec2(0.f, velocity.y));
            if (checkCollision(dynamicRectangle, staticRectangle)) {
                velocity.y = 0;
            }
        }
    } catch (...) {
        return;
    }
}

void tryCollideWithKinematic(World& world, World::EntityId a, World::EntityId b)
{
    // MY_TODO
}
void tryCollideWithDynamic(World& world, World::EntityId a, World::EntityId b)
{
    // MY_TODO
}

void CollisionUpdate::update(const Game& game)
{
    auto& world = game.getWorld();

    const auto& dynamicRigidyBodies = world.dynamicRigidyBodies;
    const auto& velocities = world.velocities;
    auto view = std::views::filter(world.getEntities(), [&](World::EntityId entityId) {
        return dynamicRigidyBodies.find(entityId) != dynamicRigidyBodies.end()
            && velocities.find(entityId) != velocities.end();
    });
    if (view.empty()) {
        return;
    }

    auto staticRigidBodiesView = std::views::filter(world.getEntities(), [&world](World::EntityId entityId) {
        return world.staticRigidBodies.find(entityId) != world.staticRigidBodies.end();
    });
    // auto kinematicRigidBodiesView = std::views::filter(world.getEntities(), [&world](World::EntityId entityId) {
    //     return world.kinematicRigidBodies.find(entityId) != world.kinematicRigidBodies.end();
    // });
    // auto dynamicRigidyBodiesView = std::views::filter(world.getEntities(), [&world](World::EntityId entityId) {
    //     return world.dynamicRigidyBodies.find(entityId) != world.dynamicRigidyBodies.end();
    // });

    for (const World::EntityId entityIdA : view) {
        for (const World::EntityId entityIdB : staticRigidBodiesView) {
            tryCollideWithStatic(world, entityIdA, entityIdB);
        }
        // for (const World::EntityId entityIdB : kinematicRigidBodiesView) {
        //     tryCollideWithKinematic(world, entityIdA, entityIdB);
        // }
        // for (const World::EntityId entityIdB : dynamicRigidyBodiesView) {
        //     tryCollideWithDynamic(world, entityIdA, entityIdB);
        // }
    }
}
