// SPDX-License-Identifier: MIT
#include <stdbool.h>
#include <stdio.h>
#include <tinyalsa/asoundlib.h>

#include <cutils/log.h>

/* Note: These parameters have little relevance (no audio data written) */
struct pcm_config pcm_config_voice_call = {
	.channels	= 1,
	.rate		= 8000,
	.period_size	= 160,
	.period_count	= 2,
	.format		= PCM_FORMAT_S16_LE,
};

struct q6voiced {
	//unsigned int card, device;
	struct pcm *tx, *rx;
};

static struct q6voiced v = { 0 };

void q6voiced_open(/* struct q6voiced *v */)
{
	if (v.tx)
		return; /* Already active */

	/*
	 * Opening the PCM devices starts the stream.
	 * This should be replaced by a codec2codec link probably.
	 */
	v.tx = pcm_open(/* v->card */ 0, /* v->device */ 4, PCM_IN, &pcm_config_voice_call);
	v.rx = pcm_open(/* v->card */ 0, /* v->device */ 4, PCM_OUT, &pcm_config_voice_call);
	if (!pcm_is_ready(v.tx) || pcm_prepare(v.tx))
		ALOGE("Failed to open tx");
	if (!pcm_is_ready(v.rx) || pcm_prepare(v.rx))
		ALOGE("Failed to open rx");

	ALOGI("q6voice PCM devices were opened.");
}

void q6voiced_close(/* struct q6voiced *v */)
{
	if (!v.tx)
		return; /* Not active */

	pcm_close(v.rx);
	pcm_close(v.tx);
	v.rx = v.tx = NULL;

	ALOGI("q6voice PCM devices were closed.");
}
