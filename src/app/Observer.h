#include "Events.h"

class Observer {
public:
    virtual void onModelChanged(const ModelEventData& event) = 0;
    virtual ~Observer() = default;
};