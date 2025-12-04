#include <functional>
#include <unordered_map>
#include <vector>
#include <typeindex>
#include <any>
#include <memory>
class Event {
    public:
    virtual ~Event() = default;
};
class EventBus {
    public:
    template<typename EventType>
    void subscribe(std::function<void(const Event&)> callback) {
        auto wrapper = [callback](const Event& e){
            callback(static_cast<const EventType&>(e));
        };
        subscribers[typeid(EventType)].push_back(wrapper);
    };
    void emit(const Event& event){
        auto it = subscribers.find(typeid(event));
        if(it!=subscribers.end()){
            for (auto& callback : it->second){
                callback(event);
            };
        };
    };
    template<typename EventType, typename... Args>
    void pushEvent(Args&&... args){
        eventQueue.push_back(std::make_unique<EventType>(std::forward<Args>(args)...));

    };
    void processEvents(){
        for(auto& e : eventQueue){
            emit(*e);
        };
        eventQueue.clear();
    };
    private:
    std::unordered_map<std::type_index, std::vector<std::function<void(const Event&)>>> subscribers;
    std::vector<std::unique_ptr<Event>> eventQueue;


};