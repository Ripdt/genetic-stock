#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <algorithm>
#include <random>
#include <set>
#include "DNA.h"

std::vector<std::string> split(const std::string& s, const char delimiter) {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(s);
    while (std::getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

std::vector<std::vector<std::pair<std::string, double>>> readCotacoes(const std::string& filename, std::vector<std::string>& codigosUnicos) {
    std::ifstream file(filename);
    std::string line;
    std::vector<std::vector<std::pair<std::string, double>>> dias;
    std::map<std::string, bool> visto;
    std::vector<std::pair<std::string, double>> diaAtual;
    std::string ultimaData;

    while (std::getline(file, line)) {
        auto partes = split(line, ';');
        if (partes.size() != 3) continue;

        std::string data = partes[0];
        std::string codigo = partes[1];
        std::string precoStr = partes[2];
        std::replace(precoStr.begin(), precoStr.end(), ',', '.');
        double preco = std::stod(precoStr);

        if (codigo.length() != 5) continue;

        if (data != ultimaData && !diaAtual.empty()) {
            dias.push_back(diaAtual);
            diaAtual.clear();
        }

        if (visto.find(codigo) == visto.end()) {
            codigosUnicos.push_back(codigo);
            visto[codigo] = true;
        }

        diaAtual.emplace_back(codigo, preco);
        ultimaData = data;
    }

    if (!diaAtual.empty()) dias.push_back(diaAtual);
    return dias;
}

bool compararFitness(const DNA& a, const DNA& b) {
    return a.fitness > b.fitness;
}

int main() {
    std::vector<std::string> acoesDisponiveis;
    auto cotacoes = readCotacoes("../res/cotacoes_b3_202_05.csv", acoesDisponiveis);
    int numDias = cotacoes.size() / 2;

    int populacaoSize = 100;
    double taxaMutacao = 0.05;
    int geracoes = 200;

    std::vector<DNA> populacao;

    // População inicial
    for (int i = 0; i < populacaoSize; ++i) {
        DNA individuo(numDias, acoesDisponiveis);
        individuo.calcularFitness(cotacoes);
        populacao.push_back(individuo);
    }

    for (int gen = 0; gen < geracoes; ++gen) {
        std::sort(populacao.begin(), populacao.end(), compararFitness);
        std::vector<DNA> novaPop;

        for (int i = 0; i < populacaoSize; ++i) {
            DNA pai = populacao[rand() % (populacaoSize / 2)];
            DNA mae = populacao[rand() % (populacaoSize / 2)];
            DNA filho = pai.crossover(mae);
            filho.mutar(taxaMutacao, acoesDisponiveis);
            filho.calcularFitness(cotacoes);
            novaPop.push_back(filho);
        }

        populacao = novaPop;
        std::cout << "Geração " << gen + 1 << " | Melhor Fitness: R$" << populacao[0].fitness << std::endl;
    }

    std::cout << "\nMelhor estratégia final resultou em: R$" << populacao[0].fitness << std::endl;
    return 0;
}
