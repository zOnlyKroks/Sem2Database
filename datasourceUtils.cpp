#pragma once

#include "csvUtils.cpp"

struct Student {
  string matrikel;
  string vorname;
  string nachname;
  string studiengangsnummer;
  string studiengang;
};

struct Studiengang {
  string nr;
  string name;
};

vector<Student> studierende;
vector<Studiengang> studiengaenge;
vector<pair<string, string>> zuordnungen;

class DataSource {
 public:
  virtual ~DataSource() = default;
  virtual bool loadData() = 0;
  virtual bool saveData() = 0;
};

class MultiFileDataSource : public DataSource {
 private:
  string students_file;
  string programs_file;
  string assignments_file;

 public:
  MultiFileDataSource(const string& students_file, const string& programs_file,
                      const string& assignments_file)
      : students_file(students_file),
        programs_file(programs_file),
        assignments_file(assignments_file) {}

  bool loadData() override {
    studierende.clear();
    studiengaenge.clear();
    zuordnungen.clear();

    auto stud_data = read_csv(students_file);
    if (stud_data.empty()) {
      cerr << "Error: Could not read " << students_file << " or file is empty"
           << endl;
      return false;
    }

    for (size_t i = 1; i < stud_data.size(); ++i) {
      Student s = {stud_data[i][0], stud_data[i][1], stud_data[i][2], "", ""};
      studierende.push_back(s);
    }

    auto sg_data = read_csv(programs_file);
    if (sg_data.empty()) {
      cerr << "Error: Could not read " << programs_file << " or file is empty"
           << endl;
      return false;
    }

    for (size_t i = 1; i < sg_data.size(); ++i) {
      if (sg_data[i].size() >= 2) {
        studiengaenge.push_back({sg_data[i][0], sg_data[i][1]});
      }
    }

    auto zuordnung_data = read_csv(assignments_file);

    if (!zuordnung_data.empty()) {
      for (size_t i = 1; i < zuordnung_data.size(); ++i) {
        if (zuordnung_data[i].size() >= 2) {
          string studentMatrikel = zuordnung_data[i][0];
          string programNr = zuordnung_data[i][1];
          zuordnungen.emplace_back(studentMatrikel, programNr);
        }
      }
    }

    for (auto& student : studierende) {
      for (const auto& zuordnung : zuordnungen) {
        if (zuordnung.first == student.matrikel) {
          student.studiengangsnummer = zuordnung.second;

          for (const auto& studiengang : studiengaenge) {
            if (studiengang.nr == zuordnung.second) {
              student.studiengang = studiengang.name;
              break;
            }
          }
          break;
        }
      }
    }
    return true;
  }

  bool saveData() override {
    vector<vector<string>> new_zuordnung;
    new_zuordnung.push_back({"Studierende_Matrikelnummer", "Studiengang_Nr"});
    for (const auto& z : zuordnungen) {
      new_zuordnung.push_back({z.first, z.second});
    }

    if (write_csv(assignments_file, new_zuordnung)) {
      cout << "\n✅ Änderungen wurden erfolgreich gespeichert!" << endl;
      return true;
    } else {
      cerr << "\n⚠️  Fehler beim Speichern der Änderungen!" << endl;
      return false;
    }
  }
};

class SingleFileDataSource : public DataSource {
 private:
  string data_file;
  vector<vector<string>> header_data;

 public:
  SingleFileDataSource(const string& data_file) : data_file(data_file) {}

  bool loadData() override {
    studierende.clear();
    studiengaenge.clear();
    zuordnungen.clear();

    auto studium_data = read_csv(data_file);
    if (studium_data.empty()) {
      cerr << "Error: Could not read " << data_file << " or file is empty"
           << endl;
      return false;
    }

    header_data = {studium_data[0]};

    map<string, string> sg_map;
    for (size_t i = 1; i < studium_data.size(); ++i) {
      if (studium_data[i].size() >= 6) {
        string matrikel = studium_data[i][0];
        string vorname = studium_data[i][1];
        string nachname = studium_data[i][2];
        string sg_nummer = studium_data[i][4];
        string sg_name = studium_data[i][5];

        studierende.push_back(
            {matrikel, vorname, nachname, sg_nummer, sg_name});

        if (!sg_nummer.empty()) {
          zuordnungen.emplace_back(matrikel, sg_nummer);
          sg_map[sg_nummer] = sg_name;
        }
      }
    }

    for (const auto& entry : sg_map) {
      studiengaenge.push_back({entry.first, entry.second});
    }

    return true;
  }

  bool saveData() override {
    vector<vector<string>> new_studium;
    new_studium = header_data;

    for (const auto& s : studierende) {
      vector<string> row = {s.matrikel, s.vorname, s.nachname, ""};

      if (header_data[0].size() >= 6) {
        row.push_back(s.studiengangsnummer);
        row.push_back(s.studiengang);
      }

      while (row.size() < header_data[0].size()) {
        row.push_back("");
      }

      new_studium.push_back(row);
    }

    if (write_csv(data_file, new_studium)) {
      cout << "\n✅ Änderungen wurden erfolgreich gespeichert!" << endl;
      return true;
    } else {
      cerr << "\n⚠️  Fehler beim Speichern der Änderungen!" << endl;
      return false;
    }
  }
};