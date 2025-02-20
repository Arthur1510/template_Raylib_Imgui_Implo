#ifndef POLIGONO_H
#define POLIGONO_H

#include <vector>
#include <set>
#include <cmath>

#define _USE_MATH_DEFINES

// Definição da estrutura de ponto
struct Point {
    float x;
    float y;

    Point(); // Construtor padrão
    Point(float x_val, float y_val); // Construtor com parâmetros

    // Operador de comparação para que o std::set funcione
    bool operator<(const Point& other) const;
};

// Definição da classe Poligono
class Poligono {
public:
    std::vector<Point> vertices;
    std::vector<Point> resultadoCorte;
    std::vector<Point> areaSuperior;
    std::vector<Point> areaInferior;
    std::vector<Point> verticesTransladados;
    std::vector<Point> verticesRotacionados;

    void setVertices(const std::vector<Point>& points);
    double area() const;
    Point centroide() const;
    void translacaoCG(const std::vector<Point>& points);
    void rotacao(double angulo);
    void MaxMin(float& yMin, float& yMax) const;
    int verificarCaso(const Point& p1, const Point& p2, double cortar);
    Point calcularIntersecao(const Point& p1, const Point& p2, double cortar);
    void cortarPoligonal(const std::vector<Point>& verticesTransladados, float& cortar);
    void fecharPoligono(std::vector<Point> &pontos);
};

#endif // POLIGONO_H
