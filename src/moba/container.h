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

#include <exception>
#include <string>
#include <map>
#include <memory>
#include <functional>

#include "position.h"
#include "symbol.h"

class ContainerException : public std::exception {
    public:
        explicit ContainerException(const std::string &err) throw() : what_{err} {
        }

        ContainerException() throw() : what_{"Unknown error"} {
        }

        virtual ~ContainerException() throw() {
        }

        virtual const char *what() const throw() {
            return this->what_.c_str();
        }

    private:
        std::string what_;
};

template<typename T>
class Container {
    public:
        Container() {
        }

        virtual ~Container() {
        }

        std::size_t getHeight() const {
            return maxPosition.y;
        }

        std::size_t getWidth() const {
            return maxPosition.x;
        }

        void addItem(const Position &pos, T item) {
            items[pos.x][pos.y] = item;
            maxPosition.grow(pos);
        }

        T get(const Position &pos) {
            if(items[pos.x][pos.y] == nullptr) {
                throw ContainerException{"no valid item"};
            }
            return items[pos.x][pos.y];
        }

        std::size_t itemsCount() const {
            std::size_t cnt = 0;
            for(std::size_t y = 0; y < maxPosition.y; ++y) {
                for(std::size_t x = 0; x < maxPosition.x; ++x) {
                    if(static_cast<bool>(items[x][y])) {
                        cnt++;;
                    }
                }
            }
            return cnt;
        }

        Position getNextFreePosition() {
            for(std::size_t y = 0; y < maxPosition.y; ++y) {
                for(std::size_t x = 0; x < maxPosition.x; ++x) {
                    if(!static_cast<bool>(items[x][y])) {
                        return {x, y};
                    }
                }
            }
            throw ContainerException{"No position found!"};
        }

        Position getNextBoundPosition() {
            for(std::size_t y = 0; y < maxPosition.y; ++y) {
                for(std::size_t x = 0; x < maxPosition.x; ++x) {
                    if(static_cast<bool>(items[x][y])) {
                        return {x, y};
                    }
                }
            }
            throw ContainerException{"No position found!"};
        }

        Position getNextMatchPosition(std::function<bool(const T&)> fn) {
            for(std::size_t y = 0; y < maxPosition.y; ++y) {
                for(std::size_t x = 0; x < maxPosition.x; ++x) {
                    if(fn(items[x][y])) {
                        return {x, y};
                    }
                }
            }
            throw ContainerException{"No position found!"};
        }

    protected:
        Position maxPosition = {0, 0};

        std::map<std::size_t, std::map<std::size_t, T>> items;
};
