package build

import "core:fmt"
import "core:os"

main :: proc() {
	// modifiable args
	args: [dynamic]string
	append(&args, ..os.args)
	
	fmt.printfln("%w", args)
	
	for arg, i in args {
		switch arg {
			case "-debug": assign_at(&args, i, "-define:GAME_DEBUG=true")
			case "-odin_debug": assign_at(&args, i, "-debug")
		}
	}
	
	append(&args, "-out:.build/coolgame", "")
}

