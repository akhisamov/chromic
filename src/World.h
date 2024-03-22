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

#pragma once

#include <cstdint>
#include <map>
#include <optional>
#include <set>
#include <vector>

namespace components {
struct Transform;
struct Velocity;
struct Sprite;
struct SpriteSheetAnimation;
struct DynamicRigidBody;
struct StaticRigidBody;
struct KinematicRigidBody;
struct Ground;
struct Jumper;
struct Runner;
}

struct World {
public:
    using EntityId = uint32_t;
    template <typename Component> using ComponentsMap = std::map<EntityId, Component>;

    World();
    ~World();

    void clear();

    EntityId createEntity();
    void destroyEntity(EntityId entityId);
    void cleanEntity(EntityId entityId);
    bool hasEntity(EntityId entityId) const;
    const std::vector<EntityId>& getEntities() const;

    EntityId createPlayer();
    void destroyPlayer();
    void cleanPlayer();
    bool hasPlayer() const;
    const std::optional<EntityId>& getPlayer() const;

    // Components
    ComponentsMap<components::Transform> transforms;
    ComponentsMap<components::Velocity> velocities;
    ComponentsMap<components::Sprite> sprites;
    ComponentsMap<components::SpriteSheetAnimation> spriteSheetAnimations;
    ComponentsMap<components::DynamicRigidBody> dynamicRigidyBodies;
    ComponentsMap<components::StaticRigidBody> staticRigidBodies;
    ComponentsMap<components::KinematicRigidBody> kinematicRigidBodies;
    ComponentsMap<components::Ground> grounds;
    ComponentsMap<components::Jumper> jumps;
    ComponentsMap<components::Runner> runners;

private:
    std::optional<EntityId> m_player {};
    std::vector<EntityId> m_entities;
    std::set<EntityId> m_entityIdQueue;
    EntityId m_nextId { 1 };
};
