/*
img_png.c - Loading PNGs for use as textures.
Copyright (C) 2017 X6Herbius

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
*/

#include "imagelib.h"
#include "stb_image.h"

qboolean Image_LoadPNG(const char *fileName, const byte *rawImageData, size_t fileSize)
{
	static const int RGBA_SIZE = 4;

	qboolean success = false;
	int width = 0;
	int height = 0;
	int depth = 0;
	stbi_uc* loadedData = NULL;

	do
	{
		loadedData = stbi_load_from_memory(rawImageData, (int)fileSize, &width, &height, &depth, RGBA_SIZE);

		if (!loadedData)
		{
			Con_DPrintf(S_WARN "Image_LoadPNG: File \"%s\" unable to be loaded. %s\n", fileName, stbi_failure_reason());
			break;
		}

		if (depth != 3 && depth != 4)
		{
			Con_DPrintf(S_WARN "Image_LoadPNG: Expected 3 or 4 colour channels, but got %d.\n", depth);
			break;
		}

		// We loaded successfully. Set all relevant image metadata.
		// Depth is used for co-ordinate space (eg. 3D textures), NOT the colour depth.
		image.width = width;
		image.height = height;
		image.depth = 1;
		image.size = image.width * image.height * RGBA_SIZE;
		image.type = PF_RGBA_32;
		image.flags |= IMAGE_HAS_COLOR;

		if (depth == RGBA_SIZE)
		{
			image.flags |= IMAGE_HAS_ALPHA;
		}

		image.rgba = Mem_Malloc(host.imagepool, image.size);
		memcpy(image.rgba, loadedData, image.size);

		success = true;
	}
	while (false);

	if (loadedData)
	{
		stbi_image_free(loadedData);
	}

	return success;
}
