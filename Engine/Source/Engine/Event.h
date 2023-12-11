#pragma once

#include <vector>
#include <functional>

class Event
{
public:
    void Subscribe(const std::function<void()>& handler)
    {
        handlers.push_back(handler);
    }

    void Unsubscribe(const std::function<void()>& handlerToRemove)
    {
        for (auto it = handlers.begin(); it != handlers.end();)
        {
            if (it->target<void()>() == handlerToRemove.target<void()>())
            {
                it = handlers.erase(it);
            }
            else
            {
                ++it;
            }
        }
    }

    void Invoke() const
    {
        for (auto& handler : handlers)
        {
            handler();
        }
    }

private:
    std::vector<std::function<void()>> handlers;
};
