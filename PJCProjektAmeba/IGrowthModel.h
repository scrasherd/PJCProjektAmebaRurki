#pragma once

class IGrowthModel {
public:
    virtual ~IGrowthModel() = default;
    virtual void simulate() = 0;
    virtual void growOneStep() = 0; 
    virtual void growOneNode() = 0;
};

