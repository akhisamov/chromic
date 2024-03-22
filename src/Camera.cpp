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

#include "Camera.h"

#include <imgui.h>

Camera::Camera(const Vec2& size, float zoom)
    : m_size(size)
    , m_zoom(zoom)
{
}

Camera::Camera(const Vec2& position, const Vec2& size, float zoom)
    : m_position(position)
    , m_size(size)
    , m_zoom(zoom)
{
}

void Camera::setZoom(float value) { m_zoom = value; }

void Camera::setSize(const Vec2& size) { m_size = size; }

void Camera::setPosition(const Vec2& position) { m_position = position; }

float Camera::getZoom() const { return m_zoom; }

const Vec2& Camera::getSize() const { return m_size; }

const Vec2& Camera::getPosition() const { return m_position; }

void Camera::attachEntity(uint32_t entityId) { m_attachedEntity = entityId; }
void Camera::detachEntity() { m_attachedEntity.reset(); }
const std::optional<uint32_t>& Camera::getAttachedEntity() const { return m_attachedEntity; }
