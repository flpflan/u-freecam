#include "main.hpp"
#include <thread>

__attribute__((constructor)) void on_load() { std::thread(Main::run).detach(); }

__attribute__((destructor)) void on_unload() {}
