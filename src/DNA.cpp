#include "DNA.h"
#include <algorithm>
#include <map>
#include <cmath>

DNA::DNA(
  int dias, 
  const std::vector<std::string>& acoesDisponiveis
) : 
  dias(dias), 
  rng(std::random_device{}()) 
{
  size_t nAcoes = acoesDisponiveis.size();
  if (nAcoes > 0) {
    std::uniform_int_distribution<> distrib(0, nAcoes - 1);
    for (int i = 0; i < dias * 10; ++i) {
      genes.push_back(acoesDisponiveis[distrib(rng)]);
    }
  }
}

void DNA::calcularFitness(
  const std::vector<std::vector<std::pair<std::string, double>>>& cotacoes,
  const double montanteInicial,
  const double numeroPotes
) {
  double montante = montanteInicial; // 1000 reais iniciais

  for (int d = 0; d < dias; ++d) {
    double pote = montante / numeroPotes; // Divide o montante igualmente entre os potes (ações; 10)
    double montanteDia = 0.0;

    for (int p = 0; p < 10; ++p) {
      std::string codigo = genes[d * 10 + p];
      double precoCompra = 0.0, precoVenda = 0.0;

      for (auto& par : cotacoes[d * 2]) {
        if (par.first == codigo) precoCompra = par.second;
      }
      for (auto& par : cotacoes[d * 2 + 1]) {
        if (par.first == codigo) precoVenda = par.second;
      }

      if (precoCompra > 0 && precoVenda > 0) {
        double qtd = pote / precoCompra;
        montanteDia += qtd * precoVenda;
      }
    }

    montante = montanteDia;
  }

  fitness = montante;
}

DNA DNA::crossover(
  const DNA& parceiro
) const 
{
  DNA filho(dias, {});
  const size_t corte = genes.size() / 2;
  filho.genes.clear();

  for (size_t i = 0; i < genes.size(); ++i) {
    filho.genes.push_back(i < corte ? genes[i] : parceiro.genes[i]);
  }

  return filho;
}

void DNA::mutar(double taxaMutacao, const std::vector<std::string>& acoesDisponiveis) {
  std::uniform_real_distribution<> prob(0.0, 1.0);                          // Distribuição uniforme para a probabilidade de mutação
  std::uniform_int_distribution<> novaAcao(0, acoesDisponiveis.size() - 1); // Distribuição uniforme para escolher uma nova ação

  for (size_t i = 0; i < genes.size(); ++i) {
    if (prob(rng) < taxaMutacao) {
      genes[i] = acoesDisponiveis[novaAcao(rng)];
    }
  }
}

int DNA::getDias() const
{
  return dias;
}
