#pragma once
#include "Model.h"
#include "UIManager.h"
#include "NetworkManager.h"

class App {
private:
    Model& model;
    UIManager& uiManager;
    NetworkManager& networkManager;

public:
    App(Model& m, UIManager& ui, NetworkManager& net)
        : model(m), uiManager(ui), networkManager(net) {}
    
    void begin(); // Initialize everything
    void loop();  // Main app loop
};