#pragma once

const unsigned int Graphical_Updates_Per_Second = 60;
const unsigned int Physical_Updates_Per_Second = 60;

const unsigned int Screen_Width = 1280u;
const unsigned int Screen_Height = 1024u;
const bool Fullscreen_Window = false;

#include <string>
const std::string Additional_Object_Storage_Path = "/ObjectStorage/;/Objects/;/Maps/";
const std::string Server_IP = "127.0.0.1";

#include "../../Shared/KeyLayout.hpp"
const KeyLayout Keys_Layout = KeyLayout(41, 37, 19, 22, 36, 51);