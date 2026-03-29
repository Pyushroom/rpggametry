cmake_minimum_required(VERSION 3.20)

project(my_project
    VERSION 0.1.0
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(ENABLE_WARNINGS "Enable extra compiler warnings" ON)
option(ENABLE_SANITIZERS "Enable sanitizers in Debug builds" OFF)

find_package(raylib CONFIG REQUIRED)

set(SOURCES
    src/main.cpp
    src/game/Game.cpp
    src/game/GameState.cpp
    src/player/Player.cpp
    src/scene/Scene.cpp
    src/world/World.cpp
    src/world/WorldData.cpp
    src/dialogue/DialogueDatabase.cpp
    src/dialogue/DialogueController.cpp
    src/npc/NpcDatabase.cpp
    src/quest/QuestDatabase.cpp
    src/quest/QuestSystem.cpp
    src/quest/QuestJournal.cpp
    src/battle/BattleController.cpp
    src/enemy/EnemyDatabase.cpp
    src/player/playerStats.cpp
)

add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME}
    PRIVATE
        ${PROJECT_SOURCE_DIR}/include
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        raylib
)

if(ENABLE_WARNINGS)
    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        target_compile_options(${PROJECT_NAME} PRIVATE
            -Wall
            -Wextra
            -Wpedantic
            -Wshadow
            -Wconversion
            -Wsign-conversion
        )
    elseif(MSVC)
        target_compile_options(${PROJECT_NAME} PRIVATE
            /W4
            /permissive-
        )
    endif()
endif()

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:Debug>:-O0 -g3>
        $<$<CONFIG:Release>:-O2>
    )

    target_compile_definitions(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:Debug>:_GLIBCXX_ASSERTIONS>
    )
elseif(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:Debug>:/Zi>
        $<$<CONFIG:Release>:/O2>
    )
endif()

if(ENABLE_SANITIZERS AND CMAKE_BUILD_TYPE STREQUAL "Debug")
    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        target_compile_options(${PROJECT_NAME} PRIVATE
            -fsanitize=address
            -fsanitize=undefined
        )
        target_link_options(${PROJECT_NAME} PRIVATE
            -fsanitize=address
            -fsanitize=undefined
        )
    endif()
endif()