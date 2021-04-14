$cd "../common_anims"
$cdtexture "../../../../afterburner/textures"

$modelname mp_mi6_tux.mdl
$cliptotextures

// No embedded textures
$flags 2048

$scale 1.0
$origin 0 0 36

$bodygroup body
{
	// Relative to common_anims
	studio "../mp_mi6_tux/mp_mi6_tux_ref"
}

$include "../common_anims/player_shared.qc"
