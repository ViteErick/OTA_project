#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(ota_device, LOG_LEVEL_INF);

static const struct gpio_dt_spec heartbeat_led = GPIO_DT_SPEC_GET(DT_ALIAS(led0), gpios);

int main(void)
{
	int result;

	if (!gpio_is_ready_dt(&heartbeat_led)) {
		LOG_ERR("Heartbeat LED controller is not ready");
		return 0;
	}

	result = gpio_pin_configure_dt(&heartbeat_led, GPIO_OUTPUT_INACTIVE);
	if (result != 0) {
		LOG_ERR("Heartbeat LED configuration failed: %d", result);
		return 0;
	}

	LOG_INF("STM32H755 OTA device bring-up started on %s", CONFIG_BOARD_TARGET);

	while (true) {
		result = gpio_pin_toggle_dt(&heartbeat_led);
		if (result != 0) {
			LOG_ERR("Heartbeat LED update failed: %d", result);
			return 0;
		}

		k_sleep(K_MSEC(500));
	}

	return 0;
}
