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

#include "InputManager.h"

#include <SDL2/SDL_events.h>

#include "Commands/ICommand.h"
#include "KeyAction.h"

InputManager::InputManager()
{
    using A = KeyAction;
    m_actions = {
        { SDLK_a, A::MOVE_LEFT },
        { SDLK_d, A::MOVE_RIGHT },
        { SDLK_SPACE, A::JUMP },
    };
}

commands::ICommandPtr InputManager::handle(const SDL_Event& event) const
{
    if (event.key.repeat != 0) {
        return nullptr;
    }

    auto actionIt = m_actions.find(event.key.keysym.sym);
    if (actionIt == m_actions.end()) {
        return nullptr;
    }

    switch (event.type) {
    case SDL_KEYDOWN:
        return createDownCommandByAction(actionIt->second);
    case SDL_KEYUP:
        return createUpCommandByAction(actionIt->second);
    default:
        return nullptr;
    }
}
