#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

bool write_csv(const string& filename, const vector<vector<string>>& data) {
  const string fullQualifiedPath =
      filesystem::current_path().generic_string() + "/" + filename;
  ofstream file(fullQualifiedPath);

  if (!file.is_open()) {
    return false;
  }

  for (const auto& row : data) {
    for (size_t i = 0; i < row.size(); ++i) {
      file << row[i];
      if (i < row.size() - 1) {
        file << ";";
      }
    }
    file << endl;
  }

  file.close();
  return true;
}

vector<vector<string>> read_csv(const string& filename) {
  const string fullQualifiedPath =
      filesystem::current_path().generic_string() + "/" + filename;
  vector<vector<string>> data;
  ifstream file(fullQualifiedPath);

  if (!file.is_open()) {
    return data;
  }

  string line;
  bool firstLine = true;
  while (getline(file, line)) {
    if (firstLine) {
      if (line.size() >= 3 && (unsigned char)line[0] == 0xEF &&
          (unsigned char)line[1] == 0xBB && (unsigned char)line[2] == 0xBF) {
        line.erase(0, 3);
      }
      firstLine = false;
    }

    vector<string> row;
    stringstream ss(line);
    string cell;

    while (getline(ss, cell, ';')) {
      row.push_back(cell);
    }

    data.push_back(row);
  }

  file.close();
  return data;
}