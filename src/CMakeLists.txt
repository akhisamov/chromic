add_subdirectory(Commands)
add_subdirectory(Components)
add_subdirectory(Systems)

target_include_directories(chromic PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_sources(chromic PRIVATE
  Camera.h Camera.cpp
  Constants.h
  Game.h Game.cpp
  GameTime.h GameTime.cpp
  InputManager.h InputManager.cpp
  KeyAction.h KeyAction.cpp
  World.h World.cpp
  Vec2.h

  main.cpp
  )
