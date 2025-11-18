cmake_minimum_required(VERSION 3.16)
project(BitModifier LANGUAGES CXX)

# Find required Qt packages
find_package(Qt6 REQUIRED COMPONENTS Core Widgets)

# This sets up standard policies
qt_standard_project_setup()

# Create the executable - note this is qt_add_executable, not add_executable
qt_add_executable(BitModifier
    src/main.cpp
    src/classes/bit_modifier/bit_modifier.cpp
    src/classes/task_executor/task_executor.cpp
    src/libs/file_extractor/file_extractor.cpp
    src/ui_elements/task_card/task_card.cpp
    src/ui_elements/task_creation_dialog/task_creation_dialog.cpp
)

# Use keyword signature for target_link_libraries
target_link_libraries(BitModifier PRIVATE Qt6::Core Qt6::Widgets)