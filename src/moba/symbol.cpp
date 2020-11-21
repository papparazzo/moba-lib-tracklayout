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

#include "symbol.h"

#include <exception>
#include <stdexcept>

Symbol::Symbol(std::uint8_t symbol) : symbolFix{symbol}, symbolDyn{symbol} {
    if(isSymbol() && !isValidSymbol()) {
        throw std::invalid_argument("invalid symbol given");
    }
}

Symbol::operator bool() const {
    return isSymbol();
}

bool Symbol::isSymbol() const {
    if(symbolFix != 0) {
        return true;
    }
    return false;
}

bool Symbol::isStartSymbol() const {
    if(symbolFix & Direction::LEFT) {
        return false;
    }
    if(symbolFix & Direction::TOP_LEFT) {
        return false;
    }
    if(symbolFix & Direction::TOP) {
        return false;
    }
    if(symbolFix & Direction::TOP_RIGHT) {
        return false;
    }
    return true;
}

bool Symbol::check(std::uint8_t i, std::uint8_t b) const {
    while(i--) {
        if(symbolFix == b) {
            return true;
        }
        b = Symbol::rotate(b);
    }
    return false;
}

bool Symbol::isEnd() const {
    return check(8, Direction::TOP);
}

bool Symbol::isStraight() const {
    return check(4, Direction::TOP | Direction::BOTTOM);
}

bool Symbol::isCrossOver() const {
    return check(2, Direction::TOP | Direction::BOTTOM | Direction::RIGHT | Direction::LEFT);
}

bool Symbol::isBend() const {
    return check(8, Direction::RIGHT | Direction::BOTTOM_LEFT);
}

bool Symbol::isTrack() const {
    if(isStraight()) {
        return true;
    }
    if(isCrossOver()) {
        return true;
    }
    if(isBend()) {
        return true;
    }
    if(isEnd()) {
        return true;
    }
    return false;
}

bool Symbol::isCrossOverSwitch() const {
    return check(4, Direction::RIGHT | Direction::LEFT | Direction::TOP_RIGHT | Direction::BOTTOM_LEFT);
}

bool Symbol::isLeftSwitch() const {
    return check(8, Direction::RIGHT | Direction::LEFT | Direction::TOP_RIGHT);
}

bool Symbol::isRightSwitch() const {
    return check(8, Direction::RIGHT | Direction::LEFT | Direction::BOTTOM_RIGHT);
}

bool Symbol::isJunktionSwitch() const {
    if(isRightSwitch()) {
        return true;
    }
    if(isLeftSwitch()) {
        return true;
    }
    return false;
}

bool Symbol::isThreeWaySwitch() const {
    return check(8, Direction::TOP | Direction::BOTTOM | Direction::TOP_RIGHT | Direction::TOP_LEFT);
}

bool Symbol::isSwitch() const {
    if(isCrossOverSwitch()) {
        return true;
    }
    if(isJunktionSwitch()) {
        return true;
    }
    if(isThreeWaySwitch()) {
        return true;
    }
    return false;
}

bool Symbol::isValidSymbol() const {
    if(isTrack()) {
        return true;
    }
    if(isSwitch()) {
        return true;
    }
    return false;
}

int Symbol::getJunktionsCount() const {
    return countJunktions(symbolFix);
}

int Symbol::getOpenJunktionsCount() const {
    return countJunktions(symbolDyn);
}

Direction Symbol::getNextJunktion(Direction start) const {
    return nextJunktion(symbolFix, start);
}

bool Symbol::hasOpenJunctionsLeft() const {
    return static_cast<bool>(symbolDyn);
}

Direction Symbol::getNextOpenJunktion(Direction start) const {
    return nextJunktion(symbolDyn, start);
}

void Symbol::reset() {
    symbolDyn = symbolFix;
}

bool Symbol::isJunctionSet(Direction dir) const {
    return symbolDyn & static_cast<std::uint8_t>(dir);
}

bool Symbol::areJunctionsSet(std::uint8_t junctions) const {
    return (junctions == (symbolDyn & junctions));
}

bool Symbol::isOpenJunctionSet(Direction dir) const {
    return symbolFix & static_cast<std::uint8_t>(dir);
}

bool Symbol::areOpenJunctionsSet(std::uint8_t junctions) const {
    return (junctions == (symbolFix & junctions));
}

void Symbol::removeJunktion(Direction dir) {
     if(!(symbolDyn & static_cast<std::uint8_t>(dir))) {
         throw std::out_of_range("junction not set");
     }
     symbolDyn &= ~static_cast<std::uint8_t>(dir);
}

std::uint8_t Symbol::rotate(std::uint8_t symbol) {
    if(symbol & 0x80) { // if last bit (Most significant bit) is set rotate it to bit 0
        return (symbol << 1) | 0x1;
    }
    return symbol << 1;
}

int Symbol::countJunktions(std::uint8_t symbol) const {
    int counter = 0;
    auto b = static_cast<std::uint8_t>(Direction::TOP);
    for(int i = 0; i < 8; ++i) {
        if(symbol & b) {
            ++counter;
        }
        b = rotate(b);
    }
    return counter;
}

Direction Symbol::nextJunktion(std::uint8_t symbol, Direction start) const {
    auto b = static_cast<std::uint8_t>(start);
    for(int i = 0; i < 8; ++i) {
        if(symbol & b) {
            return static_cast<Direction>(b);
        }
        b = rotate(b);
    }
    return Direction::UNSET;
}