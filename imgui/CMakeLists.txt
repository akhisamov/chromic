set(IMGUI_SOURCE_FILES
  imconfig.h
  imgui.cpp
  imgui.h
  imgui_demo.cpp
  imgui_draw.cpp
  imgui_impl_sdl2.cpp
  imgui_impl_sdl2.h
  imgui_impl_sdlrenderer2.cpp
  imgui_impl_sdlrenderer2.h
  imgui_internal.h
  imgui_tables.cpp
  imgui_widgets.cpp
  imstb_rectpack.h
  imstb_textedit.h
  imstb_truetype.h
)

add_library(ImGui ${IMGUI_SOURCE_FILES})
target_include_directories(ImGui PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(ImGui PUBLIC
  SDL2::SDL2
)
