#include "raylib.h"
#include "imgui.h"
#include "rlImGui.h"
#include "implot.h"
#include "poligono.h"
#include <iostream>
#include <vector>

int main()
{
    // Inicializa a janela
    const int screenWidth = 1280;
    const int screenHeight = 720;
    InitWindow(screenWidth, screenHeight, "Exemplo de Polígono com Raylib e ImGui");
    SetTargetFPS(60);
    rlImGuiSetup(true);
    ImPlot::CreateContext();

    Poligono poligono;
    std::vector<Point> collectedPoints;
    float x = 0.0f, y = 0.0f; // Coordenadas do ponto a ser adicionado
    bool showGraficoWindow = true;

    while (!WindowShouldClose())
    {
        // Início do desenho
        BeginDrawing();
        ClearBackground(RAYWHITE);

        rlImGuiBegin();

        // Janela para adicionar pontos
        ImGui::Begin("Adicionar Ponto");
        ImGui::InputFloat("X", &x);
        ImGui::InputFloat("Y", &y);
        if (ImGui::Button("Adicionar Ponto"))
        {
            collectedPoints.emplace_back(x, y);
            poligono.setVertices(collectedPoints); // Atualiza os vértices do polígono
        }
        ImGui::End();

        // Janela do gráfico
        if (showGraficoWindow)
        {
            ImGui::Begin("Gráfico do Polígono", &showGraficoWindow);
            if (collectedPoints.size() >= 3)
            {
                // Fechar o polígono
                std::vector<Point> collectedPointsFechados = collectedPoints;
                poligono.fecharPoligono(collectedPointsFechados);

                // Converter para arrays de float para os gráficos
                std::vector<float> x_data(collectedPointsFechados.size());
                std::vector<float> y_data(collectedPointsFechados.size());

                for (size_t i = 0; i < collectedPointsFechados.size(); i++)
                {
                    x_data[i] = collectedPointsFechados[i].x;
                    y_data[i] = collectedPointsFechados[i].y;
                }

                // Plota o polígono
                if (ImPlot::BeginPlot("Gráfico do Polígono"))
                {
                    ImPlot::PlotLine("Polígono", x_data.data(), y_data.data(), (int)collectedPointsFechados.size());
                    ImPlot::EndPlot();
                }
            }
            else
            {
                ImGui::Text("Insira pelo menos 3 vértices para formar um polígono.");
            }
            ImGui::End();
        }

        rlImGuiEnd();
        EndDrawing();
    }

    // Finaliza o contexto
    ImPlot::DestroyContext();
    rlImGuiShutdown();
    CloseWindow(); // Fecha a janela

    return 0;
}