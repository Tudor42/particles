#pragma once

class Solver {
public:
    virtual void step() = 0;
    virtual void init() = 0;
};