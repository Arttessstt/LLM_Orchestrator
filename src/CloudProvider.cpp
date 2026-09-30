#include "CloudProvider.hpp"
#include "httplib.h"
#include "json.hpp"
#include <iostream>

using json = nlohmann::json;

namespace llm_providers {

    struct CloudProvider::Impl {
        std::string api_key;
        std::string host;
        std::string path;
        std::string model;
    };

    CloudProvider::CloudProvider(const std::string& api_key, const std::string& base_url, const std::string& endpoint, const std::string& model)
        : pimpl_(std::make_unique<Impl>()) {
        pimpl_->api_key = api_key;
        pimpl_->host = base_url;
        pimpl_->path = endpoint; 
        pimpl_->model = model;
    }

    CloudProvider::~CloudProvider() = default;

    llm_core::Response CloudProvider::generate(const std::vector<llm_core::Message>& prompt,
                                               const llm_core::GenerationConfig& config) {

        // Упаковка в JSON
        json j_request;
        j_request["model"] = pimpl_->model;
        j_request["temperature"] = config.temperature;
        j_request["max_tokens"] = config.max_tokens;

        json j_messages = json::array();
        for (const auto& msg : prompt) {
            json j_msg;
            if (msg.role == llm_core::Role::System) j_msg["role"] = "system";
            else if (msg.role == llm_core::Role::User) j_msg["role"] = "user";
            else j_msg["role"] = "assistant";
            j_msg["content"] = msg.content;
            j_messages.push_back(j_msg);
        }
        j_request["messages"] = j_messages;

        // Отправка запроса
        httplib::Client cli(pimpl_->host);
        cli.set_bearer_token_auth(pimpl_->api_key);

        auto res = cli.Post(pimpl_->path, j_request.dump(), "application/json");

        // Распаковка ответа
        llm_core::Response response;
        if (res && res->status == 200) {
            json j_response = json::parse(res->body);
            response.text = j_response["choices"][0]["message"]["content"].get<std::string>();
        } else {
            response.text = res ? ("Ошибка API: " + res->body) : "Ошибка сети";
        }
        return response;
    }

    void CloudProvider::stream(const std::vector<llm_core::Message>& prompt,
                               const llm_core::GenerationConfig& config,
                               llm_core::StreamCallback callback) {
        // Стриминг оставим на следующие недели, пока заглушка
    }

    std::string CloudProvider::get_provider_name() const {
        return "CloudProvider";
    }
}