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

#include <string>
#include <cstdint>
#include <stdexcept>
#include <ostream>

struct Direction {
    enum Position {
        UNSET        = 0,
        TOP          = 1,
        TOP_RIGHT    = 2,
        RIGHT        = 4,
        BOTTOM_RIGHT = 8,

        BOTTOM       = 16,
        BOTTOM_LEFT  = 32,
        LEFT         = 64,
        TOP_LEFT     = 128
    };

    constexpr Direction() : position{UNSET} {
    }

    constexpr Direction(Position position) : position{position} {
    }

    Direction(std::uint8_t position) {
        switch(position) {
            case UNSET:
            case TOP:
            case TOP_RIGHT:
            case RIGHT:
            case BOTTOM_RIGHT:

            case BOTTOM:
            case BOTTOM_LEFT:
            case LEFT:
            case TOP_LEFT:
                this->position = position;
                break;

            default:
                throw std::out_of_range{"invalid position given"};
        }
    }

    static Direction getDirectionByArrowKeys(int key) {
        switch(key) {
            case 1:
                return Direction::BOTTOM_LEFT;
            case 2:
                return Direction::BOTTOM;
            case 3:
                return Direction::BOTTOM_RIGHT;
            case 4:
                return Direction::LEFT;
            case 5:
                return Direction::UNSET;
            case 6:
                return Direction::RIGHT;
            case 7:
                return Direction::TOP_LEFT;
            case 8:
                return Direction::TOP;
            case 9:
                return Direction::TOP_RIGHT;
        }
    }

    operator int() const {
        return position;
    }

    Direction& operator++() {
        position = (position << 1) | (position >> 7);
        return *this;
    }

    Direction operator++(int) {
        Direction tmp(*this);
        operator++();
        return tmp;
    }

    Direction& operator+=(std::uint8_t steps) {
        steps &= 7;
        position = (position << steps) | (position >> (-steps & 7));
        return *this;
    }

    friend Direction operator+(Direction lhs, std::uint8_t steps) {
        lhs += steps;
        return lhs;
    }

    Direction& operator--() {
        position = (position >> 1) | (position << 7);
        return *this;
    }

    Direction operator--(int) {
        Direction tmp(*this);
        operator--();
        return tmp;
    }

    Direction& operator-=(std::uint8_t steps) {
        steps &= 7;
        position = (position >> steps) | (position << (-steps & 7));
        return *this;
    }

    friend Direction operator-(Direction lhs, std::uint8_t steps) {
        lhs -= steps;
        return lhs;
    }

    friend std::ostream& operator<<(std::ostream& os, const Direction& dir) {
        switch(dir.position) {
            case Direction::UNSET:
                return os << "UNSET";

            case Direction::TOP:
                return os << "TOP";

            case Direction::TOP_RIGHT:
                return os << "TOP_RIGHT";

            case Direction::RIGHT:
                return os << "RIGHT";

            case Direction::BOTTOM_RIGHT:
                return os << "BOTTOM_RIGHT";

            case Direction::BOTTOM:
                return os << "BOTTOM";

            case Direction::BOTTOM_LEFT:
                return os << "BOTTOM_LEFT";

            case Direction::LEFT:
                return os << "LEFT";

            case Direction::TOP_LEFT:
                return os << "TOP_LEFT";

            default:
                throw std::out_of_range{"invalid position given"};
        }
    }

    enum DistanceType {
        INVALID = 0,
        STRAIGHT = 1,
        LEFT_BEND = 2,
        RIGHT_BEND = 3
    };

    /**
    * Die Distanz zwischen zwei Verbindungspunkte muss mindestens 3 Bit betragen, damit
    * zwei 2 Verbindungspunkte (auch als Teil einer Weiche) ein gültiges Gleis bilden.
    * Zu einem Verbindungspunkt "dirIn" kommen nur 3 mögliche Verbindungspunkte "dirOut" in Frage:
    * 1. Der komplemntäre Verbindungspunkt (also ein gerades Gleis)
    * 2. Der komplemntäre Verbindungspunkt + 1 Bit (also rechts gebogenes Gleis)
    * 3. Der komplemntäre Verbindungspunkt - 1 Bit (also links gebogenes Gleis)
    *
    * @param dir1
    * @param dir2
    * @return DistanceType
    */
    inline DistanceType getDistanceType(Direction dirOut) {
       auto dirInCom = getComplementaryDirection();

       if(dirOut == ++dirInCom) {
           return DistanceType::RIGHT_BEND;
       }

       if(dirOut == --dirInCom) {
           return DistanceType::STRAIGHT;
       }

       if(dirOut == --dirInCom) {
           return DistanceType::LEFT_BEND;
       }

       return DistanceType::INVALID;
    }

    Direction getComplementaryDirection() const {
        return (position << 4) | (position >> 4);
    }

protected:
    std::uint8_t position;

};
