package main

import "core:fmt"
import "core:path/filepath"
import "core:strings"
import "engine:app"
import "engine:physics"
import "engine:render"
import "engine:render/queue"
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
vsyncEnabled :: false

camX, camY: f32

gameRunning := false

main :: proc() {
	if !init() do return
	defer exit()

	when app.DEBUG do debug_init()

	for gameRunning {
		timer.start(&fpsTimer)
		currentTick := SDL.GetTicks()
		defer deltaTime = f32(SDL.GetTicks() - currentTick) / 1000.0

		event: SDL.Event
		for SDL.PollEvent(&event) {
			input(&event)
		}

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
		{string(SDL.GetBasePath()), "assets/cozette.fnt"},
		context.allocator,
	)
	app.font = TTF.OpenFont(strings.clone_to_cstring(fontPath), 13)

	// game config
	SDL.SetRenderVSync(app.renderer, i32(vsyncEnabled))

	SDL.SetRenderDrawBlendMode(app.renderer, SDL.BLENDMODE_BLEND)

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
	render.setDrawColor(app.renderer, {0, 0, 0, 255})
	SDL.RenderClear(app.renderer)

	queue.render(app.renderer)
	physics.draw(app.renderer)

	SDL.RenderPresent(app.renderer)
}

input :: proc(event: ^SDL.Event) {
	#partial switch event.type {
	case .QUIT:
		gameRunning = false

	case .WINDOW_RESIZED:
		SDL.GetWindowSizeInPixels(app.window, &app.windowSize.x, &app.windowSize.y)

	case .KEY_DOWN, .KEY_UP:
		app.keyboardState = SDL.GetKeyboardState(nil)

	case .MOUSE_MOTION, .MOUSE_BUTTON_DOWN, .MOUSE_BUTTON_UP:
		app.mouse.button = SDL.GetMouseState(&app.mouse.pos.x, &app.mouse.pos.y)
	}
}

tick :: proc() {
	physics.update(deltaTime)
}

debug_init :: proc() {
	physics.add_static_body({300, 300}, {320, 10})
	physics.add_body({300, 100}, {10, 10})
	physics.add_body({300, 100}, {10, 10}, velocity = {100, -1000})
}

debug_tick :: proc() {
	if app.keyboardState[SDL.Scancode.LEFT] {
		physics.get_body(1).velocity.x += -1000
	}

	if app.keyboardState[SDL.Scancode.RIGHT] {
		physics.get_body(1).velocity.x += 1000
	}

	queue.drawDebugTextFormat(0, 0, "%w", physics.get_body(1).velocity.x)
}

