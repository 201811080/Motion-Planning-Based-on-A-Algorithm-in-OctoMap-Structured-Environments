#ifndef NODE_H_INCLUDED
#define NODE_H_INCLUDED

#include <random>
#include <chrono>
#include "Point.h"

struct Node{
    Point topLeft;
    Point botRight;
    bool occupied = false;
    int depth;

    Node* parent = nullptr;
    Node* topLeftTree = nullptr;
    Node* topRightTree = nullptr;
    Node* botLeftTree = nullptr;
    Node* botRightTree = nullptr;

    double fScore = std::numeric_limits<double>::infinity();
    double gScore = std::numeric_limits<double>::infinity();
    Node* cameFrom = nullptr;

    Node(Node* parent, Point topLeft, Point botRight, int depth, bool occupied = false) : parent(parent), topLeft(topLeft), botRight(botRight), occupied(occupied), depth(depth){}

    ~Node(){
        delete topLeftTree;
        delete topRightTree;
        delete botLeftTree;
        delete botRightTree;
    }

    int Count() const{
        if(topLeftTree)
            return topLeftTree -> Count() + topRightTree -> Count() + botLeftTree -> Count() + botRightTree -> Count() + 1;
        else
            return 1;
    }

    Point GetCenter() const ;
    void Unite();
    bool Overlap(const Point &_topLeft, const Point &_botRight) const;
    void FindBordering(std::vector<Node*> &neighbors, const Point &_topLeft, const Point &_botRight);
    void FindBorderingNoOccupied(std::vector<Node*> &neighbors, const Point &_topLeft, const Point &_botRight);
    void Write() const;
    void WriteAll() const;
    std::string WriteMatlab() const;
    void Reset();
    Point RandomPoint() const;
};

Point Node::GetCenter() const {
    return {(topLeft.x + botRight.x)/2, (topLeft.y + botRight.y)/2};
}

void Node::Unite(){
    if(topLeftTree && topLeftTree->occupied && topRightTree->occupied && botLeftTree->occupied && botRightTree->occupied){
        delete topLeftTree;
        delete topRightTree;
        delete botLeftTree;
        delete botRightTree;
        topLeftTree = nullptr;
        topRightTree = nullptr;
        botLeftTree = nullptr;
        botRightTree = nullptr;
        occupied = true;
        if(parent)
            parent->Unite();
    }
}

bool Node::Overlap(const Point &_topLeft, const Point &_botRight) const {
    Point topLeftCopy{topLeft};
    Point botRightCopy{botRight};

    /* Diagonal passing
    if(topLeftCopy.x == _botRight.x && topLeftCopy.y == _botRight.y
        || botRightCopy.x == _topLeft.x && topLeftCopy.y == _botRight.y
        || topLeftCopy.x == _botRight.x && botRightCopy.y == _topLeft.y
        || botRightCopy.x == _topLeft.x && botRightCopy.y == _topLeft.y){
        return false;
    }
    */
    
    return (topLeftCopy.x <= _topLeft.x
            && _topLeft.x <= botRightCopy.x
            && (topLeftCopy.y <= _topLeft.y
                && _topLeft.y <= botRightCopy.y
                || topLeftCopy.y <= _botRight.y
                && _botRight.y <= botRightCopy.y
                || _topLeft.y <= topLeftCopy.y
                && botRightCopy.y <= _botRight.y)
            || topLeftCopy.x <= _botRight.x
            && _botRight.x <= botRightCopy.x
            && (topLeftCopy.y <= _botRight.y
                && _botRight.y <= botRightCopy.y
                || topLeftCopy.y <= _topLeft.y
                && _topLeft.y <= botRightCopy.y
                || _topLeft.y <= topLeftCopy.y
                && botRightCopy.y <= _botRight.y)
            || _topLeft.x <= topLeftCopy.x
            && botRightCopy.x <= _botRight.x
            && (_topLeft.y == botRightCopy.y
                || _botRight.y == topLeftCopy.y));
}

void Node::FindBordering(std::vector<Node*> &neighbors, const Point &_topLeft, const Point &_botRight){
    if(topLeft == _topLeft && botRight == _botRight)
        return;

    if(!topLeftTree){
        neighbors.push_back(this);
        return;
    }

    if(topLeftTree -> Overlap(_topLeft, _botRight)){
        topLeftTree -> FindBordering(neighbors, _topLeft, _botRight);
    }

    if(topRightTree -> Overlap(_topLeft, _botRight)){
        topRightTree -> FindBordering(neighbors, _topLeft, _botRight);
    }

    if(botLeftTree -> Overlap(_topLeft, _botRight)){
        botLeftTree -> FindBordering(neighbors, _topLeft, _botRight);
    }

    if(botRightTree -> Overlap(_topLeft, _botRight)){
        botRightTree -> FindBordering(neighbors, _topLeft, _botRight);
    }
}

void Node::FindBorderingNoOccupied(std::vector<Node*> &neighbors, const Point &_topLeft, const Point &_botRight){
    if(topLeft == _topLeft && botRight == _botRight)
        return;

    if(occupied)
        return;

    if(!topLeftTree){
        neighbors.push_back(this);
        return;
    }

    if(topLeftTree -> Overlap(_topLeft, _botRight)){
        topLeftTree -> FindBorderingNoOccupied(neighbors, _topLeft, _botRight);
    }

    if(topRightTree -> Overlap(_topLeft, _botRight)){
        topRightTree -> FindBorderingNoOccupied(neighbors, _topLeft, _botRight);
    }

    if(botLeftTree -> Overlap(_topLeft, _botRight)){
        botLeftTree -> FindBorderingNoOccupied(neighbors, _topLeft, _botRight);
    }

    if(botRightTree -> Overlap(_topLeft, _botRight)){
        botRightTree -> FindBorderingNoOccupied(neighbors, _topLeft, _botRight);
    }
}

void Node::Write() const {
        std::cout << " (" << topLeft.x << "," << topLeft.y << "),(" << botRight.x << "," << botRight.y << ")" << " fScore: " << fScore << ", gScore: " << gScore;
        if(cameFrom)
            std::cout << ", Came From: " << "(" << cameFrom -> topLeft.x << "," << cameFrom -> topLeft.y << "),(" << cameFrom -> botRight.x << "," << cameFrom -> botRight.y << ")";
        else
            std::cout << ", Came From Nothing";
}

void Node::WriteAll() const {
    if(!topLeftTree){
        Write();
    }
    else{
        topLeftTree -> WriteAll();
        topRightTree -> WriteAll();
        botLeftTree -> WriteAll();
        botRightTree -> WriteAll();
    }
}

std::string Node::WriteMatlab() const {
    double x = topLeft.x;
    double y = -botRight.y;
    double width = botRight.x - x;
    std::stringstream help;
    help << x;
    help << " ";
    help << y;
    help << " ";
    help << width;
    help << " ";
    help << width;
    return help.str();
}

double Distance(const Node* a, const Node* b) {
    return sqrt(pow(b -> GetCenter().x - a -> GetCenter().x, 2) + pow(b -> GetCenter().y - a -> GetCenter().y, 2));
}

void Node::Reset() {
    fScore = std::numeric_limits<double>::infinity();
    gScore = std::numeric_limits<double>::infinity();
    cameFrom = nullptr;
    if(!topLeftTree)
        return;
    topLeftTree -> Reset();
    topRightTree -> Reset();
    botLeftTree -> Reset();
    botRightTree -> Reset();
}

Point Node::RandomPoint() const{
    std::default_random_engine generator;
    generator.seed(std::chrono::system_clock::now().time_since_epoch().count());
    std::uniform_real_distribution<double> distribution_x(topLeft.x, botRight.x);
    std::uniform_real_distribution<double> distribution_y(botRight.y, topLeft.y);
    return {distribution_x(generator), distribution_y(generator)};
}

#endif // NODE_H_INCLUDED
