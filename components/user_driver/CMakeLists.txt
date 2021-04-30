set(COMPONENT_SRCS "stm32_com/stm32_com.c"
"oled_driver/oled_display.c"
"oled_driver/fonts2/fonts.c"
"mpu9250/ak8963.c"
"mpu9250/calibrate.c"
"mpu9250/common.c"
"mpu9250/i2c-easy.c"
"mpu9250/mpu9250.c"

)

set(COMPONENT_ADD_INCLUDEDIRS ".")

register_component()