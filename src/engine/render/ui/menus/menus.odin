package menus

import "core:fmt"
import "engine:render/ui"

main_menu :: proc() {
	button("Play", {0, 0}, proc() {
		fmt.println("play")
	})

	button("Create", {0, 0}, proc() {
		fmt.println("create")
	})
}

