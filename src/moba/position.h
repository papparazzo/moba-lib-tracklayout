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

#include <cstddef>
#include <iostream>

#include "direction.h"

struct Position {
    constexpr Position() : x{0}, y{0} {
    }

    constexpr Position(size_t x, size_t y) : x{x}, y{y} {
    }

    Position(Direction dir) : x{0}, y{0} {
        setNewPosition(dir);
    }

    std::size_t x;
    std::size_t y;

    friend std::ostream& operator<< (std::ostream& os, const Position& pos) {
        os << "x:" << pos.x << ", y:" << pos.y;
        return os;
    }

    friend bool operator==(const Position& lhs, const Position& rhs) {
        return lhs.x == rhs.x && lhs.y == rhs.y;
    }
    friend bool operator!=(const Position& lhs, const Position& rhs) {
        return !(lhs == rhs);
    }

    friend bool operator<(const Position& lhs, const Position& rhs) {
        if(lhs.y < rhs.y) {
            return true;
        }
        if(lhs.y > rhs.y) {
            return false;
        }
        if(lhs.x < rhs.x) {
            return true;
        }
        return false;
    }

    friend bool operator>(const Position& lhs, const Position& rhs) {
        return (rhs < lhs);
    }

    friend bool operator<=(const Position& lhs, const Position& rhs) {
        return !(rhs < lhs);
    }

    friend bool operator>=(const Position& lhs, const Position& rhs) {
        return !(lhs < rhs);
    }

    Position getDistance(const Position &pos) const {
        return {pos.x - x, pos.y - y};
    }

    Position grow(const Position &pos) {
        x = pos.x > x ? pos.x : x;
        y = pos.y > y ? pos.y : y;
        return {x, y};
    }

    /**
     * setzt den Cursor (Position) in die Richtung welche mit Direction
     * angegeben ist. Beispiel: Direction RIGHT -> x einen weiter nach rechts
     * @param d
     */
    void setNewPosition(Direction d) {
        switch(d) {
            case Direction::UNSET:
                return;

            case Direction::TOP_RIGHT:
                x++;  // fall-through

            case Direction::TOP:
                y--;
                break;

            case Direction::BOTTOM_RIGHT:
                y++; // fall-through

            case Direction::RIGHT:
                x++;
                return;

            case Direction::BOTTOM:
                y++;
                return;

            case Direction::BOTTOM_LEFT:
                y++;  // fall-through

            case Direction::LEFT:
                x--;
                break;

            case Direction::TOP_LEFT:
                y--;
                x--;
                break;
        }
    }
};
