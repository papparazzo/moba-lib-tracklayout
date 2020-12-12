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

Symbol Symbol::getLeftSwitch() {
    return Symbol{Direction::TOP | Direction::BOTTOM | Direction::TOP_LEFT};
}

Symbol Symbol::getRightSwitch() {
    return Symbol{Direction::TOP | Direction::BOTTOM | Direction::TOP_RIGHT};
}

Symbol Symbol::getStraight() {
    return Symbol{Direction::TOP | Direction::BOTTOM};
}

Symbol Symbol::getThreeWaySwitch() {
    return Symbol{Direction::TOP | Direction::BOTTOM | Direction::TOP_LEFT | Direction::TOP_RIGHT};
}

Symbol Symbol::getCrossOverSwitch() {
    return Symbol{Direction::TOP | Direction::BOTTOM | Direction::TOP_RIGHT | Direction::BOTTOM_LEFT};
}

void Symbol::rotateLeft(std::uint8_t count) {
    count &= 7;
    symbolFix = (symbolFix << count) | (symbolFix >> (-count & 7));
    symbolDyn = (symbolDyn << count) | (symbolDyn >> (-count & 7));
}

void Symbol::rotateRight(unsigned int count) {
    count &= 7;
    symbolFix = (symbolFix >> count) | (symbolFix << (-count & 7));
    symbolDyn = (symbolDyn >> count) | (symbolDyn << (-count & 7));
}

std::uint8_t Symbol::getDistance(Symbol symbol) const {
    for(std::uint8_t i = 0; i < 8; ++i) {
        if(symbolFix == symbol.symbolFix) {
            return i;
        }
        symbol.rotateRight();
    }
    throw std::invalid_argument("given symbol does not match");
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

bool Symbol::check(std::uint8_t i, Symbol symbol) const {
    while(i--) {
        if(symbolFix == symbol.symbolFix) {
            return true;
        }
        symbol.rotateRight();
    }
    return false;
}

bool Symbol::isEnd() const {
    return check(8, Direction::TOP);
}

bool Symbol::isStraight() const {
    return check(4, Symbol::getStraight());
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
    return check(4, Symbol::getCrossOverSwitch());
}

bool Symbol::isLeftSwitch() const {
    return check(8, Symbol::getLeftSwitch());
}

bool Symbol::isRightSwitch() const {
    return check(8, Symbol::getRightSwitch());
}

bool Symbol::isSimpleSwitch() const {
    if(isRightSwitch()) {
        return true;
    }
    if(isLeftSwitch()) {
        return true;
    }
    return false;
}

bool Symbol::isThreeWaySwitch() const {
    return check(8, Symbol::getThreeWaySwitch());
}

bool Symbol::isSwitch() const {
    if(isCrossOverSwitch()) {
        return true;
    }
    if(isSimpleSwitch()) {
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

std::uint8_t Symbol::getJunctionsCount() const {
    return countJunctions(symbolFix);
}

std::uint8_t Symbol::getOpenJunctionsCount() const {
    return countJunctions(symbolDyn);
}

Direction Symbol::getNextJunction(Direction start) const {
    return nextJunction(symbolFix, start);
}

bool Symbol::hasOpenJunctionsLeft() const {
    return static_cast<bool>(symbolDyn);
}

Direction Symbol::getNextOpenJunction(Direction start) const {
    return nextJunction(symbolDyn, start);
}

void Symbol::reset() {
    symbolDyn = symbolFix;
}

bool Symbol::isBlockSymbol() const {
    return false;
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

void Symbol::removeJunction(Direction dir) {
     if(!(symbolDyn & static_cast<std::uint8_t>(dir))) {
         throw std::out_of_range{"junction not set"};
     }
     symbolDyn &= ~static_cast<std::uint8_t>(dir);
}

std::uint8_t Symbol::countJunctions(std::uint8_t symbol) const {
    std::uint8_t counter = 0;
    auto b = static_cast<std::uint8_t>(Direction::TOP);
    for(std::uint8_t i = 0; i < 8; ++i) {
        if(symbol & b) {
            ++counter;
        }
        b <<= 1;
    }
    return counter;
}

Direction Symbol::nextJunction(std::uint8_t symbol, Direction start) const {
    auto b = static_cast<std::uint8_t>(start);
    for(std::uint8_t i = 0; i < 8; ++i) {
        if(symbol & b) {
            return static_cast<Direction>(b);
        }
        b = (b << 1) | (b >> 7);
    }
    return Direction::UNSET;
}
