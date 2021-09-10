#include "libratbag-private.h"

static int marsgaming_probe(struct ratbag_device * device)
{
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
