package main

import "core:path/filepath"
import "core:strings"
import "engine:app"
import "engine:physics"
import "engine:physics/player"
import "engine:render/queue"
import "engine:render/ui"
import "engine:util/timer"
import SDL "vendor:sdl3"
import TTF "vendor:sdl3/ttf"

deltaTime: f32
fpsTimer: timer.Timer = {
	type = .NS,
}
renderingNS: u64
targetFPS :: 60
nsPerFrame: u64 : 1000000000 / targetFPS
vsyncEnabled :: true

gameRunning := false

main :: proc() {
	if !init() do return
	defer exit()

	when app.DEBUG do debug_init()

	for gameRunning {
		defer free_all(context.temp_allocator)

		timer.start(&fpsTimer)
		currentTick := SDL.GetTicks()
		defer deltaTime = f32(SDL.GetTicks() - currentTick) / 1000.0

		event: SDL.Event
		for SDL.PollEvent(&event) {
			input(&event)
		}
		app.keyboardState = SDL.GetKeyboardState(nil)

		tick()
		when app.DEBUG do debug_tick()

		draw()
		renderingNS = timer.getTicks(&fpsTimer)
		if !vsyncEnabled {
			if renderingNS < nsPerFrame {
				SDL.DelayNS(nsPerFrame - renderingNS)
				renderingNS = timer.getTicks(&fpsTimer)
			}
		}
	}
}

init :: proc() -> bool {
	// sdl stuff
	if !(SDL.SetAppMetadata("cool game", "0.1", "com.food.coolgame") && SDL.Init(SDL.INIT_VIDEO)) do return false

	app.window = SDL.CreateWindow(
		"cool game",
		app.windowSize.x,
		app.windowSize.y,
		SDL.WINDOW_RESIZABLE,
	)
	app.renderer = SDL.CreateRenderer(app.window, nil)

	// ttf stuff
	TTF.Init()
	app.textEngine = TTF.CreateRendererTextEngine(app.renderer)
	fontPath, _ := filepath.join(
		{string(SDL.GetBasePath()), "../src/assets/cozette.fnt"},
		context.allocator,
	)
	app.font = TTF.OpenFont(strings.clone_to_cstring(fontPath), 13)
	if app.textEngine == nil || app.font == nil do return false

	// game config
	SDL.SetRenderVSync(app.renderer, i32(vsyncEnabled))

	SDL.SetRenderDrawBlendMode(app.renderer, SDL.BLENDMODE_BLEND)
	player.init({300, 280})

	gameRunning = true
	return true
}

exit :: proc() {
	TTF.CloseFont(app.font)
	TTF.Quit()

	SDL.DestroyRenderer(app.renderer)
	SDL.DestroyWindow(app.window)
	SDL.Quit()
}

draw :: proc() {
	SDL.SetRenderDrawColor(app.renderer, 0, 0, 0, 255)
	SDL.RenderClear(app.renderer)

	physics.draw(app.renderer)
	queue.render(app.renderer)
	ui.draw(app.renderer)

	SDL.RenderPresent(app.renderer)
}

input :: proc(event: ^SDL.Event) {
	#partial switch event.type {
	case .QUIT:
		gameRunning = false

	case .WINDOW_RESIZED:
		SDL.GetWindowSizeInPixels(app.window, &app.windowSize.x, &app.windowSize.y)

	case .MOUSE_MOTION, .MOUSE_BUTTON_DOWN, .MOUSE_BUTTON_UP:
		app.mouse.button = SDL.GetMouseState(&app.mouse.pos.x, &app.mouse.pos.y)
		ui.update_pointer_state(app.mouse.pos, app.mouse.button)
	}
}

tick :: proc() {
	physics.update(deltaTime)

	player.input()

	// center camera over player
	app.cameraPos = -player.body.aabb.pos
	// put camera origin at center of window
	app.cameraPos.x += f32(app.windowSize.x) / 2
	app.cameraPos.y += f32(app.windowSize.y) / 2
}

debug_init :: proc() {
	physics.add_static_body({300, 300}, {5000, 10})
	physics.add_static_body({200, 0}, {20, 300}, collision_layer = .Layer_3)
}

debug_tick :: proc() {
	if ui.rectangle()({sizing = {.Grow, .Grow}, color = {0, 0, 0, 127}}) {
		ui.rectangle()(
		{
			rect = {0, 0, 100, 50},
			anchor = {.Center},
			origin = {.Center},
			color = ui.hovered() ? {255, 0, 0, 255} : {255, 255, 255, 255},
		},
		)
	}
}

