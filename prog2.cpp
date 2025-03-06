#include "shared.cpp"

struct Student {
  string matrikel, vorname, nachname, studiengangsnummer, studiengang;
};

struct Studiengang {
  string nr, name;
};

vector<Student> studierende;
vector<Studiengang> studiengaenge;

Student* find_student_by_id(const string& matrikel) {
  auto it = find_if(studierende.begin(), studierende.end(),
                    [&matrikel](const Student& s) { return s.matrikel == matrikel; });
  if (it != studierende.end()) {
    return &(*it);
  }
  return nullptr;
}

Student* find_student_by_name(const string& vorname, const string& nachname) {
  auto it = find_if(studierende.begin(), studierende.end(), [&](const Student& s) {
    return s.vorname == vorname && s.nachname == nachname;
  });
  if (it != studierende.end()) {
    return &(*it);
  }
  return nullptr;
}

Studiengang* find_studiengang(const string& nr) {
  auto it = find_if(studiengaenge.begin(), studiengaenge.end(),
                    [&nr](const Studiengang& sg) { return sg.nr == nr; });
  if (it != studiengaenge.end()) {
    return &(*it);
  }
  return nullptr;
}

void print_studierende() {
  size_t max_vorname = 8;
  size_t max_nachname = 9;
  size_t max_studiengang = 12;
  map<string, vector<Student*>> students_by_program;

  for (auto& student : studierende) {
    max_vorname = max(max_vorname, student.vorname.length());
    max_nachname = max(max_nachname, student.nachname.length());
    string program =
        student.studiengang.empty() ? "Kein Studiengang" : student.studiengang;
    max_studiengang = max(max_studiengang, program.length());
    students_by_program[program].push_back(&student);
  }

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
  cout << "\nAktuelle Studiengänge von " << student->vorname << " "
       << student->nachname << ":" << endl;
  cout << "--------------------------------------------" << endl;
  bool found = false;
  for (const auto& s : studierende) {
    if (s.matrikel == student->matrikel) {
      cout << "  → [" << s.studiengangsnummer << "] "
           << (s.studiengang.empty() ? "Kein Studiengang" : s.studiengang)
           << endl;
      found = true;
    }
  }
  if (!found) {
    cout << "  → Keine Studiengänge zugeordnet" << endl;
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

    vector<Student*> enrollments;
    for (auto& s : studierende) {
      if (s.vorname == vorname && s.nachname == nachname) {
        enrollments.push_back(&s);
      }
    }

    if (enrollments.empty()) {
      cerr << "\n⚠️  Student '" << vorname << " " << nachname
           << "' wurde nicht gefunden!" << endl;
      cout << "Verfügbare Studierende:" << endl;
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

    cout << "\nAktuelle Studiengänge von " << vorname << " " << nachname << ":"
         << endl;
    for (auto s : enrollments) {
      cout << "  → [" << s->studiengangsnummer << "] "
           << (s->studiengang.empty() ? "Kein Studiengang" : s->studiengang)
           << endl;
    }

    cout << "\nVerfügbare Studiengänge:" << endl;
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
      bool exists = false;
      for (auto s : enrollments) {
        if (s->studiengangsnummer == sg_nr) {
          exists = true;
          break;
        }
      }
      if (exists) {
        cout << "\n⚠️  " << vorname << " " << nachname
             << " ist bereits im Studiengang '" << studiengang->name
             << "' eingeschrieben!" << endl;
      } else {
        Student newEnrollment;
        newEnrollment.matrikel = enrollments[0]->matrikel;
        newEnrollment.vorname = vorname;
        newEnrollment.nachname = nachname;
        newEnrollment.studiengangsnummer = sg_nr;
        newEnrollment.studiengang = studiengang->name;
        studierende.push_back(newEnrollment);
        cout << "\n✅ " << vorname << " " << nachname
             << " wurde erfolgreich zum Studiengang '" << studiengang->name
             << "' hinzugefügt!" << endl;
      }
    } else if (action == 'e' || action == 'E') {
      if (enrollments.size() == 1 &&
          enrollments[0]->studiengangsnummer == sg_nr) {
        enrollments[0]->studiengangsnummer = "";
        enrollments[0]->studiengang = "Kein Studiengang";
        cout << "\n✅ " << vorname << " " << nachname
             << " wurde erfolgreich aus dem Studiengang '" << studiengang->name
             << "' entfernt (letzte Zuordnung, jetzt 'Kein Studiengang')!"
             << endl;
      } else {
        size_t initial_size = studierende.size();
        auto it = remove_if(
            studierende.begin(), studierende.end(), [&](const Student& s) {
              return s.vorname == vorname && s.nachname == nachname &&
                     s.studiengangsnummer == sg_nr;
            });
        studierende.erase(it, studierende.end());
        if (studierende.size() < initial_size) {
          cout << "\n✅ " << vorname << " " << nachname
               << " wurde erfolgreich aus dem Studiengang '"
               << studiengang->name << "' entfernt!" << endl;
        } else {
          cout << "\n⚠️  " << vorname << " " << nachname
               << " ist nicht im Studiengang '" << studiengang->name
               << "' eingeschrieben!" << endl;
        }
      }
    } else {
      cout << "\n⚠️  Ungültige Aktion! Bitte 'h' für hinzufügen oder 'e' für "
              "entfernen eingeben."
           << endl;
    }

    vector<Student*> updated;
    for (auto& s : studierende) {
      if (s.vorname == vorname && s.nachname == nachname) {
        updated.push_back(&s);
      }
    }
    cout << "\nAktualisierte Studiengänge von " << vorname << " " << nachname
         << ":" << endl;
    if (updated.empty()) {
      cout << "  → Keine Studiengänge zugeordnet" << endl;
    } else {
      for (auto s : updated) {
        cout << "  → [" << s->studiengangsnummer << "] "
             << (s->studiengang.empty() ? "Kein Studiengang" : s->studiengang)
             << endl;
      }
    }

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
  auto studium_data = read_csv("studium.csv");
  if (studium_data.empty()) {
    cerr << "Error: Could not read studium.csv or file is empty" << endl;
    return 1;
  }

  for (size_t i = 1; i < studium_data.size(); ++i) {
    if (studium_data[i].size() >= 6) {
      studierende.push_back({studium_data[i][0], studium_data[i][1],
                             studium_data[i][2], studium_data[i][4],
                             studium_data[i][5]});
    }
  }

  map<string, string> sg_map;
  for (size_t i = 1; i < studium_data.size(); ++i) {
    const auto& row = studium_data[i];
    if (row.size() >= 6) {
      string nummer = row[4];
      string name = row[5];
      if (!nummer.empty() && !name.empty()) {
        sg_map[nummer] = name;
      }
    }
  }

  for (const auto& entry : sg_map) {
    studiengaenge.push_back({entry.first, entry.second});
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
          vector<vector<string>> new_studium;
          new_studium.push_back(studium_data[0]);
          for (const auto& s : studierende) {
            new_studium.push_back({s.matrikel, s.vorname, s.nachname, "",
                                   s.studiengangsnummer, s.studiengang, "", ""});
          }
          if (write_csv("studium.csv", new_studium)) {
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
              vector<vector<string>> new_studium;
              new_studium.push_back(studium_data[0]);
              for (const auto& s : studierende) {
                new_studium.push_back({s.matrikel, s.vorname, s.nachname, "",
                                       s.studiengangsnummer, s.studiengang, "",
                                       ""});
              }
              write_csv("studium.csv", new_studium);
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