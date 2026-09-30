#pragma once
#include "Types.hpp"
#include <functional>

namespace llm_core {

    // Callback для асинхронного стриминга
    using StreamCallback = std::function<bool(const std::string& chunk)>;

    class IProvider {
    public:
        // Виртуальный деструктор обязателен для интерфейсов
        virtual ~IProvider() = default;

        // Чистая виртуальная функция генерации (синхронная)
        [[nodiscard]]
        virtual Response generate(const std::vector<Message>& prompt,
                                  const GenerationConfig& config) = 0;

        // Чистая виртуальная функция для стриминга (асинхронная)
        virtual void stream(const std::vector<Message>& prompt,
                            const GenerationConfig& config,
                            StreamCallback callback) = 0;

        [[nodiscard]]
        virtual std::string get_provider_name() const = 0;
    };

} // namespace llm_core