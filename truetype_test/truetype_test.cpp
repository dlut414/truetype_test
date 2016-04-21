// truetype_test.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include <stb_truetype.h>

#define STB_RECT_PACK_IMPLEMENTATION
#include "stb_rect_pack.h"
#define STB_TRUETYPE_IMPLEMENTATION
#include "stb_truetype.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"


#include <stdio.h>

unsigned char ttf_buffer[1 << 25];
unsigned char output[512 * 100];

void debug(void) {
	stbtt_fontinfo font;
	fread(ttf_buffer, 1, 1 << 25, fopen("c:/x/lm/LiberationMono-Regular.ttf", "rb"));
	stbtt_InitFont(&font, ttf_buffer, 0);

	stbtt_MakeGlyphBitmap(&font, output, 6, 9, 512, 5.172414E-03f, 5.172414E-03f, 54);
}

#define BITMAP_W  1024
#define BITMAP_H  1024
unsigned char temp_bitmap[BITMAP_H][BITMAP_W];
stbtt_bakedchar cdata[256 * 2]; // ASCII 32..126 is 95 glyphs
stbtt_packedchar pdata[256 * 2];
#if 0

int main(int argc, char **argv) {
	stbtt_fontinfo font;
	unsigned char *bitmap;
	int w, h, i, j,
		//(argc > 1 ? atoi(argv[1]) : 34807)
		s = (argc > 2 ? atoi(argv[2]) : 32);

	const char c[] = { "ABCDEFG" };

	//debug();

	// @TODO: why is minglui.ttc failing? 
	fread(ttf_buffer, 1, 1 << 25, fopen(argc > 3 ? argv[3] : "c:/windows/fonts/mingliu.ttc", "rb"));

	//fread(ttf_buffer, 1, 1<<25, fopen(argc > 3 ? argv[3] : "c:/x/DroidSansMono.ttf", "rb"));
	{
		static stbtt_pack_context pc;
		static stbtt_packedchar cd[256];
		static unsigned char atlas[1024 * 1024];

		stbtt_PackBegin(&pc, atlas, 1024, 1024, 1024, 1, NULL);
		stbtt_PackFontRange(&pc, ttf_buffer, 0, 32.0, 0, 256, cd);
		stbtt_PackEnd(&pc);
	}

	stbtt_InitFont(&font, ttf_buffer, stbtt_GetFontOffsetForIndex(ttf_buffer, 0));
	bitmap = stbtt_GetCodepointBitmap(&font, 0, stbtt_ScaleForPixelHeight(&font, (float)s), '3', &w, &h, 0, 0);

	for (j = 0; j < h; ++j) {
		for (i = 0; i < w; ++i)
			putchar(" .:ioVM@"[bitmap[j*w + i] >> 5]);
		putchar('\n');
	}
	getchar();
	return 0;
}

#endif // 0
unsigned char buffer[24 << 20];
unsigned char screen[20][79];

int main(int arg, char **argv) {
	stbtt_fontinfo font;
	int i, j, ascent, baseline, ch = 0;
	float scale, xpos = 2; // leave a little padding in case the character extends left
	char *text = "f_ck123!"; // intentionally misspelled to show 'lj' brokenness

	fread(buffer, 1, 1000000, fopen("c:/windows/fonts/arialbd.ttf", "rb"));
	stbtt_InitFont(&font, buffer, 0);

	scale = stbtt_ScaleForPixelHeight(&font, 15);
	stbtt_GetFontVMetrics(&font, &ascent, 0, 0);
	baseline = (int)(ascent*scale);

	while (text[ch]) {
		int advance, lsb, x0, y0, x1, y1;
		float x_shift = xpos - (float)floor(xpos);
		stbtt_GetCodepointHMetrics(&font, text[ch], &advance, &lsb);
		stbtt_GetCodepointBitmapBoxSubpixel(&font, text[ch], scale, scale, x_shift, 0, &x0, &y0, &x1, &y1);
		stbtt_MakeCodepointBitmapSubpixel(&font, &screen[baseline + y0][(int)xpos + x0], x1 - x0, y1 - y0, 79, scale, scale, x_shift, 0, text[ch]);
		// note that this stomps the old data, so where character boxes overlap (e.g. 'lj') it's wrong
		// because this API is really for baking character bitmaps into textures. if you want to render
		// a sequence of characters, you really need to render each bitmap to a temp buffer, then
		// "alpha blend" that into the working buffer
		xpos += (advance * scale);
		if (text[ch + 1])
			xpos += scale*stbtt_GetCodepointKernAdvance(&font, text[ch], text[ch + 1]);
		++ch;
	}

	for (j = 0; j < 20; ++j) {
		for (i = 0; i < 78; ++i)
			putchar(" .:ioVM@"[screen[j][i] >> 5]);
		putchar('\n');
	}

	return 0;
}