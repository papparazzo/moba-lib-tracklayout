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
    enum SymbolType {
        END               = Direction::TOP,
        STRAIGHT          = Direction::TOP | Direction::BOTTOM,
        RIGHT_SWITCH      = Direction::TOP | Direction::BOTTOM | Direction::TOP_RIGHT,
        CROSS_OVER_SWITCH = Direction::TOP | Direction::BOTTOM | Direction::TOP_RIGHT | Direction::BOTTOM_LEFT,
        LEFT_SWITCH       = Direction::TOP | Direction::BOTTOM | Direction::TOP_LEFT,
        THREE_WAY_SWITCH  = Direction::TOP | Direction::BOTTOM | Direction::TOP_LEFT | Direction::TOP_RIGHT,
        CROSS_OVER        = Direction::TOP | Direction::BOTTOM | Direction::RIGHT | Direction::LEFT,
        BEND              = Direction::TOP | Direction::BOTTOM_LEFT,
    };

    explicit Symbol(std::uint8_t symbol = 0);

    explicit Symbol(SymbolType symbol);

    virtual ~Symbol() noexcept = default;

    [[nodiscard]] std::uint8_t getType() const {
        return symbolFix;
    }

    /**
     * rotiert ein Symbol mit "Überschlag" nach links. D.h. das letzte gesetzte Bit
     * rotiert wieder zum Anfang.
     *
     * @param std::uint8_t count
     */
    void rotateLeft(std::uint8_t count = 1);

    /**
     * rotiert ein Symbol mit "Überschlag" nach rechts. D.h. das erste gesetzte Bit
     * rotiert wieder zum En de.
     *
     * @param std::uint8_t count
     */
    void rotateRight(unsigned int count = 1);

    /**
     * Prüft den Abstand zum übrergebenen Symbol
     */
    [[nodiscard]] std::uint8_t getDistance(Symbol symbol) const;

    /**
     * Prüft, ob ein Symbol gesetzt ist
     */
    [[nodiscard]] bool isSymbol() const;

    /**
     * Prüft, ob ein Symbol auf der linken oberen Seite Anschlüsse hat
     */
    [[nodiscard]] bool isStartSymbol() const;

    /**
     * Prüft, ob das Symbol ein Endsymbol (Prellbock) ist
     */
    [[nodiscard]] bool isEnd() const;

    /**
     * Prüft, ob das Symbol ein gerades Gleis ist
     */
    [[nodiscard]] bool isStraight() const;

    /**
     * Prüft, ob das Symobl eine einfache Kreuzung ist
     */
    [[nodiscard]] bool isCrossOver() const;

    /**
     * Prüft, ob das Symbol gebogen ist
     */
    [[nodiscard]] bool isBend() const;

    /**
     * Prüft, ob das Symbol ein einfaches Gleis ist (keine Weiche ...)
     */
    [[nodiscard]] bool isTrack() const;

    /**
     * Prüft, ob das Symob eine Doppelte Kreuzungsweiche ist
     */
    [[nodiscard]] bool isCrossOverSwitch() const;

    /**
     * Prüft, ob Symbol eine Linksweiche ist
     */
    [[nodiscard]] bool isLeftSwitch() const;

    /**
     * Prüft, ob Symbol eine Rechtsweiche ist
     */
    [[nodiscard]] bool isRightSwitch() const;

    /**
     * Prüft, ob Symbol eine Weiche ist
     */
    [[nodiscard]] bool isSimpleSwitch() const;

    /**
     * Prüft, ob Symbol eine Dreiwegweiche ist
     */
    [[nodiscard]] bool isThreeWaySwitch() const;

    /**
     * Prüft, ob Symbol kein einfaches Gleis ist
     */
    [[nodiscard]] bool isSwitch() const;

    /**
     * Prüft, ob Symbol ein gültiges Symbol ist
     */
    [[nodiscard]] bool isValidSymbol() const;

    [[nodiscard]] bool isJunctionSet(Direction d) const;
    bool areJunctionsSet(std::uint8_t junctions) const;

    bool isOpenJunctionSet(Direction dir) const;
    bool areOpenJunctionsSet(std::uint8_t junctions) const;

    std::uint8_t getJunctionsCount() const;
    std::uint8_t getOpenJunctionsCount() const;

    Direction getNextJunction(Direction start = Direction::TOP_LEFT) const;
    [[nodiscard]] Direction getNextOpenJunction(Direction start = Direction::TOP_LEFT) const;

    Direction getComplementaryDirection(Direction dir);

    bool removeJunction(Direction curDir);

    explicit operator bool() const;

    [[nodiscard]] bool hasOpenJunctionsLeft() const;
    void reset();

protected:
    std::uint8_t symbolFix; // Symbol mit festen Verbindungen
    std::uint8_t symbolDyn; // Symbol mit dynamischen Verbindungen

    /**
     * Prüft, ob ein bestimmtes Bit-Muster (z.B. für gerades / gebogenes Gleis)
     * gesetzt ist. Durch Rotieren der Bits Symbol::rotate kann jede Position durch-
     * probiert werden. Simples Beispiel: Bei einem geraden Gleis sind die jeweiligen
     * gegenüberliegende Verbindungspunkte gesetzt. Mit 4-maligen rotieren hat man
     * sämtliche Kombinationen von geraden Gleisen durch
     *
     * @param std::uint8_t i anzahl iteration (2, 4, 8)
     * @param std::uint8_t b start bitmaske -> Das Startsymbol, was i-mal gedreht wird
     * @return bool true -> Symbol stimmt mit Vorlage überein, sonst false
     */
    [[nodiscard]] bool check(std::uint8_t i, std::uint8_t b) const;

    [[nodiscard]] bool check(const Symbol &symbol) const;

    /**
     * Gibt die Anzahl der Verbindungspunkte zurück
     */
    std::uint8_t countJunctions(std::uint8_t symbol) const;

    /**
     * Gibt die nächste offene Verbindung zurück
     */
    Direction nextJunction(std::uint8_t symbol, Direction start = Direction::TOP) const;
};
