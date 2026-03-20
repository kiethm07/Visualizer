cmake_minimum_required(VERSION 3.28)
project(CMakeSFMLProject LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.2
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_MakeAvailable(SFML)

add_executable(Visualizer 
                            "src/main.cpp" 
                            "src/UI/Camera.cpp"
                            "src/UI/UI.cpp"
                            "src/DataStructures/LinkedList.cpp"  
                            "src/Model/Button.cpp" 

                            "include/Model/MenuState.h" 
                            "include/Model/Button.h"                         
                            "include/UI/Camera.h" 
                            "include/UI/UI.h" 
                            "include/DataStructures/LinkedList.h"  
                            "include/UI/DSMenu.h" 
                            "src/UI/DSMenu.cpp" 
                            "include/AssetManager/AssetManager.h" 
                            "src/AssetManager/AssetManager.cpp" "include/UI/MainMenu.h" "src/UI/MainMenu.cpp" "include/DSUI/LinkedListUI.h" "include/Model/ListNode.h" "src/Model/ListNode.cpp" "src/DSUI/LinkedListUI.cpp" "include/Panel/LinkedListPanel.h" "src/Panel/LinkedListPanel.cpp" "include/Model/LinkedListModel/LinkedListState.h" "include/Model/LinkedListModel/ListOperation.h" "include/Model/LinkedListModel/LinkedListTimeline.h" "src/Model/LinkedListModel/LinkedListTimeline.cpp" "src/Model/LinkedListModel/LinkedListState.cpp" "include/Model/LinkedListModel/LinkedListRenderer.h" "src/Model/LinkedListModel/LinkedListRenderer.cpp" "include/Model/Arrow.h" "src/Model/Arrow.cpp" "include/Model/TextBox.h" "src/Model/TextBox.cpp" "include/Animation/LinkedList/LinkedListAnimationType.h" "include/Animation/LinkedList/LinkedListRecorder.h" "include/Animation/LinkedList/LinkedListAnimationCommand.h")

target_compile_features(Visualizer PRIVATE cxx_std_17)
target_link_libraries(Visualizer PRIVATE SFML::Graphics)

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT Visualizer)

target_include_directories(Visualizer PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_compile_definitions(Visualizer PRIVATE ASSET_DIR="${CMAKE_SOURCE_DIR}/assets")