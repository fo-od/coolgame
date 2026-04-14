package build

import "core:fmt"
import "core:os"
import "core:strings"

PROJECT_NAME :: "coolgame"

main :: proc() {
	args: [dynamic]string
	windows := false
	build := true

	for arg, i in os.args[1:] {
		switch arg {
		case "help":
			build = false
			if i == 0 do print_help()
		case "run":
			if i == 0 do append(&args, "odin", "run", "src", "-keep-executable")
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


	if len(args) == 0 do append(&args, "odin", "build", "src")
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

		fmt.println(exec("git rev-parse --short HEAD"))
		fmt.println(exec("git rev-parse HEAD"))

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
		"    bulid [arguments]            Builds the project.",
		"    build run [arguments]        Builds the project then runs the executable.",
		"    build help                   Displays this message",
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

exec :: proc(cmd: string) -> (exit_code: int, stdout: string) {
	out_r, out_w, _ := os.pipe()

	process, _ := os.process_start(
		{command = strings.split(cmd, " "), stdin = os.stdin, stdout = out_w, stderr = os.stderr},
	)
	state, _ := os.process_wait(process)
	os.close(out_w)

	out, _ := os.read_entire_file(out_r, context.allocator)
	os.close(out_r)

	return state.exit_code, strings.clone_from_bytes(out)
}

