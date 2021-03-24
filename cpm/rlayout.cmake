CPMAddPackage(
    NAME rlayout
    GITHUB_REPOSITORY randrew/layout
    GIT_TAG b0c02829df41b066c5aaedcb946498209bcb38e5
)

add_library(rlayout INTERFACE)
target_include_directories(rlayout INTERFACE ${rlayout_SOURCE_DIR})
