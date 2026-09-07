package menus

import "core:log"
import "engine:render/ui"

main_menu :: proc() {
	if ui.rectangle()(
	{layout = {sizing = {.Grow, .Grow}}, style = {color = {0, 0, 0, 127}, filled = true}},
	) {
		button("Play", {pos = {0, -10}, anchor = {.Center}, origin = {.Center, .Bottom}}, proc() {
			log.debug("play")
		})

		button("Create", {pos = {0, 10}, anchor = {.Center}, origin = {.Center, .Top}}, proc() {
			log.debug("create")
		})
	}
}

