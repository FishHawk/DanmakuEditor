add_executable(danmaku-editor)

target_sources(danmaku-editor
  PRIVATE
    generators/DirectionGenerator.cpp
    generators/ModifierGenerator.cpp
    generators/OriginGenerator.cpp
    generators/RepetitionGenerator.cpp
    systems/LaunchSystem.cpp
    systems/LivetimeSystem.cpp
    systems/MoveSystem.cpp
    systems/RenderSystem.cpp
    graphic/Program.cpp
    graphic/Texture.cpp
    resources/ResourceManager.cpp
    resources/ProgramLoader.cpp
    resources/TextureLoader.cpp
    ui/Ui.cpp
    Game.cpp
    main.cpp
    TestCase.cpp
    Window.cpp
)

target_include_directories(danmaku-editor
  PRIVATE
    ${glad_SOURCE_DIR}/include
    ${glfw_SOURCE_DIR}/include
    ${glm_SOURCE_DIR}
    ${imgui_SOURCE_DIR}
    ${imgui_SOURCE_DIR}/examples
    ${implot_SOURCE_DIR}
)

target_link_libraries(danmaku-editor
  PRIVATE
    glad
    glfw
    fmt::fmt
    spdlog::spdlog
    EnTT::EnTT
    imgui
    implot
    stb
)

set_target_properties(danmaku-editor
  PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
    FOLDER ${PROJECT_NAME}
)