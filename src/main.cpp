#include "FallbackOrchestrator.hpp"
#include "CloudProvider.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#ifdef _WIN32
#include <windows.h>
#endif

using namespace llm_core;
using namespace llm_providers;
using namespace llm_orchestrator;

std::string load_env_var(const std::string& env_path, const std::string& target_key) {
    std::ifstream file(env_path);
    if (!file.is_open()) return "";
    std::string line;
    while (std::getline(file, line)) {
        auto pos = line.find('=');
        if (pos != std::string::npos) {
            std::string key = line.substr(0, pos);
            if (key == target_key) return line.substr(pos + 1);
        }
    }
    return "";
}

int main() {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
#endif

    // Вытаскиваем универсальные настройки
    std::string api_key = load_env_var("../.env", "API_KEY");
    std::string base_url = load_env_var("../.env", "BASE_URL");
    std::string model_name = load_env_var("../.env", "MODEL_NAME");

    if (base_url.empty() || model_name.empty()) {
        std::cerr << "Ошибка: Не удалось загрузить BASE_URL или MODEL_NAME из .env!" << std::endl;
        return 1;
    }

    // Собираем провайдер полностью из переменных конфигурации
    auto universal_provider = std::make_shared<CloudProvider>(
        api_key, base_url, "/v1/chat/completions", model_name
    );

    FallbackOrchestrator llm;
    llm.add_provider(universal_provider);

    std::vector<Message> prompt = {
        {Role::System, "You are a helpful assistant. Reply in Russian."},
        {Role::User, "Почему ООП в C++ это круто? Напиши 2 коротких аргумента."}
    };

    GenerationConfig config;
    config.temperature = 0.5f;

    std::cout << "--- Начинаем генерацию ---\n";
    std::cout << "Сервер: " << base_url << "\nМодель: " << model_name << "\n\n";

    Response response = llm.generate(prompt, config);

    std::cout << "Ответ нейросети:\n" << response.text << std::endl;

    return 0;
}