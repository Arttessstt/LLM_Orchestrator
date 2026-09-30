#pragma once
#include "IProvider.hpp"
#include <memory>
#include <string>

namespace llm_providers {

    // Адаптер для облачных API (например, Groq или OpenAI)
    class CloudProvider : public llm_core::IProvider {
    public:
        
        CloudProvider(const std::string& api_key, const std::string& base_url, const std::string& endpoint, const std::string& model);
        ~CloudProvider() override;

        llm_core::Response generate(const std::vector<llm_core::Message>& prompt,
                                    const llm_core::GenerationConfig& config) override;

        void stream(const std::vector<llm_core::Message>& prompt,
                    const llm_core::GenerationConfig& config,
                    llm_core::StreamCallback callback) override;

        std::string get_provider_name() const override;

    private:
        struct Impl; // Предварительное объявление
        std::unique_ptr<Impl> pimpl_; // Умный указатель на скрытую реализацию
    };

} // namespace llm_providers