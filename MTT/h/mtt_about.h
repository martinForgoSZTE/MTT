#pragma once

#include <QString>
#include <QTextCodec>
#include <QByteArray>

namespace About
{
    inline const QString& AboutThesis = u8"A szakdolgozatom t�m�jak�nt a feladat egy olyan alkalmaz�s elk�sz�t�se, mely a Magyarorsz�gra vonatkoz� statisztikai adatokat grafikonosan jelen�ti meg. A c�l az, hogy egyszer�en lehessen a k�l�nb�z� statisztikai adatokat megjelen�teni, el�seg�tve az �tl�that�s�gukat."
        "\n\nA programban egy Magyarorsz�g t�rk�p jelenik meg a v�gfelhaszn�l�nak, a megyek�zpontokkal.A v�rosokat line�ris interpol�ci�val sz�molom ki �s helyezem el a t�rk�pen."
        "\n\nA k�l�nb�z�, Magyarorsz�g megy�ire vonatakoz� statisztikai adatokat adott form�tum� f�jlban t�rolva bet�ltheti a felhaszn�l�.A bet�lt�s ut�n az adatokat megy�kre lebontva jelen�theti meg k�l�nb�z� grafikonokon.A f�bb v�rosok a megy�vel a t�rk�pen ki�r�dnak, melyeket kiv�lasztva adhatja hozz� a felhaszn�l� a grafikon(ok)hoz."
        "\n\nA rendszer nyitott lesz az adott form�tum� adatf�jlokra."
        "\n\nA statisztikai adatok bet�lt�s�t k�vet�en lehet�s�g lesz azok permanens elment�s�re adatb�zis haszn�lat�val.SQLite adatb�ziskezel� rendszert fogok haszn�lni.Ezek ut�n adatb�zisb�l is bet�lthet�ek lesznek a statisztikai adatok.Az adatt�bl�kban t�rolt rekordok k�s�bb m�dos�that�ak lesznek a felhaszn�l� sz�m�ra.";

    inline const QString& ABoutLicense = u8"This work is licensed under the Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License."
        "\nTo view a copy of this license, visit http://creativecommons.org/licenses/by-nc-sa/4.0/."
        "\n\nCopyright � 2021 Forg� Martin";
}
