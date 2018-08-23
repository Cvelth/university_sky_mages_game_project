#pragma once
#include <istream>
#include <memory>
#include <map>
class Map;
class MainActorQueue;
class Objects;
using Clients = std::map<std::pair<std::string, size_t>, size_t>;

void map_(std::shared_ptr<Map> &map, std::istream &input);
void actors_(MainActorQueue &actors, std::istream &input);
void clients_(Clients &clients, std::istream &input);
void help_();
void exit_(bool &server_should_close);

void actors_add(std::shared_ptr<Objects> objects, MainActorQueue &actors);
void map_broadcast(std::shared_ptr<Map> &map);

void server_process(std::shared_ptr<Objects> objects);