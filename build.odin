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
			if i == 0 {
				build = false
				print_help()
				break
			}
		case "run":
			if i == 0 do append(&args, "odin", "run", "src", "-keep-executable")
		case "clean":
			if i == 0 {
				build = false
				os.remove_all(".build/")
				os.mkdir(".build")
				break
			}
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

		_, git_hash := exec("git rev-parse HEAD")
		_, short_git_hash := exec("git rev-parse --short HEAD")
		append(
			&args,
			strings.join({"-define:GIT_HASH=", git_hash}, ""),
			strings.join({"-define:GIT_HASH_SHORT=", short_git_hash}, ""),
		)

		os.mkdir(".build")

		fmt.println(strings.join(args[:], " ")) // print full command

		exit_code := build_project(args[:])
		if exit_code != 0 {
			fmt.printfln("Build exited with code %i", exit_code)
			print_help()
			os.exit(1)
		}

		os.copy_directory_all(".build/assets", "src/assets")
	}
}

print_help :: proc() {
	fmt.println(
		"cool build tool",
		"Usage:",
		"    bulid [arguments]        Builds the project.",
		"    build run [arguments]    Builds the project then runs the executable.",
		"    build help               Displays this message",
		"    build clean              Cleans the build directory",
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

