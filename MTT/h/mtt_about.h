#pragma once

#include <QString>
#include <QTextCodec>
#include <QByteArray>

namespace About
{
    inline const QString& AboutThesis = u8"A szakdolgozatom témájaként a feladat egy olyan alkalmazás elkészítése, mely a Magyarországra vonatkozó statisztikai adatokat grafikonosan jeleníti meg. A cél az, hogy egyszerûen lehessen a különbözõ statisztikai adatokat megjeleníteni, elõsegítve az átláthatóságukat."
        "\n\nA programban egy Magyarország térkép jelenik meg a végfelhasználónak, a megyeközpontokkal.A városokat lineáris interpolációval számolom ki és helyezem el a térképen."
        "\n\nA különbözõ, Magyarország megyéire vonatakozó statisztikai adatokat adott formátumú fájlban tárolva betöltheti a felhasználó.A betöltés után az adatokat megyékre lebontva jelenítheti meg különbözõ grafikonokon.A fõbb városok a megyével a térképen kiíródnak, melyeket kiválasztva adhatja hozzá a felhasználó a grafikon(ok)hoz."
        "\n\nA rendszer nyitott lesz az adott formátumú adatfájlokra."
        "\n\nA statisztikai adatok betöltését követõen lehetõség lesz azok permanens elmentésére adatbázis használatával.SQLite adatbáziskezelõ rendszert fogok használni.Ezek után adatbázisból is betölthetõek lesznek a statisztikai adatok.Az adattáblákban tárolt rekordok késõbb módosíthatóak lesznek a felhasználó számára.";

    inline const QString& ABoutLicense = u8"This work is licensed under the Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License."
        "\nTo view a copy of this license, visit http://creativecommons.org/licenses/by-nc-sa/4.0/."
        "\n\nCopyright © 2021 Forgó Martin";
}
