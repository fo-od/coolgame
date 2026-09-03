package menus

import "core:log"
import "engine:render/ui"

main_menu :: proc() {
	if ui.rectangle()({layout = {sizing = {.Grow, .Grow}}, style = {color = {0, 0, 0, 127}}}) {
		button("Play", {0, 0}, proc() {
			log.debug("play")
		})

		// button("Create", {0, 0}, proc() {
		// 	log.debug("create")
		// })
	}
}

