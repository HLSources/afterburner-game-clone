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

// If image.depth = 3, assumes image.size = (4*size)/3
static void CopyImageData(const stbi_uc* loadedData, size_t size)
{
	switch (image.depth)
	{
		case 4:
		{
			memcpy(image.rgba, loadedData, size);
			break;
		}

		case 3:
		{
			size_t source = 0;
			size_t dest = 0;

			for (dest = 0; dest < image.size; ++dest)
			{
				image.rgba[dest] = (dest % 4 != 3) ? loadedData[source++] : 255;
			}

			break;
		}

		default:
		{
			break;
		}
	}
}

qboolean Image_LoadPNG(const char *fileName, const byte *rawImageData, size_t fileSize)
{
	int width = 0;
	int height = 0;
	int depth = 0;
	stbi_uc* loadedData = NULL;

	do
	{
		loadedData = stbi_load_from_memory(rawImageData, (int)fileSize, &width, &height, &depth, 0);

		if (!loadedData)
		{
			MsgDev(D_WARN, "Image_LoadPNG: File \"%s\" unable to be loaded. %s\n", fileName, stbi_failure_reason());
			break;
		}

		if (depth < 3)
		{
			MsgDev(D_WARN, "Image_LoadPNG: Only 24-bit and 32-bit PNGs are supported at the current time.\n");
			break;
		}

		// We loaded successfully. Set all relevant image metadata.
		image.width = width;
		image.height = height;
		image.depth = depth;
		image.size = image.width * image.height * 4;
		image.type = PF_RGBA_32;
		image.flags |= IMAGE_HAS_COLOR;
		
		if (depth == 4)
		{
			image.flags |= IMAGE_HAS_ALPHA;
		}

		image.rgba = Mem_Alloc(host.imagepool, image.size);
		CopyImageData(loadedData, width * height * depth);
		stbi_image_free(loadedData);

		return true;
	}
	while (false);

	if (loadedData)
	{
		stbi_image_free(loadedData);
	}

	return false;
}