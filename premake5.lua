workspace "Dragonite Engine"
location "."
architecture "x64"
startproject "launcher"

configurations {
    "Debug",
    "Release",
    "Retail"
}

include "launcher"
include "vendor"