/*
 *  Project:    moba-lib-tracklayout
 * 
 *  Copyright (C) 2023 Stefan Paproth <pappi-@gmx.de>
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

#include <memory>
#include <moba-common/enumswitchstand.h>

#include "direction.h"
#include "nodeexception.h"

struct Node;
using NodePtr = std::shared_ptr<Node>;

struct Node {
    Node(unsigned int id, moba::SwitchStand switchStand = moba::SwitchStand::STRAIGHT_1): 
    id{id}, currentState{switchStand} {
    }

    virtual ~Node() noexcept = default;
    virtual NodePtr getJunctionNode(NodePtr node) const = 0;
    virtual NodePtr getJunctionNode(Direction dir) const = 0;
    virtual void setJunctionNode(Direction dir, NodePtr node) = 0;

    void turn(moba::SwitchStand stand) {
        currentState = stand;
    }

    unsigned int getId() const {
        return id;
    }

protected:
    unsigned int id;
    moba::SwitchStand currentState;

};
