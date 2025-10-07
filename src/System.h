#pragma once

class System {
public:
    virtual ~System() = default;

    virtual void fixedUpdate(float) = 0;
    virtual void update(float) = 0;
    virtual void render() = 0;

    // по умолчанию не сигнализирует об остановке
    virtual bool shouldStop() const { return false; }
};
