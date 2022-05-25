/*
 * SPDX-FileCopyrightText: Hanspeter Portner <dev@open-music-kontrollers.ch>
 * SPDX-License-Identifier: Artistic-2.0
 */

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

#include <timely.lv2/timely.h>

#include <lv2/log/log.h>
#include <lv2/log/logger.h>

#define TIMELY_PREFIX		"http://open-music-kontrollers.ch/lv2/timely#"
#define TIMELY_TEST_URI	TIMELY_PREFIX"test"

typedef struct _plughandle_t plughandle_t;

struct _plughandle_t {
	LV2_URID_Map *map;
	LV2_Log_Log *log;
	LV2_Log_Logger logger;
	timely_t timely;


	const LV2_Atom_Sequence *event_in;
};

static void
_timely_cb(timely_t *timely, int64_t frames, LV2_URID type, void *data)
{
	plughandle_t *handle = data;

	const int64_t frame = TIMELY_FRAME(timely);

	if(type == TIMELY_URI_BAR_BEAT(timely))
	{
		const float bar_beat = TIMELY_BAR_BEAT_RAW(timely);
		lv2_log_trace(&handle->logger, "0x%08"PRIx64" %4"PRIi64" time:barBeat          %f\n",
			frame, frames, bar_beat);
	}
	else if(type == TIMELY_URI_BAR(timely))
	{
		const int64_t bar = TIMELY_BAR(timely);
		lv2_log_trace(&handle->logger, "0x%08"PRIx64" %4"PRIi64" time:bar              %"PRIi64"\n",
			frame, frames, bar);
	}
	else if(type == TIMELY_URI_BEAT_UNIT(timely))
	{
		const int32_t beat_unit = TIMELY_BEAT_UNIT(timely);
		lv2_log_trace(&handle->logger, "0x%08"PRIx64" %4"PRIi64" time:beatUnit         %"PRIi32"\n",
			frame, frames, beat_unit);
	}
	else if(type == TIMELY_URI_BEATS_PER_BAR(timely))
	{
		const float bpb = TIMELY_BEATS_PER_BAR(timely);
		lv2_log_trace(&handle->logger, "0x%08"PRIx64" %4"PRIi64" time:beatsPerBar      %f\n",
			frame, frames, bpb);
	}
	else if(type == TIMELY_URI_BEATS_PER_MINUTE(timely))
	{
		const float bpm = TIMELY_BEATS_PER_MINUTE(timely);
		lv2_log_trace(&handle->logger, "0x%08"PRIx64" %4"PRIi64" time:beatsPerMinute   %f\n",
			frame, frames, bpm);
	}
	else if(type == TIMELY_URI_FRAME(timely))
	{
		/*
		lv2_log_trace(&handle->logger, "0x%08"PRIx64" %4"PRIi64" time:frame            %"PRIi64"\n",
			frame, frames, frame);
		*/
	}
	else if(type == TIMELY_URI_FRAMES_PER_SECOND(timely))
	{
		const float fps = TIMELY_FRAMES_PER_SECOND(timely);
		lv2_log_trace(&handle->logger, "0x%08"PRIx64" %4"PRIi64" time:framesPerSecond  %f\n",
			frame, frames, fps);
	}
	else if(type == TIMELY_URI_SPEED(timely))
	{
		const float speed = TIMELY_SPEED(timely);
		lv2_log_trace(&handle->logger, "0x%08"PRIx64" %4"PRIi64" time:speed            %f\n",
			frame, frames, speed);
	}
}

static LV2_Handle
instantiate(const LV2_Descriptor* descriptor, double rate,
	const char *bundle_path __attribute__((unused)),
	const LV2_Feature *const *features)
{
	plughandle_t *handle = calloc(1, sizeof(plughandle_t));
	if(!handle)
		return NULL;

	for(unsigned i=0; features[i]; i++)
	{
		if(!strcmp(features[i]->URI, LV2_URID__map))
			handle->map = features[i]->data;
		else if(!strcmp(features[i]->URI, LV2_LOG__log))
			handle->log = features[i]->data;
	}

	if(!handle->map)
	{
		fprintf(stderr,
			"%s: Host does not support urid:map\n", descriptor->URI);
		free(handle);
		return NULL;
	}
	if(!handle->log)
	{
		fprintf(stderr,
			"%s: Host does not support log:log\n", descriptor->URI);
		free(handle);
		return NULL;
	}

	lv2_log_logger_init(&handle->logger, handle->map, handle->log);

	timely_mask_t mask = TIMELY_MASK_BAR_BEAT
		| TIMELY_MASK_BAR
		| TIMELY_MASK_BEAT_UNIT
		| TIMELY_MASK_BEATS_PER_BAR
		| TIMELY_MASK_BEATS_PER_MINUTE
		//| TIMELY_MASK_FRAME
		| TIMELY_MASK_FRAMES_PER_SECOND
		| TIMELY_MASK_SPEED
		| TIMELY_MASK_BAR_BEAT_WHOLE
		| TIMELY_MASK_BAR_WHOLE;
	timely_init(&handle->timely, handle->map, rate, mask, _timely_cb, handle);
	timely_set_multiplier(&handle->timely, 1.f);

	return handle;
}

static void
connect_port(LV2_Handle instance, uint32_t port, void *data)
{
	plughandle_t *handle = (plughandle_t *)instance;

	switch(port)
	{
		case 0:
			handle->event_in = (const LV2_Atom_Sequence *)data;
			break;
		default:
			break;
	}
}

static void
run(LV2_Handle instance, uint32_t nsamples)
{
	plughandle_t *handle = instance;
	int64_t from = 0;

	LV2_ATOM_SEQUENCE_FOREACH(handle->event_in, ev)
	{
		const int64_t to = ev->time.frames;
		const LV2_Atom_Object *obj = (const LV2_Atom_Object *)&ev->body;

		const int handled = timely_advance(&handle->timely, obj, from, to);
		(void)handled;
		from = to;
	}

	timely_advance(&handle->timely, NULL, from, nsamples);
}

static void
cleanup(LV2_Handle instance)
{
	plughandle_t *handle = instance;

	free(handle);
}

const LV2_Descriptor timely_test = {
	.URI						= TIMELY_TEST_URI,
	.instantiate		= instantiate,
	.connect_port		= connect_port,
	.activate				= NULL,
	.run						= run,
	.deactivate			= NULL,
	.cleanup				= cleanup,
	.extension_data	= NULL
};

#ifdef _WIN32
__declspec(dllexport)
#else
__attribute__((visibility("default")))
#endif
const LV2_Descriptor*
lv2_descriptor(uint32_t index)
{
	switch(index)
	{
		case 0:
			return &timely_test;
		default:
			return NULL;
	}
}
