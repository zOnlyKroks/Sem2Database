#include "datasourceUtils.cpp"
#include <stdlib.h>

Student* find_student_by_name(const string& vorname, const string& nachname) {
  for (auto& student : studierende) {
    if (student.vorname == vorname && student.nachname == nachname) {
      return &student;
    }
  }
  return nullptr;
}

Studiengang* find_studiengang(const string& nr) {
  for (auto& sg : studiengaenge) {
    try {
      if (std::stoi(sg.nr) == std::stoi(nr)) {
        return &sg;
      }
    } catch (const std::exception&) {
      if (sg.nr == nr) {
        return &sg;
      }
    }
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

string get_formatted_student_programs(const string& matrikel) {
  vector<Studiengang*> programs = get_student_programs(matrikel);
  
  if (programs.empty()) {
    return "Kein Studiengang";
  }
  
  string result;
  for (size_t i = 0; i < programs.size(); i++) {
    result += programs[i]->name;
    if (i < programs.size() - 1) {
      result += ", ";
    }
  }
  
  return result;
}

void display_menu() {
  cout << "\n╔════════════════════════════════════════════════════╗" << endl;
  cout << "║              STUDENTENVERWALTUNGSSYSTEM            ║" << endl;
  cout << "╠════════════════════════════════════════════════════╣" << endl;
  cout << "║ 1. Alle Studierende anzeigen                       ║" << endl;
  cout << "║ 2. Studiengänge verwalten                          ║" << endl;
  cout << "║ 3. Änderungen speichern                            ║" << endl;
  cout << "║ 4. Beenden                                         ║" << endl;
  cout << "╚════════════════════════════════════════════════════╝" << endl;
  cout << "Ihre Wahl: ";
}

void print_studierende() {
  setlocale(LC_ALL, "");

  size_t max_vorname = 8;
  size_t max_nachname = 9;
  size_t max_studiengang = 12;
  
  for (auto& student : studierende) {
    student.studiengang = get_formatted_student_programs(student.matrikel);
    max_vorname = max(max_vorname, student.vorname.length());
    max_nachname = max(max_nachname, student.nachname.length());
    max_studiengang = max(max_studiengang, student.studiengang.length());
  }

  string top_left     = "╔", top_right    = "╗",
         bottom_left  = "╚", bottom_right = "╝",
         horizontal   = "═", vertical     = "║",
         top_sep      = "╦", middle_left  = "╠",
         middle_sep   = "╬", middle_right = "╣",
         bottom_sep   = "╩";

  cout << "\n" << top_left;
  for (size_t i = 0; i < max_vorname + 2; i++) cout << horizontal;
  cout << top_sep;
  for (size_t i = 0; i < max_nachname + 2; i++) cout << horizontal;
  cout << top_sep;
  for (size_t i = 0; i < max_studiengang + 2; i++) cout << horizontal;
  cout << top_right << "\n";

  cout << vertical << " " << setw(max_vorname) << left << "Vorname" << " " << vertical;
  cout << " " << setw(max_nachname) << left << "Nachname" << " " << vertical;
  cout << " " << setw(max_studiengang) << left << "Studiengang" << " " << vertical << "\n";

  cout << middle_left;
  for (size_t i = 0; i < max_vorname + 2; i++) cout << horizontal;
  cout << middle_sep;
  for (size_t i = 0; i < max_nachname + 2; i++) cout << horizontal;
  cout << middle_sep;
  for (size_t i = 0; i < max_studiengang + 2; i++) cout << horizontal;
  cout << middle_right << "\n";

  for (const auto& student : studierende) {
    cout << vertical << " " << setw(max_vorname) << left << student.vorname << " " << vertical;
    cout << " " << setw(max_nachname) << left << student.nachname << " " << vertical;
    cout << " " << setw(max_studiengang) << left << student.studiengang << " " << vertical << "\n";
  }

  cout << bottom_left;
  for (size_t i = 0; i < max_vorname + 2; i++) cout << horizontal;
  cout << bottom_sep;
  for (size_t i = 0; i < max_nachname + 2; i++) cout << horizontal;
  cout << bottom_sep;
  for (size_t i = 0; i < max_studiengang + 2; i++) cout << horizontal;
  cout << bottom_right << "\n";
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
      auto existing = find_if(
          zuordnungen.begin(), zuordnungen.end(),
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
        
        student->studiengang = get_formatted_student_programs(student->matrikel);
        
        cout << "\n✅ " << student->vorname << " " << student->nachname
             << " wurde erfolgreich zum Studiengang '" << studiengang->name
             << "' hinzugefügt!" << endl;
      }
    } else if (action == 'e' || action == 'E') {
      size_t initial_size = zuordnungen.size();

      auto it = remove_if(
          zuordnungen.begin(), zuordnungen.end(),
          [&](const pair<string, string>& z) {
            try {
              return z.first == student->matrikel &&
                     std::stoi(z.second) == std::stoi(sg_nr);
            } catch (const std::exception& e) {
              return z.first == student->matrikel && z.second == sg_nr;
            }
          });

      zuordnungen.erase(it, zuordnungen.end());

      if (zuordnungen.size() < initial_size) {
        student->studiengang = get_formatted_student_programs(student->matrikel);
        
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

int main(int argc, char* argv[]) {
  unique_ptr<DataSource> dataSource;

  cout << "Single file mode (s) | Multi file mode (m): " << endl;

  std::string line;
  std::getline( std::cin, line );
  
  if (line == "s") {
    dataSource = make_unique<SingleFileDataSource>("studium.csv");
    cout << "Using single file data source (studium.csv)" << endl;
  } else if(line == "m") {
    dataSource = make_unique<MultiFileDataSource>(
        "Studierende.csv", "studiengaenge.csv", "zugeordnete_studiengaenge.csv");
    cout << "Using multi-file data source" << endl;
  }else {
    cout << "Invalid mode!" << endl;
    return -1;
  }
  
  if (!dataSource->loadData()) {
    cerr << "Failed to load data. Exiting." << endl;
    return 1;
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

      case 3:
        if (dataSource->saveData()) {
          unsaved_changes = false;
        }
        break;

      case 4:
        if (unsaved_changes) {
          char save;
          cout << "\nSie haben ungespeicherte Änderungen. Möchten Sie diese "
                  "speichern? (j/n): ";
          cin >> save;
          if (save == 'j' || save == 'J') {
            dataSource->saveData();
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