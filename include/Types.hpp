#pragma once
#include <string>
#include <vector>
#include <optional>

namespace llm_core {

    // Строго типизированные роли
    enum class Role {
        System,
        User,
        Assistant
    };

    // Структура сообщения
    struct Message {
        Role role;
        std::string content;
    };

    // Настройки генерации
    struct GenerationConfig {
        float temperature = 0.7f;
        int max_tokens = 1024;
        std::optional<float> top_p = std::nullopt;
        std::vector<std::string> stop_sequences;
    };

    // Унифицированный ответ
    struct Response {
        std::string text;
        int prompt_tokens = 0;
        int completion_tokens = 0;
        std::string model_name;
    };

} // namespace llm_core