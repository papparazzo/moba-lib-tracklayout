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

    constexpr Direction(): position{UNSET} {
    }

    Direction(const Position position):
    position{static_cast<std::uint8_t>(position)} {
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
            default:
                throw std::out_of_range{"invalid value given"};
        }
    }

    operator int() const {
        return position;
    }

    Direction& operator++() {
        position = position << 1 | position >> 7;
        return *this;
    }

    Direction operator++(int) {
        const Direction tmp(*this);
        operator++();
        return tmp;
    }

    Direction& operator+=(std::uint8_t steps) {
        steps &= 7;
        position = position << steps | position >> (-steps & 7);
        return *this;
    }

    friend Direction operator+(Direction lhs, const std::uint8_t steps) {
        lhs += steps;
        return lhs;
    }

    Direction& operator--() {
        position = position >> 1 | position << 7;
        return *this;
    }

    Direction operator--(int) {
        const Direction tmp(*this);
        operator--();
        return tmp;
    }

    Direction& operator-=(std::uint8_t steps) {
        steps &= 7;
        position = position >> steps | position << (-steps & 7);
        return *this;
    }

    friend Direction operator-(Direction lhs, const std::uint8_t steps) {
        lhs -= steps;
        return lhs;
    }

    friend std::ostream& operator<<(std::ostream& os, const Direction& dir) {
        switch(dir.position) {
            case UNSET:
                return os << "UNSET";

            case TOP:
                return os << "TOP";

            case TOP_RIGHT:
                return os << "TOP_RIGHT";

            case RIGHT:
                return os << "RIGHT";

            case BOTTOM_RIGHT:
                return os << "BOTTOM_RIGHT";

            case BOTTOM:
                return os << "BOTTOM";

            case BOTTOM_LEFT:
                return os << "BOTTOM_LEFT";

            case LEFT:
                return os << "LEFT";

            case TOP_LEFT:
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
    * Die Distanz zwischen zwei Verbindungspunkten muss mindestens 3 Bits betragen, damit
    * zwei 2 Verbindungspunkte (auch als Teil einer Weiche) ein gültiges Gleis bilden.
    * Zu einem Verbindungspunkt "dirIn" kommen nur 3 mögliche Verbindungspunkte "dirOut" infrage:
    * 1. Der komplementäre Verbindungspunkt (also ein gerades Gleis)
    * 2. Der komplementäre Verbindungspunkt + 1 Bit (also rechts gebogenes Gleis)
    * 3. Der komplementäre Verbindungspunkt - 1 Bit (also links gebogenes Gleis)
    *
    * @param dirOut
    * @return DistanceType
    */
    [[nodiscard]]
    DistanceType getDistanceType(const Direction dirOut) const {
       auto dirInCom = getComplementaryDirection();

       if(dirOut == ++dirInCom) {
           return RIGHT_BEND;
       }

       if(dirOut == --dirInCom) {
           return STRAIGHT;
       }

       if(dirOut == --dirInCom) {
           return LEFT_BEND;
       }

       return INVALID;
    }

    [[nodiscard]]
    Direction getComplementaryDirection() const {
        return position << 4 | position >> 4;
    }

protected:
    std::uint8_t position;

};
