package menus

import "engine:audio"
import "engine:render/ui"
import "vendor:sdl3/mixer"

button :: proc(text: string, pos: [2]f32, callback: proc()) {
	if ui.rectangle()(
	{
		rect = {pos.x, pos.y, 100, 50},
		layout = {anchor = {.Center}, origin = {.Center}},
		style = {color = ui.pressed() ? ui.GRAY : ui.WHITE, filled = ui.hovered()},
	},
	) {
		ui.text(
			text,
			{
				style = {color = ui.hovered() ? ui.BLACK : ui.WHITE},
				layout = {anchor = {.Center}, origin = {.Center}},
			},
		)

		// if ui.clicked() {
		// 	callback()
		// }

		if ui.entered() {
			_ = mixer.SetTrackAudio(audio.sfxTrack, audio.sfx_uiHover)
			_ = mixer.PlayTrack(audio.sfxTrack, 0)
		}
	}
}

