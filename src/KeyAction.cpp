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

#include "KeyAction.h"

#include "Commands/Jump.h"
#include "Commands/Move.h"

#include <format>
#include <memory>

using namespace commands;

ICommandPtr createDownCommandByAction(KeyAction action)
{
    using namespace commands;
    using A = KeyAction;
    switch (action) {
    case A::MOVE_LEFT:
        return std::make_unique<Move>(Move::Type::LEFT, false);
    case A::MOVE_RIGHT:
        return std::make_unique<Move>(Move::Type::RIGHT, false);
    case A::JUMP:
        return std::make_unique<Jump>(false);
    case A::NONE:
        return nullptr;
    default:
        throw std::runtime_error(
            std::format("Could not find a down command for action \"{}\"", static_cast<int>(action)));
    }
}

ICommandPtr createUpCommandByAction(KeyAction action)
{
    using namespace commands;
    using A = KeyAction;
    switch (action) {
    case A::MOVE_LEFT:
        return std::make_unique<Move>(Move::Type::LEFT, true);
    case A::MOVE_RIGHT:
        return std::make_unique<Move>(Move::Type::RIGHT, true);
    case A::JUMP:
        return std::make_unique<Jump>(true);
    case A::NONE:
        return nullptr;
    default:
        throw std::runtime_error(
            std::format("Could not find an up command for action \"{}\"!", static_cast<int>(action)));
    }
}
