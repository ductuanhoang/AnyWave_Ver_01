if(ESP_PLATFORM)

file(GLOB_RECURSE SOURCES *.c)

idf_component_register(SRCS ${SOURCES}
                       INCLUDE_DIRS .
                       REQUIRES json_parser logger_file peripheral user_driver mqtt esp_https_ota app_update json
                       EMBED_TXTFILES ${project_dir}/components/task/server_certs/ca_cert.pem
                       )

else()
    message(FATAL_ERROR "LVGL LV examples: ESP_PLATFORM is not defined. Try reinstalling ESP-IDF.")
endif()
