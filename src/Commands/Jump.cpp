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

#include "Jump.h"

#include "Components/Jumper.h"
#include "Game.h"
#include "World.h"

namespace commands {
Jump::Jump(bool isStop)
    : m_isStop(isStop)
{
}

void Jump::execute(const Game& game, uint32_t entityId)
{
    auto& world = game.getWorld();
    auto& jumps = world.jumps;
    auto jumpIt = jumps.find(entityId);

    if (jumpIt != jumps.end() && m_isStop) {
        jumpIt->second.lastHeight = jumpIt->second.height;
    } else if (jumpIt == jumps.end() && !m_isStop) {
        world.jumps.try_emplace(entityId, components::Jumper(20.0f));
    }
}
}
