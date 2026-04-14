package build

import "core:fmt"
import "core:os"
import "core:strings"

PROJECT_NAME :: "coolgame"

main :: proc() {
	args: [dynamic]string
	windows := false
	build := false

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
					append(&args, "odin", "run", "src", "-keep-executable")
				} else {
					append(&args, "odin", "build", "src")
				}
				build = true
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

		if strings.contains(arg, "-target:") {
			if strings.contains(arg, "windows") do windows = true
		} else if ODIN_OS == .Windows do windows = true
	}


	build_args := []string {
		// horrible line of code that basically just adds .exe if the target platform is windows
		"-out:.build" + os.Path_Separator_String + PROJECT_NAME + ".exe" if windows else "-out:.build" + os.Path_Separator_String + PROJECT_NAME,
		"-collection:engine=src" +
		os.Path_Separator_String +
		"engine",
		"-show-timings",
	}

	if build {
		append(&args, ..build_args)
		os.mkdir(".build")

		fmt.println(strings.join(args[:], " ")) // print full command

		exit_code := build_project(args[:])
		if exit_code != 0 {
			fmt.printfln("Build exited with code %i", exit_code)
			print_help()
			os.exit(1)
		}
	}
}

print_help :: proc() {
	fmt.println(
		"cool build tool",
		"Usage:",
		"    cool build [arguments]      Builds the project.",
		"    cool run [arguments]        Builds the project then runs the executable.",
		"    cool build run [arguments]  Same as run, but keeps the executable.",
		"",
		"Flags:",
		"    -debug",
		"        Enables debug mode in the game.",
		"",
		"    -odin-debug",
		"        Enables Odin's debugging information.",
		"",
		"    Look at 'odin help build' for other flags.",
		sep = "\n",
	)
}

build_project :: proc(args: []string) -> int {
	process, _ := os.process_start(
		{command = args, stdin = os.stdin, stdout = os.stdout, stderr = os.stderr},
	)
	state, _ := os.process_wait(process)

	return state.exit_code
}

