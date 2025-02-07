#include "Core/Application.h"
#include "Utils/Logger.h"

Application::Application() {
    Logger::Init();
    LOG_ENGINE_INFO("Application Initialized");
}

void Application::Run() {
    LOG_ENGINE_INFO("Game Engine Running...");
}
