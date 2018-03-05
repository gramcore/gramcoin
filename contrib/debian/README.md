
Debian
====================
This directory contains files used to package gramd/gram-qt
for Debian-based Linux systems. If you compile gramd/gram-qt yourself, there are some useful files here.

## gram: URI support ##


gram-qt.desktop  (Gnome / Open Desktop)
To install:

	sudo desktop-file-install gram-qt.desktop
	sudo update-desktop-database

If you build yourself, you will either need to modify the paths in
the .desktop file or copy or symlink your gram-qt binary to `/usr/bin`
and the `../../share/pixmaps/gram128.png` to `/usr/share/pixmaps`

gram-qt.protocol (KDE)

