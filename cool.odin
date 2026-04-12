package build

import "core:fmt"
import "core:os"

build_args :: []string{"-out:.build/coolgame", "-collection:engine=src/engine", "-show-timings"}

main :: proc() {
	args: [dynamic]string

	if len(os.args) == 1 {
		print_help()
	}

	for arg, i in os.args[1:] {
		switch arg {
		case "help":
			if i == 0 do print_help()
		case "build":
			if i == 0 {
				if len(os.args) > 2 && os.args[2] == "run" {
					append(&args, "odin", "run", "-keep-executable")
				} else {
					append(&args, "odin", "build")
				}
			}
		case "run":
			if i == 0 do append(&args, "odin", "run")
		case "-debug":
			append(&args, "-define:GAME_DEBUG=true")
		case "-odin-debug":
			append(&args, "-debug")
		case:
			append(&args, arg)
		}
	}

	append(&args, ..build_args)
	fmt.printfln("%w", args)
}

print_help :: proc() {
	fmt.println(
		"cool build tool",
		"Usage:",
		"    ./cool build [arguments]      Builds the project.",
		"    ./cool run [arguments]        Builds the project then runs the executable.",
		"    ./cool build run [arguments]  Same as run, but keeps the executable.",
		"\nFlags:",
		"    -debug",
		"        Enables debug mode in the game.\n",
		"    -odin-debug",
		"        Enables Odin's debugging information.\n",
		"    Look at 'odin help build' for other flags.",
		sep = "\n",
	)
}

