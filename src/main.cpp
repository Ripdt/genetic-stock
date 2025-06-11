#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <algorithm>
#include <random>
#include <set>
#include <iomanip> // Para std::setprecision
#include <locale>
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
    std::getline(file, line); // legenda
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

int main() 
{
    std::setlocale(LC_ALL, ".UTF8");

    std::cout << "================== INÍCIO DA SIMULAÇÃO ==================\n";
    std::vector<std::string> acoesDisponiveis;
    std::vector<double> historicoFitness;
    const auto cotacoes = readCotacoes("../res/cotacoes_b3_202_05.csv", acoesDisponiveis);
    const double montanteInicial = 1000; // 1000 reais
    const double numPotes = 10; // 10 ações
    const size_t numDias = cotacoes.size() / 2;

    const int populacaoSize = 100;
    const double taxaMutacao = 0.05;
    const int geracoes = 200;

    std::vector<DNA> populacao;

    // População inicial
    for (int i = 0; i < populacaoSize; ++i) {
        DNA individuo(numDias, acoesDisponiveis);
        individuo.calcularFitness(cotacoes, montanteInicial, numPotes);
        populacao.push_back(individuo);
    }

    for (int gen = 0; gen < geracoes; ++gen) {
      std::vector<DNA> novaPop;
      std::sort(populacao.begin(), populacao.end(), compararFitness);

      for (int i = 0; i < populacaoSize; ++i) {
        DNA pai = populacao[rand() % (populacaoSize / 2)];
        DNA mae = populacao[rand() % (populacaoSize / 2)];
        DNA filho = pai.crossover(mae);
        filho.mutar(taxaMutacao, acoesDisponiveis);
        filho.calcularFitness(cotacoes, montanteInicial, numPotes);
        novaPop.push_back(filho);
      }

      populacao = novaPop;
      historicoFitness.push_back(populacao[0].fitness);
      std::cout << "Geração " << gen + 1 << " | Melhor Fitness: R$"
        << std::fixed << std::setprecision(2) << populacao[0].fitness << std::endl;
    }

    std::sort(populacao.begin(), populacao.end(), compararFitness);

    const double valorFinal = populacao[0].fitness;
    const double percentual = 100.0 * (valorFinal - montanteInicial) / montanteInicial;

    std::cout << "\n================== RELATÓRIO FINAL ==================\n";
    std::cout << "Valor inicial: R$" << std::fixed << std::setprecision(2) << montanteInicial << std::endl;
    std::cout << "Valor final:   R$" << std::fixed << std::setprecision(2) << valorFinal << std::endl;
    std::cout << "Valorização:   " << std::fixed << std::setprecision(2) << percentual << "%\n";
    std::cout << "Parâmetros: " << numPotes << " potes, " << geracoes << " gerações, "
      << "população " << populacaoSize << ", taxa de mutação " << taxaMutacao << std::endl;

    std::cout << "\nEvolução do melhor fitness por geração:\n";
    for (size_t i = 0; i < historicoFitness.size(); ++i) {
      if ((i + 1) % 10 == 0 || i == historicoFitness.size() - 1)
        std::cout << "Geração " << i + 1 << ": R$" << std::fixed << std::setprecision(2) << historicoFitness[i] << std::endl;
    }

    std::cout << "\nEstratégia do melhor DNA (ações escolhidas por pote a cada rodada):\n";
    for (size_t rodada = 0; rodada < populacao[0].getDias(); ++rodada) {
      std::cout << "Rodada " << rodada + 1 << ": ";
      for (int pote = 0; pote < numPotes; ++pote) {
        size_t idx = rodada * numPotes + pote;
        if (idx < populacao[0].genes.size())
          std::cout << populacao[0].genes[idx] << (pote < numPotes - 1 ? ", " : "");
      }
      std::cout << std::endl;
    }

    std::map<std::string, int> contagemAcoes;
    for (const auto& acao : populacao[0].genes) {
      contagemAcoes[acao]++;
    }
    std::cout << "\nAções mais escolhidas na estratégia vencedora:\n";
    std::vector<std::pair<std::string, int>> rankingAcoes(contagemAcoes.begin(), contagemAcoes.end());
    std::sort(rankingAcoes.begin(), rankingAcoes.end(), [](auto& a, auto& b) { return a.second > b.second; });
    for (const auto& par : rankingAcoes) {
      std::cout << par.first << ": " << par.second << " vezes\n";
    }
    std::cout << "=====================================================\n";
    return 0;
}
