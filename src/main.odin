package main

import "core:log"
import "core:path/filepath"
import "core:strings"
import "engine:app"
import "engine:audio"
import "engine:physics"
import "engine:physics/player"
import "engine:render/queue"
import "engine:render/ui"
import "engine:render/ui/menus"
import "engine:util/timer"
import SDL "vendor:sdl3"
import TTF "vendor:sdl3/ttf"

deltaTime: f32
fpsTimer: timer.TimerNS
renderingNS: u64
targetFPS :: 60
nsPerFrame: u64 : 1000000000 / targetFPS
vsyncEnabled :: true

gameRunning := false
gameState := GameState.MainMenu

GameState :: enum {
	Paused,
	Playing,
	MainMenu,
	LevelCreator,
}

main :: proc() {
	context.logger = log.create_console_logger(app.DEBUG ? .Debug : .Info)
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
		app.mouse.button = SDL.GetMouseState(&app.mouse.pos.x, &app.mouse.pos.y)
		ui.update_pointer_state(app.mouse.pos, app.mouse.button)

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
	if !(SDL.SetAppMetadata("coolgame", "0.1", "dev.fooduhh.coolgame") && SDL.Init({.VIDEO, .AUDIO})) do return false

	app.window = SDL.CreateWindow(
		"cool game",
		app.windowSize.x,
		app.windowSize.y,
		SDL.WINDOW_RESIZABLE,
	)
	app.renderer = SDL.CreateRenderer(app.window, nil)

	// audio stuff
	if !audio.init() do return false

	// ttf stuff
	log.info("Initializing TTF...")
	TTF.Init()

	log.info("Creating TextEngine...")
	app.textEngine = TTF.CreateRendererTextEngine(app.renderer)

	if app.textEngine == nil {
		log.errorf("Error creating TextEngine: %v", SDL.GetError())
		return false
	}

	fontPath, _ := filepath.join(
		{string(SDL.GetBasePath()), "../src/assets/cozette.fnt"},
		context.allocator,
	)
	log.infof("Creating font...")
	app.font = TTF.OpenFont(strings.clone_to_cstring(fontPath), 13)

	if app.font == nil {
		log.errorf("Error creating font: %v", SDL.GetError())
		return false
	}

	// game config
	if !SDL.SetRenderVSync(app.renderer, i32(vsyncEnabled)) do log.warnf("Couldn't enable VSync: %v", SDL.GetError())
	if !SDL.SetRenderDrawBlendMode(app.renderer, SDL.BLENDMODE_BLEND) do log.warnf("Couldn't set render blend mode: %v", SDL.GetError())

	gameRunning = true
	return true
}

exit :: proc() {
    log.info("Uninitializing audio...")
	audio.deinit()

	log.info("Closing font...")
	TTF.CloseFont(app.font)
	log.info("Uninitializing TTF...")
	TTF.Quit()

	log.info("Destroying renderer...")
	SDL.DestroyRenderer(app.renderer)
	log.info("Destroying window...")
	SDL.DestroyWindow(app.window)
	log.info("Quitting...")
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
	}
}

tick :: proc() {
	if gameState == GameState.Playing do physics.update(deltaTime)

	player.input()

	// center camera over player
	app.cameraPos = -player.body.aabb.pos
	// put camera origin at center of window
	app.cameraPos.x += f32(app.windowSize.x) / 2
	app.cameraPos.y += f32(app.windowSize.y) / 2

	ui.begin_frame()
}

debug_init :: proc() {
	physics.add_static_body({300, 300}, {5000, 10})
	physics.add_static_body({200, 0}, {20, 300}, collision_layer = .Layer_3)
}

debug_tick :: proc() {
	menus.main_menu()
}

