#include "bootstrap.hpp"
#include <thread>

__attribute__((constructor)) void on_load() { std::thread(BootStrap::Run).detach(); }

__attribute__((destructor)) void on_unload() { BootStrap::Shutdown(); }
