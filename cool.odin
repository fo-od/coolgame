package build

import "core:fmt"
import "core:os"

build_args :: []string{"-out:.build/coolgame", "-collection:engine=src/engine", "-show-timings"}

main :: proc() {
	args: [dynamic]string

	for arg, i in os.args[1:] {
		switch arg {
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

	if len(os.args) == 1 {
		fmt.println(
			"cool build tool",
			"Usage:",
			"\t./cool build [arguments]\tBuilds the project.",
			"\t./cool run [arguments]\tBuilds the project then runs the executable.",
			"\t./cool build run [arguments]\tSame as run, but keeps the executable.",
			"Flags:",
			"\t-debug\n\tEnables debug mode in the game.",
			"\t-odin-debug\n\tEnables Odin's debugging information.",
			"\tLook at 'odin help build' for other flags.",
			sep = "\n",
		)
	}

	append(&args, ..build_args)
	fmt.printfln("%w", args)
}

