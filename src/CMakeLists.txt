# Some common variables 
cmake_minimum_required(VERSION 3.14)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Name of the project
project(BrickGame-CPP VERSION 2.0.0)
include(CTest)
find_package(GTest REQUIRED)
find_package(PkgConfig REQUIRED)
include(GoogleTest)

# Tetris lib
set(TETRIS_DIR "brick_game/tetris")
set(TETRIS_SRC_DIR ${TETRIS_DIR}/lib)
set(TETRIS_SRCS 
    ${TETRIS_SRC_DIR}/scoring.c
    ${TETRIS_SRC_DIR}/tetramino.c
    ${TETRIS_SRC_DIR}/fsm.c
    ${TETRIS_SRC_DIR}/field.c
    ${TETRIS_SRC_DIR}/api/api.c
    ${TETRIS_SRC_DIR}/api/view.c
)
add_library(tetris STATIC ${TETRIS_SRCS})
target_include_directories(tetris PUBLIC "brick_game" 
	${TETRIS_DIR}/include
    brick_game/common/include
)
target_compile_options(tetris PUBLIC 
	-Wall
	-Wextra
	-Werror
)

set(TETRIS_TEST_SRCS ${TETRIS_DIR}/tests)
add_executable(test_tetris
	${TETRIS_TEST_SRCS}/main.c
	${TETRIS_TEST_SRCS}/test_field.c
	${TETRIS_TEST_SRCS}/test_fsm.c
	${TETRIS_TEST_SRCS}/test_scoring.c
	${TETRIS_TEST_SRCS}/test_tetramino.c 
)
target_link_libraries(test_tetris PRIVATE
	tetris
	check
)
add_test(NAME test_tetris COMMAND test_tetris)

# Snake lib
set(SNAKE_DIR "brick_game/snake")
set(SNAKE_SRC_DIR ${SNAKE_DIR}/lib)
set(SNAKE_SRCS
    ${SNAKE_SRC_DIR}/fsm.cc  
    ${SNAKE_SRC_DIR}/snake.cc 
    ${SNAKE_SRC_DIR}/controller.cc
)
add_library(snake STATIC ${SNAKE_SRCS})
target_include_directories(snake PUBLIC "brick_game"
	${SNAKE_DIR}/include
    brick_game/common/include
)

# Tests for snake
add_executable(test_snake ${SNAKE_DIR}/tests/test_snake.cc)
target_link_libraries(test_snake PRIVATE
	snake
	GTest::gtest_main
)
gtest_discover_tests(test_snake)
# add_test(NAME test_snake COMMAND test_snake)

# Brick game Curses CLI
add_library(brick_game_cli OBJECT gui/cli/cli.c)
target_include_directories(brick_game_cli PUBLIC
    brick_game/common/include
)
target_link_libraries(brick_game_cli PUBLIC ncursesw)

# Tetris CLI app
add_executable(tetris.out gui/cli/tetris_cli.c)
target_link_libraries(tetris.out PRIVATE general 
    tetris; brick_game_cli
)

# Snake CLI app
add_executable(snake.out gui/cli/snake_cli.c)
target_link_libraries(snake.out PRIVATE general 
    snake; brick_game_cli
)

# Brick game GTK GUI
set(GUI_DIR "gui/desktop")
add_library(brick_game_gui_gtk4 OBJECT 
    ${GUI_DIR}/input_handler.cc
    ${GUI_DIR}/sidebar.cc
    ${GUI_DIR}/stat_widget.cc
    ${GUI_DIR}/next_piece.cc
    ${GUI_DIR}/board.cc
    ${GUI_DIR}/brickgame_window.cc
)
target_include_directories(brick_game_gui_gtk4 PUBLIC 
	${GUI_DIR}/include
    brick_game/common/include
)
pkg_check_modules(GTKMM REQUIRED IMPORTED_TARGET gtkmm-4.0)
target_link_libraries(brick_game_gui_gtk4 PUBLIC PkgConfig::GTKMM)

# Tetris GTK4 GUI App
add_executable(tetris_desk.out 
    ${GUI_DIR}/tetris_app.cc
)
target_link_libraries(tetris_desk.out PRIVATE general
    tetris
    brick_game_gui_gtk4
)

# Snake GTK4 GUI App
add_executable(snake_desk.out 
    ${GUI_DIR}/snake_app.cc
)
target_link_libraries(snake_desk.out PRIVATE general
    snake
    brick_game_gui_gtk4
)

# Source - https://stackoverflow.com/a/37967856
# Posted by galsh83, modified by community. See post 'Timeline' for change history
# Retrieved 2026-07-27, License - CC BY-SA 3.0
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/scripts/cmake)

if (CMAKE_BUILD_TYPE STREQUAL "Coverage")
    include(CodeCoverage)

    append_coverage_compiler_flags()

    # 2. Создаём цель для генерации отчёта (например, с lcov)
    setup_target_for_coverage_lcov(
        NAME tetris_coverage                   # имя цели: make coverage
        EXECUTABLE test_tetris       # исполняемый файл (тесты)
        DEPENDENCIES test_tetris    # сборка перед запуском
        BASE_DIRECTORY ${CMAKE_SOURCE_DIR}
		LCOV_ARGS --ignore-errors inconsistent,inconsistent 
		# GENHTML_ARGS --ignore-errors inconsistent
		EXCLUDE "${CMAKE_SOURCE_DIR}/${SNAKE_DIR}/*" "${CMAKE_SOURCE_DIR}/${TETRIS_DIR}/tests/*" "${CMAKE_SOURCE_DIR}/${TETRIS_SRC_DIR}/api/*" "/usr/include/*"
    )

    setup_target_for_coverage_lcov(
        NAME snake_coverage                   # имя цели: make coverage
        EXECUTABLE test_snake       # исполняемый файл (тесты)
        DEPENDENCIES test_snake    # сборка перед запуском
        BASE_DIRECTORY ${CMAKE_SOURCE_DIR}
		LCOV_ARGS --ignore-errors inconsistent,inconsistent 
		# GENHTML_ARGS --ignore-errors inconsistent
		EXCLUDE "${CMAKE_SOURCE_DIR}/${TETRIS_DIR}/*" "${CMAKE_SOURCE_DIR}/${SNAKE_DIR}/tests/*" "/usr/include/*"
    )
endif()
