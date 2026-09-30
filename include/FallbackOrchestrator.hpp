#pragma once
#include "IProvider.hpp"
#include <memory>
#include <vector>

namespace llm_orchestrator {

    class FallbackOrchestrator : public llm_core::IProvider {
    public:
        FallbackOrchestrator() = default;

        // Внедрение зависимостей через умные указатели[cite: 1]
        void add_provider(std::shared_ptr<llm_core::IProvider> provider);

        llm_core::Response generate(const std::vector<llm_core::Message>& prompt,
                                    const llm_core::GenerationConfig& config) override;

        void stream(const std::vector<llm_core::Message>& prompt,
                    const llm_core::GenerationConfig& config,
                    llm_core::StreamCallback callback) override;

        [[nodiscard]]
        std::string get_provider_name() const override { return "FallbackOrchestrator"; }

    private:
        std::vector<std::shared_ptr<llm_core::IProvider>> providers_;
    };

} // namespace llm_orchestrator