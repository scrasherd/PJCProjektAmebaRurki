#pragma once

class IFlowModel {
public:
    virtual ~IFlowModel() = default;
    virtual void simulate() = 0;
};
