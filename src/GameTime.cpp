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

/*
 * MY_TODO
 * get rid of SDL_timer
*/

#include "GameTime.h"

#include <SDL2/SDL_timer.h>

GameTime::GameTime()
    : m_lastUpdate(SDL_GetTicks())
    , m_current(0)
{
}

float GameTime::getDeltaTime() const { return static_cast<float>(m_current - m_lastUpdate) / 1000.f; }

uint32_t GameTime::getCurrentTime() const { return m_current; }

void GameTime::reset()
{
    m_lastUpdate = m_current;
    m_current = SDL_GetTicks();
}
