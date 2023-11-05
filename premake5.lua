workspace "hash"
    configurations {"Debug", "Release"}
    location "build"

project "hash"
    kind "ConsoleApp"
    language "C++"
    architecture "x64"
    cppdialect "c++latest"
    staticruntime "On"
    targetdir "bin/%{cfg.buildcfg}"

files {"src/main.cpp"}
includedirs {"include"}

filter "configurations:Debug"
    runtime "Debug"
    defines {"DEBUG"}
    symbols "On"

filter "configurations:Release"
    runtime "Release"
    defines {"NDEBUG"}
    optimize "On"

filter {}
----------------------------------------------------