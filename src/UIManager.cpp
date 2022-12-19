#include "UIManager.h"
using namespace UIConstants;

UIManager::UIManager() {
    window = NULL;
}

UIManager::UIManager(GLFWwindow* w, int width, int height) {
    window = w;
    windowHeight = height;
    windowWidth = width;

    clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    // Setup Dear ImGui context
    //glfwMakeContextCurrent(window);
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();
    ImGui_ImplGlfw_InitForOpenGL(w, true);
    ImGui_ImplOpenGL3_Init();
    ImGui_ImplOpenGL3_NewFrame();
}

void UIManager::setWindow(GLFWwindow* w) {
    window = w;
}

UIManager::~UIManager() {
}

void UIManager::render() {
    // Start the Dear ImGui frame
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    //select and show the ui components
    ImGui::ShowDemoWindow();
    ImVec2 menuSize = showTopMenu();

    ImGui::SetNextWindowPos(ImVec2(0, menuSize.y));
    ImVec2 resourceDockSize = showResourceDock();

    ImGui::SetNextWindowPos(ImVec2(windowWidth - rightPaneWidth, menuSize.y));
    ImVec2 sceneEditorSize = showSceneEditor();

    ImGui::SetNextWindowPos(ImVec2(windowWidth - rightPaneWidth, menuSize.y + sceneEditorHeight));
    ImVec2 instancePropertiesSize = showInstanceProperties();
    //

    // Rendering
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void UIManager::updateWindowSize(int w, int h) {
    windowHeight = h;
    windowWidth = w;
}

ImVec2 UIManager::showTopMenu() {
    ImGui::BeginMainMenuBar();
    ImVec2 size = ImGui::GetWindowSize();
    //
    ImGui::SetWindowFontScale(1.4);
    ImGui::MenuItem("File", NULL);
    ImGui::Spacing();
    ImGui::MenuItem("Edit", NULL);
    ImGui::Spacing();
    ImGui::MenuItem("Project", NULL);
    //
    ImGui::EndMainMenuBar();
    return size;
}

ImVec2 UIManager::showResourceDock() {
    ImGui::Begin("Resources", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);
    ImGui::SetWindowSize(ImVec2(leftPaneWidth, resourceDockHeight));
    ImVec2 size = ImGui::GetWindowSize();

    //loading dock
    ImGui::PushItemWidth(180.f);
    char buf[100] = "";
    if (ImGui::InputText("new resource", buf, sizeof(buf), ImGuiInputTextFlags_EnterReturnsTrue)) {
        std::string path("Assets/");
        std::string fileName(buf);
        ResourceManager::loadObject((path+fileName).c_str());
    }
    
    ImGui::Separator();
    
    //display loaded resources list, and allow creation of instances
    static bool displayResources = true;
    ImGui::SetNextItemOpen(displayResources);
    if (displayResources = ImGui::CollapsingHeader("Loaded Resources")) {
        //loop through all the loaded objects
        std::list<Object*>::iterator it = ResourceManager::objList.begin(); // for each obj file loaded
        while (it != ResourceManager::objList.end()) {
            if (ImGui::TreeNode((*it)->objFileName.c_str())) {
                //ImGui::Text((*it)->objFileName.c_str());
                //ImGui::SameLine();
                //ImGui::PushID((*it)->objFileName.c_str());
                if (ImGui::Button("Create Instance")) { //adding a new instance of an object
                    Scene* scene = EngineApp::getCurrentScene();
                    if (scene != NULL) {
                        scene->addInstance(new Instance((*it)));
                        std::cout << (*it)->objFileName << std::endl;
                    }
                }

                std::vector<Mesh*>::iterator meshIt = (*it)->meshList.begin();
                while (meshIt != (*it)->meshList.end()) {
                    if (ImGui::TreeNode((*meshIt)->meshName.c_str())) {
                        //std::cout << "ambX " << (*meshIt)->material->ambient.x << std::endl;
                        //std::cout << "ambY " << (*meshIt)->material->ambient.y << std::endl;
                        //std::cout << "ambZ " << (*meshIt)->material->ambient.z << std::endl;
                        ImGui::SliderFloat("Amb R", &((*meshIt)->material->ambient.x), 0.0f, 1.0f, "R = %.2f");
                        ImGui::SliderFloat("Amb G", &((*meshIt)->material->ambient.y), 0.0f, 1.0f, "G = %.2f");
                        ImGui::SliderFloat("Amb B", &((*meshIt)->material->ambient.z), 0.0f, 1.0f, "B = %.2f");
                        ImGui::SliderFloat("Diff R", &((*meshIt)->material->diffuse.x), 0.0f, 1.0f, "R = %.2f");
                        ImGui::SliderFloat("Diff G", &((*meshIt)->material->diffuse.y), 0.0f, 1.0f, "G = %.2f");
                        ImGui::SliderFloat("Diff B", &((*meshIt)->material->diffuse.z), 0.0f, 1.0f, "B = %.2f");
                        ImGui::SliderFloat("Spec R", &((*meshIt)->material->specular.x), 0.0f, 1.0f, "R = %.2f");
                        ImGui::SliderFloat("Spec G", &((*meshIt)->material->specular.y), 0.0f, 1.0f, "G = %.2f");
                        ImGui::SliderFloat("Spec B", &((*meshIt)->material->specular.z), 0.0f, 1.0f, "B = %.2f");
                        ImGui::InputFloat("Spec Focus", &((*meshIt)->material->specularFocus));
                        
                        ImGui::TreePop();
                    }
                    meshIt++;
                }

                //ImGui::PopID();
                ImGui::TreePop();
            }

            it++;
        }
    }

    ImGui::End();
    return size;
}

ImVec2 UIManager::showSceneEditor() {
    ImGui::Begin("Scene Editor", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);
    ImGui::SetWindowSize(ImVec2(rightPaneWidth, sceneEditorHeight));
    ImVec2 size = ImGui::GetWindowSize();

    static bool displaySceneObjects = true;
    ImGui::SetNextItemOpen(displaySceneObjects);
    if (displaySceneObjects = ImGui::CollapsingHeader("Scene Instances")) {
        //loop through all the instantiated objects
        std::vector<Instance*> sceneInstanceList = EngineApp::getCurrentScene()->getInstanceList();
        std::vector<Instance*>::iterator it = sceneInstanceList.begin();
        while (it != sceneInstanceList.end()) {
            if (ImGui::TreeNode((*it)->instanceName.c_str())) { // list the unique instance color id
                ImGui::PushID((*it)->instanceName.c_str());
                
                ImGui::SameLine();
                ImGui::Button("delete");
                
                ImGui::PopID();
                ImGui::TreePop();
            }

            it++;
        }
    }

    ImGui::End();
    return size;
}

ImVec2 UIManager::showInstanceProperties() {
    ImGui::Begin("Instance Properties", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);
    ImGui::SetWindowSize(ImVec2(rightPaneWidth, windowHeight-sceneEditorHeight));
    ImVec2 size = ImGui::GetWindowSize();

    ImGui::End();
    return size;
}


