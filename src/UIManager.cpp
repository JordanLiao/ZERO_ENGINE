#include "UIManager.h"

UIManager::UIManager() {}

UIManager::UIManager(GLFWwindow* w) {
    window = w;
    showDemo = false;
    showAnotherWindow = true;
    clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();
    ImGui_ImplGlfw_InitForOpenGL(w, true);

    ImGui_ImplOpenGL3_NewFrame();
}

UIManager::~UIManager() {
}

void UIManager::render() {
    // Start the Dear ImGui frame
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    //select and show the ui components
    showTopManu();

    // Rendering
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void UIManager::showTopManu() {
    ImGui::BeginMainMenuBar();
    ImGui::SetWindowFontScale(1.4);
    ImGui::MenuItem("File", NULL);
    ImGui::Spacing();
    ImGui::MenuItem("Edit", NULL);
    ImGui::Spacing();
    ImGui::MenuItem("Project", NULL);
    ImGui::EndMainMenuBar();
}
