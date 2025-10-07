#pragma once

class System {
public:
    virtual ~System() = default;
    virtual void fixedUpdate(float fixedDelta) = 0;
    virtual void update(float dt) = 0;
    virtual void render() = 0;
};
