# Contributing
## Installation
### Für Windows / Bash on Ubuntu on Windows (WSL)
Sowohl Git als auch Docker können im Terminal *Bash on Ubuntu on Windows* ausgeführt werden. Damit kann im Terminal am Projekt gearbeitet werden.
Unter diesem Link ist die Installation erklärt: (https://www.tecchannel.de/a/windows-10-subsystem-fuer-linux-wsl-einrichten,3277816)
Als nächstes muss Git im WSL installiert werden. Öffne dazu Bash mit Administratorrechten (Aufs Startsymbol klicken und in die Suche *bash* eingeben, dann Rechtsklick und mit Administratorrechten ausführen).
Installiere dann Git und klone das Verzeichnis:

	$ sudo apt-get install git
	$ git clone https://github.com/tobjaw/distillation-automation.git

Die Entwicklungstools sind im Moment noch in feat/devtools. Daher muss dieser Branch getrackt werden:

	$ git checkout --track origin/feat/devtools

Als nächstes muss Docker installiert werden:

	$ sudo apt install docker.io
	$ sudo usermod -aG docker $USER

Anschließend muss der Docker-Deamon gestartet werden:

	$ sudo cgroupfs-mount
	$ sudo service docker start

Das Starten des Daemons muss wahrscheinlich nach jedem Neustart vorgenommen werden.
Nun können die Skripte aus dem bin-Verzeichnis ausgeführt werden, zum Beispiel:

	$ sudo ./bin/build.sh

oder

	$ sudo ./bin/run.sh


## Etwas über Git
Möchte man ein Issue bearbeiten, erstellt man dafür ersteinmal einen Branch

	$ git checkout -b <Name des Branches, am besten issue-23 zum Beispiel>

Git wechselt dann auch gleich automatisch in den neuen Branch.
Wurde der Code dann geändert, kann der Status abgefragt werden:

	$ git status

Hier werden veränderte Dateien angezeigt sowie Dateien, die bisher noch nicht von Git
verfolgt werden - diese müssen bei Bedarf mithilfe von `git add` hinzugefügt werden.
Anschließend kann commitet werden:

	$ git commit -am "<Kommentar>"

Das erste pushen muss mit

	$ git push -u origin <Name des Branches, hier issue-23>

erfolgen, danach reicht `git push` aus.

Nach getaner Arbeit kann auf der Github-Seite ein Pull Request ausgeführt werden.
Wurde der Branch dann gemergt, so kann er lokal gelöscht werden mit:

	$ git branch -d <Name des Branches>

## Empfangen über UART

sudo cu -l /dev/cu.usbserial-AK05A8A6 -s 9600
