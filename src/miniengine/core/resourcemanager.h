//
// Created by matheus on 18/09/2026.
//

#ifndef MINIENGINE_RESOURCEMANAGER_H
#define MINIENGINE_RESOURCEMANAGER_H
#include <unordered_map>
#include <concepts>
#include <limits>
#include <list>
#include <memory>
#include <vector>

namespace MiniEngine
{
    struct IResource
    {
        virtual ~IResource() = default;
    };
    template<typename T>
    concept Resource = std::derived_from<T, IResource>;

    class ResourceManager
    {
    public:
        static constexpr size_t MAX_CAPACITY = std::numeric_limits<uint16_t>::max();

        explicit ResourceManager() : capacity(MAX_CAPACITY)
        {
            freeIds.reserve(capacity);
            for (size_t i = capacity; i >= 1; --i)
            {
                freeIds.push_back(i);
            }

            p_resource = this;
        }

        ~ResourceManager()
        {
            clean();
        }

        template<Resource T, typename... Args>
        uint16_t create(Args&&... args)
        {
            if (order.size() >= capacity) {
                evictOldest();
            }

            const uint16_t id = freeIds.back();
            freeIds.pop_back();

            auto res = std::make_unique<T>(std::forward<Args>(args)...);

            order.push_front({.id = id, .resource = std::move(res)});
            resources[id] = order.begin();

            return id;
        }

        template<Resource T>
        T* get(const uint16_t id)
        {
            const auto it = resources.find(id);
            if (it == resources.end()) return nullptr;

            order.splice(order.begin(), order, it->second);
            return dynamic_cast<T*>(it->second->resource.get());
        }

        bool destroy(const uint16_t id) {
            const auto it = resources.find(id);
            if (it == resources.end()) return false;

            order.erase(it->second);
            resources.erase(it);
            freeIds.push_back(id);
            return true;
        }

        void clean()
        {
            order.clear();
            resources.clear();

            freeIds.clear();
            freeIds.reserve(capacity);
            for (size_t i = capacity; i >= 1; --i) {
                freeIds.push_back(static_cast<uint16_t>(i));
            }
        }

        size_t size() const { return order.size(); }
        static ResourceManager& get() { return *p_resource; }

    private:
        struct Entry
        {
            uint16_t id;
            std::unique_ptr<IResource> resource;
        };
        void evictOldest() {
            const uint16_t oldestId = order.back().id;
            order.pop_back();
            resources.erase(oldestId);
            freeIds.push_back(oldestId);
        }

        static ResourceManager* p_resource;

        std::vector<uint16_t> freeIds;
        std::list<Entry> order;
        std::unordered_map<uint16_t, std::list<Entry>::iterator> resources;
        size_t capacity;
    };
} // MiniEngine

#endif //MINIENGINE_RESOURCEMANAGER_H
