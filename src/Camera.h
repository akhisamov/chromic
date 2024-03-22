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

#include "Vec2.h"

#include <cstdint>
#include <optional>

namespace systems {
class CameraDebugDraw;
}

class Camera {
    friend systems::CameraDebugDraw;

public:
    explicit Camera(const Vec2& size, float zoom = 1.f);
    explicit Camera(const Vec2& position, const Vec2& size, float zoom = 1.f);

    void setZoom(float value);
    void setSize(const Vec2& size);
    void setPosition(const Vec2& position);

    float getZoom() const;
    const Vec2& getSize() const;
    const Vec2& getPosition() const;

    void attachEntity(uint32_t entityId);
    void detachEntity();
    const std::optional<uint32_t>& getAttachedEntity() const;

private:
    Vec2 m_size {};
    Vec2 m_position {};
    float m_zoom { 1.0f };
    std::optional<uint32_t> m_attachedEntity {};
};
