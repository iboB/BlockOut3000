CPMAddPackage(gh:ocornut/imgui@1.86)

add_library(imgui STATIC
    ${imgui_SOURCE_DIR}/imgui.cpp
    ${imgui_SOURCE_DIR}/imgui_demo.cpp
    ${imgui_SOURCE_DIR}/imgui_draw.cpp
    ${imgui_SOURCE_DIR}/imgui_widgets.cpp
    ${imgui_SOURCE_DIR}/imgui_tables.cpp
)
target_include_directories(imgui INTERFACE ${imgui_SOURCE_DIR})
target_compile_definitions(imgui PUBLIC -DIMGUI_DISABLE_OBSOLETE_FUNCTIONS)
set_target_properties(imgui PROPERTIES FOLDER third-party)
