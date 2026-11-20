//
// Created by matheus on 18/09/2026.
//

#ifndef MINIENGINE_RESOURCEMANAGER_H
#define MINIENGINE_RESOURCEMANAGER_H
#include <concepts>
#include <memory>
#include <vector>
#include "spdlog/spdlog.h"

namespace MiniEngine
{
    struct IResource
    {
        virtual ~IResource() = default;
    };

    struct ResourceHandle
    {
        uint32_t index = 0;
        uint32_t generation = 0;

        [[nodiscard]] constexpr bool isValid() const noexcept { return generation != 0; }
        constexpr explicit operator bool() const noexcept { return isValid(); }

        auto operator<=>(const ResourceHandle&) const = default;
    };

    template <typename T>
    concept Resource = std::derived_from<T, IResource>;

    class ResourceManager
    {
    public:
        ResourceManager()
        {
            p_resource = this;
        }

        ~ResourceManager() = default;

        template <Resource T, typename... Args>
        ResourceHandle create(Args&&... args)
        {
            auto resource = std::make_unique<T>(std::forward<Args>(args)...);
            uint16_t index;

            if (!freeIds.empty())
            {
                index = freeIds.back();
                freeIds.pop_back();
                slots[index].data = std::move(resource);
            }
            else
            {
                index = static_cast<uint32_t>(slots.size());
                slots.push_back(Slot{
                    .data = std::move(resource),
                    .generation = 1
                });
            }

            return {.index = index, .generation = slots[index].generation};
        }

        template <Resource T>
        T* get(const ResourceHandle handle)
        {
            if (!isValid(handle)) return nullptr;
            auto* resource = dynamic_cast<T*>(slots[handle.index].data.get());
            if (!resource)
            {
                spdlog::error("Resource {} exists, but requested type is wrong", handle.index);
            }
            return resource;
        }

        template <Resource T>
        const T* get(const ResourceHandle handle) const
        {
            if (!isValid(handle)) return nullptr;
            const auto* resource = dynamic_cast<const T*>(slots[handle.index].data.get());
            if (!resource)
            {
                spdlog::error("Resource {} exists, but requested type is wrong", handle.index);
            }
            return resource;
        }

        bool destroy(const ResourceHandle handle)
        {
            if (!isValid(handle)) return false;

            auto& [data, generation] = slots[handle.index];
            data.reset();

            generation++;
            if (generation == 0) generation = 1;

            freeIds.push_back(handle.index);
            return true;
        }

        [[nodiscard]] bool isValid(const ResourceHandle handle) const noexcept
        {
            if (!handle.isValid() || handle.index >= slots.size()) return false;
            const auto& [data, generation] = slots[handle.index];
            return data != nullptr && generation == handle.generation;
        }

        static void assertValid(const ResourceHandle handle)
        {
            const auto& resourceManager = get();
            const bool valid = resourceManager.isValid(handle);
            if (!valid)
            {
                spdlog::warn("Value with id {} is not valid.", handle.index);
            }
            assert(valid && "Value doesn't valid.");
        }

        void clear()
        {
            for (uint32_t i = 0; i < static_cast<uint32_t>(slots.size()); ++i)
            {
                if (slots[i].data)
                {
                    slots[i].data.reset();
                    slots[i].generation++;
                    if (slots[i].generation == 0) slots[i].generation = 1;
                    freeIds.push_back(i);
                }
            }
        }

        [[nodiscard]] size_t size() const { return slots.size(); }

        static ResourceManager& get();

    private:
        struct Slot
        {
            std::unique_ptr<IResource> data;
            uint32_t generation = 0;
        };

        static ResourceManager* p_resource;

        std::vector<Slot> slots;
        std::vector<uint32_t> freeIds;
    };
} // MiniEngine

#endif //MINIENGINE_RESOURCEMANAGER_H
