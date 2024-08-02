mod command_line;

use command_line::parse_args;

pub fn run() {
    parse_args();
}
