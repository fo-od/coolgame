package audio

import "core:log"
import "core:strings"
import "engine:app"
import "vendor:sdl3"
import "vendor:sdl3/mixer"

// sfx
sfxTrack: ^mixer.Track

uiHover: ^mixer.Audio
uiPress: ^mixer.Audio
uiRelease: ^mixer.Audio

// music, progress not started currently
musicTrack: ^mixer.Track

@(private)
initialized := false

init :: proc() -> bool {


	log.info("Initalizing mixer")
	mixer.Init() or_return

	log.info("Creating mixer using default playback device")
	app.audioMixer = mixer.CreateMixerDevice(sdl3.AUDIO_DEVICE_DEFAULT_PLAYBACK, nil)
	if app.audioMixer == nil do return false

	log.info("Creating sfx track")
	sfxTrack = mixer.CreateTrack(app.audioMixer)

	initialized = true

	// TODO: load audio at a different time? perhaps implement a loading screen later on
	loadAudio() or_return

	return true
}

loadAudio :: proc() -> bool {
	log.info("Loading audio assets")

	if !initialized {
		log.error("Failed to load audio: sound has not been initialized")
		return false
	}

	uiHover = load("hover.ogg")
	uiPress = load("press.ogg")
	uiRelease = load("release.ogg")

	return true
}

@(private)
load :: proc(name: string) -> ^mixer.Audio {
	path := strings.concatenate({"../src/assets/audio/", name}, context.temp_allocator)
	log.infof("Loading audio from \"%v\"", path)
	return mixer.LoadAudio(app.audioMixer, strings.clone_to_cstring(path), true)
}

