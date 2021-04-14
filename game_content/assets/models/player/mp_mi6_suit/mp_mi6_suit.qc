$cd "../common_anims"
$cdtexture "../../../../afterburner/textures"

$modelname mp_mi6_suit.mdl
$cliptotextures

// No embedded textures
$flags 2048

$scale 1.0
$origin 0 0 36

$bodygroup body
{
	// Relative to common_anims
	studio "../mp_mi6_suit/mp_mi6_suit_ref"
}

$include "../common_anims/player_shared.qc"
