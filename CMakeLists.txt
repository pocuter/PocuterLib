idf_component_register(SRCS "Pocuter.cpp" "PocketStarPause.cpp"
                    INCLUDE_DIRS "."
		    REQUIRES fatfs nvs_flash app_update mbedtls wpa_supplicant esp_adc_cal esp_http_client esp_http_server
)

