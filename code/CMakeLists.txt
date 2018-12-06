find_package(Magnum REQUIRED GL Shaders Sdl2Application)

set_directory_properties(PROPERTIES CORRADE_USE_PEDANTIC_FLAGS ON)

set(sources)

src_group(Main sources
    Application.cpp
    Application_State.hpp
    AppState.hpp
    AppState.cpp
    State_Test.cpp
    State_Test.hpp
    Resources.cpp
    Resources.hpp
    Resources_Figures.cpp
    Flat3DPVM.cpp
    Flat3DPVM.hpp
    Random.hpp
    Random.cpp
)

src_group(Game sources
    State_InGame.cpp
    State_InGame.hpp
    Pit.cpp
    Pit.hpp
    LayerPreview.hpp
    LayerPreview.cpp
    Preferences.hpp
    Preferences.cpp
)

src_group(Game~Figure sources
    Figure.hpp
    Figure.cpp
    Primitives.hpp
    CubeTemplate.hpp
    CubeTemplate.cpp
    FigureTemplate.cpp
    FigureTemplate.hpp
    FigureSet.cpp
    FigureSet.hpp
    FigureManager.hpp
    FigureManager.cpp
    SimpleLexer.hpp
    SimpleLexer.cpp
)

add_executable(BlockOut3000
    ${sources}
)

target_link_libraries(BlockOut3000 PRIVATE
    Magnum::Application
    Magnum::GL
    Magnum::Magnum
    Magnum::Shaders
)
