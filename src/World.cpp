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

#include "World.h"

#include "Components/DynamicRigidBody.h"
#include "Components/Ground.h"
#include "Components/Jumper.h"
#include "Components/KinematicRigidBody.h"
#include "Components/Runner.h"
#include "Components/Sprite.h"
#include "Components/SpriteSheetAnimation.h"
#include "Components/StaticRigidBody.h"
#include "Components/Transform.h"
#include "Components/Velocity.h"

#include <algorithm>
#include <cassert>
#include <cstdint>

World::World() = default;
World::~World() = default;

void World::clear()
{
    transforms.clear();
    velocities.clear();
    sprites.clear();
    spriteSheetAnimations.clear();
    dynamicRigidyBodies.clear();
    staticRigidBodies.clear();
    kinematicRigidBodies.clear();
    grounds.clear();
    jumps.clear();
    runners.clear();

    m_player.reset();
    m_entities.clear();
    m_entityIdQueue.clear();
    m_nextId = 1;
}

World::EntityId World::createEntity()
{
    EntityId id;
    if (!m_entityIdQueue.empty()) {
        id = *m_entityIdQueue.begin();
        m_entityIdQueue.erase(id);
    } else {
        assert(m_nextId != UINT32_MAX);
        id = m_nextId++;
    }
    assert(!hasEntity(id));
    m_entities.push_back(id);
    return id;
}

void World::destroyEntity(EntityId entityId)
{
    cleanEntity(entityId);
    auto it = std::find(m_entities.begin(), m_entities.end(), entityId);
    if (it != m_entities.end()) {
        m_entities.erase(it);
    }
    m_entityIdQueue.emplace(entityId);
    if (m_player.has_value() && m_player.value() == entityId) {
        m_player.reset();
    }
}

void World::cleanEntity(EntityId entityId)
{
    transforms.erase(entityId);
    velocities.erase(entityId);
    sprites.erase(entityId);
    spriteSheetAnimations.erase(entityId);
    dynamicRigidyBodies.erase(entityId);
    staticRigidBodies.erase(entityId);
    kinematicRigidBodies.erase(entityId);
    grounds.erase(entityId);
    jumps.erase(entityId);
    runners.erase(entityId);
}

bool World::hasEntity(EntityId entityId) const
{
    return std::any_of(m_entities.begin(), m_entities.end(), [entityId](EntityId id) { return entityId == id; });
}

const std::vector<World::EntityId>& World::getEntities() const { return m_entities; }

World::EntityId World::createPlayer()
{
    if (!m_player.has_value()) {
        m_player = createEntity();
    }
    return m_player.value();
}

const std::optional<World::EntityId>& World::getPlayer() const { return m_player; }

void World::destroyPlayer()
{
    if (m_player.has_value()) {
        destroyEntity(m_player.value());
    }
}

void World::cleanPlayer()
{
    if (m_player.has_value()) {
        cleanEntity(m_player.value());
    }
}

bool World::hasPlayer() const { return m_player.has_value(); }
