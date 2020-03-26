import struct
import format_defs
import io
from PIL import Image

class SingleFrameSpriteData:
	def __init__(self):
		self.header = None
		self.frameType = None
		self.data = None

def imageToByteArray(imageFile : Image):
	imgIO = io.BytesIO()
	imageFile.save(imgIO, format=imageFile.format)
	return imgIO.getvalue()

def createSingleFrameSprite(imageFile : Image):
	# Required:
	# - Header ident = SPRITE_HEADER_IDENT
	# - Version = SPRITE_VERSION_32
	# - Angle type = SPRITE_ANGLETYPE_FWD_PARALLEL (will do for now)
	# - Bounding radius - appears to be specified as a float but interpreted as an int??? Leave as 0.
	# - Min/max (should be image dimensions)
	# - Num frames = 1
	# - Beam length = 0
	# - Sync type = SPRITE_SYNCTYPE_RAND
	# - Frame type = SPRITE_FRAMETYPE_SINGLE

	sprite = SingleFrameSpriteData()

	sprite.header = struct.pack(format_defs.SPRITE_QUAKE_HEADER_FORMAT,
								format_defs.SPRITE_HEADER_IDENT,
								format_defs.SPRITE_VERSION_32,
								format_defs.SPRITE_ANGLETYPE_FWD_PARALLEL,
								0,
								imageFile.width,
								imageFile.height,
								1,
								0,
								format_defs.SPRITE_SYNCTYPE_RAND)

	sprite.frameType = struct.pack(format_defs.SPRITE_FRAMETYPE_FORMAT, format_defs.SPRITE_FRAMETYPE_SINGLE)
	sprite.data = imageToByteArray(imageFile)
	return sprite
