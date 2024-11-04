#include "raylib.h"
#include "raymath.h"
#include "imgui.h"
#include "rlImGui.h"
#include "rlImGuiColors.h"
#include "implot.h"
#include "poligono.h"
#include "window.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <stdio.h>
#include <set>

std::vector<Point> collectedPoints = {
    {0, 190}, {0, 178}, {50, 170}, {50, 45}, {25, 25}, {25, 0}, {95, 0}, {95, 25}, {70, 45}, {70, 170}, {120, 178}, {120, 190}
};

Poligono poligono;

// Funções de inicialização da interface
void IniciarInterface()
{
    const int screenWidth = 1280;
    const int screenHeight = 720;
    InitWindow(screenWidth, screenHeight, "Software de cálculo de esforços em seções de concreto armado");
    SetTargetFPS(60);
    rlImGuiSetup(true);
    ImPlot::CreateContext();
}

void loopPrograma()
{
    float KeyDownDelay = 0.0f;
    float KeyDownDelayTime = 0.1f;
    static float VLN = 0;
    static float cortar = 0;
    static bool showGraficoWindow = true;
    static bool showDadosWindowTwo = true;
    
    while (!WindowShouldClose())
    {
        DrawFPS(20, 20);
        KeyDownDelay = KeyDownDelay + GetFrameTime();

        BeginDrawing();
        ClearBackground(BLACK);
        rlImGuiBegin();

        if (showDadosWindowTwo)
        {
            ImGui::Begin("Central de operações com polígono", &showDadosWindowTwo);
            ImGui::Text("Insira a coordenada de corte");

            // cortar.clear(); // Limpa o vetor de cortes antes de adicionar novos valores

            // Declare VLN fora do loop, se ainda não estiver declarado
            // Usar static para manter o valor entre as chamadas
            ImGui::SetNextItemWidth(100);
            ImGui::InputFloat("Y", &VLN); // Permite ao usuário inserir a coordenada Y

            // Adiciona a coordenada Y ao vetor de cortes
            if (ImGui::Button("Adicionar Corte"))
            { // Botão para adicionar o corte
                cortar = VLN;
            }

            if (ImGui::Button("Mostrar Valores"))
            {

                TraceLog(LOG_INFO, "Valores armazenados:");
                for (const auto &point : collectedPoints)
                {
                    TraceLog(LOG_INFO, "x = %.2f, y = %.2f", point.x, point.y);
                }

                TraceLog(LOG_INFO, "Valores após o corte");
                for (const auto &point : poligono.resultadoCorte)
                {
                    TraceLog(LOG_INFO, "x = %.2f, y = %.2f", point.x, point.y);
                }

                TraceLog(LOG_INFO, "Vertices superiores");
                for (const auto &point : poligono.areaSuperior)
                {
                    TraceLog(LOG_INFO, "x = %.2f, y = %.2f", point.x, point.y);
                }

                TraceLog(LOG_INFO, "Vertices inferiores");
                for (const auto &point : poligono.areaInferior)
                {
                    TraceLog(LOG_INFO, "x = %.2f, y = %.2f", point.x, point.y);
                }
            }

            if (ImGui::Button("Calcular Área e Centróide"))
            {
                poligono.setVertices(collectedPoints); // Transfere os pontos para o polígono
                double poligonoArea = poligono.area();  // Calcula a área
                Point centroid = poligono.centroide();  // Calcula o centróide

                TraceLog(LOG_INFO, "Área: %.2f", poligonoArea);
                TraceLog(LOG_INFO, "Centróide: (%.2f, %.2f)", centroid.x, centroid.y);
            }

            ImGui::End();
        }

        if (showGraficoWindow)
        {
            std::cout << "Desenhando a janela do gráfico..." << std::endl;
            ImGui::Begin("Gráfico da Seção Transversal", &showGraficoWindow); // Título da janela

            // Adicione logs para verificar o conteúdo de collectedPoints
            std::cout << "Número de pontos coletados: " << collectedPoints.size() << std::endl;
            for (const auto& point : collectedPoints) {
                std::cout << "Ponto: (" << point.x << ", " << point.y << ")" << std::endl;
            }

            static float x_values[2]; // teste
            static float y_values[2];

            float valorMenor = FLT_MAX;  // Número de ponto flutuante representável máximo.
            float valorMaior = -FLT_MAX; // Número de ponto flutuante representável máximo.

            for (int i = 0; i < sizeof(x_values); i++)
            {
                y_values[i] = VLN;
            }

            if (IsKeyPressed(KEY_UP))
            {
                VLN = VLN + 1;
                cortar = VLN;
                poligono.setVertices(collectedPoints);
                poligono.cortarPoligonal(poligono.vertices, cortar);

                KeyDownDelay = 0.0f;
            }
            if (IsKeyPressed(KEY_DOWN))
            {
                VLN = VLN - 1;

                cortar = VLN;
                poligono.setVertices(collectedPoints);
                poligono.cortarPoligonal(poligono.vertices, cortar);

                KeyDownDelay = 0.0f;
            }

            if (KeyDownDelay >= KeyDownDelayTime)
            {
                if (IsKeyDown(KEY_UP))
                {

                    VLN = VLN + 1;
                    cortar = VLN;
                    poligono.setVertices(collectedPoints);
                    poligono.cortarPoligonal(poligono.vertices, cortar);

                    KeyDownDelay = 0.0f;
                }
                if (IsKeyDown(KEY_DOWN))
                {
                    VLN = VLN + -1;
                    cortar = VLN;
                    poligono.setVertices(collectedPoints);
                    poligono.cortarPoligonal(poligono.vertices, cortar);

                    KeyDownDelay = 0.0f;
                }
            }

            int numPoints = collectedPoints.size();

            for (int i = 0; i < numPoints; i++)
            {
                if (collectedPoints[i].x > valorMaior)
                {
                    valorMaior = collectedPoints[i].x;
                }

                if (collectedPoints[i].x < valorMenor)
                {
                    valorMenor = collectedPoints[i].x;
                }
            }

            x_values[0] = valorMenor - (valorMaior - valorMenor) * 0.1;
            x_values[1] = valorMaior + (valorMaior - valorMenor) * 0.1;

            // Ajuste o código de desenho no gráfico
            if (numPoints >= 3)
            {
                // Fechar os vetores adicionando o primeiro ponto ao final
                std::vector<Point> collectedPointsFechados = collectedPoints;
                poligono.fecharPoligono(collectedPointsFechados);
                std::vector<Point> resultadoCorteFechado = poligono.resultadoCorte;
                poligono.fecharPoligono(resultadoCorteFechado);
                std::vector<Point> AreaSuperiorFechado = poligono.areaSuperior;
                poligono.fecharPoligono(AreaSuperiorFechado);
                std::vector<Point> AreaInferiorFechado = poligono.areaInferior;
                poligono.fecharPoligono(AreaInferiorFechado);

                // Converter para arrays de float para os gráficos
                std::vector<float> x_data;
                std::vector<float> y_data;
                std::vector<float> x_corte;
                std::vector<float> y_corte;
                std::vector<float> x_superior;
                std::vector<float> y_superior;
                std::vector<float> x_inferior;
                std::vector<float> y_inferior;

                x_data.resize(collectedPointsFechados.size());
                y_data.resize(collectedPointsFechados.size());
                x_corte.resize(resultadoCorteFechado.size());
                y_corte.resize(resultadoCorteFechado.size());
                x_superior.resize(AreaSuperiorFechado.size());
                y_superior.resize(AreaSuperiorFechado.size());
                x_inferior.resize(AreaInferiorFechado.size());
                y_inferior.resize(AreaInferiorFechado.size());
                
                for (size_t i = 0; i < collectedPointsFechados.size(); i++)
                {
                    x_data[i] = collectedPointsFechados[i].x;
                    y_data[i] = collectedPointsFechados[i].y;
                }

                for (size_t i = 0; i < resultadoCorteFechado.size(); i++)
                {
                    x_corte[i] = resultadoCorteFechado[i].x;
                    y_corte[i] = resultadoCorteFechado[i].y;
                }

                for (size_t i = 0; i < AreaSuperiorFechado.size(); i++)
                {
                    x_superior[i] = AreaSuperiorFechado[i].x;
                    y_superior[i] = AreaSuperiorFechado[i].y;
                }

                for (size_t i = 0; i < AreaInferiorFechado.size(); i++)
                {
                    x_inferior[i] = AreaInferiorFechado[i].x;
                    y_inferior[i] = AreaInferiorFechado[i].y;
                }

                // Obter o tamanho disponível para o gráfico
                ImVec2 plotSize = ImGui::GetContentRegionAvail();

                // Plota os pontos e desenha o polígono
                if (ImPlot::BeginPlot("Gráfico", ImVec2(plotSize.x, plotSize.y)))
                {
                    ImPlot::PlotScatter("Vértices", x_data.data(), y_data.data(), (int)collectedPointsFechados.size());
                    ImPlot::PlotScatter("Vértices cortadas", x_corte.data(), y_corte.data(), (int)resultadoCorteFechado.size());
                    ImPlot::PlotScatter("Vértices superiores", x_superior.data(), y_superior.data(), (int)AreaSuperiorFechado.size());
                    ImPlot::PlotScatter("Vértices inferiores", x_inferior.data(), y_inferior.data(), (int)AreaInferiorFechado.size());
                    ImPlot::PlotLine("Polígono", x_data.data(), y_data.data(), (int)collectedPointsFechados.size());
                    ImPlot::PlotLine("Polígono cortado", x_corte.data(), y_corte.data(), (int)resultadoCorteFechado.size());
                    ImPlot::PlotLine("Polígono superior", x_superior.data(), y_superior.data(), (int)AreaSuperiorFechado.size());
                    ImPlot::PlotLine("Polígono inferior", x_inferior.data(), y_inferior.data(), (int)AreaInferiorFechado.size());
                    ImPlot::PlotLine("Linha de corte", x_values, y_values, 2);
                    ImPlot::EndPlot();
                }
            }
            else
            {
                ImGui::Text("Insira pelo menos 3 vértices para formar um polígono.");
            }

            ImGui::End(); // Finaliza a janela do gráfico
        }

        // Verifique se os vetores têm elementos antes de acessá-los
        if (!collectedPoints.empty()) {
            std::cout << "Número de pontos coletados: " << collectedPoints.size() << std::endl;
        } else {
            std::cout << "Nenhum ponto coletado!" << std::endl;
        }

        rlImGuiEnd();
        EndDrawing();
    }

    // Adicione uma pausa antes de fechar a janela
    std::cout << "Pressione Enter para sair..." << std::endl;
    std::cin.get(); // Espera que o usuário pressione Enter

    // Finaliza o contexto
    ImPlot::DestroyContext();
    rlImGuiShutdown();
}