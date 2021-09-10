#include "libratbag-private.h"

#define MARSGAMING_MM4_NUM_PROFILES 5
typedef struct __attribute__((__packed__)) {
	uint8_t header[2];
	uint8_t red;
	uint8_t green;
	uint8_t blue;
	uint8_t footer[2];
	uint8_t zeros[9];
} change_color_packet;
static void init_color_packet(change_color_packet* packet, uint8_t red, uint8_t green, uint8_t blue);

static int marsgaming_probe(struct ratbag_device * device)
{
	static const unsigned int rates[4] = {125, 250, 500, 1000};
	int rc;
	rc = ratbag_open_hidraw(device);
	if(rc) return rc;
	if(!ratbag_hidraw_has_report(device, 1)) {
		ratbag_close_hidraw(device);
		return -ENODEV;
	}

	ratbag_device_init_profiles(device,
		5,
		5,
		18,
		2
	);
	bool first = true;
	struct ratbag_profile * profile;
	ratbag_device_for_each_profile(device, profile){

		ratbag_profile_set_report_rate_list(profile, rates, ARRAY_LENGTH(rates));
		if(first) {
			profile->is_active = true;
			first = false;
		}
		struct ratbag_resolution * resolution;
		ratbag_profile_for_each_resolution(profile, resolution) {
			ratbag_resolution_set_dpi_list_from_range(resolution,
				50,
				16400
			);
		}
		struct ratbag_led * led;
		ratbag_profile_for_each_led(profile, led) {
			led->type = RATBAG_LED_TYPE_SIDE;
			ratbag_led_set_mode_capability(led, RATBAG_LED_OFF);
			ratbag_led_set_mode_capability(led, RATBAG_LED_ON);
			ratbag_led_set_mode_capability(led, RATBAG_LED_BREATHING);
		}
	}




	/*
	change_color_packet packet;
	init_color_packet(&packet, 0xFF, 0xFF, 0xFF);
	ratbag_hidraw_set_feature_report(device, 0x02, (uint8_t*) &packet, 16);
	ratbag_hidraw_set_feature_report(device, 0x02, (uint8_t*) &packet, 16);
	*/
	return 0;
}

static int marsgaming_commit(struct ratbag_device * device)
{
	return 0;
}

static void marsgaming_remove(struct ratbag_device * device)
{
}
static int marsgaming_set_active_profile(struct ratbag_device * device, unsigned int profile)
{
	return 0;
}
struct ratbag_driver marsgaming_driver = {
	.name = "Mars Gaming",
	.id = "marsgaming",
	.probe = marsgaming_probe,
	.remove = marsgaming_remove,
	.commit = marsgaming_commit,
	.set_active_profile = marsgaming_set_active_profile
};


static void init_color_packet(change_color_packet* packet, uint8_t red, uint8_t green, uint8_t blue) {
	static const uint8_t header_values[2] = {0x02, 0x04};
	static const uint8_t footer_values[2] = {0x02, 0x03};
	memcpy(packet->header, header_values, sizeof(header_values));
	packet->red = 0xFF - red;
	packet->green = 0xFF - green;
	packet->blue = 0xFF - blue;
	memcpy(packet->footer, footer_values, sizeof(footer_values));
	memset(packet->zeros, 0x00, sizeof(packet->zeros));
}