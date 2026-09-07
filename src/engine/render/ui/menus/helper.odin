package menus

import "core:log"
import "engine:audio"
import "engine:render/ui"
import "vendor:sdl3/mixer"

Layout :: struct {
	pos:    [2]f32,
	anchor: bit_set[ui.Anchor],
	origin: bit_set[ui.Anchor],
}

button :: proc(text: string, layout: Layout, callback: proc()) {
	if ui.rectangle()(
	{
		rect = {layout.pos.x, layout.pos.y, 100, 50},
		layout = {anchor = layout.anchor, origin = layout.origin},
		style = {color = ui.WHITE, filled = false},
	},
	) {
		ui.set_current_style({color = ui.pressed() ? ui.GRAY : ui.WHITE, filled = ui.hovered()})

		ui.text(
			text,
			{
				style = {color = ui.hovered() ? ui.BLACK : ui.WHITE},
				layout = {anchor = {.Center}, origin = {.Center}},
			},
		)

		if ui.clicked() {
			_ = mixer.SetTrackAudio(audio.sfxTrack, audio.sfx_uiPress)
			_ = mixer.PlayTrack(audio.sfxTrack, 0)
		}

		if ui.released() {
			_ = mixer.SetTrackAudio(audio.sfxTrack, audio.sfx_uiRelease)
			_ = mixer.PlayTrack(audio.sfxTrack, 0)
			callback()
		}

		if ui.entered() || ui.exited() {
			_ = mixer.SetTrackAudio(audio.sfxTrack, audio.sfx_uiHover)
			_ = mixer.PlayTrack(audio.sfxTrack, 0)
		}
	}
}

