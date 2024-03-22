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

#include "Systems/Types.h"

#include <vector>

struct SDL_Window;
struct SDL_Renderer;
struct SDL_Texture;
union SDL_Event;

class GameTime;
class Camera;
struct World;

class Game {
public:
    Game();
    ~Game();

    int run();

    [[nodiscard]] bool isRunning() const;

    [[nodiscard]] SDL_Renderer* getRenderer() const;
    [[nodiscard]] SDL_Texture* getTexture() const;
    [[nodiscard]] const std::unique_ptr<Camera>& getCamera() const;
    [[nodiscard]] const GameTime& getGameTime() const;
    [[nodiscard]] World& getWorld() const;

    template <typename... Ts> Game& addStartupSystems();
    template <typename... Ts> Game& addUpdateSystems();
    template <typename... Ts> Game& addDrawSystems();

private:
    bool init();
    void handleEvents();
    void handleInput(const SDL_Event& event);
    void update() const;
    void draw() const;

    template <typename... Ts> struct SystemsHelper;
    template <typename T, typename... Ts> struct SystemsHelper<T, Ts...> {
        template <typename I> static void add(std::vector<I>& data);
    };

    struct Context;
    std::unique_ptr<Context> m_ctx;

    std::vector<systems::IStartupSystemPtr> m_startupSystems;
    std::vector<systems::IUpdateSystemPtr> m_updateSystems;
    std::vector<systems::IDrawSystemPtr> m_drawSystems;

    bool m_running { false };
};

template <> struct Game::SystemsHelper<> {
    template <typename I> static void add(std::vector<I>& /*unused*/) { }
};

template <typename... Ts> Game& Game::addStartupSystems()
{
    SystemsHelper<Ts...>::add(m_startupSystems);
    return *this;
}

template <typename... Ts> Game& Game::addUpdateSystems()
{
    SystemsHelper<Ts...>::add(m_updateSystems);
    return *this;
}

template <typename... Ts> Game& Game::addDrawSystems()
{
    SystemsHelper<Ts...>::add(m_drawSystems);
    return *this;
}

template <typename T, typename... Ts>
template <typename I>
inline void Game::SystemsHelper<T, Ts...>::add(std::vector<I>& data)
{
    data.emplace_back(std::make_unique<T>());
    SystemsHelper<Ts...>::add(data);
}
