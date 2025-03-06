#include "shared.cpp"

struct Student {
  string matrikel, vorname, nachname;
};

struct Studiengang {
  string nr, name;
};

vector<Student> studierende;
vector<Studiengang> studiengaenge;
vector<pair<string, string>> zuordnungen;

Student* find_student_by_id(const string& matrikel) {
  auto it =
      find_if(studierende.begin(), studierende.end(),
              [&matrikel](const Student& s) { return s.matrikel == matrikel; });

  if (it != studierende.end()) {
    return &(*it);
  }
  return nullptr;
}

Student* find_student_by_name(const string& vorname, const string& nachname) {
  auto it =
      find_if(studierende.begin(), studierende.end(), [&](const Student& s) {
        return s.vorname == vorname && s.nachname == nachname;
      });

  if (it != studierende.end()) {
    return &(*it);
  }
  return nullptr;
}

Studiengang* find_studiengang(const string& nr) {
  auto it = find_if(studiengaenge.begin(), studiengaenge.end(),
                    [&nr](const Studiengang& sg) {
                      try {
                        return std::stoi(sg.nr) == std::stoi(nr);
                      } catch (const std::exception& e) {
                        return sg.nr == nr;
                      }
                    });

  if (it != studiengaenge.end()) {
    return &(*it);
  }
  return nullptr;
}

vector<Studiengang*> get_student_programs(const string& matrikel) {
  vector<Studiengang*> programs;
  for (const auto& z : zuordnungen) {
    if (z.first == matrikel) {
      Studiengang* sg = find_studiengang(z.second);
      if (sg) {
        programs.push_back(sg);
      }
    }
  }
  return programs;
}

void print_studierende() {
  size_t max_vorname = 8;
  size_t max_nachname = 9;
  size_t max_studiengang = 12;

  map<string, vector<Student*>> students_by_program;

  for (auto& student : studierende) {
    max_vorname = max(max_vorname, student.vorname.length());
    max_nachname = max(max_nachname, student.nachname.length());

    vector<Studiengang*> programs = get_student_programs(student.matrikel);

    if (programs.empty()) {
      students_by_program["Kein Studiengang"].push_back(&student);
      max_studiengang =
          max(max_studiengang, string("Kein Studiengang").length());
    } else {
      for (auto& sg : programs) {
        max_studiengang = max(max_studiengang, sg->name.length());
        students_by_program[sg->name].push_back(&student);
      }
    }
  }

  // Wenn keine Studierende vorhanden sind
  if (studierende.empty()) {
    cout << "\n╔════════════════════════════════════════════════════╗" << endl;
    cout << "║            Keine Studierende gefunden              ║" << endl;
    cout << "╚════════════════════════════════════════════════════╝" << endl;
    return;
  }

  cout << "\n╔";
  for (size_t i = 0; i < max_vorname + 2; i++) cout << "═";
  cout << "╦";
  for (size_t i = 0; i < max_nachname + 2; i++) cout << "═";
  cout << "╦";
  for (size_t i = 0; i < max_studiengang + 2; i++) cout << "═";
  cout << "╗" << endl;

  cout << "║ " << setw(max_vorname) << left << "Vorname" << " ║ ";
  cout << setw(max_nachname) << left << "Nachname" << " ║ ";
  cout << setw(max_studiengang) << left << "Studiengang" << " ║" << endl;

  cout << "╠";
  for (size_t i = 0; i < max_vorname + 2; i++) cout << "═";
  cout << "╬";
  for (size_t i = 0; i < max_nachname + 2; i++) cout << "═";
  cout << "╬";
  for (size_t i = 0; i < max_studiengang + 2; i++) cout << "═";
  cout << "╣" << endl;

  for (const auto& [program_name, students] : students_by_program) {
    for (const auto& student : students) {
      cout << "║ " << setw(max_vorname) << left << student->vorname << " ║ ";
      cout << setw(max_nachname) << left << student->nachname << " ║ ";
      cout << setw(max_studiengang) << left << program_name << " ║" << endl;
    }
  }

  cout << "╚";
  for (size_t i = 0; i < max_vorname + 2; i++) cout << "═";
  cout << "╩";
  for (size_t i = 0; i < max_nachname + 2; i++) cout << "═";
  cout << "╩";
  for (size_t i = 0; i < max_studiengang + 2; i++) cout << "═";
  cout << "╝" << endl;
}

void show_student_programs(Student* student) {
  if (!student) return;

  vector<Studiengang*> programs = get_student_programs(student->matrikel);

  cout << "\nAktuelle Studiengänge von " << student->vorname << " "
       << student->nachname << ":" << endl;
  cout << "--------------------------------------------" << endl;

  if (programs.empty()) {
    cout << "  → Keine Studiengänge zugeordnet" << endl;
  } else {
    for (const auto& sg : programs) {
      cout << "  → [" << sg->nr << "] " << sg->name << endl;
    }
  }
  cout << endl;
}

void modify_studiengang() {
  string vorname, nachname, sg_nr;
  char action;
  bool first = true;

  while (true) {
    if (!first) {
      cout << "\nAktuelle Zuordnungen:" << endl;
      print_studierende();
    }

    cout << "\n╔════════════════════════════════════════════════════╗" << endl;
    cout << "║              STUDIENGANG VERWALTUNG                ║" << endl;
    cout << "╚════════════════════════════════════════════════════╝" << endl;

    cout << "Geben Sie den Vornamen des Studenten ein: ";
    cin >> vorname;
    cout << "Geben Sie den Nachnamen des Studenten ein: ";
    cin >> nachname;

    Student* student = find_student_by_name(vorname, nachname);

    if (!student) {
      cerr << "\n⚠️  Student '" << vorname << " " << nachname
           << "' wurde nicht gefunden!" << endl;
      cout << "Verfügbare Studierende:" << endl;
      cout << "----------------------" << endl;
      for (const auto& s : studierende) {
        cout << "  → " << s.vorname << " " << s.nachname
             << " (Matrikel: " << s.matrikel << ")" << endl;
      }

      char retry;
      cout << "\nMöchten Sie es erneut versuchen? (j/n): ";
      cin >> retry;
      if (retry != 'j' && retry != 'J') {
        break;
      }
      continue;
    }

    show_student_programs(student);

    cout << "\nVerfügbare Studiengänge:" << endl;
    cout << "----------------------" << endl;
    for (const auto& sg : studiengaenge) {
      cout << "  → [" << sg.nr << "] " << sg.name << endl;
    }

    cout << "\nGeben Sie die Studiengangsnummer ein: ";
    cin >> sg_nr;

    Studiengang* studiengang = find_studiengang(sg_nr);

    if (!studiengang) {
      cerr << "\n⚠️  Studiengang mit Nummer '" << sg_nr
           << "' wurde nicht gefunden!" << endl;
      continue;
    }

    cout << "\nMöchten Sie den Studiengang hinzufügen (h) oder entfernen (e)? ";
    cin >> action;

    if (action == 'h' || action == 'H') {
      auto existing =
          find_if(zuordnungen.begin(), zuordnungen.end(),
                  [&](const pair<string, string>& z) {
                    try {
                      return z.first == student->matrikel &&
                             std::stoi(z.second) == std::stoi(sg_nr);
                    } catch (const std::exception& e) {
                      return z.first == student->matrikel && z.second == sg_nr;
                    }
                  });

      if (existing != zuordnungen.end()) {
        cout << "\n⚠️  " << student->vorname << " " << student->nachname
             << " ist bereits im Studiengang '" << studiengang->name
             << "' eingeschrieben!" << endl;
      } else {
        zuordnungen.emplace_back(student->matrikel, sg_nr);
        cout << "\n✅ " << student->vorname << " " << student->nachname
             << " wurde erfolgreich zum Studiengang '" << studiengang->name
             << "' hinzugefügt!" << endl;
      }
    } else if (action == 'e' || action == 'E') {
      size_t initial_size = zuordnungen.size();

      auto it = remove_if(zuordnungen.begin(), zuordnungen.end(),
                          [&](const pair<string, string>& z) {
                            try {
                              return z.first == student->matrikel &&
                                     std::stoi(z.second) == std::stoi(sg_nr);
                            } catch (const std::exception& e) {
                              return z.first == student->matrikel &&
                                     z.second == sg_nr;
                            }
                          });

      zuordnungen.erase(it, zuordnungen.end());

      if (zuordnungen.size() < initial_size) {
        cout << "\n✅ " << student->vorname << " " << student->nachname
             << " wurde erfolgreich aus dem Studiengang '" << studiengang->name
             << "' entfernt!" << endl;
      } else {
        cout << "\n⚠️  " << student->vorname << " " << student->nachname
             << " ist nicht im Studiengang '" << studiengang->name
             << "' eingeschrieben!" << endl;
      }
    } else {
      cout << "\n⚠️  Ungültige Aktion! Bitte 'h' für hinzufügen oder 'e' für "
              "entfernen eingeben."
           << endl;
    }

    show_student_programs(student);

    char another;
    cout << "\nMöchten Sie eine weitere Änderung vornehmen? (j/n): ";
    cin >> another;
    first = false;
    if (another != 'j' && another != 'J') {
      break;
    }
  }
}

int main() {
  auto stud_data = read_csv("Studierende.csv");
  if (stud_data.empty()) {
    cerr << "Error: Could not read Studierende.csv or file is empty" << endl;
    return 1;
  }

  for (size_t i = 1; i < stud_data.size(); ++i) {
    if (stud_data[i].size() >= 3) {
      studierende.push_back(
          {stud_data[i][0], stud_data[i][1], stud_data[i][2]});
    }
  }

  auto sg_data = read_csv("studiengaenge.csv");
  if (sg_data.empty()) {
    cerr << "Error: Could not read studiengaenge.csv or file is empty" << endl;
    return 1;
  }

  for (size_t i = 1; i < sg_data.size(); ++i) {
    if (sg_data[i].size() >= 2) {
      studiengaenge.push_back({sg_data[i][0], sg_data[i][1]});
    }
  }

  auto zuordnung_data = read_csv("zugeordnete_studiengaenge.csv");
  if (!zuordnung_data.empty()) {
    for (size_t i = 1; i < zuordnung_data.size(); ++i) {
      if (zuordnung_data[i].size() >= 2) {
        zuordnungen.emplace_back(zuordnung_data[i][0], zuordnung_data[i][1]);
      }
    }
  }

  cout << "Willkommen zum Studentenverwaltungssystem!" << endl;

  int choice = 0;
  bool unsaved_changes = false;

  while (choice != 4) {
    display_menu();
    cin >> choice;

    switch (choice) {
      case 1:
        cout << "\nListe aller Studierenden und ihrer Studiengänge:" << endl;
        print_studierende();
        break;

      case 2:
        modify_studiengang();
        unsaved_changes = true;
        break;

      case 3: {
        vector<vector<string>> new_zuordnung;
        new_zuordnung.push_back(
            {"Studierende_Matrikelnummer", "Studiengang_Nr"});
        for (const auto& z : zuordnungen) {
          new_zuordnung.push_back({z.first, z.second});
        }

        if (write_csv("zugeordnete_studiengaenge.csv", new_zuordnung)) {
          cout << "\n✅ Änderungen wurden erfolgreich gespeichert!" << endl;
          unsaved_changes = false;
        } else {
          cerr << "\n⚠️  Fehler beim Speichern der Änderungen!" << endl;
        }
        break;
      }

      case 4:
        if (unsaved_changes) {
          char save;
          cout << "\nSie haben ungespeicherte Änderungen. Möchten Sie diese "
                  "speichern? (j/n): ";
          cin >> save;

          if (save == 'j' || save == 'J') {
            vector<vector<string>> new_zuordnung;
            new_zuordnung.push_back(
                {"Studierende_Matrikelnummer", "Studiengang_Nr"});
            for (const auto& z : zuordnungen) {
              new_zuordnung.push_back({z.first, z.second});
            }

            write_csv("zugeordnete_studiengaenge.csv", new_zuordnung);
            cout << "\n✅ Änderungen wurden erfolgreich gespeichert!" << endl;
          }
        }
        cout << "\nVielen Dank für die Nutzung des "
                "Studentenverwaltungssystems. Auf Wiedersehen!"
             << endl;
        break;

      default:
        cout << "\n⚠️  Ungültige Eingabe! Bitte wählen Sie eine Zahl zwischen 1 "
                "und 4."
             << endl;
    }
  }

  return 0;
}