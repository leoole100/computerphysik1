# Blatt 1

Leon Oleschko, Aurel Müller-Schoenau

## Aufgabe 1

### a) uns bekannte Betriebssysteme

Linux: Android

UNIX: Solaris, MacOS, IOS, IpadOS

DOS: Windows

RTOS: FreeRTOS

[TetrisOS](https://github.com/vladcc/Tetris-OS)

### b) Was ist eine Linux Distro?

Eine Sammlung aus einem Linux Kernel, Package Manager, optionaler Software wie Window Manager, XServer, etc.

Funktioniert meistens out-of-the-box inklusive grafischer Oberfläche.

### c) Hilfe?

`help`, `man` und Foren wie Reddit, Arch-Wiki.

### d) Infos über den Computer:

`uname -a`: Otto;
Linux Kernel:5.3.18-150300.59.49-default;
Architektur: x86_48

`lscpu`:
Intel Core i5-4590 @ 3.38 Ghz
4-Kerne, 800 Mhz - 3.7 Ghz, Cache: L1i, L1d, L2, L3

`top`: 16 Gb RAM

`lscsi`:
ATA Festplatte WD
DVD Laufwerk

`df`:
root: 60 G
tempfs for user: 1,6G
software: 1.5T

`inix -fx`: 
openSuse Leap 15.3
KDE Plasma 5.18.6
GPU: Intel XEON E3-1200 Integrated Graphics

### e) Vorteile des Terminals gegenüber einer GUI:

Programm mit allen Optionen kann in nur einer Zeile gestartet werden. -> oft schneller als klicken, wenn man weiß, was man tut

Komplexe Aufgaben lassen sich etwa durch das Verketten ("pipe") von Befehlen, oder durch Skripte ausführen. Skripte können ganze Programme ersetzen.

Paketverwaltungen erlauben das Installieren oder updaten vieler Programme in nur einer Zeile.

Wenn mal nichts mehr funktioniert (z.B. Grafiktreiber kaputt) ist immernoch die Shell da.


## Aufgabe 2

### a) Wichtige Befehle:

`man` Anleitung ("Manual") zu den meisten Befehlen, etwa `man ls`

`ls` Liste der Dateien und Ordner im aktuellen Verzeichnis

`cd` "Change Directory`, also Verzeichniswechsel. Verwende `ls ../` um ins Elternverzeichnis zu wechseln

`mkdir`, `touch` Verzeichnis, bzw. Datei erstellen

`mv`, `cp`, `rm` Verschieben, Kopieren und Löschen von Dateien

`lshw`, `lspci`, `lscpu`, `lshw` Listet Information zu Hardware / PCI-Geräten / CPU / Hardware

`top` viele Informationen über alles mögliche, mehr dazu in e)

`kill` Einen Prozess beenden

`head`, `tail` Die ersten / letzten paar Zeilen einer Datei ansehen. Funktioniert auch mit Output eines anderen Programms.

`cat` gibt die ganze Datei aus

`dd` erstellt (bitgenaues) Abbild einer Datei oder sogar Dateisystemstruktur, kann z.B. Iso-Image 'entpacken'

`echo` gibt seine eigenen Optionen wieder als Output aus



`[Befehl1] | [Befehl2]`   Führt Befehl 1 aus, und ruft Befehl 2 mit dessen Output als Input auf

Insbesondere lohnt sich `[Befehl] | grep [Suchwort]`, um aus dem Output von [Befehl] nur solche Stellen, die das Suchwort enthalten, zu filtern

`[Befehl] > [Dateiname]` schreibt den Output von `[Befehl]` in die Datei `[Dateiname]`

`[Befehl] &` Startet Befehl als Hintergrundjob. Terminal wird durch das Weiterlaufen des Hintergrundjobs nicht blockiert.

`[Befehl] --help` liefert in der Regeel Hilfemenü, `man` ist aber oft besser.

Drückt man bei halbfertiger Eingabe ´[TAB]`, so schlägt die Shell Vervollständigungen vor, sogar kontextgebunden. Kann etwa Befehle, Befehlsoptionen, aber auch Dateinamen ergänzen.

`[Strg + C]` bricht den laufenden Befehl ab.

`exit` schließt Terminal


### b) Erklärung einiger Shelleingaben:

`ls > test.txt` Schreibt die Ausgabe von `ls` in die Datei `test.txt`. Wenn die Datei schon existiert, wird sie überschrieben.

`ls >> test.txt` hängt die Ausgabe von `ls` an die Datei `test.txt` an. Was vorher schon darin stand, bleibt erhalten.

`ls | sort > test.txt` Ruft `ls` auf, ruft anschließend `sort` mit der Ausgabe des ersten Befehls auf, und gibt die Ausgabe des `sort`-Befehls schließlich in die Datei `test.txt`


### c) Befehle `wc` und `file`

Der Befehl `wc` gibt die Anzahl der Wörter, Bytes und Zeilensprünge einer Datei aus.

`file` liefert den Dateityp.


### d) Optionen und Argumente

Optionen werden mit zwei Bindestrichen eingeleitet: `--help`, wobei oft eine Kurzform mit nur einem Bindestrich existiert: `-h`

Argumente können einfach ausgeschrieben werden, oder man setzt sie in Anführungszeichen. Letzteres umgeht etwaige Probleme mit Leerstellen, die eigentlich signalisieren, dass dort ein neues Argument beginnt.


### e) Der Top-Befehl

liefert Informationen wie

-Uhrzeit
-uptime des Systems (Zeit seit dem Start)
-Anzahl angemeldeter Nutzer
-Anzahl laufende Tasks, wie viele davon aktuell im Vordergrund laufen, wie viele im Standby sind
-relative Prozessorauslastung
-Auslastung Arbeitsspeicher und Swap


Gefolgt wird das ganze von einer Tabelle die die aktuell laufenden Prozesse auflistet mit

-Prozess-ID (braucht man etwa für `kill`)
-Besitzer des Tasks (User, der ihn gestartet hat)
-relative Prozessorzeit für den Prozess
-relative Speicherauslastung des Prozesses
-Kommando, dem der Task gehört

und mehr... Die manpage hat über 2000 Zeilen.


### f) Kopieren von Dateien zwischen Rechnern

Funktioniert über eine ssh-Verbindung. Diese muss zuerst bestehen, dann kann man einfach mit `scp` deine Datei von `[user1@host1]` zu `[user2@host2]` sicher kopieren. Probiert haben wir es aber nicht...


### g) X-Option des ssh-Befehls

Laut `man ssh`: `-X Enables X11 forwarding.` (Vorsicht: das "X" muss groß geschrieben sein, sonst wird die Option deaktiviert). Das bedeutet, der remote client kann auf den lokalen X-Server zugreifen, und so die grafische Oberfläche sehen. So kann man zum Beispiel grafisch an einem remote-PC arbeiten, zum Beispiel an einem Server, der keine eigene Grafikausgabe besitzt.


### (*) Dateirechte

Dateizugriffsrechte werden in drei Kategorien geregelt: Der superuser kann in der Regel alle Dateien lesen und auch ändern. Der Owner einer Datei ebenfalls. Schließĺich gibt es noch eigene Zugriffsrechte für dritte, die meist keine Änderungen an Dateien vornehmen dürfen.

Schließlich ist noch geregelt, welche Dateien von wem ausgeführt werden dürfen.

Ändern lassen sich die Optionen mit `chmod`


### (*)

`df` listet die Speicherauslastung von Laufwerken. Ohne Argumente liefert der Befehl eine Liste von Dateisystemen mit der Anzahl der gesamten, verwendeten und verfügbaren Speicherblöcke, die prozentuale Belegung und den Ort, an dem das jeweilige System eingebunden ist.

`du` Gibt die ungefäre Speicherbelegung aller Dateien im aktuellen Verzeichnis aus. Vorsicht: Ausgabe kann lang werden...
