#include "FallbackOrchestrator.hpp"
#include <iostream>

namespace llm_orchestrator {

    void FallbackOrchestrator::add_provider(std::shared_ptr<llm_core::IProvider> provider) {
        providers_.push_back(std::move(provider));
    }

    llm_core::Response FallbackOrchestrator::generate(const std::vector<llm_core::Message>& prompt,
                                                      const llm_core::GenerationConfig& config) {
        llm_core::Response last_response;
        last_response.text = "Ошибка: Нет доступных провайдеров.";

        for (const auto& provider : providers_) {
            std::cout << "[Orchestrator] Пробую провайдер: " << provider->get_provider_name() << "...\n";

            llm_core::Response current_response = provider->generate(prompt, config);

            // Если ответ не содержит слово "Ошибка", считаем его успешным
            // (В будущем здесь можно сделать более строгую проверку через коды HTTP)
            if (current_response.text.find("Ошибка") == std::string::npos) {
                std::cout << "[Orchestrator] Успех! Получен ответ от: " << provider->get_provider_name() << "\n";
                return current_response;
            } else {
                std::cout << "[Orchestrator] Сбой. Переключаюсь на следующий...\n";
                last_response = current_response;
            }
        }

        // Если все провайдеры упали, возвращаем последнюю ошибку
        return last_response;
    }

    void FallbackOrchestrator::stream(const std::vector<llm_core::Message>& prompt,
                                      const llm_core::GenerationConfig& config,
                                      llm_core::StreamCallback callback) {
        // Стриминг реализуем позже
    }
}