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

#include "node.h"
#include "direction.h"

struct SimpleSwitch: public Node {

    using Node::Node;

    virtual ~SimpleSwitch() noexcept = default;

    void setJunctionNode(Direction dir, NodePtr node) {
        switch(dir) {
            case Direction::TOP:
                outStraight = node;
                return;

            case Direction::TOP_LEFT:
            case Direction::TOP_RIGHT:
                outBend = node;
                return;

            case Direction::BOTTOM:
                in = node;
                return;
        }
        throw NodeException{"invalid direction given!"};
    }

    NodePtr getJunctionNode(NodePtr node) const {
        if(node != in && node != outStraight && node != outBend) {
            throw NodeException{"invalid node given!"};
        }
        if(
            node == outStraight && (
                currentState == moba::SwitchStand::STRAIGHT_1 || 
                currentState == moba::SwitchStand::STRAIGHT_2
            )
        ) {
            return in;
        }
        
        if(
            node == outBend && (
                currentState == moba::SwitchStand::BEND_1 || 
                currentState == moba::SwitchStand::BEND_2
            )
        ) {
            return in;
        }
        
        if(
            node == in && (
                currentState == moba::SwitchStand::BEND_1 ||
                currentState == moba::SwitchStand::BEND_2
            )
        ) {
            return outBend;
        }
        
        if(
            node == in && (
                currentState == moba::SwitchStand::STRAIGHT_1 ||
                currentState == moba::SwitchStand::STRAIGHT_2
            )
        ) {
            return outStraight;
        }
        return NodePtr{};
    }

    NodePtr getJunctionNode(Direction dir) const {
        switch(dir) {
            case Direction::TOP:
                return outStraight;

            case Direction::TOP_LEFT:
            case Direction::TOP_RIGHT:
                return outBend;

            case Direction::BOTTOM:
                return in;
        }
        throw NodeException{"invalid direction given!"};
    }
    
protected:
    NodePtr in;
    NodePtr outStraight;
    NodePtr outBend;
};
