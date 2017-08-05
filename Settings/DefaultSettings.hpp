#pragma once
#include <string>
#include "Settings\KeyLayout.hpp"
const std::string Program_Name = "SkyMages";
const unsigned int Program_Major_Version = 0u;
const unsigned int Program_Minor_Version = 0u;
const unsigned int Program_Build_Version = 1u;
const std::string Program_Version_Suffix = "dev";
const unsigned int Settings_Syntax_Major_Version = 0u;
const unsigned int Settings_Syntax_Minor_Version = 1u;

const unsigned int Graphical_Updates_Per_Second = 60;
const unsigned int Physical_Updates_Per_Second = 60;

const unsigned int Screen_Width = 1280u; 
const unsigned int Screen_Height = 1024u;
const bool Fullscreen_Window = false;

const KeyLayout Keys_Layout = KeyLayout(41, 37, 19, 22);