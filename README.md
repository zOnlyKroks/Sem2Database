# Studentenverwaltungssystem

## Überblick

Diese Repository enthält zwei C++-Anwendungen zur Verwaltung von Studierenden und ihren Studiengängen. Beide Programme lesen CSV-Dateien ein, ermöglichen das Anzeigen von Studierenden samt Studiengängen und bieten die Möglichkeit, die Studiengangszuordnung zu ändern und zu speichern.

## Programme

### 1. `prog1.cpp`

Diese Anwendung liest folgende Dateien ein:

- `studierende.csv` (Liste der Studierenden mit Matrikelnummer, Vorname und Nachname)
- `studiengaenge.csv` (Liste der Studiengänge mit Nummer und Name)
- `zugeordnete_studiengaenge.csv` (Zuordnung der Studierenden zu Studiengängen)

### 2. `prog2.cpp`

Diese Anwendung liest folgende Datei ein:

- `studium.csv` (Liste der Studierenden mit Matrikelnummer, Vorname, Nachname und Studiengang)

## Nutzung

1. Kompilieren Sie die Programme mit g++:

   ```bash
   g++ -o prog1 prog1.cpp
   g++ -o prog2 prog2.cpp
   ```

2. Führen Sie die Programme aus:

   ```bash
   ./prog1
   ./prog2
   ```

3. Folgen Sie den Anweisungen im Programm, um die Studierendenliste anzuzeigen und Änderungen vorzunehmen.

## Oder

```bash
   ./start.sh
   ./start2.sh
```

## Ineffizienzen

### `prog1.cpp`

- Die CSV-Dateien werden vollständig in Speicher geladen, was bei großen Dateien ineffizient sein kann.
- Das Suchen von Studierenden oder Studiengängen erfolgt mit `std::find_if`, was eine lineare Suche ist (O(n)).
- Beim Speichern wird die gesamte Datei neu geschrieben, was bei großen Datenmengen langsam sein kann.

### `prog2.cpp`

- Ähnliche Probleme wie `prog1.cpp` (vollständiges Laden in den Speicher, lineare Suche, Neuschreiben der Datei).
- Studierende mit mehreren Studiengängen werden als separate Einträge gespeichert, was Redundanz verursacht und das Bearbeiten erschwert.

## Mögliche Optimierungen

- Verwenden einer Datenbank anstelle von CSV-Dateien zur schnelleren Suche und Aktualisierung.
- Nutzung von Hashmaps (`unordered_map`) für effizientere Suchen (O(1) statt O(n)).
- Nutzung von In-Place-Bearbeitung der CSV-Dateien, um das komplette Neuschreiben zu vermeiden.
