#ifndef DNA_H
#define DNA_H

#include <vector>
#include <string>
#include <unordered_map>
#include <random>

class DNA {
public:
    DNA(int dias, const std::vector<std::string>& acoesDisponiveis);
    
    std::vector<std::string> genes;
    double fitness = 0.0;

    void calcularFitness(
      const std::vector<std::vector<std::pair<std::string, double>>>& cotacoes, 
      const double montanteInicial,
      const double numeroPotes
    );
    DNA crossover(const DNA& parceiro) const;
    void mutar(double taxaMutacao, const std::vector<std::string>& acoesDisponiveis);

    int getDias() const;

private:
    int dias;
    std::mt19937 rng; // gerador de n�meros aleat�rios https://cplusplus.com/reference/random/mt19937/
};

#endif
