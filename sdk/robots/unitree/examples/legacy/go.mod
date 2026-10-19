// Offline only: stage actual SDK at its original relative path and the complete
// Noetic-bound generated Go family under customtypes/. No network resolution.
module example.local/uwvm-unitree-legacy

go 1.23

require uwvm2.local/roswasm v0.0.0

replace uwvm2.local/roswasm => ../../../../tinygo
