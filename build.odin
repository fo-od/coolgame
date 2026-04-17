package build

import "core:fmt"
import "core:os"
import "core:strings"

PROJECT_NAME :: "coolgame"

Action :: enum {
	Build,
	Run,
	Debug,
	Odin_Debug,
}

Actions :: bit_set[Action]

main :: proc() {
	args: [dynamic]string
	target_os := ODIN_OS
	actions: Actions = {.Build}

	for arg, i in os.args[1:] {
		switch arg {
		case "help":
			if i == 0 {
				actions = {}
				print_help()
				break
			}
		case "clean":
			if i == 0 {
				actions = {}
				os.remove_all(".build/"); os.mkdir(".build")
				break
			}
		case "run":
			actions += {.Run}
		case "debug":
			actions += {.Debug}
		case "odin-debug":
			actions += {.Odin_Debug}
		case:
			append(&args, arg)
		}

		if strings.contains(arg, "-target:") {
			if strings.contains(arg, "windows") do target_os = .Windows
			if strings.contains(arg, "darwin") do target_os = .Darwin
		}
	}


	if len(args) == 0 do inject_at(&args, 0, "odin", "build", "src")
	if .Run in actions do assign_at(&args, 1, "run"); append(&args, "-keep-executable")
	if .Debug in actions do append(&args, "-define:GAME_DEBUG=true")
	if .Odin_Debug in actions do append(&args, "-debug")

	build_args := []string {
		// horrible line of code that basically just adds .exe if the target platform is windows
		"-out:.build" + os.Path_Separator_String + PROJECT_NAME + ".exe" if target_os == .Windows else "-out:.build" + os.Path_Separator_String + PROJECT_NAME,
		"-collection:engine=src" +
		os.Path_Separator_String +
		"engine",
		"-show-timings",
	}

	if .Build in actions {
		if args[0] != "odin" {fmt.println("Invalid argument(s)"); print_help(); os.exit(1)}
		// add default build args
		append(&args, ..build_args)

		// add git hash to project constants
		_, git_hash := exec("git rev-parse --short HEAD", false)
		append(
			&args,
			strings.join({"-define:GIT_HASH=", git_hash.(string)}, ""),
		)

		// make build dir if it doesnt exist
		os.mkdir(".build")
		// copy assets to build dir if not on macOS
		if target_os != .Darwin do os.copy_directory_all(".build/assets", "src/assets")

		// print then run build command
		fmt.println(strings.join(args[:], " "))

		exit_code, _ := exec(args[:])
		if exit_code != 0 {
			fmt.printfln("Build exited with code %i", exit_code)
			print_help()
			os.exit(1)
		}

		// macOS bundling
		if target_os == .Darwin {
			// TODO: bundle project in a .app
		}
	}
}

print_help :: proc() {
	fmt.println(
		"cool build tool",
		"Usage:",
		"    bulid [arguments]               Builds the project.",
		"    build run [arguments]           Builds the project then runs the executable.",
		"    build debug [arguments]         Builds the project and enables debug mode in the game",
		"    build odin-debug [arguments]    Builds the project and enables Odin's debugging information",
		"    build help                      Displays this message",
		"    build clean                     Cleans the build directory",
		"",
		"You can use a combination of run, debug, and odin-debug, in any order.",
		"Example usage:",
		"    build run debug                 Builds the project with debug enabled, then runs it.",
		"    build debug run                 Does the same thing.",
		"",
		"Flags:",
		"    Look at 'odin help build' or 'odin build -help' for bulid flags.",
		sep = "\n",
	)
}

exec :: proc {
	exec_args,
	exec_string,
}

exec_args :: proc(args: []string, stdout := true) -> (exit_code: int, out: Maybe(string)) {
	if stdout {
		process, _ := os.process_start(
			{command = args, stdin = os.stdin, stdout = os.stdout, stderr = os.stderr},
		)
		state, _ := os.process_wait(process)

		return state.exit_code, nil
	} else {
		out_r, out_w, _ := os.pipe()

		process, _ := os.process_start(
			{command = args, stdin = os.stdin, stdout = out_w, stderr = os.stderr},
		)
		state, _ := os.process_wait(process)
		os.close(out_w)

		out, _ := os.read_entire_file(out_r, context.allocator)
		os.close(out_r)

		return state.exit_code, strings.clone_from_bytes(out)
	}
}

exec_string :: proc(cmd: string, stdout := true) -> (exit_code: int, out: Maybe(string)) {
	return exec_args(strings.split(cmd, " "), stdout)
}

