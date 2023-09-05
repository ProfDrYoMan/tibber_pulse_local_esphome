// TibberLocal.hpp

#pragma once
#include "esphome.h"

class TibberLocal : public PollingComponent, public UARTDevice {
public:
    TibberLocal(uint32_t interval_ms, UARTComponent *parent, const String url, const char* user, const char* pass) : 
        PollingComponent(interval_ms), UARTDevice(parent), url_(url), user_(user), pass_(pass) { }

    void setup() override {
        wifi_client_ = new WiFiClient();
        http_client_ = new HTTPClient();
    }

    void update() override {
        if (!wifi_client_) {
            ESP_LOGD("TibberLocal", "No WiFi");
            return;
        }
        
        if (!http_client_) {
            ESP_LOGD("TibberLocal", "No HTTP");
            return;
        }
        
        if (!network::is_connected()) {
            ESP_LOGD("TibberLocal", "No network");
            return;
        }    

        auto begin_status = http_client_->begin(*wifi_client_, url_);

        if (!begin_status) {
            http_client_->end();
            ESP_LOGD("TibberLocal", "Begin failed");
            return;
        }

        http_client_->setAuthorization(user_, pass_);

        auto start_time = millis();
        auto http_code = http_client_->sendRequest("GET", "");
        auto duration = millis() - start_time;
        
        if (http_code < 0) {
            http_client_->end();
            ESP_LOGD("TibberLocal", "HTTP error %d", http_code);
            return;
        }

        ESP_LOGD("TibberLocal", "Duration %d", duration);
        ESP_LOGD("TibberLocal", "HTTP code %d", http_code);

        if (http_code != 200) {
            http_client_->end();
            ESP_LOGD("TibberLocal", "HTTP code not 200");
            return;
        }

        uint8_t buffer[sizeof(buffer_)];

        auto payload_size = http_client_->getSize();

        ESP_LOGD("TibberLocal", "Payload size %d", payload_size);

        if (payload_size > sizeof(buffer)) {
            http_client_->end();
            ESP_LOGD("TibberLocal", "Buffer too small, %d > %d", payload_size, sizeof(buffer));
            return;
        }
        
        auto bytes_read = wifi_client_->readBytes(buffer, payload_size);

        if (payload_size != bytes_read) {
            http_client_->end();
            ESP_LOGD("TibberLocal", "Read missmatch, %d != %d", payload_size, bytes_read);
            return;
        }
        
        ESP_LOGD("TibberLocal", "Bytes read %d", bytes_read);

        if (bytes_read == bytes_read_) {
            if (memcmp(buffer, buffer_, bytes_read) == 0 ) {
                http_client_->end();
                ESP_LOGD("TibberLocal", "Same data");
                return;
            }
        }

        bytes_read_ = bytes_read;
        memcpy(buffer_, buffer, bytes_read);

        http_client_->end();
        
        write_array(buffer_, bytes_read_);
    }

private:
    const String url_;
    const char* user_;
    const char* pass_;
    WiFiClient* wifi_client_;
    HTTPClient* http_client_;
    int bytes_read_;
    uint8_t buffer_[1024];
};
