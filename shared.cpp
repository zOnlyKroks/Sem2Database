#pragma once

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <sstream>
#include <vector>

using namespace std;

vector<vector<string>> read_csv(const string& filename) {
  std::string fullFilePath = std::filesystem::current_path() / filename;

  vector<vector<string>> data;
  ifstream file(fullFilePath);

  if (!file.is_open()) {
    cerr << "Error: Could not open file " << filename << endl;
    return data;
  }

  string line;
  while (getline(file, line)) {
    stringstream ss(line);
    vector<string> row;
    string cell;
    while (getline(ss, cell, ';')) {
      row.push_back(cell);
    }
    data.push_back(row);
  }

  file.close();
  return data;
}

bool write_csv(const string& filename, const vector<vector<string>>& data) {
  std::string fullFilePath = std::filesystem::current_path() / filename;

  ofstream file(fullFilePath);

  if (!file.is_open()) {
    cerr << "Error: Could not write to file " << filename << endl;
    return false;
  }

  for (const auto& row : data) {
    for (size_t i = 0; i < row.size(); ++i) {
      file << row[i];
      if (i != row.size() - 1) file << ";";
    }
    file << "\n";
  }

  file.close();

  return true;
}

void display_menu() {
  cout << "\n╔════════════════════════════════════════════════════╗" << endl;
  cout << "║            STUDENTENVERWALTUNGSSYSTEM              ║" << endl;
  cout << "╠════════════════════════════════════════════════════╣" << endl;
  cout << "║ 1. Alle Studierende anzeigen                       ║" << endl;
  cout << "║ 2. Studiengang hinzufügen/entfernen                ║" << endl;
  cout << "║ 3. Änderungen speichern                            ║" << endl;
  cout << "║ 4. Beenden                                         ║" << endl;
  cout << "╚════════════════════════════════════════════════════╝" << endl;
  cout << "Ihre Wahl: ";
}