package audio

import "core:log"
import "core:path/filepath"
import "core:strings"
import "engine:app"
import "vendor:sdl3"
import "vendor:sdl3/mixer"

// sfx
sfxTrack: ^mixer.Track

sfx_uiHover: ^mixer.Audio
sfx_uiPress: ^mixer.Audio
sfx_uiRelease: ^mixer.Audio

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

deinit :: proc() {
	if !initialized {
		log.error("Audio is already deinitialized")
		return
	}
	initialized = false

	// sfx
	mixer.DestroyTrack(sfxTrack)

	mixer.DestroyAudio(sfx_uiHover)
	mixer.DestroyAudio(sfx_uiPress)
	mixer.DestroyAudio(sfx_uiRelease)

	// music
	mixer.DestroyTrack(musicTrack)

	mixer.DestroyMixer(app.audioMixer)
}

loadAudio :: proc() -> bool {
	log.info("Loading audio assets")

	if !initialized {
		log.error("Failed to load audio: sound has not been initialized")
		return false
	}

	sfx_uiHover = load("hover.ogg")
	sfx_uiPress = load("press.ogg")
	sfx_uiRelease = load("release.ogg")

	return true
}

@(private)
load :: proc(name: string) -> (audio: ^mixer.Audio) {
	path, _ := filepath.join(
		{string(sdl3.GetBasePath()), "../src/assets/audio/", name},
		context.temp_allocator,
	)

	log.infof("Loading audio from \"%v\"", path)

	audio = mixer.LoadAudio(app.audioMixer, strings.clone_to_cstring(path), true)

	if audio == nil {
		log.errorf("Error loading audio: %v", sdl3.GetError())
	}
	return
}

