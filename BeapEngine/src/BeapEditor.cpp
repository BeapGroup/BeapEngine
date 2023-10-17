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
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void beap::editor::BeapEditor::EditorLoop()
{
    ImGui::Begin("Editor");
    ImGui::Text("Editor Test");
    if (ImGui::Button("Test"))
    {
        
    }
    for (int i = 0; i < scene->GetChildren().size(); i++)
    {
        if (scene->GetChildren()[i]->Contents->GetParent() == scene)
        {
            ImGui::Text(scene->GetChildren()[i]->Contents->name.c_str());

        }

        scene->GetChildren()[i]->Contents->in_tree->Traverse([](Node<Instance*>* mynode)
        {
            ImGui::Text((" - " + mynode->Contents->name).c_str());
        });
        
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
