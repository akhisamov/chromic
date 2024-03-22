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

#include <cmath>
#include <limits>

struct Vec2 {
    float x { 0.0f };
    float y { 0.0f };

    constexpr Vec2() = default;
    constexpr Vec2(float x, float y);
    constexpr explicit Vec2(float scalar);

    constexpr Vec2 operator+(const Vec2& rhs) const;
    constexpr Vec2 operator-(const Vec2& rhs) const;
    constexpr Vec2 operator*(const Vec2& rhs) const;
    constexpr Vec2 operator/(const Vec2& rhs) const;
    constexpr Vec2 operator*(float rhs) const;

    constexpr Vec2& operator+=(const Vec2& rhs);
    constexpr Vec2& operator-=(const Vec2& rhs);
    constexpr Vec2& operator*=(const Vec2& rhs);
    constexpr Vec2& operator/=(const Vec2& rhs);
    constexpr Vec2& operator*=(float rhs);

    constexpr bool operator==(const Vec2& rhs) const;
    constexpr bool operator!=(const Vec2& rhs) const;
};

constexpr inline Vec2::Vec2(float x, float y)
    : x(x)
    , y(y)
{
}

constexpr inline Vec2::Vec2(float scalar)
    : x(scalar)
    , y(scalar)
{
}

constexpr inline Vec2 Vec2::operator+(const Vec2& rhs) const
{
    Vec2 result;
    result.x = x + rhs.x;
    result.y = y + rhs.y;
    return result;
}

constexpr inline Vec2 Vec2::operator-(const Vec2& rhs) const
{
    Vec2 result;
    result.x = x - rhs.x;
    result.y = y - rhs.y;
    return result;
}

constexpr inline Vec2 Vec2::operator*(const Vec2& rhs) const
{
    Vec2 result;
    result.x = x * rhs.x;
    result.y = y * rhs.y;
    return result;
}

constexpr inline Vec2 Vec2::operator/(const Vec2& rhs) const
{
    Vec2 result;
    result.x = x / rhs.x;
    result.y = y / rhs.y;
    return result;
}

constexpr inline Vec2 Vec2::operator*(float rhs) const
{
    Vec2 result;
    result.x = x * rhs;
    result.y = y * rhs;
    return result;
}

constexpr inline Vec2& Vec2::operator+=(const Vec2& rhs)
{
    x += rhs.x;
    y += rhs.y;
    return *this;
}

constexpr inline Vec2& Vec2::operator-=(const Vec2& rhs)
{
    x -= rhs.x;
    y -= rhs.y;
    return *this;
}

constexpr inline Vec2& Vec2::operator*=(const Vec2& rhs)
{
    x *= rhs.x;
    y *= rhs.y;
    return *this;
}

constexpr inline Vec2& Vec2::operator/=(const Vec2& rhs)
{
    x /= rhs.x;
    y /= rhs.y;
    return *this;
}

constexpr inline Vec2& Vec2::operator*=(float rhs)
{
    x *= rhs;
    y *= rhs;
    return *this;
}

constexpr inline bool Vec2::operator==(const Vec2& rhs) const
{
    return std::fabs(x - rhs.x) <= std::numeric_limits<float>::epsilon()
        && std::fabs(y - rhs.y) <= std::numeric_limits<float>::epsilon();
}

constexpr inline bool Vec2::operator!=(const Vec2& rhs) const { return !(*this == rhs); }
