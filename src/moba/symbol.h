/*
 *  Project:    moba-lib-tracklayout
 *
 *  Copyright (C) 2019 Stefan Paproth <pappi-@gmx.de>
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Affero General Public License as
 *  published by the Free Software Foundation, either version 3 of the
 *  License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU Affero General Public License for more details.
 *
 *  You should have received a copy of the GNU Affero General Public License
 *  along with this program. If not, see <http://www.gnu.org/licenses/agpl.txt>.
 *
 */

#pragma once

#include <cstdint>
#include "direction.h"

class Symbol {
    public:
        Symbol(std::uint8_t symbol = 0);

        virtual ~Symbol() {
        }

        std::uint8_t getType() const {
            return symbolFix;
        }

        /**
         * rotiert ein Symbol mit "Überschlag" nach links. D.h. das letzte gesetzte Bit
         * rotiert wieder zum Anfang.
         *
         * @param int symbol
         * @return
         */
        std::uint8_t static rotate(std::uint8_t s);

        bool isSymbol() const;
        bool isStartSymbol() const;

        /**
         * Prüft, ob das Symbol ein Endsymbol (Prellbock) ist
         */
        bool isEnd() const;

        /**
         * Prüft, ob das Symbol ein gerades Gleis ist
         */
        bool isStraight() const;

        /**
         * Prüft, ob das Symobl eine einfache Kreuzung ist
         */
        bool isCrossOver() const;

        /**
         * Prüft, ob das Symbol gebogen ist
         */
        bool isBend() const;

        /**
         * Prüft, ob das Symbol ein einfaches Gleis ist (keine Weiche...)
         */
        bool isTrack() const;

        /**
         * Prüft, ob das Symob eine Doppelte Kreuzungsweiche ist
         */
        bool isCrossOverSwitch() const;

        /**
         * Prüft, ob Symbol eine Linksweiche ist
         */
        bool isLeftSwitch() const;

        /**
         * Prüft, ob Symbol eine Rechtsweiche ist
         */
        bool isRightSwitch() const;

        /**
         * Prüft, ob Symbol eine Weiche ist
         */
        bool isJunktionSwitch() const;

        /**
         * Prüft, ob Symbol eine Dreiwegweiche ist
         */
        bool isThreeWaySwitch() const;

        /**
         * Prüft, ob Symbol kein einfaches Gleis ist
         */
        bool isSwitch() const;

        /**
         * Prüft, ob Symbol ein gültiges Symbol ist
         */
        bool isValidSymbol() const;

        bool isJunctionSet(Direction d) const;
        bool isOpenJunctionSet(Direction dir) const;

        int getJunctionsCount() const;
        int getOpenJunctionsCount() const;

        Direction getNextJunction(Direction start = Direction::TOP) const;
        Direction getNextOpenJunction(Direction start = Direction::TOP) const;

        Direction getComplementaryDirection(Direction dir);

        void removeJunction(Direction curDir);

        operator bool() const;

        bool hasOpenJunctionsLeft() const;
        void reset();

    protected:
        std::uint8_t symbolFix; // Symbol mit festen Verbindungen
        std::uint8_t symbolDyn; // Symbol mit dynamischen Verbindungen

        /**
         * Prüft, ob ein bestimmtes Bit-Muster (z.B. für gerades / gebogenes Gleis)
         * gesetzt ist. Durch rotieren der Bits (@see rotate) kann jede Position durch
         * probiert werden. Simples Beispiel: Bei einem geraden Gleis sind die jeweiligen
         * Gegenüberliegende Verbindungspunkte gesetzt. Mit 4maligen rotieren hat man
         * sämtliche Kombinationen von geraden Gleisen durch
         *
         * @param int i anzahl iteration (2, 4, 8)
         * @param int b start bitmaske -> Das Startsymbol, was i-mal gedreht wird
         * @return bool true -> Symbol stimmt mit Vorlage überein, sonst false
         */
        bool check(std::uint8_t i, std::uint8_t b) const;

        /**
         * Gibt die Anzahl der Verbindungspunkte zurück
         */
        int countJunctions(std::uint8_t symbol) const;

        /**
         * Gibt die nächste offene Verbindung zurück
         */
        Direction nextJunction(std::uint8_t symbol, Direction start = Direction::TOP) const;
};


