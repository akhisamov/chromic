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

#include "Move.h"

#include "Components/DynamicRigidBody.h"
#include "Components/KinematicRigidBody.h"
#include "Components/Runner.h"
#include "Game.h"
#include "World.h"

namespace {
float tryGetSpeed(const World& world, uint32_t entityId)
{
    const auto kinematicRBIt = world.kinematicRigidBodies.find(entityId);
    if (kinematicRBIt != world.kinematicRigidBodies.end()) {
        return kinematicRBIt->second.speed;
    }

    const auto dynamicRBIt = world.dynamicRigidyBodies.find(entityId);
    if (dynamicRBIt != world.dynamicRigidyBodies.end()) {
        return dynamicRBIt->second.speed;
    }

    return 0.0f;
}
}

namespace commands {
Move::Move(Type type, bool isStop)
    : m_type(type)
    , m_isStop(isStop)
{
}

void Move::execute(const Game& game, uint32_t entityId)
{
    auto& world = game.getWorld();
    auto& runners = world.runners;
    const float speed = tryGetSpeed(world, entityId);

    auto runnerIt = runners.find(entityId);
    if (m_isStop && runnerIt != runners.end()) {
        const float convertedSpeed = m_type == Type::RIGHT ? -speed : speed;
        runnerIt->second.speed += convertedSpeed;
    } else if (!m_isStop) {
        const float convertedSpeed = m_type == Type::RIGHT ? speed : -speed;
        if (runnerIt == runners.end()) {
            runners.emplace(entityId, components::Runner { .speed = convertedSpeed });
        } else {
            runnerIt->second.speed += convertedSpeed;
        }
    }
}
}
