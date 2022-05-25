/*
 * SPDX-FileCopyrightText: Hanspeter Portner <dev@open-music-kontrollers.ch>
 * SPDX-License-Identifier: Artistic-2.0
 */

#include <src/props_private.h>

static inline void
_props_impl_spin_lock(props_impl_t *impl, int from, int to)
{
	int expected = from;
	const int desired = to;

	while(!atomic_compare_exchange_strong_explicit(&impl->state, &expected, desired,
		memory_order_acquire, memory_order_acquire))
	{
		// spin
	}
}

static inline bool
_props_impl_try_lock(props_impl_t *impl, int from, int to)
{
	int expected = from;
	const int desired = to;

	return atomic_compare_exchange_strong_explicit(&impl->state, &expected, desired,
		memory_order_acquire, memory_order_acquire);
}

static inline void
_props_impl_unlock(props_impl_t *impl, int to)
{
	atomic_store_explicit(&impl->state, to, memory_order_release);
}

static inline bool
_props_restoring_get(props_t *props)
{
	return atomic_exchange_explicit(&props->restoring, false, memory_order_acquire);
}

static inline void
_props_restoring_set(props_t *props)
{
	atomic_store_explicit(&props->restoring, true, memory_order_release);
}

static inline void
_props_qsort(props_impl_t *A, int n)
{
	if(n < 2)
		return;

	const props_impl_t *p = A;

	int i = -1;
	int j = n;

	while(true)
	{
		do {
			i += 1;
		} while(A[i].property < p->property);

		do {
			j -= 1;
		} while(A[j].property > p->property);

		if(i >= j)
			break;

		const props_impl_t tmp = A[i];
		A[i] = A[j];
		A[j] = tmp;
	}

	_props_qsort(A, j + 1);
	_props_qsort(A + j + 1, n - j - 1);
}

props_impl_t *
_props_impl_get(props_t *props, LV2_URID property)
{
	props_impl_t *base = props->impls;

	for(int N = props->nimpls, half; N > 1; N -= half)
	{
		half = N/2;
		props_impl_t *dst = &base[half];
		base = (dst->property > property) ? base : dst;
	}

	return (base->property == property) ? base : NULL;
}

static inline LV2_Atom_Forge_Ref
_props_patch_set(props_t *props, LV2_Atom_Forge *forge, uint32_t frames,
	props_impl_t *impl, int32_t sequence_num)
{
	LV2_Atom_Forge_Frame obj_frame;

	LV2_Atom_Forge_Ref ref = lv2_atom_forge_frame_time(forge, frames);

	if(ref)
		ref = lv2_atom_forge_object(forge, &obj_frame, 0, props->urid.patch_set);
	{
		if(props->urid.subject) // is optional
		{
			if(ref)
				ref = lv2_atom_forge_key(forge, props->urid.patch_subject);
			if(ref)
				ref = lv2_atom_forge_urid(forge, props->urid.subject);
		}

		if(sequence_num) // is optional
		{
			if(ref)
				ref = lv2_atom_forge_key(forge, props->urid.patch_sequence);
			if(ref)
				ref = lv2_atom_forge_int(forge, sequence_num);
		}

		if(ref)
			ref = lv2_atom_forge_key(forge, props->urid.patch_property);
		if(ref)
			ref = lv2_atom_forge_urid(forge, impl->property);

		if(ref)
			lv2_atom_forge_key(forge, props->urid.patch_value);
		if(ref)
			ref = lv2_atom_forge_atom(forge, impl->value.size, impl->type);
		if(ref)
			ref = lv2_atom_forge_write(forge, impl->value.body, impl->value.size);
	}
	if(ref)
		lv2_atom_forge_pop(forge, &obj_frame);

	if(ref)
		ref = lv2_atom_forge_frame_time(forge, frames);
	if(ref)
		ref = lv2_atom_forge_object(forge, &obj_frame, 0, props->urid.state_StateChanged);
	if(ref)
		lv2_atom_forge_pop(forge, &obj_frame);

	return ref;
}

static inline LV2_Atom_Forge_Ref
_props_patch_get(props_t *props, LV2_Atom_Forge *forge, uint32_t frames,
	props_impl_t *impl, int32_t sequence_num)
{
	LV2_Atom_Forge_Frame obj_frame;

	LV2_Atom_Forge_Ref ref = lv2_atom_forge_frame_time(forge, frames);

	if(ref)
		ref = lv2_atom_forge_object(forge, &obj_frame, 0, props->urid.patch_get);
	{
		if(props->urid.subject) // is optional
		{
			if(ref)
				ref = lv2_atom_forge_key(forge, props->urid.patch_subject);
			if(ref)
				ref = lv2_atom_forge_urid(forge, props->urid.subject);
		}

		if(sequence_num) // is optional
		{
			if(ref)
				ref = lv2_atom_forge_key(forge, props->urid.patch_sequence);
			if(ref)
				ref = lv2_atom_forge_int(forge, sequence_num);
		}

		if(ref)
			ref = lv2_atom_forge_key(forge, props->urid.patch_property);
		if(ref)
			ref = lv2_atom_forge_urid(forge, impl->property);
	}
	if(ref)
		lv2_atom_forge_pop(forge, &obj_frame);

	return ref;
}

static inline LV2_Atom_Forge_Ref
_props_patch_error(props_t *props, LV2_Atom_Forge *forge, uint32_t frames,
	int32_t sequence_num)
{
	LV2_Atom_Forge_Frame obj_frame;

	LV2_Atom_Forge_Ref ref = lv2_atom_forge_frame_time(forge, frames);

	if(ref)
		ref = lv2_atom_forge_object(forge, &obj_frame, 0, props->urid.patch_error);
	{
		if(ref)
			ref = lv2_atom_forge_key(forge, props->urid.patch_sequence);
		if(ref)
			ref = lv2_atom_forge_int(forge, sequence_num);
	}
	if(ref)
		lv2_atom_forge_pop(forge, &obj_frame);

	return ref;
}

static inline LV2_Atom_Forge_Ref
_props_patch_ack(props_t *props, LV2_Atom_Forge *forge, uint32_t frames,
	int32_t sequence_num)
{
	LV2_Atom_Forge_Frame obj_frame;

	LV2_Atom_Forge_Ref ref = lv2_atom_forge_frame_time(forge, frames);

	if(ref)
		ref = lv2_atom_forge_object(forge, &obj_frame, 0, props->urid.patch_ack);
	{
		if(ref)
			ref = lv2_atom_forge_key(forge, props->urid.patch_sequence);
		if(ref)
			ref = lv2_atom_forge_int(forge, sequence_num);
	}
	if(ref)
		lv2_atom_forge_pop(forge, &obj_frame);

	return ref;
}

static inline void
_props_impl_stash(props_t *props, props_impl_t *impl)
{
	if(_props_impl_try_lock(impl, PROP_STATE_NONE, PROP_STATE_LOCK))
	{
		impl->stashing = false;
		impl->stash.size = impl->value.size;
		memcpy(impl->stash.body, impl->value.body, impl->value.size);

		_props_impl_unlock(impl, PROP_STATE_NONE);
	}
	else
	{
		impl->stashing = true; // try again later
		props->stashing = true;
	}
}

static inline void
_props_impl_restore(props_t *props, LV2_Atom_Forge *forge, uint32_t frames,
	props_impl_t *impl, LV2_Atom_Forge_Ref *ref)
{
	if(_props_impl_try_lock(impl, PROP_STATE_RESTORE, PROP_STATE_LOCK))
	{
		impl->stashing = false; // makes no sense to stash a recently restored value
		impl->value.size = impl->stash.size;
		memcpy(impl->value.body, impl->stash.body, impl->stash.size);

		_props_impl_unlock(impl, PROP_STATE_NONE);

		if(*ref && !impl->def->hidden)
			*ref = _props_patch_set(props, forge, frames, impl, 0);

		const props_def_t *def = impl->def;
		if(def->event_cb)
			def->event_cb(props->data, 0, impl);
	}
}

static inline void
_props_impl_set(props_t *props, props_impl_t *impl, LV2_URID type,
	uint32_t size, const void *body)
{
	if(  (impl->type == type)
		&& ( (impl->def->max_size == 0) || (size <= impl->def->max_size)) )
	{
		impl->value.size = size;
		memcpy(impl->value.body, body, size);

		_props_impl_stash(props, impl);
	}
}

static inline int
_props_impl_init(props_t *props, props_impl_t *impl, const props_def_t *def,
	void *value_base, void *stash_base, LV2_URID_Map *map)
{
	if(!def->property || !def->type)
		return 0;

	const LV2_URID type = map->map(map->handle, def->type);
	const LV2_URID property = map->map(map->handle, def->property);
	const LV2_URID access = def->access
		? map->map(map->handle, def->access)
		: map->map(map->handle, LV2_PATCH__writable);

	if(!type || !property || !access)
		return 0;

	impl->property = property;
	impl->access = access;
	impl->def = def;
	impl->value.body = (uint8_t *)value_base + def->offset;
	impl->stash.body = (uint8_t *)stash_base + def->offset;

	uint32_t size;
	if(  (type == props->urid.atom_int)
		|| (type == props->urid.atom_float)
		|| (type == props->urid.atom_bool)
		|| (type == props->urid.atom_urid) )
	{
		size = 4;
	}
	else if((type == props->urid.atom_long)
		|| (type == props->urid.atom_double) )
	{
		size = 8;
	}
	else if(type == props->urid.atom_literal)
	{
		size = sizeof(LV2_Atom_Literal_Body);
	}
	else if(type == props->urid.atom_vector)
	{
		size = sizeof(LV2_Atom_Vector_Body);
	}
	else if(type == props->urid.atom_object)
	{
		size = sizeof(LV2_Atom_Object_Body);
	}
	else if(type == props->urid.atom_sequence)
	{
		size = sizeof(LV2_Atom_Sequence_Body);
	}
	else
	{
		size = 0; // assume everything else as having size 0
	}

	impl->type = type;
	impl->value.size = size;
	impl->stash.size = size;

	atomic_init(&impl->state, PROP_STATE_NONE);

	// update maximal value size
	const uint32_t max_size = def->max_size
		? def->max_size
		: size;

	if(max_size > props->max_size)
	{
		props->max_size = max_size;
	}

	return 1;
}

int
props_init(props_t *props, const char *subject,
	const props_def_t *defs, int nimpls,
	void *value_base, void *stash_base,
	LV2_URID_Map *map, void *data)
{
	if(!props || !defs || !value_base || !stash_base || !map)
		return 0;

	props->nimpls = nimpls;
	props->data = data;

	props->urid.subject = subject ? map->map(map->handle, subject) : 0;

	props->urid.patch_get = map->map(map->handle, LV2_PATCH__Get);
	props->urid.patch_set = map->map(map->handle, LV2_PATCH__Set);
	props->urid.patch_put = map->map(map->handle, LV2_PATCH__Put);
	props->urid.patch_patch = map->map(map->handle, LV2_PATCH__Patch);
	props->urid.patch_wildcard = map->map(map->handle, LV2_PATCH__wildcard);
	props->urid.patch_add = map->map(map->handle, LV2_PATCH__add);
	props->urid.patch_remove = map->map(map->handle, LV2_PATCH__remove);
	props->urid.patch_subject = map->map(map->handle, LV2_PATCH__subject);
	props->urid.patch_body = map->map(map->handle, LV2_PATCH__body);
	props->urid.patch_property = map->map(map->handle, LV2_PATCH__property);
	props->urid.patch_value = map->map(map->handle, LV2_PATCH__value);
	props->urid.patch_writable = map->map(map->handle, LV2_PATCH__writable);
	props->urid.patch_readable = map->map(map->handle, LV2_PATCH__readable);
	props->urid.patch_sequence = map->map(map->handle, LV2_PATCH__sequenceNumber);
	props->urid.patch_ack = map->map(map->handle, LV2_PATCH__Ack);
	props->urid.patch_error = map->map(map->handle, LV2_PATCH__Error);

	props->urid.atom_int = map->map(map->handle, LV2_ATOM__Int);
	props->urid.atom_long = map->map(map->handle, LV2_ATOM__Long);
	props->urid.atom_float = map->map(map->handle, LV2_ATOM__Float);
	props->urid.atom_double = map->map(map->handle, LV2_ATOM__Double);
	props->urid.atom_bool = map->map(map->handle, LV2_ATOM__Bool);
	props->urid.atom_urid = map->map(map->handle, LV2_ATOM__URID);
	props->urid.atom_path = map->map(map->handle, LV2_ATOM__Path);
	props->urid.atom_literal = map->map(map->handle, LV2_ATOM__Literal);
	props->urid.atom_vector = map->map(map->handle, LV2_ATOM__Vector);
	props->urid.atom_object = map->map(map->handle, LV2_ATOM__Object);
	props->urid.atom_sequence = map->map(map->handle, LV2_ATOM__Sequence);

	props->urid.state_StateChanged = map->map(map->handle, LV2_STATE__StateChanged);

	atomic_init(&props->restoring, false);

	int status = 1;
	for(unsigned i = 0; i < props->nimpls; i++)
	{
		props_impl_t *impl = &props->impls[i];

		status = status
			&& _props_impl_init(props, impl, &defs[i], value_base, stash_base, map);
	}

	_props_qsort(props->impls, props->nimpls);

	return status;
}

void
props_dyn(props_t *props, const props_dyn_t *dyn)
{
	props->dyn = dyn;
}

void
props_idle(props_t *props, LV2_Atom_Forge *forge, uint32_t frames,
	LV2_Atom_Forge_Ref *ref)
{
	if(_props_restoring_get(props))
	{
		for(unsigned i = 0; i < props->nimpls; i++)
		{
			props_impl_t *impl = &props->impls[i];

			_props_impl_restore(props, forge, frames, impl, ref);
		}
	}

	if(props->stashing)
	{
		props->stashing = false;

		for(unsigned i = 0; i < props->nimpls; i++)
		{
			props_impl_t *impl = &props->impls[i];

			if(impl->stashing)
				_props_impl_stash(props, impl);
		}
	}
}

int
props_advance(props_t *props, LV2_Atom_Forge *forge, uint32_t frames,
	const LV2_Atom_Object *obj, LV2_Atom_Forge_Ref *ref)
{
	if(!lv2_atom_forge_is_object_type(forge, obj->atom.type))
	{
		return 0;
	}

	if(obj->body.otype == props->urid.patch_get)
	{
		const LV2_Atom_URID *subject = NULL;
		const LV2_Atom_URID *property = NULL;
		const LV2_Atom_Int *sequence = NULL;

		lv2_atom_object_get(obj,
			props->urid.patch_subject, &subject,
			props->urid.patch_property, &property,
			props->urid.patch_sequence, &sequence,
			0);

		// check for a matching optional subject
		if(  (subject && props->urid.subject)
			&& ( (subject->atom.type != props->urid.atom_urid)
				|| (subject->body != props->urid.subject) ) )
		{
			return 0;
		}

		int32_t sequence_num = 0;
		if(sequence && (sequence->atom.type == props->urid.atom_int))
		{
			sequence_num = sequence->body;
		}

		if(!property)
		{
			for(unsigned i = 0; i < props->nimpls; i++)
			{
				props_impl_t *impl = &props->impls[i];

				if(*ref && !impl->def->hidden)
					*ref = _props_patch_set(props, forge, frames, impl, sequence_num);
			}

			return 1;
		}
		else if(property->atom.type == props->urid.atom_urid)
		{
			props_impl_t *impl = _props_impl_get(props, property->body);

			if(impl)
			{
				if(*ref && !impl->def->hidden)
					*ref = _props_patch_set(props, forge, frames, impl, sequence_num);

				return 1;
			}
			else if(sequence_num)
			{
				if(*ref)
					*ref = _props_patch_error(props, forge, frames, sequence_num);
			}
		}
		else if(sequence_num)
		{
			if(*ref)
				*ref = _props_patch_error(props, forge, frames, sequence_num);
		}
	}
	else if(obj->body.otype == props->urid.patch_set)
	{
		const LV2_Atom_URID *subject = NULL;
		const LV2_Atom_URID *property = NULL;
		const LV2_Atom_Int *sequence = NULL;
		const LV2_Atom *value = NULL;

		lv2_atom_object_get(obj,
			props->urid.patch_subject, &subject,
			props->urid.patch_property, &property,
			props->urid.patch_sequence, &sequence,
			props->urid.patch_value, &value,
			0);

		// check for a matching optional subject
		if(  (subject && props->urid.subject)
			&& ( (subject->atom.type != props->urid.atom_urid)
				|| (subject->body != props->urid.subject) ) )
		{
			return 0;
		}

		int32_t sequence_num = 0;
		if(sequence && (sequence->atom.type == props->urid.atom_int))
		{
			sequence_num = sequence->body;
		}

		if(!property || (property->atom.type != props->urid.atom_urid) || !value)
		{
			if(sequence_num)
			{
				if(ref)
					*ref = _props_patch_error(props, forge, frames, sequence_num);
			}

			return 0;
		}

		props_impl_t *impl = _props_impl_get(props, property->body);
		if(impl)
		{
			_props_impl_set(props, impl, value->type, value->size,
				LV2_ATOM_BODY_CONST(value));

			// send on (e.g. to UI)
			if(*ref && !impl->def->hidden)
				*ref = _props_patch_set(props, forge, frames, impl, sequence_num);

			const props_def_t *def = impl->def;
			if(def->event_cb)
				def->event_cb(props->data, frames, impl);

			if(sequence_num)
			{
				if(*ref)
					*ref = _props_patch_ack(props, forge, frames, sequence_num);
			}

			return 1;
		}
		else if(props->dyn && props->dyn->prop)
		{
			const LV2_URID subj = subject ? subject->body : 0;

			props->dyn->prop(props->data, PROPS_DYN_EV_SET, subj, property->body, value);

			//TODO send ack
		}
		else if(sequence_num)
		{
			if(*ref)
				*ref = _props_patch_error(props, forge, frames, sequence_num);
		}
	}
	else if(obj->body.otype == props->urid.patch_put)
	{
		const LV2_Atom_URID *subject = NULL;
		const LV2_Atom_Int *sequence = NULL;
		const LV2_Atom_Object *body = NULL;

		lv2_atom_object_get(obj,
			props->urid.patch_subject, &subject,
			props->urid.patch_sequence, &sequence,
			props->urid.patch_body, &body,
			0);

		// check for a matching optional subject
		if(  (subject && props->urid.subject)
			&& ( (subject->atom.type != props->urid.atom_urid)
				|| (subject->body != props->urid.subject) ) )
		{
			return 0;
		}

		int32_t sequence_num = 0;
		if(sequence && (sequence->atom.type == props->urid.atom_int))
		{
			sequence_num = sequence->body;
		}

		if(!body || !lv2_atom_forge_is_object_type(forge, body->atom.type))
		{
			if(sequence_num)
			{
				if(*ref)
					*ref = _props_patch_error(props, forge, frames, sequence_num);
			}

			return 0;
		}

		LV2_ATOM_OBJECT_FOREACH(body, prop)
		{
			const LV2_URID property = prop->key;
			const LV2_Atom *value = &prop->value;

			props_impl_t *impl = _props_impl_get(props, property);
			if(impl)
			{
				_props_impl_set(props, impl, value->type, value->size,
					LV2_ATOM_BODY_CONST(value));

				// send on (e.g. to UI)
				if(*ref && !impl->def->hidden)
					*ref = _props_patch_set(props, forge, frames, impl, sequence_num);

				const props_def_t *def = impl->def;
				if(def->event_cb)
					def->event_cb(props->data, frames, impl);
			}
			else if(props->dyn && props->dyn->prop)
			{
				const LV2_URID subj = subject ? subject->body : 0;

				props->dyn->prop(props->data, PROPS_DYN_EV_SET, subj, property, value);

			//TODO send ack
			}
		}

		if(sequence_num)
		{
			if(*ref)
				*ref = _props_patch_ack(props, forge, frames, sequence_num);
		}

		return 1;
	}
	else if(obj->body.otype == props->urid.patch_patch)
	{
		const LV2_Atom_URID *subject = NULL;
		const LV2_Atom_Int *sequence = NULL;
		const LV2_Atom_Object *add = NULL;
		const LV2_Atom_Object *rem = NULL;

		lv2_atom_object_get(obj,
			props->urid.patch_subject, &subject,
			props->urid.patch_sequence, &sequence,
			props->urid.patch_add, &add,
			props->urid.patch_remove, &rem,
			0);

		LV2_URID subj = 0;
		if(subject && (subject->atom.type == props->urid.atom_urid))
		{
			subj = subject->body;
		}

		int32_t sequence_num = 0;
		if(sequence && (sequence->atom.type == props->urid.atom_int))
		{
			sequence_num = sequence->body;
		}

		if(rem && lv2_atom_forge_is_object_type(forge, rem->atom.type))
		{
			LV2_ATOM_OBJECT_FOREACH(rem, prop)
			{
				const LV2_URID property = prop->key;
				const LV2_Atom *value = &prop->value;

				if(props->dyn && props->dyn->prop)
				{
					props->dyn->prop(props->data, PROPS_DYN_EV_REM, subj, property, value);
				}
			}
		}

		if(add && lv2_atom_forge_is_object_type(forge, add->atom.type))
		{
			LV2_ATOM_OBJECT_FOREACH(add, prop)
			{
				const LV2_URID property = prop->key;
				const LV2_Atom *value = &prop->value;

				if(props->dyn && props->dyn->prop)
				{
					props->dyn->prop(props->data, PROPS_DYN_EV_ADD, subj, property, value);
				}
			}
		}

		if(sequence_num && *ref)
		{
			*ref = _props_patch_ack(props, forge, frames, sequence_num);
		}

		return 1;
	}

	return 0; // did not handle a patch event
}

void
props_set(props_t *props, LV2_Atom_Forge *forge, uint32_t frames,
	LV2_URID property, LV2_Atom_Forge_Ref *ref)
{
	props_impl_t *impl = _props_impl_get(props, property);

	if(impl)
	{
		_props_impl_stash(props, impl);

		if(*ref && !impl->def->hidden) //TODO use patch:sequenceNumber
			*ref = _props_patch_set(props, forge, frames, impl, 0);
	}
}

void
props_get(props_t *props, LV2_Atom_Forge *forge, uint32_t frames,
	LV2_URID property, LV2_Atom_Forge_Ref *ref)
{
	props_impl_t *impl = _props_impl_get(props, property);

	if(impl)
	{
		if(*ref && !impl->def->hidden) //TODO use patch:sequenceNumber
			*ref = _props_patch_get(props, forge, frames, impl, 0);
	}
}

void
props_stash(props_t *props, LV2_URID property)
{
	props_impl_t *impl = _props_impl_get(props, property);

	if(impl)
		_props_impl_stash(props, impl);
}

LV2_URID
props_map(props_t *props, const char *uri)
{
	for(unsigned i = 0; i < props->nimpls; i++)
	{
		props_impl_t *impl = &props->impls[i];

		if(!strcmp(impl->def->property, uri))
			return impl->property;
	}

	return 0;
}

const char *
props_unmap(props_t *props, LV2_URID property)
{
	props_impl_t *impl = _props_impl_get(props, property);

	if(impl)
		return impl->def->property;

	return NULL;
}

void
props_update(props_t *props, LV2_URID property, LV2_URID type, uint32_t size,
	const void *body)
{
	props_impl_t *impl = _props_impl_get(props, property);

	if (impl)
	{
		_props_impl_set(props, impl, type, size, body);
	}
}

static inline int
_copy_file(const char *to, const char *from)
{
	FILE *dst = NULL;
	FILE *src = NULL;
	int ch;

	dst = fopen(to, "wb");
	if(!dst)
	{
		return 1;
	}

	src = fopen(from, "rb");
	if(!src)
	{
		fclose(dst);

		return 1;
	}

	while( (ch = fgetc(src)) != EOF)
	{
		fputc(ch, dst);
	}

	fclose(dst);
	fclose(src);

	return 0;
}

static inline void
_free_path(const LV2_State_Free_Path *free_path, char *path)
{
	if(free_path && free_path->free_path)
	{
		free_path->free_path(free_path->handle, path);
	}
	else
	{
		free(path);
	}
}

LV2_State_Status
props_save(props_t *props, LV2_State_Store_Function store,
	LV2_State_Handle state, uint32_t flags, const LV2_Feature *const *features)
{
	const LV2_State_Map_Path *map_path = NULL;
	const LV2_State_Make_Path *make_path = NULL;
	const LV2_State_Free_Path *free_path = NULL;

	// set POD flag if not already set by host
	flags |= LV2_STATE_IS_POD;

	for(unsigned i = 0; features[i]; i++)
	{
		if(!strcmp(features[i]->URI, LV2_STATE__mapPath))
		{
			map_path = features[i]->data;
		}
		else if(!strcmp(features[i]->URI, LV2_STATE__makePath))
		{
			make_path = features[i]->data;
		}
		else if(!strcmp(features[i]->URI, LV2_STATE__freePath))
		{
			free_path = features[i]->data;
		}
	}

	void *body = malloc(props->max_size); // create memory to store widest value
	if(body)
	{
		for(unsigned i = 0; i < props->nimpls; i++)
		{
			props_impl_t *impl = &props->impls[i];

			if(impl->access == props->urid.patch_readable)
				continue; // skip read-only, as it makes no sense to restore them

			// always clear memory
			memset(body, 0x0, props->max_size);

			_props_impl_spin_lock(impl, PROP_STATE_NONE, PROP_STATE_LOCK);

			// create temporary copy of value, store() may well be blocking
			const uint32_t size = impl->stash.size;
			memcpy(body, impl->stash.body, size);

			_props_impl_unlock(impl, PROP_STATE_NONE);

			if(  map_path && map_path->abstract_path
				&& (impl->type == props->urid.atom_path) )
			{
				const char *path = strstr(body, "file://") == body
					? (char *)body + 7 // skip "file://"
					: (char *)body;

				char *abstract = NULL;

				if(  make_path && make_path->path
					&& (strstr(path, "/tmp") == path) )
				{
					char *absolute = make_path->path(make_path->handle, basename(path));

					if(absolute)
					{
						if(_copy_file(absolute, path) == 0)
						{
							abstract = map_path->abstract_path(map_path->handle, absolute);
						}

						_free_path(free_path, absolute);
					}
				}
				else
				{
					abstract = map_path->abstract_path(map_path->handle, path);
				}

				if(abstract)
				{
					const uint32_t sz = strlen(abstract) + 1;
					store(state, impl->property, abstract, sz, impl->type, flags);

					_free_path(free_path, abstract);
				}
			}
			else // !Path
			{
				store(state, impl->property, body, size, impl->type, flags);
			}
		}

		free(body);
	}

	return LV2_STATE_SUCCESS;
}

LV2_State_Status
props_restore(props_t *props, LV2_State_Retrieve_Function retrieve,
	LV2_State_Handle state, uint32_t flags __attribute__((unused)),
	const LV2_Feature *const *features)
{
	const LV2_State_Map_Path *map_path = NULL;
	const LV2_State_Free_Path *free_path = NULL;

	for(unsigned i = 0; features[i]; i++)
	{
		if(!strcmp(features[i]->URI, LV2_STATE__mapPath))
		{
			map_path = features[i]->data;
		}
		if(!strcmp(features[i]->URI, LV2_STATE__freePath))
		{
			free_path = features[i]->data;
		}
	}

	for(unsigned i = 0; i < props->nimpls; i++)
	{
		props_impl_t *impl = &props->impls[i];

		if(impl->access == props->urid.patch_readable)
			continue; // skip read-only, as it makes no sense to restore them

		size_t size;
		uint32_t type;
		uint32_t _flags;
		const void *body = retrieve(state, impl->property, &size, &type, &_flags);

		if(  body
			&& (type == impl->type)
			&& ( (impl->def->max_size == 0) || (size <= impl->def->max_size) ) )
		{
			if(  map_path && map_path->absolute_path
				&& (type == props->urid.atom_path) )
			{
				char *absolute = map_path->absolute_path(map_path->handle, body);
				if(absolute)
				{
					const uint32_t sz = strlen(absolute) + 1;

					_props_impl_spin_lock(impl, PROP_STATE_NONE, PROP_STATE_LOCK);

					impl->stash.size = sz;
					memcpy(impl->stash.body, absolute, sz);

					_props_impl_unlock(impl, PROP_STATE_RESTORE);

					_free_path(free_path, absolute);
				}
			}
			else // !Path
			{
				_props_impl_spin_lock(impl, PROP_STATE_NONE, PROP_STATE_LOCK);

				impl->stash.size = size;
				memcpy(impl->stash.body, body, size);

				_props_impl_unlock(impl, PROP_STATE_RESTORE);
			}
		}
	}

	_props_restoring_set(props);

	return LV2_STATE_SUCCESS;
}
