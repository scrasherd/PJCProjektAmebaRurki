#pragma once

class IPlasmodiumModel {
public:
    virtual void simulate() = 0;
    virtual ~IPlasmodiumModel() = default;
};

