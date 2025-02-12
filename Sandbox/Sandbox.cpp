#include "Core/Application.h"
#include "Utils/Logger.h"

int main() {
    Logger::Init();  // Initialize loggers if needed

    Core::Application app;
    if (!app.Init()) {
        Logger::GetEngineLogger()->error("Application init failed!");
        return -1;
    }

    app.Run();
    app.Shutdown();
    return 0;
}
