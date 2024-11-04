#include "raylib.h"
#include "raymath.h"
#include "imgui.h"
#include "rlImGui.h"
#include "rlImGuiColors.h"
#include "implot.h"
#include <stdio.h>
#include "window.h"
#include <iostream>

int main()
{
    IniciarInterface(); // Inicializa a interface

    // Verifique se a janela foi inicializada corretamente
    if (IsWindowReady()) {
        loopPrograma(); // Inicia o loop do programa
    } else {
        std::cerr << "Falha ao inicializar a janela." << std::endl;
        return -1; // Retorna um código de erro
    }

    return 0;
}
