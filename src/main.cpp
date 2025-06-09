#include <iostream>

#include <fstream>
#include <sstream>

#include <vector>

std::vector<std::string> split(const std::string& s, const char delimiter) {
  std::vector<std::string> tokens;
  std::string token;
  std::istringstream tokenStream(s);
  while (std::getline(tokenStream, token, delimiter)) {
    tokens.push_back(token);
  }
  return tokens;
}

std::vector<std::vector<std::string>> readCSV(const std::string& filename) {
  std::vector<std::vector<std::string>> data;
  std::ifstream file(filename);

  if (!file.is_open()) {
    std::cerr << "Failed to open file: " << filename << std::endl;
    return data;
  }

  std::string line;
  while (std::getline(file, line)) {
    std::vector<std::string> row = split(line, ';');
    data.push_back(row);
  }

  file.close();
  return data;
}

int main()
{
  std::string filename = "../res/distances.csv";
  for (std::vector<std::string>& row : readCSV(filename)) {
		for (std::string& data : row) {
			std::cout << data.c_str() << "\t";
		}
		std::cout << std::endl;
	}
	return 0;
}