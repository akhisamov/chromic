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

#include "Systems/AnimationUpdate.h"
#include "Systems/BackgroundColorStartup.h"
#include "Systems/CameraDebugDraw.h"
#include "Systems/CameraPositionUpdate.h"
#include "Systems/CollisionUpdate.h"
#include "Systems/GravityUpdate.h"
#include "Systems/GroundStartup.h"
#include "Systems/JumpingUpdate.h"
#include "Systems/LevelStartup.h"
#include "Systems/MovingUpdate.h"
#include "Systems/PlayerAnimationUpdate.h"
#include "Systems/PlayerStartup.h"
#include "Systems/PositionUpdate.h"
#include "Systems/SpriteDraw.h"

#include "Game.h"

int main(int argc, char** argv)
{
    Game game;
    game.addStartupSystems<systems::LevelStartup, systems::BackgroundColorStartup, systems::PlayerStartup,
            systems::GroundStartup>()
        .addUpdateSystems<systems::JumpingUpdate, systems::GravityUpdate, systems::MovingUpdate,
            systems::CollisionUpdate, systems::PositionUpdate, systems::CameraPositionUpdate,
            systems::PlayerAnimationUpdate, systems::AnimationUpdate>()
        .addDrawSystems<systems::CameraDebugDraw, systems::SpriteDraw>();

    return game.run();
}
