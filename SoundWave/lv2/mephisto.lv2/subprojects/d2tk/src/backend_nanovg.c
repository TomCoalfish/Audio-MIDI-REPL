/*
 * SPDX-FileCopyrightText: Hanspeter Portner <dev@open-music-kontrollers.ch>
 * SPDX-License-Identifier: Artistic-2.0
 */

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/stat.h>

#include <nanovg.h>

#if defined(__APPLE__)
#	include <OpenGL/gl.h>
#	include <OpenGL/glext.h>
#else
#	include <GL/glew.h>
#endif

#define NANOVG_GLES3_IMPLEMENTATION
#include <nanovg_gl.h>
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wsign-compare"
#include <nanovg_gl_utils.h>
#pragma GCC diagnostic pop

#if defined(NANOVG_GL2_IMPLEMENTATION)
#	define nvgCreate nvgCreateGL2
#	define nvgDelete nvgDeleteGL2
#elif defined(NANOVG_GL3_IMPLEMENTATION)
#	define nvgCreate nvgCreateGL3
#	define nvgDelete nvgDeleteGL3
#elif defined(NANOVG_GLES2_IMPLEMENTATION)
#	define nvgCreate nvgCreateGLES2
#	define nvgDelete nvgDeleteGLES2
#elif defined(NANOVG_GLES3_IMPLEMENTATION)
#	define nvgCreate nvgCreateGLES3
#	define nvgDelete nvgDeleteGLES3
#endif

#include "core_internal.h"
#include <d2tk/backend.h>
#include <d2tk/hash.h>

#define D2TK_BACKEND_NANOVG_FBO_MAX 2

typedef enum _sprite_type_t {
	SPRITE_TYPE_NONE = 0,
	SPRITE_TYPE_FBO  = 1,
	SPRITE_TYPE_IMG  = 2,
	SPRITE_TYPE_FONT = 3
} sprite_type_t;

typedef struct _d2tk_backend_nanovg_t d2tk_backend_nanovg_t;

struct _d2tk_backend_nanovg_t {
	NVGcontext *ctx;
	char *bundle_path;
	NVGLUframebuffer *fbo [D2TK_BACKEND_NANOVG_FBO_MAX];
	bool fbop;
	d2tk_coord_t w;
	d2tk_coord_t h;
	int mask;
};

static void
d2tk_nanovg_free(void *data)
{
	d2tk_backend_nanovg_t *backend = data;

	for(unsigned f = 0; f < D2TK_BACKEND_NANOVG_FBO_MAX; f++)
	{
		if(backend->fbo[f])
		{
			nvgluDeleteFramebuffer(backend->fbo[f]);
			backend->fbo[f] = NULL;
		}
	}

	if(backend->mask)
	{
		nvgDeleteImage(backend->ctx, backend->mask);
		backend->mask = 0;
	}

	if(backend->ctx)
	{
		nvgDelete(backend->ctx);
		backend->ctx = NULL;
	}

	free(backend->bundle_path);
	free(backend);
}

static void *
d2tk_nanovg_new(const char *bundle_path)
{
#if defined(__APPLE__)
//FIXME
#else
	glewExperimental = GL_TRUE;
	const GLenum err = glewInit();
	if(err != GLEW_OK)
	{
		fprintf(stderr, "glewInit failed: %s\n", glewGetErrorString(err));
		return NULL;
	}
#endif

	d2tk_backend_nanovg_t *backend = calloc(1, sizeof(d2tk_backend_nanovg_t));
	if(!backend)
	{
		fprintf(stderr, "calloc failed\n");
		return NULL;
	}

	NVGcontext *ctx = nvgCreate(NVG_ANTIALIAS | NVG_STENCIL_STROKES);
	if(!ctx)
	{
		fprintf(stderr, "nvgCreate failed\n");
		free(backend);
		return NULL;
	}

	backend->ctx = ctx;
	backend->bundle_path = strdup(bundle_path);

	return backend;
}

static int
d2tk_nanovg_context(void *data __attribute__((unused)),
	void *pctx __attribute__((unused)))
{
	// nothing to do
	
	return 0;
}

static inline void
d2tk_nanovg_pre(void *data, d2tk_core_t *core, d2tk_coord_t w, d2tk_coord_t h,
	unsigned pass)
{
	d2tk_backend_nanovg_t *backend = data;
	NVGcontext *ctx = backend->ctx;;

	if(pass == 0) // is this 1st pass?
	{
		return;
	}

	const bool configured = (backend->w != w) || (backend->h != h);

	if(configured) // window size changed
	{
		backend->w = w;
		backend->h = h;

		for(unsigned f = 0; f < D2TK_BACKEND_NANOVG_FBO_MAX; f++)
		{
			if(backend->fbo[f])
			{
				nvgluDeleteFramebuffer(backend->fbo[f]);
				backend->fbo[f] = NULL;
			}
		}

		if(backend->mask)
		{
			nvgDeleteImage(ctx, backend->mask);
			backend->mask = 0;
		}
	}

	for(unsigned f = 0; f < D2TK_BACKEND_NANOVG_FBO_MAX; f++)
	{
		if(!backend->fbo[f])
		{
			backend->fbo[f] = nvgluCreateFramebuffer(ctx, w, h, NVG_IMAGE_NEAREST);
			assert(backend->fbo[f]);
		}
	}

	// draw to foreground framebuffer object
	NVGLUframebuffer *fbo_fg = backend->fbo[backend->fbop];
	nvgluBindFramebuffer(fbo_fg);

	glViewport(0, 0, w, h);
	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	nvgBeginFrame(ctx, w, h, 1.f);
	nvgSave(ctx);

	if(!configured)
	{
		// draw background framebuffer object (aka last foreground framebuffer object)
		NVGLUframebuffer *fbo_bg = backend->fbo[!backend->fbop];
		const NVGpaint bg = nvgImagePattern(ctx, 0, 0, w, h, 0, fbo_bg->image, 1.0f);
		nvgBeginPath(ctx);
		nvgRect(ctx, 0, 0, w, h);
		nvgStrokeWidth(ctx, 0);
		nvgFillPaint(ctx, bg);
		nvgFill(ctx);
	}

	{
		// draw mask
		d2tk_rect_t rect;
		uint32_t *pixels = d2tk_core_get_pixels(core, &rect);

		if(backend->mask)
		{
			nvgUpdateSubImage(ctx, backend->mask, (const uint8_t *)pixels,
				rect.x, rect.y, rect.w, rect.h);
		}
		else
		{
			backend->mask = nvgCreateImageRGBA(ctx, w, h, NVG_IMAGE_NEAREST,
				(const uint8_t *)pixels);
		}

		const NVGpaint bg = nvgImagePattern(ctx, 0, 0, w, h, 0, backend->mask, 1.f);
		nvgBeginPath(ctx);
		nvgRect(ctx, rect.x, rect.y, rect.w, rect.h);
		nvgStrokeWidth(ctx, 0);
		nvgFillPaint(ctx, bg);
		nvgFill(ctx);
	}
}

static inline bool
d2tk_nanovg_post(void *data, d2tk_core_t *core __attribute__((unused)),
	d2tk_coord_t w __attribute__((unused)), d2tk_coord_t h __attribute__((unused)),
	unsigned pass)
{
	if(pass == 0) // is this 1st pass?
	{
		return true; // do enter 2nd pass
	}

	d2tk_backend_nanovg_t *backend = data;
	NVGcontext *ctx = backend->ctx;;

	nvgRestore(ctx);
	nvgEndFrame(ctx);

	nvgluBindFramebuffer(NULL);

	// switch foreground and background framebuffer objects
	backend->fbop = !backend->fbop;

	return false; // do NOT enter 3rd pass
}

static inline void
d2tk_nanovg_end(void *data __attribute__((unused)),
	d2tk_core_t *core, d2tk_coord_t w, d2tk_coord_t h)
{
	d2tk_backend_nanovg_t *backend = data;
	NVGcontext *ctx = backend->ctx;;

	glViewport(0, 0, w, h);
	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	nvgBeginFrame(ctx, w, h, 1.f);
	nvgSave(ctx);

	// draw former foreground framebuffer object to main framebuffer
	NVGLUframebuffer *fbo_fg = backend->fbo[!backend->fbop];
	const NVGpaint fg = nvgImagePattern(ctx, 0, 0, w, h, 0, fbo_fg->image, 1.0f);
	nvgBeginPath(ctx);
	nvgRect(ctx, 0, 0, w, h);
	nvgStrokeWidth(ctx, 0);
	nvgFillPaint(ctx, fg);
	nvgFill(ctx);

#if D2TK_DEBUG
	{
		d2tk_rect_t rect;
		uint32_t *pixels = d2tk_core_get_pixels(core, &rect);

		// brighten up the pixels for proper hilighting
		for(d2tk_coord_t y = 0, Y = 0; y < h; y++, Y+=w)
		{
			for(d2tk_coord_t x = 0; x < w; x++)
			{
				if(pixels[Y + x])
				{
					pixels[Y + x] = 0x5fffff00;; // ABGR
				}
			}
		}

		if(backend->mask)
		{
			nvgUpdateImage(ctx, backend->mask, (const uint8_t *)pixels);
		}
		else
		{
			backend->mask = nvgCreateImageRGBA(ctx, w, h, NVG_IMAGE_NEAREST,
				(const uint8_t *)pixels);
		}
	
		const NVGpaint bg = nvgImagePattern(ctx, 0, 0, w, h, 0, backend->mask, 1.f);
		nvgBeginPath(ctx);
		nvgRect(ctx, 0, 0, w, h);
		nvgStrokeWidth(ctx, 0);
		nvgFillPaint(ctx, bg);
		nvgFill(ctx);
	}
#else
	(void)core;
#endif

	nvgRestore(ctx);
	nvgEndFrame(ctx);
}

static inline void
d2tk_nanovg_sprite_free(void *data, uint8_t type, uintptr_t body)
{
	d2tk_backend_nanovg_t *backend = data;
	NVGcontext *ctx = backend->ctx;

	switch((sprite_type_t)type)
	{
		case SPRITE_TYPE_FBO:
		{
			NVGLUframebuffer *fbo = (NVGLUframebuffer *)body;

			nvgluDeleteFramebuffer(fbo);
		} break;
		case SPRITE_TYPE_IMG:
		{
			int img = body;

			nvgDeleteImage(ctx, img);
		} break;
		case SPRITE_TYPE_FONT:
		{
			// fonts are automatically freed
		} break;
		case SPRITE_TYPE_NONE:
		{
			// nothing to do
		} break;
	}
}

static inline int 
d2tk_nanovg_text_extent(void *data, size_t len, const char *buf, d2tk_coord_t h)
{
	d2tk_backend_nanovg_t *backend = data;
	NVGcontext *ctx = backend->ctx;

	nvgFontSize(ctx, h);
	//FIXME we need to take font face into account, too

	return nvgTextBounds(ctx, 0, 0, buf, &buf[len], NULL);
}

static inline void
_d2tk_nanovg_surf_draw(NVGcontext *ctx, int img, d2tk_coord_t xo,
	d2tk_coord_t yo, d2tk_align_t align, const d2tk_rect_t *rect)
{
	int W, H;
	nvgImageSize(ctx, img, &W, &H);

	d2tk_coord_t w = W;
	d2tk_coord_t h = H;
	float scale = 1.f;

	if(h != rect->h)
	{
		scale = (float)rect->h / h;
		w *= scale;
		h = rect->h;
	}

	if(w > rect->w)
	{
		const float scale_t = (float)rect->w / w;
		scale *= scale_t;
		h *= scale_t;
		w = rect->w;
	}

	d2tk_coord_t x = rect->x + xo;
	d2tk_coord_t y = rect->y + yo;

	if(align & D2TK_ALIGN_LEFT)
	{
		x += 0;
	}
	else if(align & D2TK_ALIGN_CENTER)
	{
		x += rect->w / 2;
		x -= w / 2;
	}
	else if(align & D2TK_ALIGN_RIGHT)
	{
		x += rect->w;
		x -= w;
	}

	if(align & D2TK_ALIGN_TOP)
	{
		y += 0;
	}
	else if(align & D2TK_ALIGN_MIDDLE)
	{
		y += rect->h / 2;
		y -= h / 2;
	}
	else if(align & D2TK_ALIGN_BOTTOM)
	{
		y += rect->h;
		y -= h;
	}

	const NVGpaint bg = nvgImagePattern(ctx, x, y, w, h, 0, img, 1.f);
	nvgBeginPath(ctx);
	nvgRect(ctx, x, y, w, h);
	nvgStrokeWidth(ctx, 0);
	nvgFillPaint(ctx, bg);
	nvgFill(ctx);
}

static inline char *
_absolute_path(d2tk_backend_nanovg_t *backend, const char *rel)
{
	char *abs = NULL;

	if(rel[0] == '/')
	{
		assert(asprintf(&abs, "%s", rel) != -1);
	}
	else
	{
		assert(asprintf(&abs, "%s%s", backend->bundle_path, rel) != -1);
	}

	return abs;
}

static inline void
d2tk_nanovg_process(void *data, d2tk_core_t *core, const d2tk_com_t *com,
	d2tk_coord_t xo, d2tk_coord_t yo, const d2tk_clip_t *clip, unsigned pass)
{
	d2tk_backend_nanovg_t *backend = data;
	NVGcontext *ctx = backend->ctx;;

	const d2tk_instr_t instr = com->instr;
	switch(instr)
	{
		case D2TK_INSTR_LINE_TO:
		{
			const d2tk_body_line_to_t *body = &com->body->line_to;

			nvgLineTo(ctx, body->x + xo, body->y + yo);
		} break;
		case D2TK_INSTR_MOVE_TO:
		{
			const d2tk_body_move_to_t *body = &com->body->move_to;

			nvgMoveTo(ctx, body->x + xo, body->y + yo);
		} break;
		case D2TK_INSTR_RECT:
		{
			const d2tk_body_rect_t *body = &com->body->rect;

			nvgRect(ctx, body->x + xo, body->y + yo, body->w, body->h);
		} break;
		case D2TK_INSTR_ROUNDED_RECT:
		{
			const d2tk_body_rounded_rect_t *body = &com->body->rounded_rect;

			if(body->r > 0)
			{
				nvgRoundedRect(ctx, body->x + xo, body->y + yo, body->w, body->h, body->r);
			}
			else
			{
				nvgRect(ctx, body->x + xo, body->y + yo, body->w, body->h);
			}
		} break;
		case D2TK_INSTR_ARC:
		{
			const d2tk_body_arc_t *body = &com->body->arc;

			static const float mul = M_PI / 180;
			const float a = body->a * mul;
			const float b = body->b * mul;
			const int dir = body->cw ? NVG_CW : NVG_CCW;

			nvgArc(ctx, body->x + xo, body->y + yo, body->r, a, b, dir);
		} break;
		case D2TK_INSTR_CURVE_TO:
		{
			const d2tk_body_curve_to_t *body = &com->body->curve_to;

			nvgBezierTo(ctx,
				body->x1 + xo, body->y1 + yo,
				body->x2 + xo, body->y2 + yo,
				body->x3 + xo, body->y3 + yo);
		} break;
		case D2TK_INSTR_COLOR:
		{
			const d2tk_body_color_t *body = &com->body->color;

			const uint8_t r = (body->rgba >> 24) & 0xff;
			const uint8_t g = (body->rgba >> 16) & 0xff;
			const uint8_t b = (body->rgba >>  8) & 0xff;
			const uint8_t a = (body->rgba >>  0) & 0xff;
			const NVGcolor col = nvgRGBA(r, g, b, a);

			nvgFillColor(ctx, col);
			nvgStrokeColor(ctx, col);
		} break;
		case D2TK_INSTR_LINEAR_GRADIENT:
		{
			const d2tk_body_linear_gradient_t *body = &com->body->linear_gradient;

			const uint8_t r0 = (body->rgba[0] >> 24) & 0xff;
			const uint8_t g0 = (body->rgba[0] >> 16) & 0xff;
			const uint8_t b0 = (body->rgba[0] >>  8) & 0xff;
			const uint8_t a0 = (body->rgba[0] >>  0) & 0xff;
			const NVGcolor col0 = nvgRGBA(r0, g0, b0, a0);

			const uint8_t r1 = (body->rgba[1] >> 24) & 0xff;
			const uint8_t g1 = (body->rgba[1] >> 16) & 0xff;
			const uint8_t b1 = (body->rgba[1] >>  8) & 0xff;
			const uint8_t a1 = (body->rgba[1] >>  0) & 0xff;
			const NVGcolor col1 = nvgRGBA(r1, g1, b1, a1);

			const d2tk_coord_t x0 = body->p[0].x + xo;
			const d2tk_coord_t y0 = body->p[0].y + yo;
			const d2tk_coord_t x1 = body->p[1].x + xo;
			const d2tk_coord_t y1 = body->p[1].y + yo;

			NVGpaint bg = nvgLinearGradient(ctx, x0, y0, x1, y1, col0, col1);
			nvgFillPaint(ctx, bg);
		} break;
		case D2TK_INSTR_ROTATE:
		{
			const d2tk_body_rotate_t *body = &com->body->rotate;

			static const float mul = M_PI / 180;
			const float rad = body->deg * mul;

			nvgRotate(ctx, rad);
		} break;
		case D2TK_INSTR_STROKE:
		{
			nvgStroke(ctx);
		} break;
		case D2TK_INSTR_FILL:
		{
			nvgFill(ctx);
		} break;
		case D2TK_INSTR_SAVE:
		{
			nvgSave(ctx);
		} break;
		case D2TK_INSTR_RESTORE:
		{
			nvgRestore(ctx);
		} break;
		case D2TK_INSTR_BBOX:
		{
			const d2tk_body_bbox_t *body = &com->body->bbox;

			if(pass == 0)
			{
				if(body->cached)
				{
					uintptr_t *sprite = d2tk_core_get_sprite(core, body->hash, SPRITE_TYPE_FBO);
					assert(sprite);

					if(!*sprite)
					{
#if D2TK_DEBUG
						//fprintf(stderr, "\tcreating sprite\n");
#endif
						NVGLUframebuffer *fbo = nvgluCreateFramebuffer(ctx, body->clip.w, body->clip.h, NVG_IMAGE_NEAREST);
						assert(fbo);

						nvgluBindFramebuffer(fbo);

						glViewport(0, 0, body->clip.w, body->clip.h);
						glClearColor(0.f, 0.f, 0.f, 0.f);
						glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

						nvgBeginFrame(ctx, body->clip.w, body->clip.h, 1.f);
						nvgSave(ctx);

						D2TK_COM_FOREACH_CONST(com, bbox)
						{
							d2tk_nanovg_process(backend, core, bbox, 0, 0, clip, pass);
						}

						nvgRestore(ctx);
						nvgEndFrame(ctx);

						nvgluBindFramebuffer(NULL);

						*sprite = (uintptr_t )fbo;
					}
					else
					{
#if D2TK_DEBUG
						//fprintf(stderr, "\texisting sprite\n");
#endif
					}
				}
				else // !body->cached
				{
					// render directly
					D2TK_COM_FOREACH_CONST(com, bbox)
					{
						d2tk_nanovg_process(backend, core, bbox, body->clip.x0, body->clip.y0, clip, pass);
					}
				}
			}
			else if(pass == 1)
			{
				nvgSave(ctx);
				if(clip)
				{
					nvgScissor(ctx, clip->x0, clip->y0, clip->w, clip->h);
				}

				if(body->cached)
				{
					uintptr_t *sprite = d2tk_core_get_sprite(core, body->hash, SPRITE_TYPE_FBO);
					assert(sprite && *sprite);

					NVGLUframebuffer *fbo = (NVGLUframebuffer *)*sprite;
					assert(fbo);

					// paint pre-rendered sprite
					const NVGpaint pat = nvgImagePattern(ctx, body->clip.x0, body->clip.y0,
						body->clip.w, body->clip.h, 0, fbo->image, 1.0f);
					nvgBeginPath(ctx);
					nvgRect(ctx, body->clip.x0, body->clip.y0, body->clip.w, body->clip.h);
					nvgStrokeWidth(ctx, 0);
					nvgFillPaint(ctx, pat);
					nvgFill(ctx);
				}
				else // !body->cached
				{
					// render directly
					D2TK_COM_FOREACH_CONST(com, bbox)
					{
						d2tk_nanovg_process(backend, core, bbox, body->clip.x0, body->clip.y0, clip, pass);
					}
				}

				nvgRestore(ctx);
			}
		} break;
		case D2TK_INSTR_BEGIN_PATH:
		{
			nvgBeginPath(ctx);
		} break;
		case D2TK_INSTR_CLOSE_PATH:
		{
			nvgClosePath(ctx);
		} break;
		case D2TK_INSTR_SCISSOR:
		{
			const d2tk_body_scissor_t *body = &com->body->scissor;

			nvgScissor(ctx, body->x + xo, body->y + yo, body->w, body->h);
		} break;
		case D2TK_INSTR_RESET_SCISSOR:
		{
			nvgResetScissor(ctx);
		} break;
		case D2TK_INSTR_FONT_FACE:
		{
			const d2tk_body_font_face_t *body = &com->body->font_face;

			const uint64_t hash = d2tk_hash(body->face, strlen(body->face));
			uintptr_t *sprite = d2tk_core_get_sprite(core, hash, SPRITE_TYPE_FONT);
			assert(sprite);

			if(!*sprite)
			{
				char ft_path [1024];
				d2tk_core_get_font_path(core, backend->bundle_path, body->face,
					sizeof(ft_path), ft_path);

				const int face = nvgCreateFont(ctx, body->face, ft_path);
				if(face == -1)
				{
					fprintf(stderr, "nvgCreateFont failed on '%s'\n", ft_path);
					break;
				}

				*sprite = (uintptr_t)face;
			}

			nvgFontFaceId(ctx, *sprite);
		} break;
		case D2TK_INSTR_FONT_SIZE:
		{
			const d2tk_body_font_size_t *body = &com->body->font_size;

			nvgFontSize(ctx, body->size);
		} break;
		case D2TK_INSTR_TEXT:
		{
			const d2tk_body_text_t *body = &com->body->text;

			int align = 0;
			d2tk_coord_t x = body->x;
			d2tk_coord_t y = body->y;

			if(body->align & D2TK_ALIGN_LEFT)
			{
				align |= NVG_ALIGN_LEFT;
				x = body->x;
			}
			else if(body->align & D2TK_ALIGN_CENTER)
			{
				align |= NVG_ALIGN_CENTER;
				x = body->x + body->w / 2;
			}
			else if(body->align & D2TK_ALIGN_RIGHT)
			{
				align |= NVG_ALIGN_RIGHT;
				x = body->x + body->w;
			}

			if(body->align & D2TK_ALIGN_TOP)
			{
				align |= NVG_ALIGN_TOP;
				y = body->y;
			}
			else if(body->align & D2TK_ALIGN_MIDDLE)
			{
				align |= NVG_ALIGN_MIDDLE;
				y = body->y + body->h / 2;
			}
			else if(body->align & D2TK_ALIGN_BOTTOM)
			{
				align |= NVG_ALIGN_BOTTOM;
				y = body->y + body->h;
			}

			nvgTextAlign(ctx, align);
			nvgText(ctx, x + xo, y + yo, body->text, NULL);
		} break;
		case D2TK_INSTR_IMAGE:
		{
			const d2tk_body_image_t *body = &com->body->image;

			const uint64_t hash = d2tk_hash(body->path, strlen(body->path));
			uintptr_t *sprite = d2tk_core_get_sprite(core, hash, SPRITE_TYPE_IMG);
			assert(sprite);

			if(!*sprite)
			{
				char *img_path = _absolute_path(backend, body->path);
				assert(img_path);

				struct stat st;
				if(stat(img_path, &st) == 0)
				{
					*sprite = nvgCreateImage(ctx, img_path, NVG_IMAGE_GENERATE_MIPMAPS);
				}

				free(img_path);
			}

			const int img = *sprite;

			if(img)
			{
				_d2tk_nanovg_surf_draw(ctx, img, xo, yo, body->align,
						&D2TK_RECT(body->x, body->y, body->w, body->h));
			}
		} break;
		case D2TK_INSTR_BITMAP:
		{
			const d2tk_body_bitmap_t *body = &com->body->bitmap;

			const uint64_t hash = d2tk_hash(&body->surf, sizeof(body->surf));
			uintptr_t *sprite = d2tk_core_get_sprite(core, hash, SPRITE_TYPE_IMG);
			assert(sprite);

			if(!*sprite)
			{
				*sprite = nvgCreateImageARGB(ctx, body->surf.w, body->surf.h,
					NVG_IMAGE_GENERATE_MIPMAPS | NVG_IMAGE_PREMULTIPLIED,
					(const uint8_t *)body->surf.argb);
				//TODO use nvgUpdateImage for changed content
			}

			const int img = *sprite;
			assert(img);

			_d2tk_nanovg_surf_draw(ctx, img, xo, yo, body->align,
					&D2TK_RECT(body->x, body->y, body->w, body->h));
		} break;
		case D2TK_INSTR_CUSTOM:
		{
			const d2tk_body_custom_t *body = &com->body->custom;

			nvgSave(ctx);
			body->custom(ctx, &D2TK_RECT(body->x + xo, body->y + yo, body->w, body->h),
				body->data);
			nvgRestore(ctx);
		} break;
		case D2TK_INSTR_STROKE_WIDTH:
		{
			const d2tk_body_stroke_width_t *body = &com->body->stroke_width;

			nvgStrokeWidth(ctx, body->width);
		} break;
		default:
		{
			fprintf(stderr, "%s: unknown command (%i)\n", __func__, com->instr);
		} break;
	}
}

const d2tk_core_driver_t d2tk_core_driver = {
	.new = d2tk_nanovg_new,
	.free = d2tk_nanovg_free,
	.context = d2tk_nanovg_context,
	.pre = d2tk_nanovg_pre,
	.process = d2tk_nanovg_process,
	.post = d2tk_nanovg_post,
	.end = d2tk_nanovg_end,
	.sprite_free = d2tk_nanovg_sprite_free,
	.text_extent = d2tk_nanovg_text_extent
};
