package main

import "core:fmt"
import "core:os"
import "core:strings"
import "engine/render/queue"
import "engine/util/timer"
import "engine/physics"
import SDL "vendor:sdl3"
import TTF "vendor:sdl3/ttf"

window: ^SDL.Window
renderer: ^SDL.Renderer
font: ^TTF.Font
textEngine: ^TTF.TextEngine

deltaTime: f32
targetFPS :: 60
nsPerFrame: u64 : 1000000000 / targetFPS
vsyncEnabled :: false

camX, camY: f32

gameRunning := false

keyboardState: [^]bool
mouse: Mouse

Mouse :: struct {
	x, y:   f32,
	button: SDL.MouseButtonFlags,
}

windowWidth, windowHeight: i32 = 640, 480

// TODO: when loading a level, generate a quadtree for that level

init :: proc() -> bool {
	// sdl stuff
	if !(SDL.SetAppMetadata("cool game", "0.1", "com.food.coolgame") && SDL.Init(SDL.INIT_VIDEO)) do return false

	window = SDL.CreateWindow("cool game", windowWidth, windowHeight, SDL.WINDOW_RESIZABLE)
	renderer = SDL.CreateRenderer(window, nil)

	// ttf stuff
	TTF.Init()
	textEngine = TTF.CreateRendererTextEngine(renderer)
	fontPath, err := os.join_path(
		{string(SDL.GetBasePath()), "assets/cozette.fnt"},
		context.temp_allocator,
	)
	font = TTF.OpenFont(strings.clone_to_cstring(fontPath), 13)
	
	// game config
	SDL.SetRenderVSync(renderer, i32(vsyncEnabled))

	gameRunning = true
	return true
}

exit :: proc() {
	TTF.CloseFont(font)
	TTF.Quit()

	SDL.DestroyRenderer(renderer)
	SDL.DestroyWindow(window)
	SDL.Quit()
}

render :: proc() {
	SDL.SetRenderDrawColor(renderer, 0, 0, 0, SDL.ALPHA_OPAQUE)
	SDL.RenderClear(renderer)

	queue.render(renderer)

	SDL.RenderPresent(renderer)
}

input :: proc(event: ^SDL.Event) {
	#partial switch event.type {
	case .QUIT:
		gameRunning = false

	case .WINDOW_RESIZED:
		SDL.GetWindowSizeInPixels(window, &windowWidth, &windowHeight)

	case .KEY_DOWN, .KEY_UP:
		keyboardState = SDL.GetKeyboardState(nil)

	case .MOUSE_MOTION, .MOUSE_BUTTON_DOWN, .MOUSE_BUTTON_UP:
		mouse.button = SDL.GetMouseState(&mouse.x, &mouse.y)
	}
}

tick :: proc() {
	physics.tick(deltaTime)
}

fpsTimer: timer.TimerNS
renderingNS: u64

main :: proc() {
	if !init() do return
	defer exit()

	for gameRunning {
		timer.start(&fpsTimer)
		currentTick := SDL.GetTicks()
		defer deltaTime = f32(SDL.GetTicks() - currentTick) / 1000.0

		event: SDL.Event
		for SDL.PollEvent(&event) {
			input(&event)
		}
		
		tick()

		render()
		renderingNS = timer.getTicksNS(&fpsTimer)
		if !vsyncEnabled {
			if renderingNS < nsPerFrame {
				SDL.DelayNS(nsPerFrame - renderingNS)
				renderingNS = timer.getTicksNS(&fpsTimer)
			}
		}
	}
}

