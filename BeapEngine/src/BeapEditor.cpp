#include "BeapEditor.h"

beap::editor::BeapEditor::BeapEditor(GLFWwindow* window, beap::Scene* scene)
{
    beap::editor::BeapEditor::window = window;
    beap::editor::BeapEditor::scene = scene;
}

void beap::editor::BeapEditor::Init()
{

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330 core");
}

void beap::editor::BeapEditor::PreRender()
{
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void beap::editor::BeapEditor::EditorLoop(unsigned int fboTexture)
{

   

    ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
    const ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->WorkPos);
    ImGui::SetNextWindowSize(viewport->WorkSize);
    ImGui::SetNextWindowViewport(viewport->ID);
    window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
    window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;


    ImGui::Begin("DockSpace Demo", nullptr, window_flags);

    ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
    ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);

    ImGui::End();


    ImGui::Begin("Scene");
    ImVec2 pos = ImGui::GetCursorScreenPos();

    int window_width, window_height;
    glfwGetWindowSize(window, &window_width, &window_height);

    ImGui::Image((void*)fboTexture, ImVec2(ImGui::GetWindowSize().x, ImGui::GetWindowSize().y), ImVec2(0, 1), ImVec2(1, 0));

    ImGui::End();


    ImGui::ShowDemoWindow();

    ImGui::Begin("Editor");
    ImGui::Text("Editor Test");
    if (ImGui::Button("Test"))
    {
        
    }
    for (int i = 0; i < scene->GetChildren().size(); i++)
    {
        if (scene->GetChildren()[i]->Contents->GetParent() == scene)
        {
            if (ImGui::Selectable(scene->GetChildren()[i]->Contents->name.c_str(), false))
            {
                std::cout << "a" << std::endl;
                selectedInstance = scene->GetChildren()[i]->Contents;
            }
        }

        scene->GetChildren()[i]->Contents->in_tree->Traverse([](Node<Instance*>* mynode)
        {
            ImGui::Text((" - " + mynode->Contents->name).c_str());
        });
        
    }
    ImGui::End();

    ImGui::Begin("Inspector");
    if (selectedInstance != nullptr)
    {
        GameObject* gObj = dynamic_cast<GameObject*>(selectedInstance);
        if (gObj != nullptr)
        {
            float pos[3]{ gObj->Position.x, gObj->Position.y, gObj->Position.z };
            if (ImGui::DragFloat3("Translation", pos, 0.01f))
            {
                gObj->SetPosition(glm::vec3(pos[0], pos[1], pos[2]));
            }

            float rot[3]{ gObj->Rotation.x, gObj->Rotation.y, gObj->Rotation.z };
            if (ImGui::DragFloat3("Rotation", rot, 0.1f))
            {
                gObj->SetRotation(glm::vec3(rot[0], rot[1], rot[2]));
            }

            float scl[3]{ gObj->Scale.x, gObj->Scale.y, gObj->Scale.z };
            if (ImGui::DragFloat3("Scale", scl, 0.01f))
            {
                gObj->SetScale(glm::vec3(scl[0], scl[1], scl[2]));
            }
        }
        

        if (selectedInstance->InstanceType() == "light")
        {
            ImGui::Text("Light Editor");
            static int lightType = 0;

            // Do some magic C++ cast
            beap::renderer::Light* light = dynamic_cast<beap::renderer::Light*>(selectedInstance);

            if (ImGui::Combo("Type", &lightType, "Directional\0Spot\0Point\0"))
            {
                

                switch (lightType)
                {
                    case 0:
                        light->type = beap::renderer::ELightType::DIRECTIONAL;
                        break;
                    case 1:
                        light->type = beap::renderer::ELightType::SPOT;
                        break;
                    case 2:
                        light->type = beap::renderer::ELightType::POINT;
                        break;
                    default:
                        break;
                }
            }

            float col[4]{ light->data.lightColor.x, light->data.lightColor.y, light->data.lightColor.z, light->data.lightColor.w};
            if (ImGui::ColorEdit4("Light Color", col))
            {
                light->data.lightColor = glm::vec4(col[0], col[1], col[2], col[3]);
            }
        }
    }
    ImGui::End();

    RenderShaderManagerEditor();
}

void beap::editor::BeapEditor::RenderShaderManagerEditor()
{
    // Create a shader manager if it doesn't exist.
    if (manager == nullptr)
    {
        ShaderManager m;
        manager = &m;
    }

    ImGui::Begin("Shader Manager Shader Editor");
    ImGui::InputText("GLSL Header", settings.GLSLHeader, 30);
    ImGui::Checkbox("hasTexture", &settings.hasTexture);
    ImGui::Checkbox("hasLighting", &settings.hasLighting);
    if (settings.hasLighting)
    {
        ImGui::BeginListBox("Lighting Settings");
            ImGui::Checkbox("useAmbientLighting", &settings.useAmbientLighting);
            ImGui::Checkbox("isBlinnPhong", &settings.isBlinnPhong);
            ImGui::Checkbox("hasDiffuse", &settings.hasDiffuse);
            ImGui::Checkbox("hasSpecular", &settings.hasSpecular);
            ImGui::Checkbox("hasShadows", &settings.hasShadows);
            ImGui::Checkbox("hasPointShadows", &settings.hasPointShadows);
        ImGui::EndListBox();
    }
    ImGui::Checkbox("hasNormalMaps", &settings.hasNormalMaps);
    ImGui::Checkbox("hasParallaxOcclusionMaps", &settings.hasParallaxOcclusionMaps);

    if (ImGui::Button("Compile Shader"))
    {
        beap::shaders::generation::FShaderGenerationResult result;
        result = manager->GenerateFragmentShader(settings);
        result.Success = true;
        manager->cachedResult = result;
    }

    ImGui::End();
}

void beap::editor::BeapEditor::Render()
{
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void beap::editor::BeapEditor::CleanUp()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}
