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

#include "Game.h"

#include <SDL2/SDL.h>

#include <SDL_timer.h>
#include <cstdint>
#include <imgui.h>
#include <imgui_impl_sdl2.h>
#include <imgui_impl_sdlrenderer2.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "Commands/ICommand.h"

#include "Systems/IDrawSystem.h"
#include "Systems/IStartupSystem.h"
#include "Systems/IUpdateSystem.h"

#include "Camera.h"
#include "Constants.h"
#include "GameTime.h"
#include "InputManager.h"
#include "Vec2.h"
#include "World.h"

#include <format>

void showErrorMessageBox(const std::string_view& msg)
{
    SDL_ShowSimpleMessageBox(SDL_MessageBoxFlags::SDL_MESSAGEBOX_ERROR, "Fatal Error", msg.data(), nullptr);
}

struct Game::Context {
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    SDL_Texture* texture = nullptr;
    std::unique_ptr<Camera> camera = nullptr;
    std::unique_ptr<InputManager> inputManager = nullptr;
    World world;
    GameTime gameTime;

    void initWindow(const std::string_view& windowTitle, const Vec2& size)
    {
        camera = std::make_unique<Camera>(size, 5.0f);
        window = SDL_CreateWindow(windowTitle.data(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
            static_cast<int>(size.x), static_cast<int>(size.y), 0);
        if (!window) {
            const std::string msg = std::format("SDL could not create window!\nREASON: {}", SDL_GetError());
            throw std::runtime_error(msg);
        }
    }

    void initRenderer()
    {
        renderer = SDL_CreateRenderer(window, -1, 0);
        if (!renderer) {
            const std::string msg = std::format("SDL could not create renderer!\nREASON: {}", SDL_GetError());
            throw std::runtime_error(msg);
        }
    }

    void initImGui() const
    {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
        ImGui::StyleColorsDark();
        ImGui_ImplSDL2_InitForSDLRenderer(window, renderer);
        ImGui_ImplSDLRenderer2_Init(renderer);
    }

    void initTexture(const std::string_view& textureFilename)
    {
        int32_t width;
        int32_t height;
        int32_t bytesPerPixel;
        std::string path(SDL_GetBasePath());
        path += textureFilename;
        void* data = stbi_load(path.c_str(), &width, &height, &bytesPerPixel, 0);
        if (!data) {
            const std::string msg = std::format(
                "stbi could not load texture \"{}\"!\nREASON: {}", textureFilename, stbi_failure_reason());
            throw std::runtime_error(msg);
        }

        int pitch = width * bytesPerPixel;
        pitch = (pitch + 3) & ~3;

        uint32_t rmask;
        uint32_t gmask;
        uint32_t bmask;
        uint32_t amask;
#if SDL_BYTEORDER == SDL_LIL_ENDIAN
        rmask = 0x000000FF;
        gmask = 0x0000FF00;
        bmask = 0x00FF0000;
        amask = (bytesPerPixel == 4) ? 0xFF000000 : 0;
#else
        int s = (bytesPerPixel == 4) ? 0 : 8;
        rmask = 0xFF000000 >> s;
        gmask = 0x00FF0000 >> s;
        bmask = 0x0000FF00 >> s;
        amask = 0x000000FF >> s;
#endif

        SDL_Surface* surface
            = SDL_CreateRGBSurfaceFrom(data, width, height, bytesPerPixel * 8, pitch, rmask, gmask, bmask, amask);
        const bool surfaceSuccess = surface != nullptr;
        if (surfaceSuccess) {
            texture = SDL_CreateTextureFromSurface(renderer, surface);
            SDL_FreeSurface(surface);
        }

        stbi_image_free(data);

        if (!surfaceSuccess) {
            const std::string msg
                = std::format("SDL could not create surface for \"{}\"!\nREASON: {}", textureFilename, SDL_GetError());
            throw std::runtime_error(msg);
        }

        if (!texture) {
            const std::string msg
                = std::format("SDL could not create texture \"{}\"!\nREASON: {}", textureFilename, SDL_GetError());
            throw std::runtime_error(msg);
        }
    }

    void initInput() { inputManager = std::make_unique<InputManager>(); }

    ~Context()
    {
        ImGui_ImplSDLRenderer2_Shutdown();
        ImGui_ImplSDL2_Shutdown();
        ImGui::DestroyContext();

        SDL_DestroyTexture(texture);
        texture = nullptr;

        SDL_DestroyRenderer(renderer);
        renderer = nullptr;

        SDL_DestroyWindow(window);
        window = nullptr;
    }
};

Game::Game()
    : m_ctx(std::make_unique<Context>())
{
}

Game::~Game()
{
    m_ctx = nullptr;
    SDL_Quit();
}

int Game::run()
{
    if (!init()) {
        return -1;
    }

    m_ctx->gameTime.reset();
    while (isRunning()) {
        const uint32_t start = getGameTime().getCurrentTime();
        handleEvents();
        m_ctx->gameTime.reset();
        update();
        draw();
        const uint32_t end = getGameTime().getCurrentTime();

        const float elapsed = static_cast<float>(end - start) / 1000.0f;
        const auto delay
            = static_cast<uint32_t>(std::max(static_cast<int>(1000.0f / constants::FRAME_RATE - elapsed), 0));
        SDL_Delay(delay);
    }

    return 0;
}

bool Game::init()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        const std::string msg = std::format("SDL could not initialize!\nREASON: {}", SDL_GetError());
        showErrorMessageBox(msg);
        return false;
    }

    try {
        constexpr auto WINDOW_SIZE = Vec2(1280.0f, 720.0f);
        m_ctx->initWindow("chromic", WINDOW_SIZE);
        m_ctx->initRenderer();
        m_ctx->initImGui();
        m_ctx->initTexture("res/tilemap.png");
        m_ctx->initInput();
    } catch (const std::exception& e) {
        showErrorMessageBox(e.what());
        SDL_Quit();
        return false;
    }

    for (const auto& system : m_startupSystems) {
        system->startup(*this);
    }

    m_running = true;
    return true;
}

void Game::handleEvents()
{
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        ImGui_ImplSDL2_ProcessEvent(&event);
        switch (event.type) {
        case SDL_QUIT:
            m_running = false;
            break;
        case SDL_KEYDOWN:
        case SDL_KEYUP:
            handleInput(event);
            break;
        default:
            break;
        }
    }
}

void Game::handleInput(const SDL_Event& event)
{
    const commands::ICommandPtr command = m_ctx->inputManager->handle(event);
    if (!command) {
        return;
    }
    const auto& player = m_ctx->world.getPlayer();
    if (!player.has_value()) {
        return;
    }
    command->execute(*this, player.value());
}

void Game::update() const
{
    for (const auto& system : m_updateSystems) {
        system->update(*this);
    }
}

void Game::draw() const
{
    ImGui_ImplSDLRenderer2_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

    SDL_RenderClear(getRenderer());
    for (const auto& system : m_drawSystems) {
        system->draw(*this);
    }

    ImGui::Render();

    ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData());

    SDL_RenderPresent(getRenderer());
}

bool Game::isRunning() const { return m_running; }

SDL_Renderer* Game::getRenderer() const { return m_ctx->renderer; }

SDL_Texture* Game::getTexture() const { return m_ctx->texture; }

const std::unique_ptr<Camera>& Game::getCamera() const { return m_ctx->camera; }

const GameTime& Game::getGameTime() const { return m_ctx->gameTime; }

World& Game::getWorld() const { return m_ctx->world; }
